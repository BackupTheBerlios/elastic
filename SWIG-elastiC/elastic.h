/* ==========================================================================
 * elastic.h
 * ==========================================================================
 * elastiC language class.
 * --------------------------------------------------------------------------
 *   AUTHOR:  Marco Pantaleoni         E-mail: panta@elasticworld.org
 *
 *   Created: 19 Aug 1999
 *
 *   $Id: elastic.h,v 1.1 2002/05/31 12:30:00 panta Exp $
 * --------------------------------------------------------------------------
 *    Copyright (C) 1999 Marco Pantaleoni. All rights reserved.
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

/**************************************************************************
 * $Header: /home/xubuntu/berlios_backup/github/tmp-cvs/elastic/Repository/SWIG-elastiC/elastic.h,v 1.1 2002/05/31 12:30:00 panta Exp $
 *
 * class ELASTIC
 *
 * elastiC implementation
 * (Caution : This is *somewhat* experimental)
 *
 * $Log: elastic.h,v $
 * Revision 1.1  2002/05/31 12:30:00  panta
 * Initial revision
 *
 *
 **************************************************************************/

class ELASTIC : public Language {
private:
  char   *elastic_path;
  char   *module;
  int     have_defarg;
  int     have_output;

  void    get_pointer(char *iname, char *srcname, char *src, char *dest,
					  DataType *t, String &f, char *ret);
  char   *convert_self(WrapperFunction &);
  void    usage_var(char *, DataType *, char **);
  void    usage_func(char *, DataType *, ParmList *, char **);
  void    usage_const(char *, DataType *, char *, char **);

  struct Primitive {            // Primitive list.  Needed to build primitives.
    char   *name;               // Array at very end.
    char   *function;
    Primitive *next;
  };
  Primitive *head;

  void  add_primitive(char *name, char *function);
  void  print_primitives();
  char *usage_func(char *iname, DataType *, ParmList *l);

public :
  ELASTIC() {
     module = 0;
     elastic_path = "elastic";
  }
  void parse_args(int, char *argv[]);
  void parse();
  void create_function(char *, char *, DataType *, ParmList *);
  void link_variable(char *, char *, DataType *);
  void declare_const(char *, char *, DataType *, char *);
  void initialize();
  void headers(void);
  void close(void);
  void set_module(char *, char **);
  void set_init(char *);
  void create_command(char *, char *);
};




