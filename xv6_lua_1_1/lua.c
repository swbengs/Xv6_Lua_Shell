/*
** lua.c
** Linguagem para Usuarios de Aplicacao
*/

//xv6 stuff
#include "types.h"
#include "user.h"

//hash.h start
//forward declares
struct Object;
struct node;
struct Hash;
int yyparse();


struct stat
{
    unsigned short st_mode;
    short          st_nlink;
    short          st_uid;
    short          st_gid;
};

typedef enum
{
    PUSHNIL,
    PUSH0, PUSH1, PUSH2,
    PUSHBYTE,
    PUSHWORD,
    PUSHFLOAT,
    PUSHSTRING,
    PUSHLOCAL0, PUSHLOCAL1, PUSHLOCAL2, PUSHLOCAL3, PUSHLOCAL4,
    PUSHLOCAL5, PUSHLOCAL6, PUSHLOCAL7, PUSHLOCAL8, PUSHLOCAL9,
    PUSHLOCAL,
    PUSHGLOBAL,
    PUSHINDEXED,
    PUSHMARK,
    PUSHOBJECT,
    STORELOCAL0, STORELOCAL1, STORELOCAL2, STORELOCAL3, STORELOCAL4,
    STORELOCAL5, STORELOCAL6, STORELOCAL7, STORELOCAL8, STORELOCAL9,
    STORELOCAL,
    STOREGLOBAL,
    STOREINDEXED0,
    STOREINDEXED,
    STORELIST0,
    STORELIST,
    STORERECORD,
    ADJUST,
    CREATEARRAY,
    EQOP,
    LTOP,
    LEOP,
    ADDOP,
    SUBOP,
    MULTOP,
    DIVOP,
    CONCOP,
    MINUSOP,
    NOTOP,
    ONTJMP,
    ONFJMP,
    JMP,
    UPJMP,
    IFFJMP,
    IFFUPJMP,
    POP,
    CALLFUNC,
    RETCODE,
    HALT,
    SETFUNCTION,
    SETLINE,
    RESET
} OpCode;

#ifndef real
#define real float
#endif
typedef unsigned char  Byte;
typedef unsigned short Word;
typedef void(*Cfunction) (void);
typedef int(*Input) (void);

//y.tab.h start
typedef union
{
    int   vInt;
    long  vLong;
    float vFloat;
    char *pChar;
    Word  vWord;
    Byte *pByte;
} YYSTYPE;
# define WRONGTOKEN 257
# define NIL 258
# define IF 259
# define THEN 260
# define ELSE 261
# define ELSEIF 262
# define WHILE 263
# define DO 264
# define REPEAT 265
# define UNTIL 266
# define END 267
# define RETURN 268
# define LOCAL 269
# define NUMBER 270
# define FUNCTION 271
# define STRING 272
# define NAME 273
# define DEBUG 274
# define AND 275
# define OR 276
# define NE 277
# define LE 278
# define GE 279
# define CONC 280
# define UNARY 281
# define NOT 282
//y.tab.h end

typedef enum
{
    T_MARK,
    T_NIL,
    T_NUMBER,
    T_STRING,
    T_ARRAY,
    T_FUNCTION,
    T_CFUNCTION,
    T_USERDATA
} Type;

typedef union
{
    Cfunction 	 f;
    real    	 n;
    char      	*s;
    Byte      	*b;
    struct Hash    *a;
    void           *u;
} Value;

typedef struct Object
{
    Type  tag;
    Value value;
} Object;

typedef struct node
{
    Object ref;
    Object val;
    struct node *next;
} Node;

typedef struct Hash
{
    char           mark;
    unsigned int   nhash;
    Node         **list;
} Hash;

typedef struct
{
    char   *name;
    Object  object;
} Symbol;

# define WRONGTOKEN 257
# define NIL 258
# define IF 259
# define THEN 260
# define ELSE 261
# define ELSEIF 262
# define WHILE 263
# define DO 264
# define REPEAT 265
# define UNTIL 266
# define END 267
# define RETURN 268
# define LOCAL 269
# define NUMBER 270
# define FUNCTION 271
# define STRING 272
# define NAME 273
# define DEBUG 274
# define AND 275
# define OR 276
# define NE 277
# define LE 278
# define GE 279
# define CONC 280
# define UNARY 281
# define NOT 282
#define yyclearin yychar = -1
#define yyerrok yyerrflag = 0
int yychar;
int yyerrflag;
#ifndef YYMAXDEPTH
#define YYMAXDEPTH 150
#endif
YYSTYPE yylval, yyval;
# define YYERRCODE 256

Hash    *lua_createarray(int nhash);
void     lua_hashmark(Hash *h);
void     lua_hashcollector(void);
Object 	*lua_hashdefine(Hash *t, Object *ref);
void     lua_next(void);
//hash.h end

//inout.h start
int  lua_openfile(char *fn);
void lua_closefile(void);
int  lua_openstring(char *s);
void lua_closestring(void);
int  lua_pushfunction(int file, int function);
void lua_popfunction(void);
void lua_reportbug(char *s);
//inout.h end

//lualib.h start
void iolib_open(void);
void strlib_open(void);
void mathlib_open(void);
//lualib.h end

//mathlib.h start
void mathlib_open(void);
//mathlib.h end

//opcode.h start
#ifndef STACKGAP
#define STACKGAP    128
#endif 

#define FIELDS_PER_FLUSH 40



typedef union
{
    struct { char c1; char c2; } m;
    Word w;
} CodeWord;

typedef union
{
    struct { char c1; char c2; char c3; char c4; } m;
    float f;
} CodeFloat;

/* Macros to access structure members */
#define tag(o)		((o)->tag)
#define nvalue(o)	((o)->value.n)
#define svalue(o)	((o)->value.s)
#define bvalue(o)	((o)->value.b)
#define avalue(o)	((o)->value.a)
#define fvalue(o)	((o)->value.f)
#define uvalue(o)	((o)->value.u)

/* Macros to access symbol table */
#define s_name(i)	(lua_table[i].name)
#define s_object(i)	(lua_table[i].object)
#define s_tag(i)	(tag(&s_object(i)))
#define s_nvalue(i)	(nvalue(&s_object(i)))
#define s_svalue(i)	(svalue(&s_object(i)))
#define s_bvalue(i)	(bvalue(&s_object(i)))
#define s_avalue(i)	(avalue(&s_object(i)))
#define s_fvalue(i)	(fvalue(&s_object(i)))
#define s_uvalue(i)	(uvalue(&s_object(i)))

#define get_word(code,pc)    {code.m.c1 = *pc++; code.m.c2 = *pc++;}
#define get_float(code,pc)   {code.m.c1 = *pc++; code.m.c2 = *pc++;\
                              code.m.c3 = *pc++; code.m.c4 = *pc++;}



/* Exported functions */
int     lua_execute(Byte *pc);
void    lua_markstack(void);
char   *lua_strdup(char *l);

void    lua_setinput(Input fn);	/* from "lex.c" module */
char   *lua_lasttext(void);		/* from "lex.c" module */
int     lua_parse(void); 		/* from "lua.stx" module */
void    lua_type(void);
void 	lua_obj2number(void);
void 	lua_print(void);
void 	lua_internaldofile(void);
void 	lua_internaldostring(void);
void    lua_travstack(void(*fn)(Object *));
//opcode.h end

//strlib.h start
void strlib_open(void);
//strlib.h end

//table.h start
Symbol *lua_table;
Word    lua_ntable;

char  **lua_constant;
Word    lua_nconstant;

char  **lua_string;
Word    lua_nstring;

Hash  **lua_array;
Word    lua_narray;

char   *lua_file[];
int     lua_nfile;

Word    lua_block;
Word    lua_nentity;



int   lua_findsymbol(char *s);
int   lua_findconstant(char *s);
void  lua_travsymbol(void(*fn)(Object *));
void  lua_markobject(Object *o);
void  lua_pack(void);
void  lua_stringcollector(void);
char *lua_createstring(char *s);
int   lua_addfile(char *fn);
int   lua_delfile(void);
char *lua_filename(void);
void  lua_nextvar(void);
//table.h end

//lua.h start
typedef void(*lua_CFunction) (void);
typedef struct Object *lua_Object;

#define lua_register(n,f)	(lua_pushcfunction(f), lua_storeglobal(n))


void           lua_errorfunction(void(*fn) (char *s));
void           lua_error(char *s);
int            lua_dofile(char *filename);
int            lua_dostring(char *string);
int            lua_call(char *functionname, int nparam);

lua_Object     lua_getparam(int number);
float          lua_getnumber(lua_Object object);
char          *lua_getstring(lua_Object object);
char 	      *lua_copystring(lua_Object object);
lua_CFunction  lua_getcfunction(lua_Object object);
void          *lua_getuserdata(lua_Object object);
lua_Object     lua_getfield(lua_Object object, char *field);
lua_Object     lua_getindexed(lua_Object object, float index);
lua_Object     lua_getglobal(char *name);

lua_Object     lua_pop(void);

int 	       lua_pushnil(void);
int            lua_pushnumber(float n);
int            lua_pushstring(char *s);
int            lua_pushcfunction(lua_CFunction fn);
int            lua_pushuserdata(void *u);
int            lua_pushobject(lua_Object object);

int            lua_storeglobal(char *name);
int            lua_storefield(lua_Object object, char *field);
int            lua_storeindexed(lua_Object object, float index);

int            lua_isnil(lua_Object object);
int            lua_isnumber(lua_Object object);
int            lua_isstring(lua_Object object);
int            lua_istable(lua_Object object);
int            lua_iscfunction(lua_Object object);
int            lua_isuserdata(lua_Object object);
//lua.h end

//headers above here
//----------------------------------------------------------------------------------------------------------------------------------
//c files below here

//hash.c start
#define streq(s1,s2)	(strcmp(s1,s2)==0)
#define strneq(s1,s2)	(strcmp(s1,s2)!=0)

#define new(s)		((s *)malloc(sizeof(s)))
#define newvector(n,s)	((s *)calloc(n,sizeof(s)))

#define nhash(t)	((t)->nhash)
#define nodelist(t)	((t)->list)
#define list(t,i)	((t)->list[i])
#define markarray(t)    ((t)->mark)
#define ref_tag(n)	(tag(&(n)->ref))
#define ref_nvalue(n)	(nvalue(&(n)->ref))
#define ref_svalue(n)	(svalue(&(n)->ref))

#ifndef ARRAYBLOCK
#define ARRAYBLOCK 50
#endif

typedef struct ArrayList
{
    Hash *array;
    struct ArrayList *next;
} ArrayList;

static ArrayList *listhead = NULL;

static int head(Hash *t, Object *ref)		/* hash function */
{
    if (tag(ref) == T_NUMBER) return (((int)nvalue(ref)) % nhash(t));
    else if (tag(ref) == T_STRING)
    {
        int h;
        char *name = svalue(ref);
        for (h = 0; *name != 0; name++)		/* interpret name as binary number */
        {
            h <<= 8;
            h += (unsigned char)*name;		/* avoid sign extension */
            h %= nhash(t);			/* make it a valid index */
        }
        return h;
    }
    else
    {
        lua_reportbug("unexpected type to index table");
        return -1;
    }
}

static Node *present(Hash *t, Object *ref, int h)
{
    Node *n = NULL;
    if (tag(ref) == T_NUMBER)
    {
        for (n = list(t, h); n != NULL; n = n->next)
            if (ref_tag(n) == T_NUMBER && nvalue(ref) == ref_nvalue(n)) break;
    }
    else if (tag(ref) == T_STRING)
    {
        for (n = list(t, h); n != NULL; n = n->next)
            if (ref_tag(n) == T_STRING && streq(svalue(ref), ref_svalue(n))) break;
    }
    if (n == NULL)				/* name not present */
        return NULL;
#if 0
    if (p != NULL)				/* name present but not first */
    {
        p->next = n->next;			/* move-to-front self-organization */
        n->next = list(t, h);
        list(t, h) = n;
    }
#endif
    return n;
}

static void freelist(Node *n)
{
    while (n)
    {
        Node *next = n->next;
        free(n);
        n = next;
    }
}

/*
** Create a new hash. Return the hash pointer or NULL on error.
*/
static Hash *hashcreate(unsigned int nhash)
{
    Hash *t = new (Hash);
    if (t == NULL)
    {
        lua_error("not enough memory");
        return NULL;
    }
    nhash(t) = nhash;
    markarray(t) = 0;
    nodelist(t) = newvector(nhash, Node*);
    if (nodelist(t) == NULL)
    {
        lua_error("not enough memory");
        return NULL;
    }
    return t;
}

/*
** Delete a hash
*/
static void hashdelete(Hash *h)
{
    int i;
    for (i = 0; i<nhash(h); i++)
        freelist(list(h, i));
    free(nodelist(h));
    free(h);
}


/*
** Mark a hash and check its elements
*/
void lua_hashmark(Hash *h)
{
    if (markarray(h) == 0)
    {
        int i;
        markarray(h) = 1;
        for (i = 0; i<nhash(h); i++)
        {
            Node *n;
            for (n = list(h, i); n != NULL; n = n->next)
            {
                lua_markobject(&n->ref);
                lua_markobject(&n->val);
            }
        }
    }
}

/*
** Garbage collection to arrays
** Delete all unmarked arrays.
*/
void lua_hashcollector(void)
{
    ArrayList *curr = listhead, *prev = NULL;
    while (curr != NULL)
    {
        ArrayList *next = curr->next;
        if (markarray(curr->array) != 1)
        {
            if (prev == NULL) listhead = next;
            else              prev->next = next;
            hashdelete(curr->array);
            free(curr);
        }
        else
        {
            markarray(curr->array) = 0;
            prev = curr;
        }
        curr = next;
    }
}


/*
** Create a new array
** This function insert the new array at array list. It also
** execute garbage collection if the number of array created
** exceed a pre-defined range.
*/
Hash *lua_createarray(int nhash)
{
    ArrayList *new = new(ArrayList);
    if (new == NULL)
    {
        lua_error("not enough memory");
        return NULL;
    }
    new->array = hashcreate(nhash);
    if (new->array == NULL)
    {
        lua_error("not enough memory");
        return NULL;
    }

    if (lua_nentity == lua_block)
        lua_pack();

    lua_nentity++;
    new->next = listhead;
    listhead = new;
    return new->array;
}


/*
** If the hash node is present, return its pointer, otherwise create a new
** node for the given reference and also return its pointer.
** On error, return NULL.
*/
Object *lua_hashdefine(Hash *t, Object *ref)
{
    int   h;
    Node *n;
    h = head(t, ref);
    if (h < 0) return NULL;

    n = present(t, ref, h);
    if (n == NULL)
    {
        n = new(Node);
        if (n == NULL)
        {
            lua_error("not enough memory");
            return NULL;
        }
        n->ref = *ref;
        tag(&n->val) = T_NIL;
        n->next = list(t, h);			/* link node to head of list */
        list(t, h) = n;
    }
    return (&n->val);
}


/*
** Internal function to manipulate arrays.
** Given an array object and a reference value, return the next element
** in the hash.
** This function pushs the element value and its reference to the stack.
*/
static void firstnode(Hash *a, int h)
{
    if (h < nhash(a))
    {
        int i;
        for (i = h; i<nhash(a); i++)
        {
            if (list(a, i) != NULL)
            {
                if (tag(&list(a, i)->val) != T_NIL)
                {
                    lua_pushobject(&list(a, i)->ref);
                    lua_pushobject(&list(a, i)->val);
                    return;
                }
                else
                {
                    Node *next = list(a, i)->next;
                    while (next != NULL && tag(&next->val) == T_NIL) next = next->next;
                    if (next != NULL)
                    {
                        lua_pushobject(&next->ref);
                        lua_pushobject(&next->val);
                        return;
                    }
                }
            }
        }
    }
    lua_pushnil();
    lua_pushnil();
}
void lua_next(void)
{
    Hash   *a;
    Object *o = lua_getparam(1);
    Object *r = lua_getparam(2);
    if (o == NULL || r == NULL)
    {
        lua_error("too few arguments to function `next'"); return;
    }
    if (lua_getparam(3) != NULL)
    {
        lua_error("too many arguments to function `next'"); return;
    }
    if (tag(o) != T_ARRAY)
    {
        lua_error("first argument of function `next' is not a table"); return;
    }
    a = avalue(o);
    if (tag(r) == T_NIL)
    {
        firstnode(a, 0);
        return;
    }
    else
    {
        int h = head(a, r);
        if (h >= 0)
        {
            Node *n = list(a, h);
            while (n)
            {
                if (memcmp(&n->ref, r, sizeof(Object)) == 0)
                {
                    if (n->next == NULL)
                    {
                        firstnode(a, h + 1);
                        return;
                    }
                    else if (tag(&n->next->val) != T_NIL)
                    {
                        lua_pushobject(&n->next->ref);
                        lua_pushobject(&n->next->val);
                        return;
                    }
                    else
                    {
                        Node *next = n->next->next;
                        while (next != NULL && tag(&next->val) == T_NIL) next = next->next;
                        if (next == NULL)
                        {
                            firstnode(a, h + 1);
                            return;
                        }
                        else
                        {
                            lua_pushobject(&next->ref);
                            lua_pushobject(&next->val);
                        }
                        return;
                    }
                }
                n = n->next;
            }
            if (n == NULL)
                lua_error("error in function 'next': reference not found");
        }
    }
}
//hash.c end

//inout.c start
/* Exported variables */
int lua_linenumber;
int lua_debug;
int lua_debugline;

/* Internal variables */
#ifndef MAXFUNCSTACK
#define MAXFUNCSTACK 32
#endif
static struct { int file; int function; } funcstack[MAXFUNCSTACK];
static int nfuncstack = 0;

static FILE *fp;
static char *st;
static void(*usererror) (char *s);

/*
** Function to set user function to handle errors.
*/
void lua_errorfunction(void(*fn) (char *s))
{
    usererror = fn;
}

/*
** Function to get the next character from the input file
*/
static int fileinput(void)
{
    int c = fgetc(fp);
    return (c == EOF ? 0 : c);
}

/*
** Function to get the next character from the input string
*/
static int stringinput(void)
{
    st++;
    return (*(st - 1));
}

/*
** Function to open a file to be input unit.
** Return 0 on success or 1 on error.
*/
int lua_openfile(char *fn)
{
    lua_linenumber = 1;
    lua_setinput(fileinput);
    fp = fopen(fn, "r");
    if (fp == NULL) return 1;
    if (lua_addfile(fn)) return 1;
    return 0;
}

/*
** Function to close an opened file
*/
void lua_closefile(void)
{
    if (fp != NULL)
    {
        lua_delfile();
        fclose(fp);
        fp = NULL;
    }
}

/*
** Function to open a string to be input unit
*/
int lua_openstring(char *s)
{
    lua_linenumber = 1;
    lua_setinput(stringinput);
    st = s;
    {
        char sn[64];
        sprintf(sn, "String: %10.10s...", s);
        if (lua_addfile(sn)) return 1;
    }
    return 0;
}

/*
** Function to close an opened string
*/
void lua_closestring(void)
{
    lua_delfile();
}

/*
** Call user function to handle error messages, if registred. Or report error
** using standard function (fprintf).
*/
void lua_error(char *s)
{
    if (usererror != NULL) usererror(s);
    else			    fprintf(stderr, "lua: %s\n", s);
}

/*
** Called to execute  SETFUNCTION opcode, this function pushs a function into
** function stack. Return 0 on success or 1 on error.
*/
int lua_pushfunction(int file, int function)
{
    if (nfuncstack >= MAXFUNCSTACK - 1)
    {
        lua_error("function stack overflow");
        return 1;
    }
    funcstack[nfuncstack].file = file;
    funcstack[nfuncstack].function = function;
    nfuncstack++;
    return 0;
}

/*
** Called to execute  RESET opcode, this function pops a function from
** function stack.
*/
void lua_popfunction(void)
{
    nfuncstack--;
}

/*
** Report bug building a message and sending it to lua_error function.
*/
void lua_reportbug(char *s)
{
    char msg[1024];
    strcpy(msg, s);
    if (lua_debugline != 0)
    {
        int i;
        if (nfuncstack > 0)
        {
            sprintf(strchr(msg, 0),
                "\n\tin statement begining at line %d in function \"%s\" of file \"%s\"",
                lua_debugline, s_name(funcstack[nfuncstack - 1].function),
                lua_file[funcstack[nfuncstack - 1].file]);
            sprintf(strchr(msg, 0), "\n\tactive stack\n");
            for (i = nfuncstack - 1; i >= 0; i--)
                sprintf(strchr(msg, 0), "\t-> function \"%s\" of file \"%s\"\n",
                    s_name(funcstack[i].function),
                    lua_file[funcstack[i].file]);
        }
        else
        {
            sprintf(strchr(msg, 0),
                "\n\tin statement begining at line %d of file \"%s\"",
                lua_debugline, lua_filename());
        }
    }
    lua_error(msg);
}
//inout.c end

//iolib.c start
//static FILE *in=stdin, *out=stdout;
static FILE *in, *out;

/*
** Open a file to read.
** LUA interface:
**			status = readfrom (filename)
** where:
**			status = 1 -> success
**			status = 0 -> error
*/
static void io_readfrom(void)
{
    lua_Object o = lua_getparam(1);
    if (o == NULL)			/* restore standart input */
    {
        if (in != stdin)
        {
            fclose(in);
            in = stdin;
        }
        lua_pushnumber(1);
    }
    else
    {
        if (!lua_isstring(o))
        {
            lua_error("incorrect argument to function 'readfrom`");
            lua_pushnumber(0);
        }
        else
        {
            FILE *fp = fopen(lua_getstring(o), "r");
            if (fp == NULL)
            {
                lua_pushnumber(0);
            }
            else
            {
                if (in != stdin) fclose(in);
                in = fp;
                lua_pushnumber(1);
            }
        }
    }
}


/*
** Open a file to write.
** LUA interface:
**			status = writeto (filename)
** where:
**			status = 1 -> success
**			status = 0 -> error
*/
static void io_writeto(void)
{
    lua_Object o = lua_getparam(1);
    if (o == NULL)			/* restore standart output */
    {
        if (out != stdout)
        {
            fclose(out);
            out = stdout;
        }
        lua_pushnumber(1);
    }
    else
    {
        if (!lua_isstring(o))
        {
            lua_error("incorrect argument to function 'writeto`");
            lua_pushnumber(0);
        }
        else
        {
            FILE *fp = fopen(lua_getstring(o), "w");
            if (fp == NULL)
            {
                lua_pushnumber(0);
            }
            else
            {
                if (out != stdout) fclose(out);
                out = fp;
                lua_pushnumber(1);
            }
        }
    }
}


/*
** Open a file to write appended.
** LUA interface:
**			status = appendto (filename)
** where:
**			status = 2 -> success (already exist)
**			status = 1 -> success (new file)
**			status = 0 -> error
*/
static void io_appendto(void)
{
    lua_Object o = lua_getparam(1);
    if (o == NULL)			/* restore standart output */
    {
        if (out != stdout)
        {
            fclose(out);
            out = stdout;
        }
        lua_pushnumber(1);
    }
    else
    {
        if (!lua_isstring(o))
        {
            lua_error("incorrect argument to function 'appendto`");
            lua_pushnumber(0);
        }
        else
        {
            int r;
            FILE *fp;
            struct stat st;
            if (stat(lua_getstring(o), &st) == -1) r = 1;
            else                                   r = 2;
            fp = fopen(lua_getstring(o), "a");
            if (fp == NULL)
            {
                lua_pushnumber(0);
            }
            else
            {
                if (out != stdout) fclose(out);
                out = fp;
                lua_pushnumber(r);
            }
        }
    }
}



/*
** Read a variable. On error put nil on stack.
** LUA interface:
**			variable = read ([format])
**
** O formato pode ter um dos seguintes especificadores:
**
**	s ou S -> para string
**	f ou F, g ou G, e ou E -> para reais
**	i ou I -> para inteiros
**
**	Estes especificadores podem vir seguidos de numero que representa
**	o numero de campos a serem lidos.
*/
static void io_read(void)
{
    lua_Object o = lua_getparam(1);
    if (o == NULL || !lua_isstring(o))	/* free format */
    {
        int c;
        char s[256];
        while (isspace(c = fgetc(in)))
            ;
        if (c == '\"')
        {
            int c, n = 0;
            while ((c = fgetc(in)) != '\"')
            {
                if (c == EOF)
                {
                    lua_pushnil();
                    return;
                }
                s[n++] = c;
            }
            s[n] = 0;
        }
        else if (c == '\'')
        {
            int c, n = 0;
            while ((c = fgetc(in)) != '\'')
            {
                if (c == EOF)
                {
                    lua_pushnil();
                    return;
                }
                s[n++] = c;
            }
            s[n] = 0;
        }
        else
        {
            char *ptr;
            double d;
            ungetc(c, in);
            if (fscanf(in, "%s", s) != 1)
            {
                lua_pushnil();
                return;
            }
            d = strtod(s, &ptr);
            if (!(*ptr))
            {
                lua_pushnumber(d);
                return;
            }
        }
        lua_pushstring(s);
        return;
    }
    else				/* formatted */
    {
        char *e = lua_getstring(o);
        char t;
        int  m = 0;
        while (isspace(*e)) e++;
        t = *e++;
        while (isdigit(*e))
            m = m * 10 + (*e++ - '0');

        if (m > 0)
        {
            char f[80];
            char s[256];
            sprintf(f, "%%%ds", m);
            if (fgets(s, m, in) == NULL)
            {
                lua_pushnil();
                return;
            }
            else
            {
                if (s[strlen(s) - 1] == '\n')
                    s[strlen(s) - 1] = 0;
            }
            switch (tolower(t))
            {
            case 'i':
            {
                long int l;
                sscanf(s, "%ld", &l);
                lua_pushnumber(l);
            }
            break;
            case 'f': case 'g': case 'e':
            {
                float f;
                sscanf(s, "%f", &f);
                lua_pushnumber(f);
            }
            break;
            default:
                lua_pushstring(s);
                break;
            }
        }
        else
        {
            switch (tolower(t))
            {
            case 'i':
            {
                long int l;
                if (fscanf(in, "%ld", &l) == EOF)
                    lua_pushnil();
                else lua_pushnumber(l);
            }
            break;
            case 'f': case 'g': case 'e':
            {
                float f;
                if (fscanf(in, "%f", &f) == EOF)
                    lua_pushnil();
                else lua_pushnumber(f);
            }
            break;
            default:
            {
                char s[256];
                if (fscanf(in, "%s", s) == EOF)
                    lua_pushnil();
                else lua_pushstring(s);
            }
            break;
            }
        }
    }
}


/*
** Write a variable. On error put 0 on stack, otherwise put 1.
** LUA interface:
**			status = write (variable [,format])
**
** O formato pode ter um dos seguintes especificadores:
**
**	s ou S -> para string
**	f ou F, g ou G, e ou E -> para reais
**	i ou I -> para inteiros
**
**	Estes especificadores podem vir seguidos de:
**
**		[?][m][.n]
**
**	onde:
**		? -> indica justificacao
**			< = esquerda
**			| = centro
**			> = direita (default)
**		m -> numero maximo de campos (se exceder estoura)
**		n -> indica precisao para
**			reais -> numero de casas decimais
**			inteiros -> numero minimo de digitos
**			string -> nao se aplica
*/
static char *buildformat(char *e, lua_Object o)
{
    static char buffer[512];
    static char f[80];
    char *string = &buffer[255];
    char t, j = 'r';
    int  m = 0, n = 0, l;
    while (isspace(*e)) e++;
    t = *e++;
    if (*e == '<' || *e == '|' || *e == '>') j = *e++;
    while (isdigit(*e))
        m = m * 10 + (*e++ - '0');
    e++;	/* skip point */
    while (isdigit(*e))
        n = n * 10 + (*e++ - '0');

    sprintf(f, "%%");
    if (j == '<' || j == '|') sprintf(strchr(f, 0), "-");
    if (m != 0)   sprintf(strchr(f, 0), "%d", m);
    if (n != 0)   sprintf(strchr(f, 0), ".%d", n);
    sprintf(strchr(f, 0), "%c", t);
    switch (tolower(t))
    {
    case 'i': t = 'i';
        sprintf(string, f, (long int)lua_getnumber(o));
        break;
    case 'f': case 'g': case 'e': t = 'f';
        sprintf(string, f, (float)lua_getnumber(o));
        break;
    case 's': t = 's';
        sprintf(string, f, lua_getstring(o));
        break;
    default: return "";
    }
    l = strlen(string);
    if (m != 0 && l>m)
    {
        int i;
        for (i = 0; i<m; i++)
            string[i] = '*';
        string[i] = 0;
    }
    else if (m != 0 && j == '|')
    {
        int i = l - 1;
        while (isspace(string[i])) i--;
        string -= (m - i) / 2;
        i = 0;
        while (string[i] == 0) string[i++] = ' ';
        string[l] = 0;
    }
    return string;
}
static void io_write(void)
{
    lua_Object o1 = lua_getparam(1);
    lua_Object o2 = lua_getparam(2);
    if (o1 == NULL)			/* new line */
    {
        fprintf(out, "\n");
        lua_pushnumber(1);
    }
    else if (o2 == NULL)   		/* free format */
    {
        int status = 0;
        if (lua_isnumber(o1))
            status = fprintf(out, "%g", lua_getnumber(o1));
        else if (lua_isstring(o1))
            status = fprintf(out, "%s", lua_getstring(o1));
        lua_pushnumber(status);
    }
    else					/* formated */
    {
        if (!lua_isstring(o2))
        {
            lua_error("incorrect format to function `write'");
            lua_pushnumber(0);
            return;
        }
        lua_pushnumber(fprintf(out, "%s", buildformat(lua_getstring(o2), o1)));
    }
}

/*
** Execute a executable program using "system".
** Return the result of execution.
*/
void io_execute(void)
{
    lua_Object o = lua_getparam(1);
    if (o == NULL || !lua_isstring(o))
    {
        lua_error("incorrect argument to function 'execute`");
        lua_pushnumber(0);
    }
    else
    {
        int res = system(lua_getstring(o));
        lua_pushnumber(res);
    }
    return;
}

/*
** Remove a file.
** On error put 0 on stack, otherwise put 1.
*/
void io_remove(void)
{
    lua_Object o = lua_getparam(1);
    if (o == NULL || !lua_isstring(o))
    {
        lua_error("incorrect argument to function 'execute`");
        lua_pushnumber(0);
    }
    else
    {
        if (remove(lua_getstring(o)) == 0)
            lua_pushnumber(1);
        else
            lua_pushnumber(0);
    }
    return;
}

/*
** Open io library
*/
void iolib_open(void)
{
    in = stdin;
    out = stdout;
    lua_register("readfrom", io_readfrom);
    lua_register("writeto", io_writeto);
    lua_register("appendto", io_appendto);
    lua_register("read", io_read);
    lua_register("write", io_write);
    lua_register("execute", io_execute);
    lua_register("remove", io_remove);
}
//iolib.c end

//lex.c start
#define next() { current = input(); }
#define save(x) { *yytextLast++ = (x); }
#define save_and_next()  { save(current); next(); }

static int current;
static char yytext[256];
static char *yytextLast;

static Input input;

void lua_setinput(Input fn)
{
    current = ' ';
    input = fn;
}

char *lua_lasttext(void)
{
    *yytextLast = 0;
    return yytext;
}


static struct
{
    char *name;
    int token;
} reserved[] = {
    { "and", AND },
    { "do", DO },
    { "else", ELSE },
    { "elseif", ELSEIF },
    { "end", END },
    { "function", FUNCTION },
    { "if", IF },
    { "local", LOCAL },
    { "nil", NIL },
    { "not", NOT },
    { "or", OR },
    { "repeat", REPEAT },
    { "return", RETURN },
    { "then", THEN },
    { "until", UNTIL },
    { "while", WHILE } };

#define RESERVEDSIZE (sizeof(reserved)/sizeof(reserved[0]))


int findReserved(char *name)
{
    int l = 0;
    int h = RESERVEDSIZE - 1;
    while (l <= h)
    {
        int m = (l + h) / 2;
        int comp = strcmp(name, reserved[m].name);
        if (comp < 0)
            h = m - 1;
        else if (comp == 0)
            return reserved[m].token;
        else
            l = m + 1;
    }
    return 0;
}


int yylex()
{
    while (1)
    {
        yytextLast = yytext;
        switch (current)
        {
        case '\n': lua_linenumber++;
        case ' ':
        case '\t':
            next();
            continue;

        case '$':
            next();
            while (isalnum(current) || current == '_')
                save_and_next();
            *yytextLast = 0;
            if (strcmp(yytext, "debug") == 0)
            {
                yylval.vInt = 1;
                return DEBUG;
            }
            else if (strcmp(yytext, "nodebug") == 0)
            {
                yylval.vInt = 0;
                return DEBUG;
            }
            return WRONGTOKEN;

        case '-':
            save_and_next();
            if (current != '-') return '-';
            do { next(); } while (current != '\n' && current != 0);
            continue;

        case '<':
            save_and_next();
            if (current != '=') return '<';
            else { save_and_next(); return LE; }

        case '>':
            save_and_next();
            if (current != '=') return '>';
            else { save_and_next(); return GE; }

        case '~':
            save_and_next();
            if (current != '=') return '~';
            else { save_and_next(); return NE; }

        case '"':
        case '\'':
        {
            int del = current;
            next();  /* skip the delimiter */
            while (current != del)
            {
                switch (current)
                {
                case 0:
                case '\n':
                    return WRONGTOKEN;
                case '\\':
                    next();  /* do not save the '\' */
                    switch (current)
                    {
                    case 'n': save('\n'); next(); break;
                    case 't': save('\t'); next(); break;
                    case 'r': save('\r'); next(); break;
                    default: save('\\'); break;
                    }
                    break;
                default:
                    save_and_next();
                }
            }
            next();  /* skip the delimiter */
            *yytextLast = 0;
            yylval.vWord = lua_findconstant(yytext);
            return STRING;
        }

        case 'a': case 'b': case 'c': case 'd': case 'e':
        case 'f': case 'g': case 'h': case 'i': case 'j':
        case 'k': case 'l': case 'm': case 'n': case 'o':
        case 'p': case 'q': case 'r': case 's': case 't':
        case 'u': case 'v': case 'w': case 'x': case 'y':
        case 'z':
        case 'A': case 'B': case 'C': case 'D': case 'E':
        case 'F': case 'G': case 'H': case 'I': case 'J':
        case 'K': case 'L': case 'M': case 'N': case 'O':
        case 'P': case 'Q': case 'R': case 'S': case 'T':
        case 'U': case 'V': case 'W': case 'X': case 'Y':
        case 'Z':
        case '_':
        {
            int res;
            do { save_and_next(); } while (isalnum(current) || current == '_');
            *yytextLast = 0;
            res = findReserved(yytext);
            if (res) return res;
            yylval.pChar = yytext;
            return NAME;
        }

        case '.':
            save_and_next();
            if (current == '.')
            {
                save_and_next();
                return CONC;
            }
            else if (!isdigit(current)) return '.';
            /* current is a digit: goes through to number */
            goto fraction;

        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':

            do { save_and_next(); } while (isdigit(current));
            if (current == '.') save_and_next();
        fraction: while (isdigit(current)) save_and_next();
            if (current == 'e' || current == 'E')
            {
                save_and_next();
                if (current == '+' || current == '-') save_and_next();
                if (!isdigit(current)) return WRONGTOKEN;
                do { save_and_next(); } while (isdigit(current));
            }
            *yytextLast = 0;
            yylval.vFloat = atof(yytext);
            return NUMBER;

        default: 		/* also end of file */
        {
            save_and_next();
            return *yytext;
        }
        }
    }
}
//lex.c end

//mathlib.c start
#define TODEGREE(a) ((a)*180.0/3.14159)
#define TORAD(a)    ((a)*3.14159/180.0)

static void math_abs(void)
{
    double d;
    lua_Object o = lua_getparam(1);
    if (o == NULL)
    {
        lua_error("too few arguments to function `abs'"); return;
    }
    if (!lua_isnumber(o))
    {
        lua_error("incorrect arguments to function `abs'"); return;
    }
    d = lua_getnumber(o);
    if (d < 0) d = -d;
    lua_pushnumber(d);
}


static void math_sin(void)
{
    double d;
    lua_Object o = lua_getparam(1);
    if (o == NULL)
    {
        lua_error("too few arguments to function `sin'"); return;
    }
    if (!lua_isnumber(o))
    {
        lua_error("incorrect arguments to function `sin'"); return;
    }
    d = lua_getnumber(o);
    lua_pushnumber(sin(TORAD(d)));
}



static void math_cos(void)
{
    double d;
    lua_Object o = lua_getparam(1);
    if (o == NULL)
    {
        lua_error("too few arguments to function `cos'"); return;
    }
    if (!lua_isnumber(o))
    {
        lua_error("incorrect arguments to function `cos'"); return;
    }
    d = lua_getnumber(o);
    lua_pushnumber(cos(TORAD(d)));
}



static void math_tan(void)
{
    double d;
    lua_Object o = lua_getparam(1);
    if (o == NULL)
    {
        lua_error("too few arguments to function `tan'"); return;
    }
    if (!lua_isnumber(o))
    {
        lua_error("incorrect arguments to function `tan'"); return;
    }
    d = lua_getnumber(o);
    lua_pushnumber(tan(TORAD(d)));
}


static void math_asin(void)
{
    double d;
    lua_Object o = lua_getparam(1);
    if (o == NULL)
    {
        lua_error("too few arguments to function `asin'"); return;
    }
    if (!lua_isnumber(o))
    {
        lua_error("incorrect arguments to function `asin'"); return;
    }
    d = lua_getnumber(o);
    lua_pushnumber(TODEGREE(asin(d)));
}


static void math_acos(void)
{
    double d;
    lua_Object o = lua_getparam(1);
    if (o == NULL)
    {
        lua_error("too few arguments to function `acos'"); return;
    }
    if (!lua_isnumber(o))
    {
        lua_error("incorrect arguments to function `acos'"); return;
    }
    d = lua_getnumber(o);
    lua_pushnumber(TODEGREE(acos(d)));
}



static void math_atan(void)
{
    double d;
    lua_Object o = lua_getparam(1);
    if (o == NULL)
    {
        lua_error("too few arguments to function `atan'"); return;
    }
    if (!lua_isnumber(o))
    {
        lua_error("incorrect arguments to function `atan'"); return;
    }
    d = lua_getnumber(o);
    lua_pushnumber(TODEGREE(atan(d)));
}


static void math_ceil(void)
{
    double d;
    lua_Object o = lua_getparam(1);
    if (o == NULL)
    {
        lua_error("too few arguments to function `ceil'"); return;
    }
    if (!lua_isnumber(o))
    {
        lua_error("incorrect arguments to function `ceil'"); return;
    }
    d = lua_getnumber(o);
    lua_pushnumber(ceil(d));
}


static void math_floor(void)
{
    double d;
    lua_Object o = lua_getparam(1);
    if (o == NULL)
    {
        lua_error("too few arguments to function `floor'"); return;
    }
    if (!lua_isnumber(o))
    {
        lua_error("incorrect arguments to function `floor'"); return;
    }
    d = lua_getnumber(o);
    lua_pushnumber(floor(d));
}

static void math_mod(void)
{
    int d1, d2;
    lua_Object o1 = lua_getparam(1);
    lua_Object o2 = lua_getparam(2);
    if (!lua_isnumber(o1) || !lua_isnumber(o2))
    {
        lua_error("incorrect arguments to function `mod'"); return;
    }
    d1 = (int)lua_getnumber(o1);
    d2 = (int)lua_getnumber(o2);
    lua_pushnumber(d1%d2);
}


static void math_sqrt(void)
{
    double d;
    lua_Object o = lua_getparam(1);
    if (o == NULL)
    {
        lua_error("too few arguments to function `sqrt'"); return;
    }
    if (!lua_isnumber(o))
    {
        lua_error("incorrect arguments to function `sqrt'"); return;
    }
    d = lua_getnumber(o);
    lua_pushnumber(sqrt(d));
}

static void math_pow(void)
{
    double d1, d2;
    lua_Object o1 = lua_getparam(1);
    lua_Object o2 = lua_getparam(2);
    if (!lua_isnumber(o1) || !lua_isnumber(o2))
    {
        lua_error("incorrect arguments to function `pow'"); return;
    }
    d1 = lua_getnumber(o1);
    d2 = lua_getnumber(o2);
    lua_pushnumber(pow(d1, d2));
}

static void math_min(void)
{
    int i = 1;
    double d, dmin;
    lua_Object o;
    if ((o = lua_getparam(i++)) == NULL)
    {
        lua_error("too few arguments to function `min'"); return;
    }
    if (!lua_isnumber(o))
    {
        lua_error("incorrect arguments to function `min'"); return;
    }
    dmin = lua_getnumber(o);
    while ((o = lua_getparam(i++)) != NULL)
    {
        if (!lua_isnumber(o))
        {
            lua_error("incorrect arguments to function `min'"); return;
        }
        d = lua_getnumber(o);
        if (d < dmin) dmin = d;
    }
    lua_pushnumber(dmin);
}


static void math_max(void)
{
    int i = 1;
    double d, dmax;
    lua_Object o;
    if ((o = lua_getparam(i++)) == NULL)
    {
        lua_error("too few arguments to function `max'"); return;
    }
    if (!lua_isnumber(o))
    {
        lua_error("incorrect arguments to function `max'"); return;
    }
    dmax = lua_getnumber(o);
    while ((o = lua_getparam(i++)) != NULL)
    {
        if (!lua_isnumber(o))
        {
            lua_error("incorrect arguments to function `max'"); return;
        }
        d = lua_getnumber(o);
        if (d > dmax) dmax = d;
    }
    lua_pushnumber(dmax);
}



/*
** Open math library
*/
void mathlib_open(void)
{
    lua_register("abs", math_abs);
    lua_register("sin", math_sin);
    lua_register("cos", math_cos);
    lua_register("tan", math_tan);
    lua_register("asin", math_asin);
    lua_register("acos", math_acos);
    lua_register("atan", math_atan);
    lua_register("ceil", math_ceil);
    lua_register("floor", math_floor);
    lua_register("mod", math_mod);
    lua_register("sqrt", math_sqrt);
    lua_register("pow", math_pow);
    lua_register("min", math_min);
    lua_register("max", math_max);
}
//mathlib.c end

//opcode.c start
#define tonumber(o) ((tag(o) != T_NUMBER) && (lua_tonumber(o) != 0))
#define tostring(o) ((tag(o) != T_STRING) && (lua_tostring(o) != 0))

#ifndef MAXSTACK
#define MAXSTACK 256
#endif
static Object stack[MAXSTACK] = { { T_MARK,{ NULL } } };
static Object *top = stack + 1, *base = stack + 1;


/*
** Concatenate two given string, creating a mark space at the beginning.
** Return the new string pointer.
*/
static char *lua_strconc(char *l, char *r)
{
    char *s = calloc(strlen(l) + strlen(r) + 2, sizeof(char));
    if (s == NULL)
    {
        lua_error("not enough memory");
        return NULL;
    }
    *s++ = 0; 			/* create mark space */
    return strcat(strcpy(s, l), r);
}

/*
** Duplicate a string,  creating a mark space at the beginning.
** Return the new string pointer.
*/
char *lua_strdup(char *l)
{
    char *s = calloc(strlen(l) + 2, sizeof(char));
    if (s == NULL)
    {
        lua_error("not enough memory");
        return NULL;
    }
    *s++ = 0; 			/* create mark space */
    return strcpy(s, l);
}

/*
** Convert, if possible, to a number tag.
** Return 0 in success or not 0 on error.
*/
static int lua_tonumber(Object *obj)
{
    char *ptr;
    if (tag(obj) != T_STRING)
    {
        lua_reportbug("unexpected type at conversion to number");
        return 1;
    }
    nvalue(obj) = strtod(svalue(obj), &ptr);
    if (*ptr)
    {
        lua_reportbug("string to number convertion failed");
        return 2;
    }
    tag(obj) = T_NUMBER;
    return 0;
}

/*
** Test if is possible to convert an object to a number one.
** If possible, return the converted object, otherwise return nil object.
*/
static Object *lua_convtonumber(Object *obj)
{
    static Object cvt;

    if (tag(obj) == T_NUMBER)
    {
        cvt = *obj;
        return &cvt;
    }

    tag(&cvt) = T_NIL;
    if (tag(obj) == T_STRING)
    {
        char *ptr;
        nvalue(&cvt) = strtod(svalue(obj), &ptr);
        if (*ptr == 0)
            tag(&cvt) = T_NUMBER;
    }
    return &cvt;
}



/*
** Convert, if possible, to a string tag
** Return 0 in success or not 0 on error.
*/
static int lua_tostring(Object *obj)
{
    static char s[256];
    if (tag(obj) != T_NUMBER)
    {
        lua_reportbug("unexpected type at conversion to string");
        return 1;
    }
    if ((int)nvalue(obj) == nvalue(obj))
        sprintf(s, "%d", (int)nvalue(obj));
    else
        sprintf(s, "%g", nvalue(obj));
    svalue(obj) = lua_createstring(lua_strdup(s));
    if (svalue(obj) == NULL)
        return 1;
    tag(obj) = T_STRING;
    return 0;
}


/*
** Execute the given opcode. Return 0 in success or 1 on error.
*/
int lua_execute(Byte *pc)
{
    Object *oldbase = base;
    base = top;
    while (1)
    {
        OpCode opcode;
        switch (opcode = (OpCode)*pc++)
        {
        case PUSHNIL: tag(top++) = T_NIL; break;

        case PUSH0: tag(top) = T_NUMBER; nvalue(top++) = 0; break;
        case PUSH1: tag(top) = T_NUMBER; nvalue(top++) = 1; break;
        case PUSH2: tag(top) = T_NUMBER; nvalue(top++) = 2; break;

        case PUSHBYTE: tag(top) = T_NUMBER; nvalue(top++) = *pc++; break;

        case PUSHWORD:
        {
            CodeWord code;
            get_word(code, pc);
            tag(top) = T_NUMBER; nvalue(top++) = code.w;
        }
        break;

        case PUSHFLOAT:
        {
            CodeFloat code;
            get_float(code, pc);
            tag(top) = T_NUMBER; nvalue(top++) = code.f;
        }
        break;

        case PUSHSTRING:
        {
            CodeWord code;
            get_word(code, pc);
            tag(top) = T_STRING; svalue(top++) = lua_constant[code.w];
        }
        break;

        case PUSHLOCAL0: case PUSHLOCAL1: case PUSHLOCAL2:
        case PUSHLOCAL3: case PUSHLOCAL4: case PUSHLOCAL5:
        case PUSHLOCAL6: case PUSHLOCAL7: case PUSHLOCAL8:
        case PUSHLOCAL9: *top++ = *(base + (int)(opcode - PUSHLOCAL0)); break;

        case PUSHLOCAL: *top++ = *(base + (*pc++)); break;

        case PUSHGLOBAL:
        {
            CodeWord code;
            get_word(code, pc);
            *top++ = s_object(code.w);
        }
        break;

        case PUSHINDEXED:
            --top;
            if (tag(top - 1) != T_ARRAY)
            {
                lua_reportbug("indexed expression not a table");
                return 1;
            }
            {
                Object *h = lua_hashdefine(avalue(top - 1), top);
                if (h == NULL) return 1;
                *(top - 1) = *h;
            }
            break;

        case PUSHMARK: tag(top++) = T_MARK; break;

        case PUSHOBJECT: *top = *(top - 3); top++; break;

        case STORELOCAL0: case STORELOCAL1: case STORELOCAL2:
        case STORELOCAL3: case STORELOCAL4: case STORELOCAL5:
        case STORELOCAL6: case STORELOCAL7: case STORELOCAL8:
        case STORELOCAL9: *(base + (int)(opcode - STORELOCAL0)) = *(--top); break;

        case STORELOCAL: *(base + (*pc++)) = *(--top); break;

        case STOREGLOBAL:
        {
            CodeWord code;
            get_word(code, pc);
            s_object(code.w) = *(--top);
        }
        break;

        case STOREINDEXED0:
            if (tag(top - 3) != T_ARRAY)
            {
                lua_reportbug("indexed expression not a table");
                return 1;
            }
            {
                Object *h = lua_hashdefine(avalue(top - 3), top - 2);
                if (h == NULL) return 1;
                *h = *(top - 1);
            }
            top -= 3;
            break;

        case STOREINDEXED:
        {
            int n = *pc++;
            if (tag(top - 3 - n) != T_ARRAY)
            {
                lua_reportbug("indexed expression not a table");
                return 1;
            }
            {
                Object *h = lua_hashdefine(avalue(top - 3 - n), top - 2 - n);
                if (h == NULL) return 1;
                *h = *(top - 1);
            }
            top--;
        }
        break;

        case STORELIST0:
        case STORELIST:
        {
            int m, n;
            Object *arr;
            if (opcode == STORELIST0) m = 0;
            else m = *(pc++) * FIELDS_PER_FLUSH;
            n = *(pc++);
            arr = top - n - 1;
            if (tag(arr) != T_ARRAY)
            {
                lua_reportbug("internal error - table expected");
                return 1;
            }
            while (n)
            {
                tag(top) = T_NUMBER; nvalue(top) = n + m;
                *(lua_hashdefine(avalue(arr), top)) = *(top - 1);
                top--;
                n--;
            }
        }
        break;

        case STORERECORD:
        {
            int n = *(pc++);
            Object *arr = top - n - 1;
            if (tag(arr) != T_ARRAY)
            {
                lua_reportbug("internal error - table expected");
                return 1;
            }
            while (n)
            {
                CodeWord code;
                get_word(code, pc);
                tag(top) = T_STRING; svalue(top) = lua_constant[code.w];
                *(lua_hashdefine(avalue(arr), top)) = *(top - 1);
                top--;
                n--;
            }
        }
        break;

        case ADJUST:
        {
            Object *newtop = base + *(pc++);
            while (top < newtop) tag(top++) = T_NIL;
            top = newtop;  /* top could be bigger than newtop */
        }
        break;

        case CREATEARRAY:
            if (tag(top - 1) == T_NIL)
                nvalue(top - 1) = 101;
            else
            {
                if (tonumber(top - 1)) return 1;
                if (nvalue(top - 1) <= 0) nvalue(top - 1) = 101;
            }
            avalue(top - 1) = lua_createarray(nvalue(top - 1));
            if (avalue(top - 1) == NULL)
                return 1;
            tag(top - 1) = T_ARRAY;
            break;

        case EQOP:
        {
            Object *l = top - 2;
            Object *r = top - 1;
            --top;
            if (tag(l) != tag(r))
                tag(top - 1) = T_NIL;
            else
            {
                switch (tag(l))
                {
                case T_NIL:       tag(top - 1) = T_NUMBER; break;
                case T_NUMBER:    tag(top - 1) = (nvalue(l) == nvalue(r)) ? T_NUMBER : T_NIL; break;
                case T_ARRAY:     tag(top - 1) = (avalue(l) == avalue(r)) ? T_NUMBER : T_NIL; break;
                case T_FUNCTION:  tag(top - 1) = (bvalue(l) == bvalue(r)) ? T_NUMBER : T_NIL; break;
                case T_CFUNCTION: tag(top - 1) = (fvalue(l) == fvalue(r)) ? T_NUMBER : T_NIL; break;
                case T_USERDATA:  tag(top - 1) = (uvalue(l) == uvalue(r)) ? T_NUMBER : T_NIL; break;
                case T_STRING:    tag(top - 1) = (strcmp(svalue(l), svalue(r)) == 0) ? T_NUMBER : T_NIL; break;
                case T_MARK:      return 1;
                }
            }
            nvalue(top - 1) = 1;
        }
        break;

        case LTOP:
        {
            Object *l = top - 2;
            Object *r = top - 1;
            --top;
            if (tag(l) == T_NUMBER && tag(r) == T_NUMBER)
                tag(top - 1) = (nvalue(l) < nvalue(r)) ? T_NUMBER : T_NIL;
            else
            {
                if (tostring(l) || tostring(r))
                    return 1;
                tag(top - 1) = (strcmp(svalue(l), svalue(r)) < 0) ? T_NUMBER : T_NIL;
            }
            nvalue(top - 1) = 1;
        }
        break;

        case LEOP:
        {
            Object *l = top - 2;
            Object *r = top - 1;
            --top;
            if (tag(l) == T_NUMBER && tag(r) == T_NUMBER)
                tag(top - 1) = (nvalue(l) <= nvalue(r)) ? T_NUMBER : T_NIL;
            else
            {
                if (tostring(l) || tostring(r))
                    return 1;
                tag(top - 1) = (strcmp(svalue(l), svalue(r)) <= 0) ? T_NUMBER : T_NIL;
            }
            nvalue(top - 1) = 1;
        }
        break;

        case ADDOP:
        {
            Object *l = top - 2;
            Object *r = top - 1;
            if (tonumber(r) || tonumber(l))
                return 1;
            nvalue(l) += nvalue(r);
            --top;
        }
        break;

        case SUBOP:
        {
            Object *l = top - 2;
            Object *r = top - 1;
            if (tonumber(r) || tonumber(l))
                return 1;
            nvalue(l) -= nvalue(r);
            --top;
        }
        break;

        case MULTOP:
        {
            Object *l = top - 2;
            Object *r = top - 1;
            if (tonumber(r) || tonumber(l))
                return 1;
            nvalue(l) *= nvalue(r);
            --top;
        }
        break;

        case DIVOP:
        {
            Object *l = top - 2;
            Object *r = top - 1;
            if (tonumber(r) || tonumber(l))
                return 1;
            nvalue(l) /= nvalue(r);
            --top;
        }
        break;

        case CONCOP:
        {
            Object *l = top - 2;
            Object *r = top - 1;
            if (tostring(r) || tostring(l))
                return 1;
            svalue(l) = lua_createstring(lua_strconc(svalue(l), svalue(r)));
            if (svalue(l) == NULL)
                return 1;
            --top;
        }
        break;

        case MINUSOP:
            if (tonumber(top - 1))
                return 1;
            nvalue(top - 1) = -nvalue(top - 1);
            break;

        case NOTOP:
            tag(top - 1) = tag(top - 1) == T_NIL ? T_NUMBER : T_NIL;
            break;

        case ONTJMP:
        {
            CodeWord code;
            get_word(code, pc);
            if (tag(top - 1) != T_NIL) pc += code.w;
        }
        break;

        case ONFJMP:
        {
            CodeWord code;
            get_word(code, pc);
            if (tag(top - 1) == T_NIL) pc += code.w;
        }
        break;

        case JMP:
        {
            CodeWord code;
            get_word(code, pc);
            pc += code.w;
        }
        break;

        case UPJMP:
        {
            CodeWord code;
            get_word(code, pc);
            pc -= code.w;
        }
        break;

        case IFFJMP:
        {
            CodeWord code;
            get_word(code, pc);
            top--;
            if (tag(top) == T_NIL) pc += code.w;
        }
        break;

        case IFFUPJMP:
        {
            CodeWord code;
            get_word(code, pc);
            top--;
            if (tag(top) == T_NIL) pc -= code.w;
        }
        break;

        case POP: --top; break;

        case CALLFUNC:
        {
            Byte *newpc;
            Object *b = top - 1;
            while (tag(b) != T_MARK) b--;
            if (tag(b - 1) == T_FUNCTION)
            {
                lua_debugline = 0;			/* always reset debug flag */
                newpc = bvalue(b - 1);
                bvalue(b - 1) = pc;		        /* store return code */
                nvalue(b) = (base - stack);		/* store base value */
                base = b + 1;
                pc = newpc;
                if (MAXSTACK - (base - stack) < STACKGAP)
                {
                    lua_error("stack overflow");
                    return 1;
                }
            }
            else if (tag(b - 1) == T_CFUNCTION)
            {
                int nparam;
                lua_debugline = 0;			/* always reset debug flag */
                nvalue(b) = (base - stack);		/* store base value */
                base = b + 1;
                nparam = top - base;			/* number of parameters */
                (fvalue(b - 1))();			/* call C function */

                                            /* shift returned values */
                {
                    int i;
                    int nretval = top - base - nparam;
                    top = base - 2;
                    base = stack + (int)nvalue(base - 1);
                    for (i = 0; i<nretval; i++)
                    {
                        *top = *(top + nparam + 2);
                        ++top;
                    }
                }
            }
            else
            {
                lua_reportbug("call expression not a function");
                return 1;
            }
        }
        break;

        case RETCODE:
        {
            int i;
            int shift = *pc++;
            int nretval = top - base - shift;
            top = base - 2;
            pc = bvalue(base - 2);
            base = stack + (int)nvalue(base - 1);
            for (i = 0; i<nretval; i++)
            {
                *top = *(top + shift + 2);
                ++top;
            }
        }
        break;

        case HALT:
            base = oldbase;
            return 0;		/* success */

        case SETFUNCTION:
        {
            CodeWord file, func;
            get_word(file, pc);
            get_word(func, pc);
            if (lua_pushfunction(file.w, func.w))
                return 1;
        }
        break;

        case SETLINE:
        {
            CodeWord code;
            get_word(code, pc);
            lua_debugline = code.w;
        }
        break;

        case RESET:
            lua_popfunction();
            break;

        default:
            lua_error("internal error - opcode didn't match");
            return 1;
        }
    }
}


/*
** Traverse all objects on stack
*/
void lua_travstack(void(*fn)(Object *))
{
    Object *o;
    for (o = top - 1; o >= stack; o--)
        fn(o);
}

/*
** Open file, generate opcode and execute global statement. Return 0 on
** success or 1 on error.
*/
int lua_dofile(char *filename)
{
    if (lua_openfile(filename)) return 1;
    if (lua_parse()) { lua_closefile(); return 1; }
    lua_closefile();
    return 0;
}

/*
** Generate opcode stored on string and execute global statement. Return 0 on
** success or 1 on error.
*/
int lua_dostring(char *string)
{
    if (lua_openstring(string)) return 1;
    if (lua_parse()) return 1;
    lua_closestring();
    return 0;
}

/*
** Execute the given function. Return 0 on success or 1 on error.
*/
int lua_call(char *functionname, int nparam)
{
    static Byte startcode[] = { CALLFUNC, HALT };
    int i;
    Object func = s_object(lua_findsymbol(functionname));
    if (tag(&func) != T_FUNCTION) return 1;
    for (i = 1; i <= nparam; i++)
        *(top - i + 2) = *(top - i);
    top += 2;
    tag(top - nparam - 1) = T_MARK;
    *(top - nparam - 2) = func;
    return (lua_execute(startcode));
}

/*
** Get a parameter, returning the object handle or NULL on error.
** 'number' must be 1 to get the first parameter.
*/
Object *lua_getparam(int number)
{
    if (number <= 0 || number > top - base) return NULL;
    return (base + number - 1);
}

/*
** Given an object handle, return its number value. On error, return 0.0.
*/
real lua_getnumber(Object *object)
{
    if (object == NULL || tag(object) == T_NIL) return 0.0;
    if (tonumber(object)) return 0.0;
    else                   return (nvalue(object));
}

/*
** Given an object handle, return its string pointer. On error, return NULL.
*/
char *lua_getstring(Object *object)
{
    if (object == NULL || tag(object) == T_NIL) return NULL;
    if (tostring(object)) return NULL;
    else                   return (svalue(object));
}

/*
** Given an object handle, return a copy of its string. On error, return NULL.
*/
char *lua_copystring(Object *object)
{
    if (object == NULL || tag(object) == T_NIL) return NULL;
    if (tostring(object)) return NULL;
    else                   return (strdup(svalue(object)));
}

/*
** Given an object handle, return its cfuntion pointer. On error, return NULL.
*/
lua_CFunction lua_getcfunction(Object *object)
{
    if (object == NULL) return NULL;
    if (tag(object) != T_CFUNCTION) return NULL;
    else                            return (fvalue(object));
}

/*
** Given an object handle, return its user data. On error, return NULL.
*/
void *lua_getuserdata(Object *object)
{
    if (object == NULL) return NULL;
    if (tag(object) != T_USERDATA) return NULL;
    else                           return (uvalue(object));
}

/*
** Given an object handle and a field name, return its field object.
** On error, return NULL.
*/
Object *lua_getfield(Object *object, char *field)
{
    if (object == NULL) return NULL;
    if (tag(object) != T_ARRAY)
        return NULL;
    else
    {
        Object ref;
        tag(&ref) = T_STRING;
        svalue(&ref) = lua_createstring(lua_strdup(field));
        return (lua_hashdefine(avalue(object), &ref));
    }
}

/*
** Given an object handle and an index, return its indexed object.
** On error, return NULL.
*/
Object *lua_getindexed(Object *object, float index)
{
    if (object == NULL) return NULL;
    if (tag(object) != T_ARRAY)
        return NULL;
    else
    {
        Object ref;
        tag(&ref) = T_NUMBER;
        nvalue(&ref) = index;
        return (lua_hashdefine(avalue(object), &ref));
    }
}

/*
** Get a global object. Return the object handle or NULL on error.
*/
Object *lua_getglobal(char *name)
{
    int n = lua_findsymbol(name);
    if (n < 0) return NULL;
    return &s_object(n);
}

/*
** Pop and return an object
*/
Object *lua_pop(void)
{
    if (top <= base) return NULL;
    top--;
    return top;
}

/*
** Push a nil object
*/
int lua_pushnil(void)
{
    if ((top - stack) >= MAXSTACK - 1)
    {
        lua_error("stack overflow");
        return 1;
    }
    tag(top) = T_NIL;
    return 0;
}

/*
** Push an object (tag=number) to stack. Return 0 on success or 1 on error.
*/
int lua_pushnumber(real n)
{
    if ((top - stack) >= MAXSTACK - 1)
    {
        lua_error("stack overflow");
        return 1;
    }
    tag(top) = T_NUMBER; nvalue(top++) = n;
    return 0;
}

/*
** Push an object (tag=string) to stack. Return 0 on success or 1 on error.
*/
int lua_pushstring(char *s)
{
    if ((top - stack) >= MAXSTACK - 1)
    {
        lua_error("stack overflow");
        return 1;
    }
    tag(top) = T_STRING;
    svalue(top++) = lua_createstring(lua_strdup(s));
    return 0;
}

/*
** Push an object (tag=cfunction) to stack. Return 0 on success or 1 on error.
*/
int lua_pushcfunction(lua_CFunction fn)
{
    if ((top - stack) >= MAXSTACK - 1)
    {
        lua_error("stack overflow");
        return 1;
    }
    tag(top) = T_CFUNCTION; fvalue(top++) = fn;
    return 0;
}

/*
** Push an object (tag=userdata) to stack. Return 0 on success or 1 on error.
*/
int lua_pushuserdata(void *u)
{
    if ((top - stack) >= MAXSTACK - 1)
    {
        lua_error("stack overflow");
        return 1;
    }
    tag(top) = T_USERDATA; uvalue(top++) = u;
    return 0;
}

/*
** Push an object to stack.
*/
int lua_pushobject(Object *o)
{
    if ((top - stack) >= MAXSTACK - 1)
    {
        lua_error("stack overflow");
        return 1;
    }
    *top++ = *o;
    return 0;
}

/*
** Store top of the stack at a global variable array field.
** Return 1 on error, 0 on success.
*/
int lua_storeglobal(char *name)
{
    int n = lua_findsymbol(name);
    if (n < 0) return 1;
    if (tag(top - 1) == T_MARK) return 1;
    s_object(n) = *(--top);
    return 0;
}

/*
** Store top of the stack at an array field. Return 1 on error, 0 on success.
*/
int lua_storefield(lua_Object object, char *field)
{
    if (tag(object) != T_ARRAY)
        return 1;
    else
    {
        Object ref, *h;
        tag(&ref) = T_STRING;
        svalue(&ref) = lua_createstring(lua_strdup(field));
        h = lua_hashdefine(avalue(object), &ref);
        if (h == NULL) return 1;
        if (tag(top - 1) == T_MARK) return 1;
        *h = *(--top);
    }
    return 0;
}


/*
** Store top of the stack at an array index. Return 1 on error, 0 on success.
*/
int lua_storeindexed(lua_Object object, float index)
{
    if (tag(object) != T_ARRAY)
        return 1;
    else
    {
        Object ref, *h;
        tag(&ref) = T_NUMBER;
        nvalue(&ref) = index;
        h = lua_hashdefine(avalue(object), &ref);
        if (h == NULL) return 1;
        if (tag(top - 1) == T_MARK) return 1;
        *h = *(--top);
    }
    return 0;
}


/*
** Given an object handle, return if it is nil.
*/
int lua_isnil(Object *object)
{
    return (object != NULL && tag(object) == T_NIL);
}

/*
** Given an object handle, return if it is a number one.
*/
int lua_isnumber(Object *object)
{
    return (object != NULL && tag(object) == T_NUMBER);
}

/*
** Given an object handle, return if it is a string one.
*/
int lua_isstring(Object *object)
{
    return (object != NULL && tag(object) == T_STRING);
}

/*
** Given an object handle, return if it is an array one.
*/
int lua_istable(Object *object)
{
    return (object != NULL && tag(object) == T_ARRAY);
}

/*
** Given an object handle, return if it is a cfunction one.
*/
int lua_iscfunction(Object *object)
{
    return (object != NULL && tag(object) == T_CFUNCTION);
}

/*
** Given an object handle, return if it is an user data one.
*/
int lua_isuserdata(Object *object)
{
    return (object != NULL && tag(object) == T_USERDATA);
}

/*
** Internal function: return an object type.
*/
void lua_type(void)
{
    Object *o = lua_getparam(1);
    lua_pushstring(lua_constant[tag(o)]);
}

/*
** Internal function: convert an object to a number
*/
void lua_obj2number(void)
{
    Object *o = lua_getparam(1);
    lua_pushobject(lua_convtonumber(o));
}

/*
** Internal function: print object values
*/
void lua_print(void)
{
    int i = 1;
    void *obj;
    while ((obj = lua_getparam(i++)) != NULL)
    {
        if (lua_isnumber(obj))    printf("%g\n", lua_getnumber(obj));
        else if (lua_isstring(obj))    printf("%s\n", lua_getstring(obj));
        else if (lua_iscfunction(obj)) printf("cfunction: %p\n", lua_getcfunction(obj));
        else if (lua_isuserdata(obj))  printf("userdata: %p\n", lua_getuserdata(obj));
        else if (lua_istable(obj))     printf("table: %p\n", obj);
        else if (lua_isnil(obj))       printf("nil\n");
        else			         printf("invalid value to print\n");
    }
}

/*
** Internal function: do a file
*/
void lua_internaldofile(void)
{
    lua_Object obj = lua_getparam(1);
    if (lua_isstring(obj) && !lua_dofile(lua_getstring(obj)))
        lua_pushnumber(1);
    else
        lua_pushnil();
}

/*
** Internal function: do a string
*/
void lua_internaldostring(void)
{
    lua_Object obj = lua_getparam(1);
    if (lua_isstring(obj) && !lua_dostring(lua_getstring(obj)))
        lua_pushnumber(1);
    else
        lua_pushnil();
}
//opcode.c end

//strlib.c start
/*
** Return the position of the first caracter of a substring into a string
** LUA interface:
**			n = strfind (string, substring)
*/
static void str_find(void)
{
    char *s1, *s2, *f;
    lua_Object o1 = lua_getparam(1);
    lua_Object o2 = lua_getparam(2);
    if (!lua_isstring(o1) || !lua_isstring(o2))
    {
        lua_error("incorrect arguments to function `strfind'"); return;
    }
    s1 = lua_getstring(o1);
    s2 = lua_getstring(o2);
    f = strstr(s1, s2);
    if (f != NULL)
        lua_pushnumber(f - s1 + 1);
    else
        lua_pushnil();
}

/*
** Return the string length
** LUA interface:
**			n = strlen (string)
*/
static void str_len(void)
{
    lua_Object o = lua_getparam(1);
    if (!lua_isstring(o))
    {
        lua_error("incorrect arguments to function `strlen'"); return;
    }
    lua_pushnumber(strlen(lua_getstring(o)));
}


/*
** Return the substring of a string, from start to end
** LUA interface:
**			substring = strsub (string, start, end)
*/
static void str_sub(void)
{
    int start, end;
    char *s;
    lua_Object o1 = lua_getparam(1);
    lua_Object o2 = lua_getparam(2);
    lua_Object o3 = lua_getparam(3);
    if (!lua_isstring(o1) || !lua_isnumber(o2))
    {
        lua_error("incorrect arguments to function `strsub'"); return;
    }
    if (o3 != NULL && !lua_isnumber(o3))
    {
        lua_error("incorrect third argument to function `strsub'"); return;
    }
    s = lua_copystring(o1);
    start = lua_getnumber(o2);
    end = o3 == NULL ? strlen(s) : lua_getnumber(o3);
    if (end < start || start < 1 || end > strlen(s))
        lua_pushstring("");
    else
    {
        s[end] = 0;
        lua_pushstring(&s[start - 1]);
    }
    free(s);
}

/*
** Convert a string to lower case.
** LUA interface:
**			lowercase = strlower (string)
*/
static void str_lower(void)
{
    char *s, *c;
    lua_Object o = lua_getparam(1);
    if (!lua_isstring(o))
    {
        lua_error("incorrect arguments to function `strlower'"); return;
    }
    c = s = strdup(lua_getstring(o));
    while (*c != 0)
    {
        *c = tolower(*c);
        c++;
    }
    lua_pushstring(s);
    free(s);
}


/*
** Convert a string to upper case.
** LUA interface:
**			uppercase = strupper (string)
*/
static void str_upper(void)
{
    char *s, *c;
    lua_Object o = lua_getparam(1);
    if (!lua_isstring(o))
    {
        lua_error("incorrect arguments to function `strlower'"); return;
    }
    c = s = strdup(lua_getstring(o));
    while (*c != 0)
    {
        *c = toupper(*c);
        c++;
    }
    lua_pushstring(s);
    free(s);
}


/*
** Open string library
*/
void strlib_open(void)
{
    lua_register("strfind", str_find);
    lua_register("strlen", str_len);
    lua_register("strsub", str_sub);
    lua_register("strlower", str_lower);
    lua_register("strupper", str_upper);
}

//strlib.c end

//table.c start
#ifndef MAXSYMBOL
#define MAXSYMBOL	512
#endif
static Symbol  		tablebuffer[MAXSYMBOL] = {
    { "type",{ T_CFUNCTION,{ lua_type } } },
    { "tonumber",{ T_CFUNCTION,{ lua_obj2number } } },
    { "next",{ T_CFUNCTION,{ lua_next } } },
    { "nextvar",{ T_CFUNCTION,{ lua_nextvar } } },
    { "print",{ T_CFUNCTION,{ lua_print } } },
    { "dofile",{ T_CFUNCTION,{ lua_internaldofile } } },
    { "dostring",{ T_CFUNCTION,{ lua_internaldostring } } }
};
Symbol	       	       *lua_table = tablebuffer;
Word   	 		lua_ntable = 7;

struct List
{
    Symbol *s;
    struct List *next;
};

static struct List o6 = { tablebuffer + 6, 0 };
static struct List o5 = { tablebuffer + 5, &o6 };
static struct List o4 = { tablebuffer + 4, &o5 };
static struct List o3 = { tablebuffer + 3, &o4 };
static struct List o2 = { tablebuffer + 2, &o3 };
static struct List o1 = { tablebuffer + 1, &o2 };
static struct List o0 = { tablebuffer + 0, &o1 };
static struct List *searchlist = &o0;

#ifndef MAXCONSTANT
#define MAXCONSTANT	256
#endif
/* pre-defined constants need garbage collection extra byte */
static char tm[] = " mark";
static char ti[] = " nil";
static char tn[] = " number";
static char ts[] = " string";
static char tt[] = " table";
static char tf[] = " function";
static char tc[] = " cfunction";
static char tu[] = " userdata";
static char  	       *constantbuffer[MAXCONSTANT] = { tm + 1, ti + 1,
tn + 1, ts + 1,
tt + 1, tf + 1,
tc + 1, tu + 1
};
char  	      	      **lua_constant = constantbuffer;
Word    		lua_nconstant = T_USERDATA + 1;

#ifndef MAXSTRING
#define MAXSTRING	512
#endif
static char 	       *stringbuffer[MAXSTRING];
char  		      **lua_string = stringbuffer;
Word    		lua_nstring = 0;

#define MAXFILE 	20
char  		       *lua_file[MAXFILE];
int      		lua_nfile;


#define markstring(s)   (*((s)-1))


/* Variables to controll garbage collection */
Word lua_block = 10; /* to check when garbage collector will be called */
Word lua_nentity;   /* counter of new entities (strings and arrays) */


                    /*
                    ** Given a name, search it at symbol table and return its index. If not
                    ** found, allocate at end of table, checking oveflow and return its index.
                    ** On error, return -1.
                    */
int lua_findsymbol(char *s)
{
    struct List *l, *p;
    for (p = NULL, l = searchlist; l != NULL; p = l, l = l->next)
        if (streq(s, l->s->name))
        {
            if (p != NULL)
            {
                p->next = l->next;
                l->next = searchlist;
                searchlist = l;
            }
            return (l->s - lua_table);
        }

    if (lua_ntable >= MAXSYMBOL - 1)
    {
        lua_error("symbol table overflow");
        return -1;
    }
    s_name(lua_ntable) = strdup(s);
    if (s_name(lua_ntable) == NULL)
    {
        lua_error("not enough memory");
        return -1;
    }
    s_tag(lua_ntable) = T_NIL;
    p = malloc(sizeof(*p));
    p->s = lua_table + lua_ntable;
    p->next = searchlist;
    searchlist = p;

    return lua_ntable++;
}

/*
** Given a constant string, search it at constant table and return its index.
** If not found, allocate at end of the table, checking oveflow and return
** its index.
**
** For each allocation, the function allocate a extra char to be used to
** mark used string (it's necessary to deal with constant and string
** uniformily). The function store at the table the second position allocated,
** that represents the beginning of the real string. On error, return -1.
**
*/
int lua_findconstant(char *s)
{
    int i;
    for (i = 0; i<lua_nconstant; i++)
        if (streq(s, lua_constant[i]))
            return i;
    if (lua_nconstant >= MAXCONSTANT - 1)
    {
        lua_error("lua: constant string table overflow");
        return -1;
    }
    {
        char *c = calloc(strlen(s) + 2, sizeof(char));
        c++;		/* create mark space */
        lua_constant[lua_nconstant++] = strcpy(c, s);
    }
    return (lua_nconstant - 1);
}


/*
** Traverse symbol table objects
*/
void lua_travsymbol(void(*fn)(Object *))
{
    int i;
    for (i = 0; i<lua_ntable; i++)
        fn(&s_object(i));
}


/*
** Mark an object if it is a string or a unmarked array.
*/
void lua_markobject(Object *o)
{
    if (tag(o) == T_STRING)
        markstring(svalue(o)) = 1;
    else if (tag(o) == T_ARRAY)
        lua_hashmark(avalue(o));
}


/*
** Garbage collection.
** Delete all unused strings and arrays.
*/
void lua_pack(void)
{
    /* mark stack strings */
    lua_travstack(lua_markobject);

    /* mark symbol table strings */
    lua_travsymbol(lua_markobject);

    lua_stringcollector();
    lua_hashcollector();

    lua_nentity = 0;      /* reset counter */
}

/*
** Garbage collection to atrings.
** Delete all unmarked strings
*/
void lua_stringcollector(void)
{
    int i, j;
    for (i = j = 0; i<lua_nstring; i++)
        if (markstring(lua_string[i]) == 1)
        {
            lua_string[j++] = lua_string[i];
            markstring(lua_string[i]) = 0;
        }
        else
        {
            free(lua_string[i] - 1);
        }
    lua_nstring = j;
}

/*
** Allocate a new string at string table. The given string is already
** allocated with mark space and the function puts it at the end of the
** table, checking overflow, and returns its own pointer, or NULL on error.
*/
char *lua_createstring(char *s)
{
    int i;
    if (s == NULL) return NULL;

    for (i = 0; i<lua_nstring; i++)
        if (streq(s, lua_string[i]))
        {
            free(s - 1);
            return lua_string[i];
        }

    if (lua_nentity == lua_block || lua_nstring >= MAXSTRING - 1)
    {
        lua_pack();
        if (lua_nstring >= MAXSTRING - 1)
        {
            lua_error("string table overflow");
            return NULL;
        }
    }
    lua_string[lua_nstring++] = s;
    lua_nentity++;
    return s;
}

/*
** Add a file name at file table, checking overflow. This function also set
** the external variable "lua_filename" with the function filename set.
** Return 0 on success or 1 on error.
*/
int lua_addfile(char *fn)
{
    if (lua_nfile >= MAXFILE - 1)
    {
        lua_error("too many files");
        return 1;
    }
    if ((lua_file[lua_nfile++] = strdup(fn)) == NULL)
    {
        lua_error("not enough memory");
        return 1;
    }
    return 0;
}

/*
** Delete a file from file stack
*/
int lua_delfile(void)
{
    lua_nfile--;
    return 1;
}

/*
** Return the last file name set.
*/
char *lua_filename(void)
{
    return lua_file[lua_nfile - 1];
}

/*
** Internal function: return next global variable
*/
void lua_nextvar(void)
{
    int index;
    Object *o = lua_getparam(1);
    if (o == NULL)
    {
        lua_error("too few arguments to function `nextvar'"); return;
    }
    if (lua_getparam(2) != NULL)
    {
        lua_error("too many arguments to function `nextvar'"); return;
    }
    if (tag(o) == T_NIL)
    {
        index = 0;
    }
    else if (tag(o) != T_STRING)
    {
        lua_error("incorrect argument to function `nextvar'");
        return;
    }
    else
    {
        for (index = 0; index<lua_ntable; index++)
            if (streq(s_name(index), svalue(o))) break;
        if (index == lua_ntable)
        {
            lua_error("name not found in function `nextvar'");
            return;
        }
        index++;
        while (index < lua_ntable && tag(&s_object(index)) == T_NIL) index++;

        if (index == lua_ntable)
        {
            lua_pushnil();
            lua_pushnil();
            return;
        }
    }
    {
        Object name;
        tag(&name) = T_STRING;
        svalue(&name) = lua_createstring(lua_strdup(s_name(index)));
        if (lua_pushobject(&name)) return;
        if (lua_pushobject(&s_object(index))) return;
    }
}
//table.c end

//y.tab.c start
#define LISTING 0

#ifndef GAPCODE
#define GAPCODE 50
#endif
static Word   maxcode;
static Word   maxmain;
static Word   maxcurr;
static Byte  *code = NULL;
static Byte  *initcode;
static Byte  *basepc;
static Word   maincode;
static Word   pc;

#define MAXVAR 32
static long    varbuffer[MAXVAR];    /* variables in an assignment list;
                                     it's long to store negative Word values */
static int     nvarbuffer = 0;	     /* number of variables at a list */

static Word    localvar[STACKGAP];   /* store local variable names */
static int     nlocalvar = 0;	     /* number of local variables */

#define MAXFIELDS FIELDS_PER_FLUSH*2
static Word    fields[MAXFIELDS];     /* fieldnames to be flushed */
static int     nfields = 0;
static int     ntemp;		     /* number of temporary var into stack */
static int     err;		     /* flag to indicate error */

                             /* Internal functions */

static void code_byte(Byte c)
{
    if (pc>maxcurr - 2)  /* 1 byte free to code HALT of main code */
    {
        maxcurr += GAPCODE;
        basepc = (Byte *)realloc(basepc, maxcurr*sizeof(Byte));
        if (basepc == NULL)
        {
            lua_error("not enough memory");
            err = 1;
        }
    }
    basepc[pc++] = c;
}

static void code_word(Word n)
{
    CodeWord code;
    code.w = n;
    code_byte(code.m.c1);
    code_byte(code.m.c2);
}

static void code_float(float n)
{
    CodeFloat code;
    code.f = n;
    code_byte(code.m.c1);
    code_byte(code.m.c2);
    code_byte(code.m.c3);
    code_byte(code.m.c4);
}

static void code_word_at(Byte *p, Word n)
{
    CodeWord code;
    code.w = n;
    *p++ = code.m.c1;
    *p++ = code.m.c2;
}

static void push_field(Word name)
{
    if (nfields < STACKGAP - 1)
        fields[nfields++] = name;
    else
    {
        lua_error("too many fields in a constructor");
        err = 1;
    }
}

static void flush_record(int n)
{
    int i;
    if (n == 0) return;
    code_byte(STORERECORD);
    code_byte(n);
    for (i = 0; i<n; i++)
        code_word(fields[--nfields]);
    ntemp -= n;
}

static void flush_list(int m, int n)
{
    if (n == 0) return;
    if (m == 0)
        code_byte(STORELIST0);
    else
    {
        code_byte(STORELIST);
        code_byte(m);
    }
    code_byte(n);
    ntemp -= n;
}

static void incr_ntemp(void)
{
    if (ntemp + nlocalvar + MAXVAR + 1 < STACKGAP)
        ntemp++;
    else
    {
        lua_error("stack overflow");
        err = 1;
    }
}

static void add_nlocalvar(int n)
{
    if (ntemp + nlocalvar + MAXVAR + n < STACKGAP)
        nlocalvar += n;
    else
    {
        lua_error("too many local variables or expression too complicate");
        err = 1;
    }
}

static void incr_nvarbuffer(void)
{
    if (nvarbuffer < MAXVAR - 1)
        nvarbuffer++;
    else
    {
        lua_error("variable buffer overflow");
        err = 1;
    }
}

static void code_number(float f)
{
    Word i = (Word)f;
    if (f == (float)i)  /* f has an (short) integer value */
    {
        if (i <= 2) code_byte(PUSH0 + i);
        else if (i <= 255)
        {
            code_byte(PUSHBYTE);
            code_byte(i);
        }
        else
        {
            code_byte(PUSHWORD);
            code_word(i);
        }
    }
    else
    {
        code_byte(PUSHFLOAT);
        code_float(f);
    }
    incr_ntemp();
}

/*
** Search a local name and if find return its index. If do not find return -1
*/
static int lua_localname(Word n)
{
    int i;
    for (i = nlocalvar - 1; i >= 0; i--)
        if (n == localvar[i]) return i;	/* local var */
    return -1;		        /* global var */
}

/*
** Push a variable given a number. If number is positive, push global variable
** indexed by (number -1). If negative, push local indexed by ABS(number)-1.
** Otherwise, if zero, push indexed variable (record).
*/
static void lua_pushvar(long number)
{
    if (number > 0)	/* global var */
    {
        code_byte(PUSHGLOBAL);
        code_word(number - 1);
        incr_ntemp();
    }
    else if (number < 0)	/* local var */
    {
        number = (-number) - 1;
        if (number < 10) code_byte(PUSHLOCAL0 + number);
        else
        {
            code_byte(PUSHLOCAL);
            code_byte(number);
        }
        incr_ntemp();
    }
    else
    {
        code_byte(PUSHINDEXED);
        ntemp--;
    }
}

static void lua_codeadjust(int n)
{
    code_byte(ADJUST);
    code_byte(n + nlocalvar);
}

static void lua_codestore(int i)
{
    if (varbuffer[i] > 0)		/* global var */
    {
        code_byte(STOREGLOBAL);
        code_word(varbuffer[i] - 1);
    }
    else if (varbuffer[i] < 0)      /* local var */
    {
        int number = (-varbuffer[i]) - 1;
        if (number < 10) code_byte(STORELOCAL0 + number);
        else
        {
            code_byte(STORELOCAL);
            code_byte(number);
        }
    }
    else				  /* indexed var */
    {
        int j;
        int upper = 0;     	/* number of indexed variables upper */
        int param;		/* number of itens until indexed expression */
        for (j = i + 1; j <nvarbuffer; j++)
            if (varbuffer[j] == 0) upper++;
        param = upper * 2 + i;
        if (param == 0)
            code_byte(STOREINDEXED0);
        else
        {
            code_byte(STOREINDEXED);
            code_byte(param);
        }
    }
}

void yyerror(char *s)
{
    static char msg[256];
    sprintf(msg, "%s near \"%s\" at line %d in file \"%s\"",
        s, lua_lasttext(), lua_linenumber, lua_filename());
    lua_error(msg);
    err = 1;
}

int yywrap(void)
{
    return 1;
}


/*
** Parse LUA code and execute global statement.
** Return 0 on success or 1 on error.
*/
int lua_parse(void)
{
    Byte *init = initcode = (Byte *)calloc(GAPCODE, sizeof(Byte));
    maincode = 0;
    maxmain = GAPCODE;
    if (init == NULL)
    {
        lua_error("not enough memory");
        return 1;
    }
    err = 0;
    if (yyparse() || (err == 1)) return 1;
    initcode[maincode++] = HALT;
    init = initcode;
#if LISTING
    PrintCode(init, init + maincode);
#endif
    if (lua_execute(init)) return 1;
    free(init);
    return 0;
}

int yyexca[] = {
    -1, 1,
    0, -1,
    -2, 2,
    -1, 20,
    40, 67,
    91, 94,
    46, 96,
    -2, 91,
    -1, 32,
    40, 67,
    91, 94,
    46, 96,
    -2, 51,
    -1, 73,
    275, 34,
    276, 34,
    61, 34,
    277, 34,
    62, 34,
    60, 34,
    278, 34,
    279, 34,
    280, 34,
    43, 34,
    45, 34,
    42, 34,
    47, 34,
    -2, 70,
    -1, 74,
    91, 94,
    46, 96,
    -2, 92,
    -1, 105,
    261, 28,
    262, 28,
    266, 28,
    267, 28,
    268, 28,
    -2, 11,
    -1, 125,
    268, 31,
    -2, 30,
    -1, 146,
    275, 34,
    276, 34,
    61, 34,
    277, 34,
    62, 34,
    60, 34,
    278, 34,
    279, 34,
    280, 34,
    43, 34,
    45, 34,
    42, 34,
    47, 34,
    -2, 72,
};
# define YYNPROD 103
# define YYLAST 364
int yyact[] = {

    58,    56,    22,    57,   132,    59,    58,    56,   137,    57,
    110,    59,    58,    56,   107,    57,    85,    59,    51,    50,
    52,    82,    23,    43,    51,    50,    52,    58,    56,     9,
    57,   157,    59,    58,    56,   165,    57,     5,    59,   162,
    6,   161,   104,   154,   155,    51,    50,    52,    64,   153,
    70,    51,    50,    52,    26,    58,    56,   127,    57,    10,
    59,   111,    25,    78,    27,    58,    56,    28,    57,    29,
    59,   131,   147,    51,    50,    52,     7,    65,    66,   115,
    150,   112,    63,    51,    50,    52,    68,    69,    31,   159,
    11,    79,    58,    76,   128,    73,    41,    59,   151,    87,
    88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
    77,   114,   148,    40,    58,    56,   102,    57,   106,    59,
    117,    32,    72,   121,   116,   100,    80,   109,    67,    48,
    20,    36,    73,    30,    45,    73,    44,   118,   149,    84,
    17,   126,    18,    46,    21,    47,   120,   119,   101,   145,
    144,   125,    71,   123,    75,    39,    38,    12,     8,   108,
    105,   136,    83,    74,   135,    24,     4,     3,   139,   140,
    2,    81,   134,   141,   133,   130,   129,    42,   113,    16,
    1,   146,   124,     0,   143,     0,     0,   152,     0,     0,
    0,    86,     0,     0,     0,     0,     0,    13,     0,     0,
    160,    14,     0,    15,   164,   163,     0,    19,   167,     0,
    0,    23,    73,     0,     0,     0,     0,     0,   168,   166,
    158,   171,   173,     0,     0,     0,   169,     0,     0,     0,
    0,     0,     0,    61,    62,    53,    54,    55,    60,    61,
    62,    53,    54,    55,    60,     0,     0,     0,     0,   103,
    60,    49,     0,    98,    99,     0,     0,     0,     0,     0,
    61,    62,    53,    54,    55,    60,    61,    62,    53,    54,
    55,    60,     0,     0,     0,     0,     0,     0,     0,     0,
    0,     0,    35,     0,     0,     0,     0,     0,    61,    62,
    53,    54,    55,    60,    33,   122,    34,    23,     0,     0,
    53,    54,    55,    60,     0,     0,    37,     0,     0,     0,
    138,     0,     0,     0,     0,   142,     0,     0,     0,     0,
    0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    0,     0,     0,   156,     0,     0,     0,     0,     0,     0,
    0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    170,     0,     0,   172 };
int yypact[] = {

    -1000,  -234, -1000, -1000, -1000,  -244, -1000,    31,   -62, -1000,
    -1000, -1000, -1000,    24, -1000, -1000,    52, -1000, -1000,  -250,
    -1000, -1000, -1000, -1000,    89,    -9, -1000,    24,    24,    24,
    -1000,    88, -1000, -1000, -1000, -1000, -1000,    24,    24, -1000,
    24,  -251,    49, -1000,   -28,    45,    86,  -252,  -257, -1000,
    24,    24,    24,    24,    24,    24,    24,    24,    24,    24,
    24, -1000, -1000,    84,    13, -1000, -1000,    24, -1000,   -15,
    -224, -1000,    74, -1000, -1000, -1000,  -259,    24,    24,  -263,
    24,   -12, -1000,    83,    76, -1000, -1000,   -30,   -30,   -30,
    -30,   -30,   -30,    50,    50, -1000, -1000,    72, -1000, -1000,
    -1000,    82,    13, -1000,    24, -1000, -1000, -1000,    74,   -36,
    -1000,    53,    74, -1000,  -269,    24, -1000,  -265, -1000,    24,
    24, -1000, -1000,    13,    31, -1000,    24, -1000, -1000,   -53,
    68, -1000, -1000,   -13,    54,    13, -1000, -1000,  -218,    23,
    23, -1000, -1000, -1000, -1000,  -237, -1000, -1000,  -269,    28,
    -1000,    24,  -226,  -228, -1000,    24,  -232,    24, -1000,    24,
    13, -1000, -1000, -1000,   -42, -1000,    31,    13, -1000, -1000,
    -1000, -1000,  -218, -1000 };
int yypgo[] = {

    0,   180,   191,    54,    61,    81,   179,   133,   178,   177,
    176,   175,   174,   172,   121,   171,   170,    76,    59,   167,
    166,   165,   162,   161,    50,   160,   158,   157,    48,    49,
    156,   155,   131,   154,   152,   151,   150,   149,   148,   147,
    146,   145,   144,   143,   141,   139,    71,   138,   136,   134 };
int yyr1[] = {

    0,     1,    16,     1,     1,     1,    21,    23,    19,    25,
    25,    26,    17,    18,    18,    27,    30,    27,    31,    27,
    27,    27,    27,    27,    29,    29,    29,    34,    35,    24,
    36,    37,    36,     2,    28,     3,     3,     3,     3,     3,
    3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
    3,     3,     3,     3,     3,     3,     3,    39,     3,    40,
    3,    41,     7,    38,    38,    43,    32,    42,     4,     4,
    5,    44,     5,    22,    22,    45,    45,    15,    15,     8,
    8,    10,    10,    11,    11,    47,    46,    12,    12,    13,
    13,     6,     6,    14,    48,    14,    49,    14,     9,     9,
    33,    33,    20 };
int yyr2[] = {

    0,     0,     1,     9,     4,     4,     1,     1,    19,     0,
    6,     1,     4,     0,     2,    17,     1,    17,     1,    13,
    7,     3,     3,     7,     0,     4,    15,     1,     1,     9,
    0,     1,     9,     1,     3,     7,     7,     7,     7,     7,
    7,     7,     7,     7,     7,     7,     7,     5,     5,     3,
    9,     3,     3,     3,     3,     3,     5,     1,    11,     1,
    11,     1,     9,     1,     2,     1,    11,     3,     1,     3,
    3,     1,     9,     0,     2,     3,     7,     1,     3,     7,
    7,     1,     3,     3,     7,     1,     9,     1,     3,     3,
    7,     3,     7,     3,     1,    11,     1,     9,     3,     7,
    0,     4,     3 };
int yychk[] = {

    -1000,    -1,   -16,   -19,   -20,   271,   274,   -17,   -26,   273,
    -18,    59,   -27,   259,   263,   265,    -6,   -32,    -7,   269,
    -14,   -42,    64,   273,   -21,   -28,    -3,    40,    43,    45,
    -7,    64,   -14,   270,   272,   258,   -32,   282,   -30,   -31,
    61,    44,    -9,   273,   -48,   -49,   -43,   -41,    40,   260,
    61,    60,    62,   277,   278,   279,    43,    45,    42,    47,
    280,   275,   276,    -3,   -28,   -28,   -28,    40,   -28,   -28,
    -24,   -34,    -5,    -3,   -14,   -33,    44,    61,    91,    46,
    40,   -15,   273,   -22,   -45,   273,    -2,   -28,   -28,   -28,
    -28,   -28,   -28,   -28,   -28,   -28,   -28,   -28,    -2,    -2,
    41,   -38,   -28,   264,   266,   -25,    44,   273,    -5,   -28,
    273,    -4,    -5,    -8,   123,    91,    41,    44,   -24,   -39,
    -40,    41,    -2,   -28,   -17,   -35,   -44,    93,    41,   -10,
    -11,   -46,   273,   -12,   -13,   -28,   -23,   273,    -2,   -28,
    -28,   -24,    -2,   -18,   -36,   -37,    -3,   125,    44,   -47,
    93,    44,   -24,   -29,   261,   262,    -2,   268,   -46,    61,
    -28,   267,   267,   -24,   -28,   267,    -4,   -28,   260,   -18,
    -2,   -24,    -2,   -29 };
int yydef[] = {

    1,    -2,    11,     4,     5,     0,   102,    13,     0,     6,
    3,    14,    12,     0,    16,    18,     0,    21,    22,     0,
    -2,    65,    61,    93,     0,     0,    34,     0,     0,     0,
    49,    61,    -2,    52,    53,    54,    55,     0,     0,    27,
    0,     0,   100,    98,     0,     0,     0,    77,    73,    33,
    0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    0,    33,    33,    34,     0,    47,    48,    63,    56,     0,
    0,     9,    20,    -2,    -2,    23,     0,     0,     0,     0,
    68,     0,    78,     0,    74,    75,    27,    36,    37,    38,
    39,    40,    41,    42,    43,    44,    45,    46,    57,    59,
    35,     0,    64,    33,     0,    -2,    71,    99,   101,     0,
    97,     0,    69,    62,    81,    87,     7,     0,    33,     0,
    0,    50,    27,    33,    13,    -2,     0,    95,    66,     0,
    82,    83,    85,     0,    88,    89,    27,    76,    24,    58,
    60,    33,    19,    10,    29,     0,    -2,    79,     0,     0,
    80,     0,     0,     0,    27,     0,     0,    68,    84,     0,
    90,     8,    15,    25,     0,    17,    13,    86,    33,    32,
    27,    33,    24,    26 };
typedef struct { char *t_name; int t_val; } yytoktype;

/*
** Skeleton parser driver for yacc output
*/

/*
** yacc user known macros and defines
*/
#define YYERROR goto yyerrlab
#define YYERROR		goto yyerrlab
#define YYACCEPT	{ free(yys); free(yyv); return(0); }
#define YYABORT		{ free(yys); free(yyv); return(1); }
#define YYBACKUP( newtoken, newvalue )\
{\
	if ( yychar >= 0 || ( yyr2[ yytmp ] >> 1 ) != 1 )\
	{\
		yyerror( "syntax error - cannot backup" );\
		goto yyerrlab;\
	}\
	yychar = newtoken;\
	yystate = *yyps;\
	yylval = newvalue;\
	goto yynewstate;\
}
#define YYRECOVERING()	(!!yyerrflag)

/*
** user known globals
*/
int yydebug;			/* set to 1 to get debugging */

                        /*
                        ** driver internal defines
                        */
#define YYFLAG		(-1000)

                        /*
                        ** static variables used by the parser
                        */
static YYSTYPE *yyv;			/* value stack */
static int *yys;			/* state stack */

static YYSTYPE *yypv;			/* top of value stack */
static int *yyps;			/* top of state stack */

static int yystate;			/* current state */
static int yytmp;			/* extra var (lasts between blocks) */

int yynerrs;			/* number of errors */

int yyerrflag;			/* error recovery flag */
int yychar;			/* current input token number */


                    /*
                    ** yyparse - return 0 if worked, 1 if syntax error not recovered from
                    */
int
yyparse()
{
    register YYSTYPE *yypvt;	/* top of value stack for $vars */
    unsigned yymaxdepth = YYMAXDEPTH;

    /*
    ** Initialize externals - yyparse may be called more than once
    */
    yyv = (YYSTYPE*)malloc(yymaxdepth*sizeof(YYSTYPE));
    yys = (int*)malloc(yymaxdepth*sizeof(int));
    if (!yyv || !yys)
    {
        yyerror("out of memory");
        return(1);
    }
    yypv = &yyv[-1];
    yyps = &yys[-1];
    yystate = 0;
    yytmp = 0;
    yynerrs = 0;
    yyerrflag = 0;
    yychar = -1;

    goto yystack;
    {
        register YYSTYPE *yy_pv;	/* top of value stack */
        register int *yy_ps;		/* top of state stack */
        register int yy_state;		/* current state */
        register int  yy_n;		/* internal state number info */

                                /*
                                ** get globals into registers.
                                ** branch to here only if YYBACKUP was called.
                                */
    yynewstate:
        yy_pv = yypv;
        yy_ps = yyps;
        yy_state = yystate;
        goto yy_newstate;

        /*
        ** get globals into registers.
        ** either we just started, or we just finished a reduction
        */
    yystack:
        yy_pv = yypv;
        yy_ps = yyps;
        yy_state = yystate;

        /*
        ** top of for (;;) loop while no reductions done
        */
    yy_stack:
        /*
        ** put a state and value onto the stacks
        */
        if (++yy_ps >= &yys[yymaxdepth])	/* room on stack? */
        {
            /*
            ** reallocate and recover.  Note that pointers
            ** have to be reset, or bad things will happen
            */
            int yyps_index = (yy_ps - yys);
            int yypv_index = (yy_pv - yyv);
            int yypvt_index = (yypvt - yyv);
            yymaxdepth += YYMAXDEPTH;
            yyv = (YYSTYPE*)realloc((char*)yyv,
                yymaxdepth * sizeof(YYSTYPE));
            yys = (int*)realloc((char*)yys,
                yymaxdepth * sizeof(int));
            if (!yyv || !yys)
            {
                yyerror("yacc stack overflow");
                return(1);
            }
            yy_ps = yys + yyps_index;
            yy_pv = yyv + yypv_index;
            yypvt = yyv + yypvt_index;
        }
        *yy_ps = yy_state;
        *++yy_pv = yyval;

        /*
        ** we have a new state - find out what to do
        */
     yy_newstate:
        if ((yy_n = yypact[yy_state]) <= YYFLAG)
            goto yydefault;		/* simple state */
        if ((yychar < 0) && ((yychar = yylex()) < 0))
            yychar = 0;		/* reached EOF */
        if (((yy_n += yychar) < 0) || (yy_n >= YYLAST))
            goto yydefault;
        if (yychk[yy_n = yyact[yy_n]] == yychar)	/*valid shift*/
        {
            yychar = -1;
            yyval = yylval;
            yy_state = yy_n;
            if (yyerrflag > 0)
                yyerrflag--;
            goto yy_stack;
        }

    yydefault:
        if ((yy_n = yydef[yy_state]) == -2)
        {
            if ((yychar < 0) && ((yychar = yylex()) < 0))
                yychar = 0;		/* reached EOF */
            /*
            ** look through exception table
            */
            {
                register int *yyxi = yyexca;

                while ((*yyxi != -1) ||
                    (yyxi[1] != yy_state))
                {
                    yyxi += 2;
                }
                while ((*(yyxi += 2) >= 0) &&
                    (*yyxi != yychar))
                    ;
                if ((yy_n = yyxi[1]) < 0)
                    YYACCEPT;
            }
        }

        /*
        ** check for syntax error
        */
        if (yy_n == 0)	/* have an error */
        {
            /* no worry about speed here! */
            switch (yyerrflag)
            {
            case 0:		/* new error */
                yyerror("syntax error");
                goto skip_init;
            //yyerrlab:
                /*
                ** get globals into registers.
                ** we have a user generated syntax type error
                */
               // yy_pv = yypv;
               // yy_ps = yyps;
              //  yy_state = yystate;
               // yynerrs++;
            skip_init:
            case 1:
            case 2:		/* incompletely recovered error */
                        /* try again... */
                yyerrflag = 3;
                /*
                ** find state where "error" is a legal
                ** shift action
                */
                while (yy_ps >= yys)
                {
                    yy_n = yypact[*yy_ps] + YYERRCODE;
                    if (yy_n >= 0 && yy_n < YYLAST &&
                        yychk[yyact[yy_n]] == YYERRCODE) {
                        /*
                        ** simulate shift of "error"
                        */
                        yy_state = yyact[yy_n];
                        goto yy_stack;
                    }
                    /*
                    ** current state has no shift on
                    ** "error", pop stack
                    */
                    yy_ps--;
                    yy_pv--;
                }
                /*
                ** there is no state on stack with "error" as
                ** a valid shift.  give up.
                */
                YYABORT;
            case 3:		/* no shift yet; eat a token */

                if (yychar == 0)	/* reached EOF. quit */
                    YYABORT;
                yychar = -1;
                goto yy_newstate;
            }
        }/* end if ( yy_n == 0 ) */
         /*
         ** reduction by production yy_n
         ** put stack tops, etc. so things right after switch
         */
        yytmp = yy_n;			/* value to switch over */
        yypvt = yy_pv;			/* $vars top of value stack */
                                /*
                                ** Look in goto table for next state
                                ** Sorry about using yy_state here as temporary
                                ** register variable, but why not, if it works...
                                ** If yyr2[ yy_n ] doesn't have the low order bit
                                ** set, then there is no action to be done for
                                ** this reduction.  So, no saving & unsaving of
                                ** registers done.  The only difference between the
                                ** code just after the if and the body of the if is
                                ** the goto yy_stack in the body.  This way the test
                                ** can be made before the choice of what to do is needed.
                                */
        {
            /* length of production doubled with extra bit */
            register int yy_len = yyr2[yy_n];

            if (!(yy_len & 01))
            {
                yy_len >>= 1;
                yyval = (yy_pv -= yy_len)[1];	/* $$ = $1 */
                yy_state = yypgo[yy_n = yyr1[yy_n]] +
                    *(yy_ps -= yy_len) + 1;
                if (yy_state >= YYLAST ||
                    yychk[yy_state =
                    yyact[yy_state]] != -yy_n)
                {
                    yy_state = yyact[yypgo[yy_n]];
                }
                goto yy_stack;
            }
            yy_len >>= 1;
            yyval = (yy_pv -= yy_len)[1];	/* $$ = $1 */
            yy_state = yypgo[yy_n = yyr1[yy_n]] +
                *(yy_ps -= yy_len) + 1;
            if (yy_state >= YYLAST ||
                yychk[yy_state = yyact[yy_state]] != -yy_n)
            {
                yy_state = yyact[yypgo[yy_n]];
            }
        }
        /* save until reenter driver code */
        yystate = yy_state;
        yyps = yy_ps;
        yypv = yy_pv;
    }
    /*
    ** code supplied by user is placed in this switch
    */
    switch (yytmp)
    {

    case 2:
    {
        pc = maincode; basepc = initcode; maxcurr = maxmain;
        nlocalvar = 0;
    } break;
    case 3:
    {
        maincode = pc; initcode = basepc; maxmain = maxcurr;
    } break;
    case 6:
    {
        if (code == NULL)	/* first function */
        {
            code = (Byte *)calloc(GAPCODE, sizeof(Byte));
            if (code == NULL)
            {
                lua_error("not enough memory");
                err = 1;
            }
            maxcode = GAPCODE;
        }
        pc = 0; basepc = code; maxcurr = maxcode;
        nlocalvar = 0;
        yyval.vWord = lua_findsymbol(yypvt[-0].pChar);
    } break;
    case 7:
    {
        if (lua_debug)
        {
            code_byte(SETFUNCTION);
            code_word(lua_nfile - 1);
            code_word(yypvt[-3].vWord);
        }
        lua_codeadjust(0);
    } break;
    case 8:
    {
        if (lua_debug) code_byte(RESET);
        code_byte(RETCODE); code_byte(nlocalvar);
        s_tag(yypvt[-6].vWord) = T_FUNCTION;
        s_bvalue(yypvt[-6].vWord) = calloc(pc, sizeof(Byte));
        if (s_bvalue(yypvt[-6].vWord) == NULL)
        {
            lua_error("not enough memory");
            err = 1;
        }
        memcpy(s_bvalue(yypvt[-6].vWord), basepc, pc*sizeof(Byte));
        code = basepc; maxcode = maxcurr;
    } break;
    case 11:
    {
        ntemp = 0;
        if (lua_debug)
        {
            code_byte(SETLINE); code_word(lua_linenumber);
        }
    } break;
    case 15:
    {
        {
            Word elseinit = yypvt[-2].vWord + sizeof(Word) + 1;
            if (pc - elseinit == 0)		/* no else */
            {
                pc -= sizeof(Word) + 1;
                elseinit = pc;
            }
            else
            {
                basepc[yypvt[-2].vWord] = JMP;
                code_word_at(basepc + yypvt[-2].vWord + 1, pc - elseinit);
            }
            basepc[yypvt[-4].vWord] = IFFJMP;
            code_word_at(basepc + yypvt[-4].vWord + 1, elseinit - (yypvt[-4].vWord + sizeof(Word) + 1));
        }
    } break;
    case 16:
    {yyval.vWord = pc; } break;
    case 17:
    {
        basepc[yypvt[-3].vWord] = IFFJMP;
        code_word_at(basepc + yypvt[-3].vWord + 1, pc - (yypvt[-3].vWord + sizeof(Word) + 1));

        basepc[yypvt[-1].vWord] = UPJMP;
        code_word_at(basepc + yypvt[-1].vWord + 1, pc - (yypvt[-6].vWord));
    } break;
    case 18:
    {yyval.vWord = pc; } break;
    case 19:
    {
        basepc[yypvt[-0].vWord] = IFFUPJMP;
        code_word_at(basepc + yypvt[-0].vWord + 1, pc - (yypvt[-4].vWord));
    } break;
    case 20:
    {
        {
            int i;
            if (yypvt[-0].vInt == 0 || nvarbuffer != ntemp - yypvt[-2].vInt * 2)
                lua_codeadjust(yypvt[-2].vInt * 2 + nvarbuffer);
            for (i = nvarbuffer - 1; i >= 0; i--)
                lua_codestore(i);
            if (yypvt[-2].vInt > 1 || (yypvt[-2].vInt == 1 && varbuffer[0] != 0))
                lua_codeadjust(0);
        }
    } break;
    case 21:
    { lua_codeadjust(0); } break;
    case 22:
    { lua_codeadjust(0); } break;
    case 23:
    { add_nlocalvar(yypvt[-1].vInt); lua_codeadjust(0); } break;
    case 26:
    {
        {
            Word elseinit = yypvt[-1].vWord + sizeof(Word) + 1;
            if (pc - elseinit == 0)		/* no else */
            {
                pc -= sizeof(Word) + 1;
                elseinit = pc;
            }
            else
            {
                basepc[yypvt[-1].vWord] = JMP;
                code_word_at(basepc + yypvt[-1].vWord + 1, pc - elseinit);
            }
            basepc[yypvt[-3].vWord] = IFFJMP;
            code_word_at(basepc + yypvt[-3].vWord + 1, elseinit - (yypvt[-3].vWord + sizeof(Word) + 1));
        }
    } break;
    case 27:
    {yyval.vInt = nlocalvar; } break;
    case 28:
    {ntemp = 0; } break;
    case 29:
    {
        if (nlocalvar != yypvt[-3].vInt)
        {
            nlocalvar = yypvt[-3].vInt;
            lua_codeadjust(0);
        }
    } break;
    case 31:
    { if (lua_debug) { code_byte(SETLINE); code_word(lua_linenumber); }} break;
    case 32:
    {
        if (lua_debug) code_byte(RESET);
        code_byte(RETCODE); code_byte(nlocalvar);
    } break;
    case 33:
    {
        yyval.vWord = pc;
        code_byte(0);		/* open space */
        code_word(0);
    } break;
    case 34:
    { if (yypvt[-0].vInt == 0) { lua_codeadjust(ntemp + 1); incr_ntemp(); }} break;
    case 35:
    { yyval.vInt = yypvt[-1].vInt; } break;
    case 36:
    { code_byte(EQOP);   yyval.vInt = 1; ntemp--; } break;
    case 37:
    { code_byte(LTOP);   yyval.vInt = 1; ntemp--; } break;
    case 38:
    { code_byte(LEOP); code_byte(NOTOP); yyval.vInt = 1; ntemp--; } break;
    case 39:
    { code_byte(EQOP); code_byte(NOTOP); yyval.vInt = 1; ntemp--; } break;
    case 40:
    { code_byte(LEOP);   yyval.vInt = 1; ntemp--; } break;
    case 41:
    { code_byte(LTOP); code_byte(NOTOP); yyval.vInt = 1; ntemp--; } break;
    case 42:
    { code_byte(ADDOP);  yyval.vInt = 1; ntemp--; } break;
    case 43:
    { code_byte(SUBOP);  yyval.vInt = 1; ntemp--; } break;
    case 44:
    { code_byte(MULTOP); yyval.vInt = 1; ntemp--; } break;
    case 45:
    { code_byte(DIVOP);  yyval.vInt = 1; ntemp--; } break;
    case 46:
    { code_byte(CONCOP);  yyval.vInt = 1; ntemp--; } break;
    case 47:
    { yyval.vInt = 1; } break;
    case 48:
    { code_byte(MINUSOP); yyval.vInt = 1; } break;
    case 49:
    { yyval.vInt = yypvt[-0].vInt; } break;
    case 50:
    {
        code_byte(CREATEARRAY);
        yyval.vInt = 1;
    } break;
    case 51:
    { lua_pushvar(yypvt[-0].vLong); yyval.vInt = 1; } break;
    case 52:
    { code_number(yypvt[-0].vFloat); yyval.vInt = 1; } break;
    case 53:
    {
        code_byte(PUSHSTRING);
        code_word(yypvt[-0].vWord);
        yyval.vInt = 1;
        incr_ntemp();
    } break;
    case 54:
    {code_byte(PUSHNIL); yyval.vInt = 1; incr_ntemp(); } break;
    case 55:
    {
        yyval.vInt = 0;
        if (lua_debug)
        {
            code_byte(SETLINE); code_word(lua_linenumber);
        }
    } break;
    case 56:
    { code_byte(NOTOP);  yyval.vInt = 1; } break;
    case 57:
    {code_byte(POP); ntemp--; } break;
    case 58:
    {
        basepc[yypvt[-2].vWord] = ONFJMP;
        code_word_at(basepc + yypvt[-2].vWord + 1, pc - (yypvt[-2].vWord + sizeof(Word) + 1));
        yyval.vInt = 1;
    } break;
    case 59:
    {code_byte(POP); ntemp--; } break;
    case 60:
    {
        basepc[yypvt[-2].vWord] = ONTJMP;
        code_word_at(basepc + yypvt[-2].vWord + 1, pc - (yypvt[-2].vWord + sizeof(Word) + 1));
        yyval.vInt = 1;
    } break;
    case 61:
    {
        code_byte(PUSHBYTE);
        yyval.vWord = pc; code_byte(0);
        incr_ntemp();
        code_byte(CREATEARRAY);
    } break;
    case 62:
    {
        basepc[yypvt[-2].vWord] = yypvt[-0].vInt;
        if (yypvt[-1].vLong < 0)	/* there is no function to be called */
        {
            yyval.vInt = 1;
        }
        else
        {
            lua_pushvar(yypvt[-1].vLong + 1);
            code_byte(PUSHMARK);
            incr_ntemp();
            code_byte(PUSHOBJECT);
            incr_ntemp();
            code_byte(CALLFUNC);
            ntemp -= 4;
            yyval.vInt = 0;
            if (lua_debug)
            {
                code_byte(SETLINE); code_word(lua_linenumber);
            }
        }
    } break;
    case 63:
    { code_byte(PUSHNIL); incr_ntemp(); } break;
    case 65:
    {code_byte(PUSHMARK); yyval.vInt = ntemp; incr_ntemp(); } break;
    case 66:
    { code_byte(CALLFUNC); ntemp = yypvt[-3].vInt - 1; } break;
    case 67:
    {lua_pushvar(yypvt[-0].vLong); } break;
    case 68:
    { yyval.vInt = 1; } break;
    case 69:
    { yyval.vInt = yypvt[-0].vInt; } break;
    case 70:
    { yyval.vInt = yypvt[-0].vInt; } break;
    case 71:
    {if (!yypvt[-1].vInt) { lua_codeadjust(ntemp + 1); incr_ntemp(); }} break;
    case 72:
    {yyval.vInt = yypvt[-0].vInt; } break;
    case 75:
    {
        localvar[nlocalvar] = lua_findsymbol(yypvt[-0].pChar);
        add_nlocalvar(1);
    } break;
    case 76:
    {
        localvar[nlocalvar] = lua_findsymbol(yypvt[-0].pChar);
        add_nlocalvar(1);
    } break;
    case 77:
    {yyval.vLong = -1; } break;
    case 78:
    {yyval.vLong = lua_findsymbol(yypvt[-0].pChar); } break;
    case 79:
    {
        flush_record(yypvt[-1].vInt%FIELDS_PER_FLUSH);
        yyval.vInt = yypvt[-1].vInt;
    } break;
    case 80:
    {
        flush_list(yypvt[-1].vInt / FIELDS_PER_FLUSH, yypvt[-1].vInt%FIELDS_PER_FLUSH);
        yyval.vInt = yypvt[-1].vInt;
    } break;
    case 81:
    { yyval.vInt = 0; } break;
    case 82:
    { yyval.vInt = yypvt[-0].vInt; } break;
    case 83:
    {yyval.vInt = 1; } break;
    case 84:
    {
        yyval.vInt = yypvt[-2].vInt + 1;
        if (yyval.vInt%FIELDS_PER_FLUSH == 0) flush_record(FIELDS_PER_FLUSH);
    } break;
    case 85:
    {yyval.vWord = lua_findconstant(yypvt[-0].pChar); } break;
    case 86:
    {
        push_field(yypvt[-2].vWord);
    } break;
    case 87:
    { yyval.vInt = 0; } break;
    case 88:
    { yyval.vInt = yypvt[-0].vInt; } break;
    case 89:
    {yyval.vInt = 1; } break;
    case 90:
    {
        yyval.vInt = yypvt[-2].vInt + 1;
        if (yyval.vInt%FIELDS_PER_FLUSH == 0)
            flush_list(yyval.vInt / FIELDS_PER_FLUSH - 1, FIELDS_PER_FLUSH);
    } break;
    case 91:
    {
        nvarbuffer = 0;
        varbuffer[nvarbuffer] = yypvt[-0].vLong; incr_nvarbuffer();
        yyval.vInt = (yypvt[-0].vLong == 0) ? 1 : 0;
    } break;
    case 92:
    {
        varbuffer[nvarbuffer] = yypvt[-0].vLong; incr_nvarbuffer();
        yyval.vInt = (yypvt[-0].vLong == 0) ? yypvt[-2].vInt + 1 : yypvt[-2].vInt;
    } break;
    case 93:
    {
        Word s = lua_findsymbol(yypvt[-0].pChar);
        int local = lua_localname(s);
        if (local == -1)	/* global var */
            yyval.vLong = s + 1;		/* return positive value */
        else
            yyval.vLong = -(local + 1);		/* return negative value */
    } break;
    case 94:
    {lua_pushvar(yypvt[-0].vLong); } break;
    case 95:
    {
        yyval.vLong = 0;		/* indexed variable */
    } break;
    case 96:
    {lua_pushvar(yypvt[-0].vLong); } break;
    case 97:
    {
        code_byte(PUSHSTRING);
        code_word(lua_findconstant(yypvt[-0].pChar)); incr_ntemp();
        yyval.vLong = 0;		/* indexed variable */
    } break;
    case 98:
    {localvar[nlocalvar] = lua_findsymbol(yypvt[-0].pChar); yyval.vInt = 1; } break;
    case 99:
    {
        localvar[nlocalvar + yypvt[-2].vInt] = lua_findsymbol(yypvt[-0].pChar);
        yyval.vInt = yypvt[-2].vInt + 1;
    } break;
    case 102:
    {lua_debug = yypvt[-0].vInt; } break;
    }
    goto yystack;		/* reset registers in driver code */
}

//y.tab.c end

//lua.c start
char *rcs_lua="$Id: lua.c,v 1.1 1993/12/17 18:41:19 celes Exp $";

int main (int argc, char *argv[])
{
    printf("start lua\n");
    //printf("argc: %d \n", argc);
    //printf("argv[0]: %s \n", argv[0]);
    //printf("argv[1]: %s \n", argv[1]);
    //printf("size of lua stack: %d \n", sizeof(stack));
 int i;
 iolib_open();
 strlib_open();
 mathlib_open();
 //printf("lib opened\n");
 if (argc < 2)
 {
     printf("start typing commands. seperate with ; once a newline is hit all commands execute together.\n");
     //char* bob = "print(\"hi there can i make this explode\"); print(\"sup sup sup sups upssss\"); print(\"hiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii\"";
     //x = "hi there good sir"; y = "wooooooooooooooooooooooot"; print(x); print(y)
   char buffer[200];
   do //this kind of loop works woo woo
   {
       memset(buffer, 0, sizeof(buffer));
       gets(buffer, sizeof(buffer));
       lua_dostring(buffer);
   } while (1);
 }
 else //read each file given on shell line and run it
 {
     printf("lua script filename: %s \n", argv[1]);
     ///*
     for (i = 1; i < argc; i++)
     {
         lua_dofile(argv[i]);
     }
     //*/
 }
 exit();
}
//lua.c end

