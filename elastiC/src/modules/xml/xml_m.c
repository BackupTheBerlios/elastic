/* ==========================================================================
 * xml_m.c
 * ==========================================================================
 * Expat XML parsing module.
 * --------------------------------------------------------------------------
 *   AUTHOR:  Marco Pantaleoni         E-mail: panta@elasticworld.org
 *
 *   Created: Mon Mar 21 15:19:09 CET 2005
 *
 *   $Id: xml_m.c,v 1.1 2005/03/22 10:09:34 panta Exp $
 * --------------------------------------------------------------------------
 *    Copyright (C) 2005 Marco Pantaleoni. All rights reserved.
 *
 *  The contents of this file are subject to the elastiC License version 1.0
 *  (the "elastiC License"); you may not use this file except in compliance
 *  with the elastiC License. You may obtain a copy of the elastiC License at
 *  http://www.elasticworld.org/LICENSE
 *
 *  IN NO EVENT SHALL THE AUTHOR OR DISTRIBUTORS BE LIABLE TO ANY PARTY
 *  FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES
 *  ARISING OUT OF THE USE OF THIS SOFTWARE, ITS DOCUMENTATION, OR ANY
 *  DERIVATIVES THEREOF, EVEN IF THE AUTHOR HAS BEEN ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 *
 *  THE AUTHOR AND DISTRIBUTORS SPECIFICALLY DISCLAIM ANY WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE, AND NON-INFRINGEMENT.  THIS SOFTWARE
 *  IS PROVIDED ON AN "AS IS" BASIS, AND THE AUTHOR AND DISTRIBUTORS HAVE
 *  NO OBLIGATION TO PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR
 *  MODIFICATIONS.
 *
 *  See the elastiC License for the specific language governing rights and
 *  limitations under the elastiC License.
 * ==========================================================================
 */

#include "basic.h"
#include "debug.h"
#include "compat.h"

#include "elastic.h"
#include "xml_m.h"

#include "expat/lib/expat.h"

static EcInt tc_xmlparser = -1;

static EcUInt s_status_error = 0, s_status_ok = 0;

static EC_OBJ EcXMLErrorClass = EC_NIL;

struct xmlparser_struct
{
	XML_Parser expat_parser;									/* expat XML parser                         */
	EC_OBJ     h_start_element;									/* start element handler (bytecode object)  */
	EC_OBJ     h_end_element;									/* end element handler (bytecode object)    */
	EC_OBJ     h_character_data;								/* character data handler (bytecode object) */
};

/* ========================================================================
 * M A C R O S
 * ======================================================================== */

#define EC_XMLPARSERP(obj)				(EC_TYPEP(obj,tc_xmlparser))
#define EC_XMLPARSER(obj)				((struct xmlparser_struct *) EC_USER(obj))
#define EC_XMLPARSER_PARSER(obj)		(EC_XMLPARSER(obj)->expat_parser)
#define EC_XMLPARSER_START_HDLR(obj)	(EC_XMLPARSER(obj)->h_start_element)
#define EC_XMLPARSER_END_HDLR(obj)		(EC_XMLPARSER(obj)->h_end_element)
#define EC_XMLPARSER_CHAR_HDLR(obj)		(EC_XMLPARSER(obj)->h_character_data)

#define EC_XMLPARSER_SET(obj,field,val)			do { struct xmlparser_struct *xmlp = EC_XMLPARSER(obj); xmlp->field = (val); } while (0)
#define EC_XMLPARSER_PARSER_SET(obj,val)		do { EC_XMLPARSER_SET(obj,expat_parser,val);     } while (0)
#define EC_XMLPARSER_START_HDLR_SET(obj,val)	do { EC_XMLPARSER_SET(obj,h_start_element,val);  } while (0)
#define EC_XMLPARSER_END_HDLR_SET(obj,val)		do { EC_XMLPARSER_SET(obj,h_end_element,val);    } while (0)
#define EC_XMLPARSER_CHAR_HDLR_SET(obj,val)		do { EC_XMLPARSER_SET(obj,h_character_data,val); } while (0)


/* ========================================================================
 * P R O T O T Y P E S
 * ======================================================================== */

/* elastiC API */

static EC_OBJ EcLibXML_ParserCreate( EC_OBJ stack, EcAny userdata );
static EC_OBJ EcLibXML_SetElementHandler( EC_OBJ stack, EcAny userdata );
static EC_OBJ EcLibXML_Parse( EC_OBJ stack, EcAny userdata );

static EC_OBJ EcLibXMLErrorClass_New( EC_OBJ self, EC_OBJ in_class, EC_OBJ stack );
static EC_OBJ EcLibXMLError_GetLine( EC_OBJ self, EC_OBJ in_class, EC_OBJ stack );

static EC_OBJ EcXMLError( const char *message, EcInt line );

/* private */

static EC_OBJ make_xmlparser( XML_Parser expat_parser, EC_OBJ h_start_element, EC_OBJ h_end_element, EC_OBJ h_character_data );

static EC_OBJ xmlparser_copy( EC_OBJ obj, EcCopyType type );
static void   xmlparser_mark( EC_OBJ obj );
static void   xmlparser_free( EC_OBJ obj );
static EcInt  xmlparser_print( ec_string *str, EC_OBJ obj, EcBool detailed );
static EcUInt xmlparser_hash( EC_OBJ obj, EcInt recur );
static EcBool xmlparser_compare( EC_OBJ obj1, EC_OBJ obj2, EcInt *res );

void XMLCALL start_element_handler(void *userData, const XML_Char *name, const XML_Char **atts);
void XMLCALL end_element_handler(void *userData, const XML_Char *name);
void XMLCALL character_data_handler(void *userData, const XML_Char *s, int len);


/* ------------------------------------------------------------------------
 * XMLError
 * ------------------------------------------------------------------------ */

static EcClassDef EcXMLErrorClassDef =
{
	"XMLError",													/* class name         */
	"xml",														/* package name       */

	EC_NIL,														/* super class        */

	NULL,														/* class methods      */
	NULL,														/* class variables    */

	NULL,														/* instance methods   */
	NULL														/* instance variables */
};

static EcCMethodDef EcXMLErrorClass_ClassMethods[] =			/* Class Methods   */
{
	{ "new",			EcLibXMLErrorClass_New   },
	{ NULL,             NULL                     }
};

static EcCMethodDef EcXMLErrorClass_Methods[] =					/* Instance Methods   */
{
	{ "getLine",		EcLibXMLError_GetLine    },
	{ NULL,				NULL                     }
};

static EcVariableDef EcXMLErrorClass_Variables[] =				/* Instance Variables */
{
	{ "line"                                     },
	{ NULL                                       }
};

/* ========================================================================
 * I M P L E M E N T A T I O N
 * ------------------------------------------------------------------------ */

/* C API */

static EC_OBJ EcLibXML_ParserCreate( EC_OBJ stack, EcAny userdata )
{
	const char *encoding = NULL;
	XML_Parser  expat_parser;
	EC_OBJ      res;

	encoding = NULL;
	res = EcParseStackFunction( "xml.parser_create", TRUE, stack, "|s", &encoding );
	if (EC_ERRORP(res)) return res;

	expat_parser = XML_ParserCreate( encoding );
	if (! expat_parser)
		return EcMemoryError();

	XML_SetElementHandler( expat_parser, start_element_handler, end_element_handler );
	XML_SetCharacterDataHandler( expat_parser, character_data_handler );

	return make_xmlparser( expat_parser, EC_NIL, EC_NIL, EC_NIL );
}

static EC_OBJ EcLibXML_SetElementHandler( EC_OBJ stack, EcAny userdata )
{
	EC_OBJ      parser;
	EC_OBJ		h_start_element  = EC_NIL;
	EC_OBJ		h_end_element    = EC_NIL;
	EC_OBJ		h_character_data = EC_NIL;
	EC_OBJ      res;

	res = EcParseStackFunction( "xml.set_element_handler", TRUE, stack, "O!|OOO", tc_xmlparser, &parser, &h_start_element, &h_end_element, &h_character_data );
	if (EC_ERRORP(res)) return res;

	if (h_start_element)
		EC_XMLPARSER_START_HDLR_SET(parser, h_start_element);
	if (h_end_element)
		EC_XMLPARSER_END_HDLR_SET(parser,   h_end_element);
	if (h_character_data)
		EC_XMLPARSER_CHAR_HDLR_SET(parser,  h_character_data);
	return parser;
}

static EC_OBJ EcLibXML_Parse( EC_OBJ stack, EcAny userdata )
{
	EC_OBJ      parser;
	char       *buffer;
	EcInt       buffer_len;
	EcBool      final = FALSE;
	EC_OBJ      res;

	enum XML_Status status;

	res = EcParseStackFunction( "xml.parse", TRUE, stack, "O!s#|b",
								tc_xmlparser, &parser, &buffer, &buffer_len, &final );
	if (EC_ERRORP(res)) return res;

	status = XML_Parse( EC_XMLPARSER_PARSER(parser), buffer, buffer_len, final );
	if (status == XML_STATUS_ERROR)
	{
		return EcXMLError( XML_ErrorString(XML_GetErrorCode(EC_XMLPARSER_PARSER(parser))), XML_GetCurrentLineNumber(EC_XMLPARSER_PARSER(parser)) );
	}

	return parser;
}


static EC_OBJ EcLibXMLErrorClass_New( EC_OBJ self, EC_OBJ in_class, EC_OBJ stack )
{
	EC_OBJ obj;
	EC_OBJ msg, line;

	EC_CHECKNARGS_CM( self, "new", 2 );

	line = EC_STACKPOP( stack );
	msg  = EC_STACKPOP( stack );

	/*
	 * This is a class method, so self is the class
	 */
	EC_STACKNARGS_REAL(stack) = EC_STACKNARGS_LOGICAL(stack) = 1;
	EC_STACKPUSH( stack, msg );
	obj = EcLibExceptionClass_NewWith( self, in_class, stack );
	EcSetInstanceVariable( obj, EcXMLErrorClass,
						   "line",
						   line );

	return obj;
}

static EC_OBJ EcLibXMLError_GetLine( EC_OBJ self, EC_OBJ in_class, EC_OBJ stack )
{
	EC_OBJ res;

	EC_CHECKNARGS_M( self, "getLine", 0 );

	ASSERT( in_class == EcXMLErrorClass );
	res = EcGetInstanceVariable( self,
								 EcXMLErrorClass,
								 "line" );
	return res;
}

static EC_OBJ EcXMLError( const char *message, EcInt line )
{
	ec_string msg;

	ec_string_init( &msg, NULL );
	ec_sprintf( &msg, "%s (line: %ld)", message, (long)line );
	
	EcMakeExceptionWithArgs( EcXMLErrorClass, ec_strdata(&msg), 1, EcMakeInt( line ) );
	ec_string_cleanup( &msg );
	return Ec_ERROR;
}



/* private */

static EC_OBJ make_xmlparser( XML_Parser expat_parser, EC_OBJ h_start_element, EC_OBJ h_end_element, EC_OBJ h_character_data )
{
	EC_OBJ obj;
	struct xmlparser_struct *xmlp;

	xmlp = (struct xmlparser_struct *) ec_malloc( sizeof(struct xmlparser_struct) );
	if (! xmlp) return EcMemoryError();

	xmlp->expat_parser     = expat_parser;
	xmlp->h_start_element  = h_start_element;
	xmlp->h_end_element    = h_end_element;
	xmlp->h_character_data = h_character_data;

	obj = EcMakeUser( tc_xmlparser, xmlp );
	if (EC_ERRORP(obj)) return obj;

	XML_SetUserData( expat_parser, (void *)obj );

	return obj;
}

static EC_OBJ xmlparser_copy( EC_OBJ obj, EcCopyType type )
{
	XML_Parser copy_expat_parser;
	EC_OBJ     copy_h_start_element, copy_h_end_element, copy_h_character_data;

	if (type == EcShallowCopyType)
	{
		copy_expat_parser     = EC_XMLPARSER_PARSER(obj);
		copy_h_start_element  = EC_XMLPARSER_START_HDLR(obj);
		copy_h_end_element    = EC_XMLPARSER_END_HDLR(obj);
		copy_h_character_data = EC_XMLPARSER_CHAR_HDLR(obj);
	} else
	{
		copy_expat_parser     = EC_XMLPARSER_PARSER(obj);		/* TODO: uhm? */

		copy_h_start_element  = EcCopy( EC_XMLPARSER_START_HDLR(obj), EcDeepCopyType );
		if (EC_ERRORP(copy_h_start_element)) return copy_h_start_element;
		copy_h_end_element    = EcCopy( EC_XMLPARSER_END_HDLR(obj),   EcDeepCopyType );
		if (EC_ERRORP(copy_h_end_element)) return copy_h_end_element;
		copy_h_character_data = EcCopy( EC_XMLPARSER_CHAR_HDLR(obj),  EcDeepCopyType );
		if (EC_ERRORP(copy_h_character_data)) return copy_h_character_data;
	}

	return make_xmlparser( copy_expat_parser, copy_h_start_element, copy_h_end_element, copy_h_character_data );
}

static void xmlparser_mark( EC_OBJ obj )
{
	if (EC_NNULLP(EC_XMLPARSER_START_HDLR(obj)))
		EcMarkObject( EC_XMLPARSER_START_HDLR(obj) );
	if (EC_NNULLP(EC_XMLPARSER_END_HDLR(obj)))
		EcMarkObject( EC_XMLPARSER_END_HDLR(obj) );
	if (EC_NNULLP(EC_XMLPARSER_CHAR_HDLR(obj)))
		EcMarkObject( EC_XMLPARSER_CHAR_HDLR(obj) );
}

static void xmlparser_free( EC_OBJ obj )
{
	if (! EC_XMLPARSER(obj)) return;

	if (EC_XMLPARSER_PARSER(obj))
	{
		XML_ParserFree( EC_XMLPARSER_PARSER(obj) );
	}

	EC_XMLPARSER_PARSER(obj) = NULL;
	EC_XMLPARSER_START_HDLR_SET(obj,EC_NIL);
	EC_XMLPARSER_END_HDLR_SET(obj,EC_NIL);
	EC_XMLPARSER_CHAR_HDLR_SET(obj,EC_NIL);

	ec_free( EC_XMLPARSER(obj) );
	EC_XMLPARSER_PARSER_SET(obj, NULL);
}

static EcInt xmlparser_print( ec_string *str, EC_OBJ obj, EcBool detailed )
{
	if (detailed)
		return ec_sprintf( str, "<xmlparser start_handler:%W end_handler:%W char_handler:%W>",
						   EC_XMLPARSER_START_HDLR(obj), EC_XMLPARSER_END_HDLR(obj), EC_XMLPARSER_CHAR_HDLR(obj) );
	else
		return ec_sprintf( str, "<xmlparser>" );
}

static EcUInt xmlparser_hash( EC_OBJ obj, EcInt recur )
{
	return ((EcUInt)EC_XMLPARSER_PARSER(obj) +
			EcHash( EC_XMLPARSER_START_HDLR(obj) ) +
			EcHash( EC_XMLPARSER_END_HDLR(obj) ) +
			EcHash( EC_XMLPARSER_CHAR_HDLR(obj) ));
}

static EcBool xmlparser_compare( EC_OBJ obj1, EC_OBJ obj2, EcInt *res )
{
	if (EC_XMLPARSER_PARSER(obj1) != EC_XMLPARSER_PARSER(obj2))
		return FALSE;
	if (! EcObjectEqual( EC_XMLPARSER_START_HDLR(obj1), EC_XMLPARSER_START_HDLR(obj2) ))
		return FALSE;
	if (! EcObjectEqual( EC_XMLPARSER_END_HDLR(obj1), EC_XMLPARSER_END_HDLR(obj2) ))
		return FALSE;
	if (! EcObjectEqual( EC_XMLPARSER_CHAR_HDLR(obj1), EC_XMLPARSER_CHAR_HDLR(obj2) ))
		return FALSE;

	return TRUE;
}

/* Expat handlers */

void XMLCALL start_element_handler(void *userData, const XML_Char *name, const XML_Char **atts)
{
	EC_OBJ obj;
	EC_OBJ callback;
	EC_OBJ arg_v[3];
	EC_OBJ attr;
	EC_OBJ res;
	int    i;

	obj = (EC_OBJ)userData;
	if (! EC_XMLPARSERP(obj))
		return;

	callback = EC_XMLPARSER_START_HDLR(obj);
	if (EC_NULLP(callback))
		return;

	if ((! EC_PRIMITIVEP(callback)) &&
		(! EC_COMPILEDP(callback)))
		return;

	attr = EcMakeHash();
	if (EC_NULLP(attr))
		return; /* return Ec_ERROR; */
	if (EC_ERRORP(attr))
		return; /* return attr; */

	for (i = 0; atts[i]; i += 2) {
		EcSequenceSetItem( attr, EcMakeString( atts[i], -1 ), EcMakeString( atts[i + 1], -1 ) );
	}

	arg_v[0] = obj;
	arg_v[1] = EcMakeString( name, -1 );
	arg_v[2] = attr;

	res = EcCallArgs( /* caller's stack */ EC_NIL, /* self */ EC_NIL, /* at_class */ EC_NIL, callback, 3, arg_v );
}

void XMLCALL end_element_handler(void *userData, const XML_Char *name)
{
	EC_OBJ obj;
	EC_OBJ callback;
	EC_OBJ arg_v[2];
	EC_OBJ res;

	obj = (EC_OBJ)userData;
	if (! EC_XMLPARSERP(obj))
		return;

	callback = EC_XMLPARSER_END_HDLR(obj);
	if (EC_NULLP(callback))
		return;

	if ((! EC_PRIMITIVEP(callback)) &&
		(! EC_COMPILEDP(callback)))
		return;

	arg_v[0] = obj;
	arg_v[1] = EcMakeString( name, -1 );

	res = EcCallArgs( /* caller's stack */ EC_NIL, /* self */ EC_NIL, /* at_class */ EC_NIL, callback, 2, arg_v );
}

void XMLCALL character_data_handler(void *userData, const XML_Char *s, int len)
{
	EC_OBJ obj;
	EC_OBJ callback;
	EC_OBJ arg_v[2];
	EC_OBJ res;

	obj = (EC_OBJ)userData;
	if (! EC_XMLPARSERP(obj))
		return;

	callback = EC_XMLPARSER_CHAR_HDLR(obj);
	if (EC_NULLP(callback))
		return;

	if ((! EC_PRIMITIVEP(callback)) &&
		(! EC_COMPILEDP(callback)))
		return;

	arg_v[0] = obj;
	arg_v[1] = EcMakeString( s, len );

	res = EcCallArgs( /* caller's stack */ EC_NIL, /* self */ EC_NIL, /* at_class */ EC_NIL, callback, 2, arg_v );
}

/* Module initialization & cleanup */

#if ECMODULE_XML_STATIC
EC_XML_MODULE_API EC_OBJ _ec_modxml_init( void )
#else
EC_XML_MODULE_API EC_OBJ ec_xml_init( void )
#endif
{
	EcTypespec xmlparser_spec = {
		/* type    */		0,
		/* name    */		"xmlparser",

		/* copy    */		xmlparser_copy,

		/* mark    */		xmlparser_mark,
		/* free    */		xmlparser_free,
		/* print   */		xmlparser_print,

		/* hash    */		xmlparser_hash,
		/* compare */		xmlparser_compare,

		/* check   */		NULL,

		/* sequence */      NULL,
		/* numeric  */      NULL
	};
	EC_OBJ pkg;

	tc_xmlparser = EcAddType( &xmlparser_spec );
	if (tc_xmlparser == 0)
		return EC_NIL;

	pkg = EcPackageIntroduce( "xml" );

	EcAddPrimitive( "xml.parser_create",			EcLibXML_ParserCreate );
	EcAddPrimitive( "xml.set_element_handler",		EcLibXML_SetElementHandler );
	EcAddPrimitive( "xml.parse",					EcLibXML_Parse );
	/* EcAddPrimitive( "xml.get_current_line_number",	EcLibXML_GetCurrentLineNumber ); */

	/* Symbols */
	s_status_error   = EcInternSymbol( "status-error" );
	s_status_ok      = EcInternSymbol( "status-ok" );

	/* XMLError exception */

	EcXMLErrorClassDef.superclass     = EcExceptionClass;
	EcXMLErrorClassDef.classmethods   = EcXMLErrorClass_ClassMethods;
	EcXMLErrorClassDef.classvariables = NULL;
	EcXMLErrorClassDef.methods        = EcXMLErrorClass_Methods;
	EcXMLErrorClassDef.variables      = EcXMLErrorClass_Variables;

	EcXMLErrorClass = EcAddClassDef( &EcXMLErrorClassDef );
	EcRegisterBuiltin( "xml.XMLError", EcXMLErrorClass );

	/*
	 * Save global forever-living objects from GC
	 */
	EcGCProtect( EcXMLErrorClass );

	return pkg;
}

#if ECMODULE_XML_STATIC
EC_XML_MODULE_API void _ec_modxml_cleanup( void )
#else
EC_XML_MODULE_API void ec_xml_cleanup( void )
#endif
{
}
