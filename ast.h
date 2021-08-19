#ifndef __AST_H
#define __AST_H

/* DO NOT MODIFY THIS FILE. */

/* ast.h

   This file contains the definition for the AST.  It also contains
   function prototypes for functions that help to build, interpret and
   print ASTs.  These functions are defined in semant.o  */

/* exp_ty_t: An exhaustive list of all the possible things a node
   could be. */
typedef enum {
  APPLY_N,  /* (Ef E1 ... En)            */
  NIL_N,    /* ()                        */
  DEFINE_N, /* (define (Vf V1 ... Vn) E) */
  LAMBDA_N, /* (lambda (V1 ... Vn) E)    */
  IF_N,     /* (if Econd Etrue Efalse)   */
  REF_N,    /* V                         */
  NUM_N     /* ...,-2,-1,0,1,2,...       */
} exp_ty_t;

/* We'll have to predeclare some structures since they're
   recursive. */
struct _exp_t;
struct _explist_t;
struct _idlist_t;
struct _program_t;

/* Now, we'll give the actual definitions. */

/* idlist_t: A linked list of identifiers. */
typedef struct _idlist_t {
  char *id;
  struct _idlist_t *next; /* if NULL, we're at the end. */
} idlist_t;

/* exp_t: A structure for holding a syntactic Scheme expression. */
typedef struct _exp_t {
  exp_ty_t exp_type; /*  exp_type determines the type of this exp. */

  /* Since an expression can be any of several things, we'll union
     them all together. */
  union {
    struct {
      idlist_t *args;      /* arguments to this lambda */
      struct _exp_t *body; /* the body of this lambda */
    } lambda;
    struct {
      struct _exp_t *f;        /* the function we're applying */
      struct _explist_t *args; /* the arguments to the function */
    } apply;
    struct {
      char *id; /* the name of this id */
    } ref;
    struct {
      int z; /* a simple integer value */
    } num;
    struct {
      struct _exp_t *cond; /* condition */
      struct _exp_t *cons; /* consequence */
      struct _exp_t *alt;  /* alternate */
    } ife;
    struct {
      char *name;          /* the name of this function */
      idlist_t *args;      /* the arguments to this function */
      struct _exp_t *body; /* the body of this function */
    } define;
  } u;
} exp_t;

/* explist_t: a struct for a list of expressions. */
typedef struct _explist_t {
  exp_t *exp;
  struct _explist_t *next; /* if NULL, we're at the end. */
} explist_t;

/* program_t: a wrapper for the entire program. */
typedef struct _program_t {
  explist_t *exps;
} program_t;

/* Functions in semant.o */

/* new_program: returns a newly allocated program_t with its list of
   expressions set to the argument. */
program_t *new_program(explist_t *el);

/* new_idlist: returns a newly allocated idlist_t with the first node set
   to id, and the rest set to next. */
idlist_t *new_idlist(char *id, idlist_t *next);

/* new_explist: returns a newly allocated explist_t with the first node
   set to e, and the rest set to next.  */
explist_t *new_explist(exp_t *e, explist_t *next);

/* new_exp: returns a new exp node with the type field set to t; the
   union fields need to be initialized. */
exp_t *new_exp(exp_ty_t t);

/* new_exp_X: returns a new exp node of type X with the fields
   properly set; after this, union fields do not need to be
   initialized. */

/* HINT: If you're having a hard time figuring out how the exp_t
   structure works, try playing with the constructors below and
   inspecting the output.

   Also, look at the structs inside the union and look at the
   arguments these constructors take.  Notice any similarities? */

exp_t *new_exp_NIL();
exp_t *new_exp_NUM(int n);
exp_t *new_exp_REF(char *id);
exp_t *new_exp_DEFINE(char *name, idlist_t *args, exp_t *body);
exp_t *new_exp_LAMBDA(idlist_t *args, exp_t *body);
exp_t *new_exp_IF(exp_t *cond, exp_t *cons, exp_t *alt);
exp_t *new_exp_APPLY(exp_t *f, explist_t *args);

/* print_program: given an AST, print out the concrete syntax. */
void print_program(program_t *p);

/* interpret: given an AST, interpret the program and print the result. */
void interpret(program_t *p);

#endif
