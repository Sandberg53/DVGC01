//Daneil Sandberg
/**********************************************************************/
/* lab 1 DVG C01 - Operator Table OBJECT                              */
/**********************************************************************/

/**********************************************************************/
/* Include files                                                      */
/**********************************************************************/
#include <stdio.h>
#include <ctype.h>
#include <string.h>

/**********************************************************************/
/* Other OBJECT's METHODS (IMPORTED)                                  */
/**********************************************************************/
#include "keytoktab.h"

/**********************************************************************/
/* OBJECT ATTRIBUTES FOR THIS OBJECT (C MODULE)                       */
/**********************************************************************/
#define NENTS 4

static int optab[][NENTS] = {
   {'+', integer, integer, integer},
   {'+', real,    real,    real},
   {'+', integer, real,    real},
   {'+', real,    integer, real},
   {'*', integer, integer, integer},
   {'*', real,    real,    real},
   {'*', integer, real,    real},
   {'*', real,    integer, real},
   {'$', undef,   undef,   undef}
   };

/**********************************************************************/
/*  PRIVATE METHODS for this OBJECT  (using "static" in C)            */
/**********************************************************************/

/**********************************************************************/
/*  PUBLIC METHODS for this OBJECT  (EXPORTED)                        */
/**********************************************************************/
/**********************************************************************/
/* display the op tab                                                 */
/**********************************************************************/
void p_optab()
{
    printf("---Printing Optable---\n");
    printf("%10s\t%10s\t%10s\t%10s\n\n", "Operator", "Arg1", "Arg2", "Result");
    int i = 0;
    do {
        printf("%10s\t%10s\t%10s\t%10s\n", tok2lex(optab[i][0]), tok2lex(optab[i][1]), tok2lex(optab[i][2]), tok2lex(optab[i][3]));
        i++;
    }while(optab[i][1] != undef);
}

/**********************************************************************/
/* return the type of a binary expression op arg1 arg2                */
/**********************************************************************/
int get_otype(int op, int arg1, int arg2)
{
    for(int i= 0; optab[i][1] != undef; i++){
        if(optab[i][0] == op &&
           optab[i][1] == arg1 &&
           optab[i][2] == arg2)
        {
            return optab[i][3]; 
        }
    }
    return undef;
}

/**********************************************************************/
/* End of code                                                        */
/**********************************************************************/
