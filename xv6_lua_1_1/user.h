#ifndef XV_USER_H
#define XV_USER_H

#include "types.h"

struct stat;
struct rtcdate;

//defines
#define NULL (void*)0
#define size_t uint
#define EOF -1
#define stdin (FILE*)0
#define stdout (FILE*)1
#define stderr (FILE*)1

//typedefs
typedef struct myfile
{
    int file; //file descriptor
    char name[50]; //path and name
    int is_open; //1 if open, 0 if not
} FILE;

// system calls
int fork(void);
int exit(void) __attribute__((noreturn));
int wait(void);
int pipe(int*);
int write(int, void*, int);
int read(int, void*, int);
int close(int);
int kill(int);
int exec(char*, char**);
int open(char*, int);
int mknod(char*, short, short);
int unlink(char*);
int fstat(int fd, struct stat*);
int link(char*, char*);
int mkdir(char*);
int chdir(char*);
int dup(int);
int getpid(void);
char* sbrk(int);
int sleep(int);
int uptime(void);

// ulib.c
int stat(char*, struct stat*);
char* strcpy(char *s, const char *t);
void *memmove(void*, void*, int);
char* strchr(const char*, char c);
int strcmp(const char*, const char*);
void printf(const char* format, ...);
char* gets(char*, int max);
uint strlen(const char*);
void* memset(void*, int, uint);
void* malloc(uint);
void free(void*);
int atoi(const char*);

//additions to xv6
//math
double atof(const char* str);
double sin(double x);
double cos(double x);
double tan(double x);
double asin(double x);
double acos(double x);
double atan(double x);
double ceil(double x);
double floor(double x);
double sqrt(double x);
double pow(double x, double y);

//memory
void* calloc(size_t num, size_t size);
int memcmp(const void* lhs, const void* rhs, size_t count);
void* realloc(void *ptr, size_t new_size);
void* memcpy(void * destination, const void * source, size_t num);

//file
int fgetc(FILE *stream);
FILE *fopen(const char *filename, const char *mode);
int fclose(FILE *stream);
int fprintf(FILE *stream, const char *format, ...);
int sprintf(char *buffer, const char *format, ...);
int fscanf(FILE *stream, const char *format, ...);
int ungetc(int character, FILE * stream);
char *fgets(char *str, int count, FILE *stream);
int sscanf(const char * s, const char * format, ...);

//strings
double strtod(const char* str, char** endptr);
int isspace(int c);
int isdigit(int c);
int tolower(int c); //not needed
int isalnum(int c);
char* strcat(char * destination, const char * source);
char* strdup(const char *str1);
int toupper(int c);  //not needed
char* strstr(const char * str1, const char * str2);  //not needed
int isalpha(int c);

//system
int system(const char* command); //not needed
int remove(const char * filename); //not needed

#endif // !XV_USER_H

