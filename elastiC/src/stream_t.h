/* ==========================================================================
 * stream_t.h
 * ==========================================================================
 * elastiC language stream type.
 * --------------------------------------------------------------------------
 *   AUTHOR:  Marco Pantaleoni         E-mail: panta@elasticworld.org
 *
 *   Created: Wed Jun 12 10:52:30 CEST 2002
 *
 *   $Id: stream_t.h,v 1.1 2002/06/12 16:00:05 panta Exp $
 * --------------------------------------------------------------------------
 *    Copyright (C) 2002 Marco Pantaleoni. All rights reserved.
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

#ifndef __EC_STREAM_T_H
#define __EC_STREAM_T_H

#include <stdio.h>
#include <stdlib.h>

#include <elastic/elastic.h>

EC_BEGIN_DECLS

extern EC_API EcInt tc_stream;

/* ========================================================================
 * M A C R O S
 * ======================================================================== */

#define EC_STREAMP(obj)					(EC_TYPEP(obj,tc_stream))
#define EC_STREAM_STR(obj)				((ec_stream *) EC_USER(obj))
#define EC_STREAM_STR_SET(obj,val)		do { EC_USER(obj) = (ec_stream *)(val); } while (0)

/* ========================================================================
 * P R O T O T Y P E S
 * ======================================================================== */

/* C API */

EC_API EC_OBJ EcMakeStream( ec_stream *stream );
EC_API EC_OBJ EcLibStreamClose( EC_OBJ obj );

EC_END_DECLS

#endif /* __EC_STREAM_T_H */
