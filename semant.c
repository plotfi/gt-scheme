#include "semant.h"
#include "ast.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_idlist(idlist_t *idl);
void print_explist(explist_t *el);

void print_idlist(idlist_t *idl) {
  if (idl) {
    if (idl->id)
      printf("%s", idl->id);
    else
      printf("<NULL:id>");

    if (idl->next)
      putchar(' ');
    print_idlist(idl->next);
  }
}

void print_exp(exp_t *e) {
  if (e) {
    switch (e->exp_type) {
    case LAMBDA_N:
      printf("(lambda (");
      print_idlist(e->u.lambda.args);
      printf(") ");
      print_exp(e->u.lambda.body);
      printf(")");
      break;

    case APPLY_N:
      printf("(");
      print_exp(e->u.apply.f);
      if (e->u.apply.args)
        printf(" ");
      print_explist(e->u.apply.args);
      printf(")");
      break;

    case IF_N:
      printf("(if ");
      print_exp(e->u.ife.cond);
      printf(" ");
      print_exp(e->u.ife.cons);
      printf(" ");
      print_exp(e->u.ife.alt);
      printf(")");
      break;

    case REF_N:
      printf("%s", e->u.ref.id);
      break;

    case NIL_N:
      printf("()");
      break;

    case NUM_N:
      printf("%i", e->u.num.z);
      break;

    case DEFINE_N:
      printf("(define (");

      if (e->u.define.name)
        printf("%s", e->u.define.name);
      else
        printf("<NULL:id>");

      if (e->u.define.args)
        putchar(' ');
      print_idlist(e->u.define.args);
      printf(") ");
      print_exp(e->u.define.body);
      printf(")");
      break;

    default:
      printf("<UNKNOWN:exp>");
      break;
    }
  } else
    printf("<NULL:exp>");
}

void print_explist(explist_t *el) {
  if (el) {
    print_exp(el->exp);
    if (el->next)
      putchar(' ');
    print_explist(el->next);
  }
}

void print_program(program_t *p) {
  explist_t *el = p->exps;
  while (el) {
    print_exp(el->exp);
    putchar('\n');
    el = el->next;
  }
  printf("\n");
}

program_t *new_program(explist_t *el) {
  program_t *p = malloc(sizeof(program_t));
  p->exps = el;
  return p;
}

explist_t *new_explist(exp_t *e, explist_t *el) {
  explist_t *nel = malloc(sizeof(explist_t));
  nel->exp = e;
  nel->next = el;
  return nel;
}

exp_t *new_exp(exp_ty_t t) {
  exp_t *e = malloc(sizeof(exp_t));
  e->exp_type = t;
  return e;
}

exp_t *new_exp_NIL() {
  exp_t *e = new_exp(NIL_N);
  return e;
}

exp_t *new_exp_NUM(int n) {
  exp_t *e = new_exp(NUM_N);
  e->u.num.z = n;
  return e;
}

exp_t *new_exp_LAMBDA(idlist_t *args, exp_t *body) {
  exp_t *e = new_exp(LAMBDA_N);
  e->u.lambda.args = args;
  e->u.lambda.body = body;
  return e;
}

exp_t *new_exp_APPLY(exp_t *f, explist_t *args) {
  exp_t *e = new_exp(APPLY_N);
  e->u.apply.f = f;
  e->u.apply.args = args;
  return e;
}

exp_t *new_exp_REF(char *id) {
  exp_t *e = new_exp(REF_N);
  e->u.ref.id = id;
  return e;
}

exp_t *new_exp_IF(exp_t *cond, exp_t *cons, exp_t *alt) {
  exp_t *e = new_exp(IF_N);
  e->u.ife.cond = cond;
  e->u.ife.cons = cons;
  e->u.ife.alt = alt;
  return e;
}

exp_t *new_exp_DEFINE(char *name, idlist_t *args, exp_t *body) {
  exp_t *e = new_exp(DEFINE_N);
  e->u.define.name = name;
  e->u.define.args = args;
  e->u.define.body = body;
  return e;
}

idlist_t *new_idlist(char *id, idlist_t *idl) {
  idlist_t *nidl = malloc(sizeof(idlist_t));
  nidl->id = id;
  nidl->next = idl;
  return nidl;
}

val_t *lookup(char *id, env_t *env) {
  while (env) {
    if (strcmp(id, env->id) == 0)
      return env->v;
    else
      env = env->next;
  }
  return NULL;
}

env_t *new_env(char *id, val_t *v, env_t *next) {
  env_t *env = malloc(sizeof(env_t));
  env->id = id;
  env->v = v;
  env->next = next;
  return env;
}

void add_to_global(env_t *glob_env, char *id, val_t *v) {
  if (glob_env) {
    while (glob_env->next)
      glob_env = glob_env->next;

    glob_env->next = new_env(id, v, NULL);
  } else
    printf("error: cannot add to NULL global env\n");
}

val_t *num(int z) {
  val_t *v = malloc(sizeof(val_t));

  v->val_type = Z;
  v->u.num.z = z;

  return v;
}

vallist_t *new_vallist(val_t *v, vallist_t *vl) {
  vallist_t *nvl = malloc(sizeof(vallist_t));
  nvl->v = v;
  nvl->next = vl;
  return nvl;
}

vallist_t *exps2vals(explist_t *el, env_t *env) {
  if (el) {
    return new_vallist(d(el->exp, env), exps2vals(el->next, env));
  } else
    return NULL;
}

clo_t *new_clo(exp_t *lambda, env_t *env) {
  clo_t *nc = malloc(sizeof(clo_t));
  nc->lambda = lambda;
  nc->e = env;
  return nc;
}

env_t *aug_env(env_t *env, idlist_t *args, vallist_t *vals) {
  while (args) {
    env = new_env(args->id, vals->v, env);
    args = args->next;
    vals = vals->next;
  }

  return env;
}

val_t *new_val(val_ty_t t) {
  val_t *nv = malloc(sizeof(val_t));
  nv->val_type = t;
  return nv;
}

proc_t *new_proc(proc_ty_t t) {
  proc_t *np = malloc(sizeof(proc_t));
  np->proc_type = t;
  return np;
}

val_t *new_nil() {
  val_t *nv = malloc(sizeof(val_t));
  nv->val_type = NIL;
  return nv;
}

val_t *NIL_VAL;

val_t *d(exp_t *e, env_t *env) {
  (void)e;
  (void)env;
  switch (e->exp_type) {
  case REF_N: {
    val_t *v = lookup(e->u.ref.id, env);
    if (v)
      return v;
    else {
      printf("error: variable '%s' not bound in this context\n", e->u.ref.id);
      exit(-1);
    }
  } break;

  case NUM_N:
    return num(e->u.num.z);
    break;

  case NIL_N:
    return NIL_VAL;
    break;

  case IF_N: {
    val_t *cond = d(e->u.ife.cond, env);
    if (cond->val_type != NIL)
      return d(e->u.ife.cons, env);
    else
      return d(e->u.ife.alt, env);
  } break;

  case DEFINE_N: {
    val_t *v = new_val(PROC);
    exp_t *lambda = new_exp(LAMBDA_N);
    v->u.proc.p = new_proc(CLO);
    lambda->u.lambda.args = e->u.define.args;
    lambda->u.lambda.body = e->u.define.body;
    v->u.proc.p->u.clo.c = new_clo(lambda, env);
    add_to_global(env, e->u.define.name, v);
    return v;
  } break;

  case LAMBDA_N: {
    val_t *v = new_val(PROC);
    v->u.proc.p = new_proc(CLO);
    v->u.proc.p->u.clo.c = new_clo(e, env);
    return v;
  } break;

  case APPLY_N: {
    val_t *f = d(e->u.apply.f, env);
    if (f->val_type == PROC) {
      switch (f->u.proc.p->proc_type) {
      case PRIMOP:
        return f->u.proc.p->u.primop.f(exps2vals(e->u.apply.args, env));
        break;

      case CLO: {
        clo_t *c = f->u.proc.p->u.clo.c;
        return d(c->lambda->u.lambda.body,
                 aug_env(c->e, c->lambda->u.lambda.args,
                         exps2vals(e->u.apply.args, env)));
      } break;

      default:
        printf("unknown proc type");
        break;
      }
    } else {
      printf("error: attempting to apply non-procedure\n");
      exit(-1);
    }
  } break;

  default:
    printf("error: evaluation not supported\n");
    exit(-1);
    break;
  }
  return NULL;
}

void print_env(env_t *e) {
  while (e) {
    if (strcmp(e->id, "_global_") == 0)
      break;
    printf("[%s->", e->id);
    print_val(e->v);
    printf("]");
    e = e->next;
  }
}

void print_proc(proc_t *p) {
  switch (p->proc_type) {
  case PRIMOP:
    printf("[primop]");
    break;

  case CLO:
    printf("[closure:");
    print_exp(p->u.clo.c->lambda);
    printf(":");
    print_env(p->u.clo.c->e);
    printf("]");
    break;

  default:
    printf("<proc>");
    break;
  }
}

void print_val(val_t *v) {
  switch (v->val_type) {
  case PROC:
    print_proc(v->u.proc.p);
    break;

  case Z:
    printf("%i", v->u.num.z);
    break;

  case NIL:
    printf("()");
    break;

  default:
    printf("<val>");
    break;
  }
}

val_t *F_PRIMOP_PLUS(vallist_t *vl) {
  return num(vl->v->u.num.z + vl->next->v->u.num.z);
}

val_t *F_PRIMOP_TIMES(vallist_t *vl) {
  return num(vl->v->u.num.z * vl->next->v->u.num.z);
}

val_t *F_PRIMOP_MINUS(vallist_t *vl) {
  return num(vl->v->u.num.z - vl->next->v->u.num.z);
}

val_t *F_PRIMOP_DIVIDE(vallist_t *vl) {
  return num(vl->v->u.num.z / vl->next->v->u.num.z);
}

val_t *F_PRIMOP_EQNP(vallist_t *vl) {
  if (vl->v->u.num.z == vl->next->v->u.num.z)
    return num(1);
  else
    return NIL_VAL;
}

void interpret(program_t *p) {
  explist_t *el = p->exps;
  env_t global_env;

  proc_t PROC_PLUS;
  val_t PRIMOP_PLUS;

  proc_t PROC_TIMES;
  val_t PRIMOP_TIMES;

  proc_t PROC_MINUS;
  val_t PRIMOP_MINUS;

  proc_t PROC_DIVIDE;
  val_t PRIMOP_DIVIDE;

  proc_t PROC_EQNP;
  val_t PRIMOP_EQNP;

  NIL_VAL = new_nil();

  global_env.id = "_global_";
  global_env.v = NULL;
  global_env.next = NULL;

  PROC_PLUS.u.primop.op = PLUS;
  PROC_PLUS.u.primop.f = F_PRIMOP_PLUS;
  PROC_PLUS.proc_type = PRIMOP;
  PRIMOP_PLUS.u.proc.p = &PROC_PLUS;
  PRIMOP_PLUS.val_type = PROC;

  PROC_TIMES.u.primop.op = TIMES;
  PROC_TIMES.u.primop.f = F_PRIMOP_TIMES;
  PROC_TIMES.proc_type = PRIMOP;
  PRIMOP_TIMES.u.proc.p = &PROC_TIMES;
  PRIMOP_TIMES.val_type = PROC;

  PROC_MINUS.u.primop.op = MINUS;
  PROC_MINUS.u.primop.f = F_PRIMOP_MINUS;
  PROC_MINUS.proc_type = PRIMOP;
  PRIMOP_MINUS.u.proc.p = &PROC_MINUS;
  PRIMOP_MINUS.val_type = PROC;

  PROC_DIVIDE.u.primop.op = DIVIDE;
  PROC_DIVIDE.u.primop.f = F_PRIMOP_DIVIDE;
  PROC_DIVIDE.proc_type = PRIMOP;
  PRIMOP_DIVIDE.u.proc.p = &PROC_DIVIDE;
  PRIMOP_DIVIDE.val_type = PROC;

  PROC_EQNP.u.primop.op = EQNP;
  PROC_EQNP.u.primop.f = F_PRIMOP_EQNP;
  PROC_EQNP.proc_type = PRIMOP;
  PRIMOP_EQNP.u.proc.p = &PROC_EQNP;
  PRIMOP_EQNP.val_type = PROC;

  add_to_global(&global_env, "+", &PRIMOP_PLUS);
  add_to_global(&global_env, "*", &PRIMOP_TIMES);
  add_to_global(&global_env, "-", &PRIMOP_MINUS);
  add_to_global(&global_env, "/", &PRIMOP_DIVIDE);
  add_to_global(&global_env, "eqn?", &PRIMOP_EQNP);

  while (el) {
    print_val(d(el->exp, &global_env));
    printf("\n");
    el = el->next;
  }
}
