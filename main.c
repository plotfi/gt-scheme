
#include "ast.h"
#include "lexer.h"
#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef _GRADING_

int main(int argc, char *argv[]) {

  program_t *t;
  char *filename;
  int closeErr;
  FILE *in;

  if (argc > 2) {
    printf("Invalid Input \n");
    return 1;
  }

  else if (argc < 2) {

    in = stdin;

    t = parse_AST(in);

    if (NULL == t)
      return 1;

  }

  else {
    filename = argv[1];
    in = fopen(filename, "r");

    t = parse_AST(in);

    if (NULL == t) {
      closeErr = fclose(in);
      return 1;
    }

    closeErr = fclose(in);
  }

  interpret(t);

  return 0;
}

#endif
