//Daniel Sandberg
/**********************************************************************/
/* lab 1 DVG C01 - Driver OBJECT                                      */
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
/**********************************************************************/
/* type definitions                                                   */
/**********************************************************************/
typedef struct tab {
	char 	* text;
	int 	token;
}tab;

/**********************************************************************/
/* data objects (tables)                                              */
/**********************************************************************/
static tab tokentab[ ] = {
    {"id", 	            id},
    {"number",      number},
    {":=", 	        assign},
    {"undef", 	     undef},
    {"predef",      predef},
    {"tempty",      tempty},
    {"error",        error},
    {"type",           typ},
    {"$",              '$'},
    {"(",              '('},
    {")",              ')'},
    {"*",              '*'},
    {"+",              '+'},
    {",",              ','},
    {"-",              '-'},
    {".",              '.'},
    {"/",              '/'},
    {":",              ':'},
    {";",              ';'},
    {"=",              '='},
    {"TERROR", 	    nfound}
};


static tab keywordtab[ ] = {
	{"program", 	program},
	{"input", 	      input},
	{"output", 	     output},
	{"var", 	        var},
	{"begin", 	      begin},
	{"end", 	        end},
	{"boolean", 	boolean},
	{"integer", 	integer},
	{"real", 	       real},
	{"KERROR", 	     nfound}
};

/**********************************************************************/
/*  PUBLIC METHODS for this OBJECT  (EXPORTED)                        */
/**********************************************************************/
/**********************************************************************/
/* Display the tables                                                 */
/**********************************************************************/
void p_toktab()
{
    printf("-----Printing_KeyWordTable-----\n");
    for(int i=0; keywordtab[i].token != nfound; i++){ printf("Keyword: %-10s  |  Code: %d\n", keywordtab[i].text, keywordtab[i].token);}
    printf("-----Printing_TokenTable-----\n");
    for(int i=0; tokentab[i].token != nfound; i++){ printf("Token: %-10s  |  Code: %d\n", tokentab[i].text, tokentab[i].token);}
    
}

/**********************************************************************/
/* lex2tok - convert a lexeme to a token                              */
/**********************************************************************/
toktyp lex2tok(char * fplex)
{
    for(int i=0; tokentab[i].token != nfound; i++){   if(strcmp(fplex, tokentab[i].text) == 0) return tokentab[i].token;}
    for(int i=0; keywordtab[i].token != nfound; i++){   if(strcmp(fplex, keywordtab[i].text) == 0) return keywordtab[i].token;}
    return id;  
}

/**********************************************************************/
/* key2tok - convert a keyword to a token                             */
/**********************************************************************/
toktyp key2tok(char * fplex)
{
                                                                                //Go through keywordtab and check for matches with fplex
    for(int i = 0; keywordtab[i].token != nfound; i++){                               //Loop through table until key is nfound
        if(strcmp(fplex, keywordtab[i].text) == 0) return keywordtab[i].token;
    }
    return id;
}

/**********************************************************************/
/* tok2lex - convert a token to a lexeme                              */
/**********************************************************************/
char * tok2lex(toktyp ftok)
{
    for(int i=0; tokentab[i].token != nfound; i++){   if(ftok ==tokentab[i].token) return tokentab[i].text;}
    for(int i=0; keywordtab[i].token != nfound; i++){   if(ftok ==keywordtab[i].token) return keywordtab[i].text;}
    return "TERROR";  //If not found retrun errror
}

/**********************************************************************/
/* End of code                                                        */
/**********************************************************************/
