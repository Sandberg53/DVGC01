//Daniel Sandberg
/**********************************************************************/
/* lab 1 DVG C01 - Parser OBJECT                                      */
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
 #include "keytoktab.h"          /* when the keytoktab is added   */
 #include "lexer.h"              /* when the lexer     is added   */
 #include "symtab.h"             /* when the symtab    is added   */
 #include "optab.h"              /* when the optab     is added   */

/**********************************************************************/
/* OBJECT ATTRIBUTES FOR THIS OBJECT (C MODULE)                       */
/**********************************************************************/
#define DEBUG 0                                                                     // 1
static int  lookahead=0;
static int  is_parse_ok=1;

/**********************************************************************/
/* RAPID PROTOTYPING - simulate the token stream & lexer (get_token)  */
/**********************************************************************/
/* define tokens + keywords NB: remove this when keytoktab.h is added */
/**********************************************************************/
//enum tvalues { program = 257, id, input, output };
/**********************************************************************/
/* Simulate the token stream for a given program                      */
/**********************************************************************/
/*static int tokens[] = {program, id, '(', input, ',', output, ')', ';',
               '$' };*/

/**********************************************************************/
/*  Simulate the lexer -- get the next token from the buffer          */
/**********************************************************************/
/*static int pget_token()
{
    static int i=0;
    if (tokens[i] != '$') return tokens[i++]; else return '$';
}*/

/**********************************************************************/
/*  PRIVATE METHODS for this OBJECT  (using "static" in C)            */
/**********************************************************************/
static void in(char* s)
{
    if(DEBUG) printf("\n *** In  %s", s);
}
static void out(char* s)
{
    if(DEBUG) printf("\n *** Out %s", s);
}
/**********************************************************************/
/* The Parser functions                                               */
/**********************************************************************/
static void match(int t)
{
    if(DEBUG) printf("\n --------In match expected: %4s, found: %4s",
                    tok2lex(t), tok2lex(lookahead));
    if (lookahead == t) lookahead = get_token();
    else {
    is_parse_ok=0;
    printf("\n *** Unexpected Token: expected: %4s found: %4s (in match)",
              tok2lex(t), tok2lex(lookahead));
    }
}

/**********************************************************************/
/* The grammar functions                                              */
/**********************************************************************/
static void program_header()
{
    in("program_header");
    match(program); 
    if(lookahead == id)
    {
        addp_name(get_lexeme());
        match(id);
    }else
    {
        printf("\nSYNTAX: ID expected found %s", get_lexeme());
        addp_name("???");
        is_parse_ok = 0;
    }
  
    match('('); match(input);
    match(','); match(output);
    match(')'); match(';');

    out("program_header");
}
static void type()
{
    in("type");

    switch(lookahead){
        case integer:
            setv_type(integer);
            match(integer);
            break;

        case real:
            setv_type(real);
            match(real);
            break;

        case boolean:
            setv_type(boolean);
            match(boolean);
            break;
        
        default: 
            printf("\nSYNTAX: Type name expected found %s", get_lexeme());
            is_parse_ok = 0;
            setv_type(error);
            break;
    }

    out("type");

}

static void id_list()
{
    in("id_list");
    if(lookahead == id)
    {
        char * var_name = get_lexeme();
        if(!find_name(var_name)) addv_name(var_name);
        else
        {
            printf("\nSEMANTIC: ID already declared: %s", get_lexeme());
            is_parse_ok = 0;
        } 
    }
    match(id);

    if(lookahead == ',')
    {
        match(',');
        id_list();
    }
    

    out("id_list");
}

static void var_dec()
{
    in("var_dec");

    id_list();
    match(':');
    type();
    match(';');

    out("var_dec");

}

static void var_dec_list()
{
    in("var_dec_list");

    var_dec();
    if(lookahead == id)
    {
        var_dec_list();
    }

    out("var_dec_list");
}

static void var_post()
{
    in("var_post");

    match(var);
    var_dec_list();

    out("var_post");
}


//**************************' STAT_PART ***********************************
static toktyp stat_expr();

static toktyp stat_operand()
{
    in("stat_operand");

    toktyp varType = error;

    if(lookahead == id)
    {
        char * var_name;
        var_name = get_lexeme();
        
        if(find_name(var_name))
        {
            varType = get_ntype(var_name);
            match(id);
        } 
        else {
            printf("\nSEMANTIC: ID NOT declared: %s", get_lexeme());
            match(id);
            varType = undef;
        }
    } else if(lookahead == number) {
        match(number);
        varType = integer; 
    } else
    {   
        printf("\nSYNTAX: Operand expected");
        is_parse_ok = 0;
        varType = error;
    }

    out("stat_operand");
    return varType;
    
}

static toktyp stat_factor()
{
    in("stat_factor");

    toktyp type;
    if(lookahead == '(')
    {
        match('(');
        type = stat_expr();
        match(')');

    }else type = stat_operand();

    out("stat_factor");
    return type;
}


static toktyp stat_term()
{
    in("stat_term");

    toktyp leftSide = stat_factor();
    if(lookahead == '*')
    {
        match('*');
        leftSide = get_otype('*', leftSide, stat_term());
    }

    out("stat_term");
    return leftSide;
}


static toktyp stat_expr()
{
    in("stat_expr");

    toktyp leftSide = stat_term();
    if(lookahead == '+')
    {
        match('+');
        leftSide = get_otype('+', leftSide, stat_expr());
    }


    out("stat_expr");
    return leftSide;
}

static void assign_stat()
{
    in("assign_stat");

    char * leftLex = get_lexeme();
    toktyp leftSide = error;
    toktyp rightSide = error;

    if(lookahead == id)
    {
        if(find_name(leftLex)) leftSide = get_ntype(leftLex);
        else printf("\nSYNTAX: ID NOT declared: %s", leftLex);
        match(id);
    }else
    {
        printf("\nSYNTAX: ID expected found %s", get_lexeme());
        is_parse_ok = 0;
    }
    
    match(assign);

    rightSide = stat_expr();

    if(leftSide != rightSide)
    {
        printf("\nSEMANTIC: Assign types: %s := %s", tok2lex(leftSide), tok2lex(rightSide));
        is_parse_ok = 0;
    }

    out("assign_stat");
    
    
}

static void stat()
{
    in("stat");
    assign_stat();

    out("stat");
}

static void stat_list()
{
    in("stat_list");

    stat();
    if(lookahead == ';')
    {
        match(';');
        stat_list();
    }

    out("stat_list");
}

static void stat_part()
{
    in("stat_part");

    match(begin); 
    stat_list();
    match(end); 
    match('.'); 

    out("stat_part");
}
/**********************************************************************/
/*  PUBLIC METHODS for this OBJECT  (EXPORTED)                        */
/**********************************************************************/

int parser()
{
     in("parser");

lookahead = get_token();       

 if(lookahead != '$')
 {
     program_header();              
     var_post();                      
     stat_part();                     

     if(lookahead != '$')
     {
         printf("\nSYNTAX: Extra symbols after end of parse!\n");    
         while(lookahead != '$')
         {
             printf("%s ", get_lexeme());
             lookahead = get_token();
         }
     } 
 } else
 {
     printf("\nSYNTAX: Input file is empty");
     is_parse_ok = 0;
 }
 /*if(is_parse_ok) printf("\nPARSE SUCCESSFUL!");
 printf("\n____________________________________________________________");*/
 p_symtab();
 
 out("parser");
 return is_parse_ok;             
   
}

/**********************************************************************/
/* End of code                                                        */
/**********************************************************************/