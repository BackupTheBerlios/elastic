/* ==========================================================================
 * oop.h
 * ==========================================================================
 * Object Oriented Programming support.
 * --------------------------------------------------------------------------
 *   AUTHOR:  Marco Pantaleoni         E-mail: panta@elasticworld.org
 *
 *   Created: Sat Dec 12 22:01:08 MET 1998
 *
 *   $Id: oop.h,v 1.1 2002/05/23 21:17:28 panta Exp $
 * --------------------------------------------------------------------------
 *    Copyright (C) 1998-2000 Marco Pantaleoni. All rights reserved.
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

#ifndef __OOP_H
#define __OOP_H

#include <elastic/basic.h>

EC_BEGIN_DECLS

/* EcObject data */

typedef struct
{
	EC_OBJ   ofclass;											/* Class of this object                              */

	EC_OBJ  *ivars;												/* Instance variables                                */

	void *user;													/* C user data                                       */
} EcObject;


typedef struct
{
	EcUInt symid;												/* Variable symbold ID                               */
	EcInt  offset;												/* Offset of variable from class offset              */
} EcVariableEntry;

typedef struct
{
	EcUInt symid;												/* Method symbold ID                                 */
	EC_OBJ impl;												/* Method implementation                             */
} EcMethodEntry;

typedef struct
{
	EC_OBJ superclass;											/* Direct Base Class                                 */

	/* Methods */
	EcInt          nmethods;									/* Number of methods in the table                    */
	EcMethodEntry *mtable;										/* Method Table                                      */

	/* Class Methods */
	EcInt          ncmethods;									/* Number of class methods in the table              */
	EcMethodEntry *cmtable;										/* Class Method Table                                */

	/* Instance Variables */
	/*
	 * ioffset:  offset of instance variables specific to this class in 'ivars' array
	 * nivars :  TOTAL number of variables in instance (specific: nivars - ioffset)
	 * ivtable:  variable descriptions specific to this class (dimension: nivars - ioffset)
	 */
	EcInt ioffset;												/* Offset of instance variables for this class       */
	EcInt nivars;												/* Number of instance variables (TOTAL)              */
	EcVariableEntry *ivtable;									/* Instance Variable Table                           */

	/* Class Variables */
	EcInt ncvars;												/* Number of class variables                         */
	EcVariableEntry *cvtable;									/* Class Variable Table                              */

	/* Misc */
	EC_OBJ package;												/* Package containing this class                     */
	EC_OBJ name;												/* (Long) Class Name (tc_string)                     */
	EC_OBJ shortname;											/* Class Name without package prefix (tc_string)     */

#if 0
	EC_OBJ lframe;												/* Literal frame (class vars, methods, consts., ...) */
#endif

	EC_OBJ code;												/* Class code (executed on class introduction)       */
} EcClass;


/*
 * Utility structures to simplify the process of adding a new class
 */

typedef struct
{
	const char *name;
	EcCMethod   impl;
} EcCMethodDef;

typedef struct
{
	const char *name;
} EcVariableDef;

typedef struct
{
	const char    *name;
	const char    *pkgname;
	EC_OBJ         superclass;

	/* Class methods and variables */
	EcCMethodDef  *classmethods;								/* list of class methods      */
	EcVariableDef *classvariables;								/* list of class variables    */

	/* Instance methods and variables */
	EcCMethodDef  *methods;										/* list of instance methods   */
	EcVariableDef *variables;									/* list of instance variables */
} EcClassDef;

EC_END_DECLS

#endif /* __OOP_H */
