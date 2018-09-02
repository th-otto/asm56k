/* OPT v3.19: options parsing tool */
/*
 * This SOFTWARE has been authored by an employee of the University of
 * California, operator of the Los Alamos National Laboratory under
 * Contract No. W-7405-ENG-36 with the U.S. Department of Energy. The
 * U.S. Government has rights to use, reproduce, and distribute this
 * SOFTWARE. Neither the Government nor the University makes any
 * warranty, express or implied, or assumes any liability or
 * responsibility for the use of this SOFTWARE. If SOFTWARE is modified
 * to produce derivative works, such modified SOFTWARE should be clearly
 * marked, so as not to confuse it with the version available from LANL.
 * 
 * Additionally, this program is free software; you can distribute it
 * and/or modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or any later version.  Accordingly, this program is
 * distributed in the hope that it will be useful, but WITHOUT A
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * for more details (http://www.gnu.org/copyleft/gpl.txt).
 * 
 * 
 * The OPT package is available from
 * http://nis-www.lanl.gov/~jt/Software
 */
/******************************************************************

   This SOFTWARE has been authored by an employee of the University of
   California, operator of the Los Alamos National Laboratory under
   Contract No. W-7405-ENG-36 with the U.S. Department of Energy. The
   U.S. Government has rights to use, reproduce, and distribute this
   SOFTWARE. Neither the Government nor the University makes any
   warranty, express or implied, or assumes any liability or
   responsibility for the use of this SOFTWARE. If SOFTWARE is
   modified to produce derivative works, such modified SOFTWARE should
   be clearly marked, so as not to confuse it with the version
   available from LANL.

   Additionally, this program is free software; you can distribute it
   and/or modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2 of
   the License, or any later version.  Accordingly, this program is
   distributed in the hope that it will be useful, but WITHOUT A
   WARRANTY; without even the implied warranty of MERCHANTABILITY or
   FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
   License for more details (http://www.gnu.org/copyleft/gpl.txt).

   You can find the latest version of OPT on the web
   http://nis-www.lanl.gov/~jt/Software
   Or you can email the author: jt@lanl.gov

*********************************************************************/

/* $Id: opt_sf.h,v 3.8 2001/09/09 05:16:38 jt Exp $ */
#define SINGLEFILE 1

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

/** You may want to edit the #define's below **/

#ifndef HAVE_STRDUP
#define HAVE_STRDUP 1
#endif

#ifndef HAVE_UNISTD_H
#define HAVE_UNISTD_H 0
#endif

#ifndef HAVE_LONGJMP
#define HAVE_LONGJMP 0
#endif

#ifndef HAVE_SIGNAL
#define HAVE_SIGNAL 0
#endif

#ifndef RETSIGTYPE
#define RETSIGTYPE void
#endif


#ifdef HAVE_READLINE
#define READLINE_LIBRARY
#include <readline/readline.h>
#include <readline/history.h>
#endif

/** end opt_sf.h **/





