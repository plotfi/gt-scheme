#include "parser.h"
#include "lexer.h"
#include <stdio.h>

/*
 *
 * INCLUDE LL(1) GRAMMAR HERE.
 *
 * <program> ::= <explist>
 *
 * <explist> ::= <exp> <explist>
 *
 * <idlist>  ::= ID <idlist>
 *
 * <exp>     ::=     '(' <exp_tail>
 *                 | ID
 *                 | NUM
 *
 * <exp_tail> ::=    LAMBDA '(' <idlist> ')' <exp> ')'
 *                 | DEFINE '(' ID <idlist> ')' <exp> ')'
 *                 | IF <exp> <exp> <exp> ')'
 *                 | <exp> <explist> ')'
 *
 *
 *
 */

/***********prototypes and globals*******/

explist_t *expListProc();
idlist_t *idListProc();
exp_t *expProc();
void eat();

FILE *inGlob;
token_t *token;
static int EOFNULL;

/*************parse_AST()***************/

program_t *parse_AST(FILE *in) {
  /* WRITE CODE HERE */

  explist_t *expListRet;
  inGlob = in;
  expListRet = expListProc();
  if ((NULL == expListRet && EOF_TOK != token->type) || EOFNULL) /* was and*/
  {
    printf("error: parse error\n");

    EOFNULL = 0;

    return NULL;
  }

  else if (EOF_TOK != token->type) {
    printf("error: parse error\n");

    EOFNULL = 0;

    return NULL;

  }

  else {

    EOFNULL = 0;

    return new_program(expListRet);
  }

  /* NOTE: YOU WILL EVENTUALLY HAVE TO REMOVE THE FOLLOWING TWO
   LINES. */

  /*(void)in ;
  return NULL ;*/
}

/***********expListProc()***************/

explist_t *expListProc() {
  exp_t *expRet;
  eat();

  expRet = expProc();

  if (NULL == expRet)
    return NULL; /*EOF_TOK change*/
  else
    return new_explist(expRet, expListProc());
}

/***********expProc()*******************/

exp_t *expProc() {
  exp_t *retVal = NULL;

  if (NULL == token)
    return NULL; /*EOF_TOK change*/

  /* identifier part*/

  else if (ID_TOK == token->type) {
    return new_exp_REF(token->val.s);
  }

  /* number part*/

  else if (NUM_TOK == token->type) {
    return new_exp_NUM(token->val.i);
  }

  /*
   * this is a little check for RPARENS
   * where they shouldn't be. I am pretty
   * sure the else return NULL; at the end
   * takes care of this.
   *
   */

  else if (RPAREN_TOK == token->type) {
    return NULL;
  }

  /* This is the part that lead into the rest after is
   * checks for the openning LPAREN
   */

  else if (LPAREN_TOK == token->type) {
    eat();

    /* nil part */

    if (RPAREN_TOK == token->type) {
      retVal = new_exp_NIL();
    }

    /* define part begin*/

    else if (DEFINE_TOK == token->type) {
      char *name;
      idlist_t *args;
      exp_t *body;

      eat();

      if (LPAREN_TOK == token->type) {
        eat();

        if (ID_TOK == token->type) {
          name = token->val.s;
        } else
          return NULL;

        args = idListProc();

        if (RPAREN_TOK != token->type)

        {

          EOFNULL = 1;

          return NULL;

        }

        else
          eat();
      } else
        return NULL;

      body = expProc();
      if (NULL == body)
        return NULL;

      eat();

      retVal = new_exp_DEFINE(name, args, body);
    }

    /* define part end*/

    /* lambda part begin*/

    else if (LAMBDA_TOK == token->type) {
      idlist_t *args;
      exp_t *body;

      eat();

      if (LPAREN_TOK == token->type) {

        args = idListProc();

        if (RPAREN_TOK != token->type)

        {

          EOFNULL = 1;

          return NULL;

        }

        else
          eat();
      } else
        return NULL;

      body = expProc();
      if (NULL == body)
        return NULL;

      eat();
      retVal = new_exp_LAMBDA(args, body);
    }

    /* lambda part end*/

    /* if part begin*/

    else if (IF_TOK == token->type) {
      exp_t *cond, *cons, *alt;

      eat();

      cond = expProc();
      if (NULL == cond)
        return NULL;
      eat();

      cons = expProc();
      if (NULL == cons)
        return NULL;
      eat();

      alt = expProc();
      if (NULL == alt)
        return NULL;
      eat();

      retVal = new_exp_IF(cond, cons, alt);
    }

    /* if part end*/

    else if (EOF_TOK == token->type) {
      EOFNULL = 1;

      return NULL;

    }

    else {
      exp_t *f;
      explist_t *args;

      f = expProc();

      if (NULL == f)
        return NULL;

      retVal = f;

      args = expListProc();

      retVal = new_exp_APPLY(f, args);
    }

    /*eat()?*/

    if (RPAREN_TOK == token->type)
      return retVal;
    else {
      EOFNULL = 1;

      return NULL;
    }

  }

  else
    return NULL;
}

idlist_t *idListProc() {
  char *retChar;

  eat();

  if (ID_TOK == token->type) {
    retChar = token->val.s;

    return new_idlist(retChar, idListProc());

  }

  else
    return NULL;
}

/***********peak() and eat()*********/

void eat() { token = get_token(inGlob); }

/*******yeah I didn't see a need
 * for a peak function when my token
 * is a global*/
