#ifndef __LEXER_H
#define __LEXER_H

/* DO NOT MODIFY THIS FILE */

#include <stdio.h>

typedef enum {
  LPAREN_TOK,
  RPAREN_TOK,
  DEFINE_TOK,
  LAMBDA_TOK,
  NUM_TOK,
  ID_TOK,
  IF_TOK,
  EOF_TOK
} token_ty_t;

typedef struct _token_t {
  token_ty_t type;
  union {
    char *s;
    int i;
  } val;
} token_t;

token_t *get_token(FILE *in);
void print_token(FILE *out, token_t *t);

#endif
