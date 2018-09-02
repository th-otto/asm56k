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
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>
#include "opt.h"
#include "ag.h"
#include "opt_p.h"

struct numexample { char *s; int isnum; } ex[] = {
  {"123.456e789",  OPT_NUM_VALID | OPT_NUM_FLOAT  }, /* Too big anyways! */
  {"5",            OPT_NUM_VALID },
  {"e5",           FALSE },
  {"1.0e+45",      OPT_NUM_VALID | OPT_NUM_FLOAT  },
  {"-88e2",        OPT_NUM_VALID | OPT_NUM_NEG },
  {"-1e-2",        OPT_NUM_VALID | OPT_NUM_NEG | OPT_NUM_FLOAT },
  {"123xyz",       FALSE },
  {"1e33.3",       FALSE },
  {"    2.5",      OPT_NUM_VALID | OPT_NUM_FLOAT },
  {"12,345",       FALSE },  /* even though you might think so... */
  {"13_456",       FALSE },  /* perl would call it a number */
  {"1.2e3",        OPT_NUM_VALID | OPT_NUM_FLOAT}, /* PERVERSE !
                                                    * really an INT, but
                                                    * we call it a FLOAT
                                                    */
};
int
main(int argc, char **argv)
{
  int incorrect=0;
  unsigned int i;
  for (i=0; i< sizeof(ex)/sizeof(struct numexample); ++i) {
    unsigned int ux;
    int ix;
    double x;
    
    printf("%-12s is%s a number?\t status=%d %s \n",ex[i].s,
           (opt_isvalidnumber(ex[i].s) ? "" : "n\'t"),
           opt_isvalidnumber(ex[i].s),
           (ex[i].isnum == opt_isvalidnumber(ex[i].s) ?
            "CORRECT" : (++incorrect, "WRONG!")));

    x = opt_atof(ex[i].s);
    printf("   f=%g\n",x);
    ix = opt_atoi(ex[i].s);
    printf("   i=%d\n",ix);
    ux = opt_atou(ex[i].s);
    printf("   u=%u\n",ux);
  }
  printf("=== t_num total incorrect: %d ===\n",incorrect);
  if (!incorrect) {
    printf("PASSED\n");
  }
  return incorrect;
}


