/* ==========================================================================
 * xml_m.h
 * ==========================================================================
 * Expat XML parsing module.
 * --------------------------------------------------------------------------
 *   AUTHOR:  Marco Pantaleoni         E-mail: panta@elasticworld.org
 *
 *   Created: Mon Mar 21 15:19:09 CET 2005
 *
 *   $Id: xml_m.h,v 1.1 2005/03/22 10:09:34 panta Exp $
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

#ifndef EC_XML_M_H
#define EC_XML_M_H

#include <elastic/elastic.h>

#if XML_MODULE_EXPORTS
#define EC_XML_MODULE_API EC_APIEXPORT
#else
#define EC_XML_MODULE_API EC_APIIMPORT
#endif

EC_BEGIN_DECLS
EC_XML_MODULE_API EC_OBJ ec_xml_init( void );
EC_XML_MODULE_API void   ec_xml_cleanup( void );
EC_END_DECLS

#endif /* EC_XML_M_H */
