/* ==========================================================================
 * unicode.h
 * ==========================================================================
 * A 'unicode' elastiC module.
 * --------------------------------------------------------------------------
 *   AUTHOR:  Emiliano                 E-mail: emile@iris-advies.nl
 *
 *   Minor modifications by:
 *            Marco Pantaleoni         E-mail: panta@elasticworld.org
 *
 *   Created: 18 Oct 2001
 *
 *   $Id: unicode.h,v 1.1 2002/05/30 18:39:45 panta Exp $
 * --------------------------------------------------------------------------
 *    Copyright (C) 2001-2002 Emiliano <emile@iris-advies.nl>. All rights reserved.
 *    Copyright (C) Marco Pantaleoni. All rights reserved.
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

#ifndef __EC_UNICODE_H
#define __EC_UNICODE_H

#include <elastic/elastic.h>

/* ========================================================================
 * A P I
 * ======================================================================== */

EC_OBJ ec_unicode_init( void );
void   ec_unicode_cleanup( void );

#endif __EC_UNICODE_H
