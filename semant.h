
typedef enum { PROC, Z, NIL } val_ty_t;
typedef enum { PRIMOP, CLO } proc_ty_t;
typedef enum { PLUS, TIMES, MINUS, DIVIDE, EQNP } primop_ty_t;

struct _val_t *v;
struct _clo_t *c;

typedef struct _vallist_t {
  struct _val_t *v;
  struct _vallist_t *next;
} vallist_t;

typedef struct _proc_t {
  proc_ty_t proc_type;
  struct {
    struct {
      primop_ty_t op;
      struct _val_t *(*f)(vallist_t *vl);
    } primop;
    struct {
      struct _clo_t *c;
    } clo;
  } u;
} proc_t;

typedef struct _val_t {
  val_ty_t val_type;
  union {
    struct {
      proc_t *p;
    } proc;
    struct {
      int z;
    } num;
  } u;
} val_t;

typedef struct _env_t {
  char *id;
  val_t *v;
  struct _env_t *next;
} env_t;

/* Closure = Lambda + Binding Environment */
typedef struct _clo_t {
  exp_t *lambda;
  env_t *e;
} clo_t;

val_t *d(exp_t *e, env_t *env);

val_t *new_val(val_ty_t t);
proc_t *new_proc(proc_ty_t t);
env_t *new_env(char *id, val_t *v, env_t *next);

void add_to_global(env_t *glob_env, char *id, val_t *v);

void print_val(val_t *v);
