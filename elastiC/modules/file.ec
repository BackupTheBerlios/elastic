/* ==========================================================================
 * file.ec
 * ==========================================================================
 * File module - compatibility layer with the old builtin `file' module.
 * --------------------------------------------------------------------------
 *   AUTHOR:  Marco Pantaleoni         E-mail: panta@elasticworld.org
 *
 *   Created: Wed Jun 12 14:34:50 CEST 2002
 *
 *   $Id: file.ec,v 1.1 2002/06/12 16:00:05 panta Exp $
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

package file;

/*
 * Backwards compatibility module.
 *
 * This interface is obsolete, consider using
 * the modules `stream' and `filestream' instead.
 */

import stream;
import filestream;

public open      = filestream.open;
public fdopen    = filestream.fdopen;
public popen     = filestream.popen;
public close     = stream.close;
public mode      = stream.mode;
public eof       = stream.eof;
public flush     = stream.flush;
public read      = stream.read;
public write     = stream.write;
public getc      = stream.getc;
public gets      = stream.gets;
public putc      = stream.putc;
public puts      = stream.puts;
public printf    = stream.printf;
public ungetc    = stream.ungetc;
public charready = stream.charready;
public seek      = stream.seek;
public tell      = stream.tell;

public readonly  = stream.readonly;
public writeonly = stream.writeonly;
public readwrite = stream.readwrite;

public stdin     = stream.stdin();
public stdout    = stream.stdout();
public stderr    = stream.stderr();
