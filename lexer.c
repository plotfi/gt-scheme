#include "lexer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int isNum(char c);
token_t *helpHelp(char c, int idSize, char *id, FILE *in);
token_t *lambdaProc(char x, FILE *in);
token_t *defineProc(char x, FILE *in);
token_t *ifProc(char x, FILE *in);
token_t *idProc(char x, FILE *in);
void semiColonEat(char x, FILE *in);

static int idSize;
static int isIDWORLD;
static int ungetErr;

#ifdef lex

int main() {
  token_t *tok;

  do {
    tok = get_token(stdin);

    print_token(stdout, tok);
    printf(" ");

  } while (tok->type != EOF_TOK);

  return 0;
}

#endif

void print_token(FILE *out, token_t *t) {

  char *toks[] = {"LPAREN", "RPAREN", "DEFINE", "LAMBDA",
                  "NUM",    "ID",     "IF",     "EOF"};

  fprintf(out, "%s", toks[t->type]);

  if (4 == t->type)
    fprintf(out, "(%d) ", t->val.i);

  else if (5 == t->type)
    fprintf(out, "(%s) ", t->val.s);
}

token_t *get_token(FILE *in) {
  char c = 0;
  token_t *retVal;

  while (EOF != c) {
    isIDWORLD = 0;

    c = fgetc(in);

    if ('(' == c) {
      retVal = malloc(sizeof(*retVal));
      retVal->type = LPAREN_TOK;
      return retVal;
    }

    else if (')' == c) {
      retVal = malloc(sizeof(*retVal));
      retVal->type = RPAREN_TOK;
      return retVal;
    }

    else if ('d' == c) {
      isIDWORLD = 1;

      return defineProc(c, in);
    }

    else if ('l' == c) {
      isIDWORLD = 1;

      return lambdaProc(c, in);
    }

    else if ('i' == c) {
      isIDWORLD = 1;

      return ifProc(c, in);
    }

    else if (EOF == c) {
      retVal = malloc(sizeof(*retVal));
      retVal->type = EOF_TOK;
      return retVal;
    }

    else if (';' == c) {
      semiColonEat(c, in);

    }

    else if (' ' == c || '\n' == c || '\t' == c || 13 == c || '\r' == c) {
      continue;
    }

    else {
      return idProc(c, in);
    }
  }

  return NULL;
}

void semiColonEat(char x, FILE *in) {
  char c = x;

  while ('\n' != c && EOF != c) {
    c = fgetc(in);
  }

  ungetErr = ungetc(c, stdin);
}

int isNum(char c) {
  if ('0' <= c && c <= '9')
    return 1;

  else
    return 0;
}

token_t *helpHelp(char c, int size, char *id, FILE *in) {
  token_t *retVal;
  char *temp;

  retVal = idProc(c, in);
  temp = realloc(id, (sizeof(*id) * size) + (sizeof(*id) * idSize) - 1);

  if (NULL == temp) {
    free(id);
    return NULL;
  } else
    id = temp;

  retVal->type = ID_TOK;
  (retVal->val).s = strcat(id, (retVal->val).s);

  return retVal;
}

token_t *ifProc(char x, FILE *in) {
  token_t *retVal;
  char *id = malloc(sizeof(*id) * 3), c = x;

  if (NULL == id)
    return NULL;

  id[0] = '\0';

  if ('i' == c) {
    id[0] = c;
    id[1] = '\0';
    c = fgetc(in);
    if ('f' == c) {
      id[1] = c;
      id[2] = '\0';
      c = fgetc(in);

      if (' ' == c || ';' == c || '(' == c || ')' == c || EOF == c ||
          '\n' == c || '\t' == c || 13 == c || '\r' == c)

      {
        ungetErr = ungetc(c, in);
        retVal = malloc(sizeof(*retVal));
        retVal->type = IF_TOK;

        return retVal;
      } else
        return helpHelp(c, 3, id, in);
    } else
      return helpHelp(c, 2, id, in);
  } else
    return NULL;
}

token_t *defineProc(char x, FILE *in) {
  token_t *retVal;
  char *id = malloc(sizeof(*id) * 7), c = x;

  if (NULL == id)
    return NULL;

  id[0] = '\0';

  if ('d' == c) /* define*/
  {
    id[0] = c;
    id[1] = '\0';
    c = fgetc(in);

    if ('e' == c) {
      id[1] = c;
      id[2] = '\0';
      c = fgetc(in);

      if ('f' == c) {
        id[2] = c;
        id[3] = '\0';
        c = fgetc(in);

        if ('i' == c) {
          id[3] = c;
          id[4] = '\0';
          c = fgetc(in);

          if ('n' == c) {
            id[4] = c;
            id[5] = '\0';
            c = fgetc(in);

            if ('e' == c) {
              id[5] = c;
              id[6] = '\0';
              c = fgetc(in);

              if (' ' == c || ';' == c || '(' == c || ')' == c || EOF == c ||
                  '\n' == c || '\t' == c || 13 == c || '\r' == c) {
                ungetErr = ungetc(c, in);
                retVal = malloc(sizeof(*retVal));
                retVal->type = DEFINE_TOK;

                return retVal;
              } else
                return helpHelp(c, 7, id, in);
            } else
              return helpHelp(c, 6, id, in);
          }

          else
            return helpHelp(c, 5, id, in);
        } else
          return helpHelp(c, 4, id, in);
      } else
        return helpHelp(c, 3, id, in);
    } else
      return helpHelp(c, 2, id, in);
  } else
    return NULL;
}

token_t *lambdaProc(char x, FILE *in) {
  token_t *retVal;
  char *id = malloc(sizeof(*id) * 7), c = x;

  if (NULL == id)
    return NULL;

  id[0] = '\0';

  if ('l' == c) /* define*/
  {
    id[0] = c;
    id[1] = '\0';
    c = fgetc(in);

    if ('a' == c) {
      id[1] = c;
      id[2] = '\0';
      c = fgetc(in);

      if ('m' == c) {
        id[2] = c;
        id[3] = '\0';
        c = fgetc(in);

        if ('b' == c) {
          id[3] = c;
          id[4] = '\0';
          c = fgetc(in);

          if ('d' == c) {
            id[4] = c;
            id[5] = '\0';
            c = fgetc(in);

            if ('a' == c) {
              id[5] = c;
              id[6] = '\0';
              c = fgetc(in);

              if (' ' == c || ';' == c || '(' == c || ')' == c || EOF == c ||
                  '\n' == c || '\t' == c || 13 == c || '\r' == c) {
                ungetErr = ungetc(c, in);
                retVal = malloc(sizeof(*retVal));
                retVal->type = LAMBDA_TOK;

                return retVal;
              } else
                return helpHelp(c, 7, id, in);
            } else
              return helpHelp(c, 6, id, in);
          } else
            return helpHelp(c, 5, id, in);
        }

        else
          return helpHelp(c, 4, id, in);
      } else
        return helpHelp(c, 3, id, in);
    } else
      return helpHelp(c, 2, id, in);
  } else
    return NULL;
}

token_t *idProc(char x, FILE *in) {
  int numProcing = 1, neg = 0;
  char *temp, *idStore, c = x;
  int number_value = 0, isID = 0;
  token_t *retVal;

  idStore = malloc(sizeof(*idStore));

  if (NULL == idStore)
    return NULL;

  idStore[0] = 0;

  while (' ' != c && ';' != c && '(' != c && ')' != c && EOF != c &&
         '\n' != c && '\t' != c && 13 != c && '\r' != c) {
    if (1 != isID && 0 == isNum(c))
      isID = 1;

    if (1 == numProcing && '-' == c)
      isID = 0;

    idStore[numProcing - 1] = c;
    numProcing++;

    temp = realloc(idStore, numProcing);

    if (NULL == temp) {
      free(idStore);
      return NULL;
    } else
      idStore = temp;

    c = fgetc(in);
  }

  ungetErr = ungetc(c, in);

  if (1 != isID && 1 != isIDWORLD) {
    int i;

    if (2 == numProcing && '-' == idStore[0]) {
      idStore[numProcing - 1] = '\0';
      retVal = malloc(sizeof(*retVal));
      retVal->type = ID_TOK;
      (retVal->val).s = idStore;
      idSize = numProcing;
      return retVal;
    }

    for (i = 1; i < numProcing; i++) {
      if (1 == i && '-' == idStore[i - 1]) {
        neg = 1;
        i++;
      }

      number_value = (number_value * 10) + (idStore[i - 1] - '0');
    }

    /* free(idStore); leak to shut splint up*/

    retVal = malloc(sizeof(*retVal));
    retVal->type = NUM_TOK;
    (retVal->val).i = number_value;

    if (1 == neg)
      (retVal->val).i = -1 * (retVal->val).i;

    return retVal;
  }

  else {
    idStore[numProcing - 1] = '\0';
    retVal = malloc(sizeof(*retVal));
    retVal->type = ID_TOK;
    (retVal->val).s = idStore;
    idSize = numProcing;

    return retVal;
  }
}
