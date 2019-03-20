#include "types.h"
#include "stat.h"
#include "fcntl.h"
#include "user.h"
#include "x86.h"

char* strcpy(char *s, const char* t)
{
  char *os;

  os = s;
  while((*s++ = *t++) != 0)
    ;
  return os;
}

int
strcmp(const char *p, const char *q)
{
  while(*p && *p == *q)
    p++, q++;
  return (uchar)*p - (uchar)*q;
}

uint
strlen(const char *s)
{
  int n;

  for(n = 0; s[n]; n++)
    ;
  return n;
}

void*
memset(void *dst, int c, uint n)
{
  stosb(dst, c, n);
  return dst;
}

char*
strchr(const char *s, char c)
{
  for(; *s; s++)
    if(*s == c)
      return (char*)s;
  return 0;
}

char*
gets(char *buf, int max)
{
  int i, cc;
  char c;

  for(i=0; i+1 < max; ){
    cc = read(0, &c, 1);
    if(cc < 1)
      break;
    buf[i++] = c;
    if(c == '\n' || c == '\r')
      break;
  }
  buf[i] = '\0';
  return buf;
}

int
stat(char *n, struct stat *st)
{
  int fd;
  int r;

  fd = open(n, O_RDONLY);
  if(fd < 0)
    return -1;
  r = fstat(fd, st);
  close(fd);
  return r;
}

int
atoi(const char *s)
{
  int n;

  n = 0;
  while('0' <= *s && *s <= '9')
    n = n*10 + *s++ - '0';
  return n;
}

void*
memmove(void *vdst, void *vsrc, int n)
{
  char *dst, *src;

  dst = vdst;
  src = vsrc;
  while(n-- > 0)
    *dst++ = *src++;
  return vdst;
}

//added c library functions and/or defines

//math
double atof(const char* str)
{
    //printf("atof\n");
    return strtod(str, NULL);
}

double sin(double x)
{
    printf("sin\n");
    return 0;
}

double cos(double x)
{
    printf("cos\n");
    return 0;
}

double tan(double x)
{
    printf("tan\n");
    return 0;
}

double asin(double x)
{
    printf("asin\n");
    return 0;
}

double acos(double x)
{
    printf("acos\n");
    return 0;
}

double atan(double x)
{
    printf("atan\n");
    return 0;
}

double ceil(double x)
{
    printf("ceil\n");
    return 0;
}

double floor(double x)
{
    printf("floor\n");
    return 0;
}

double sqrt(double x)
{
    printf("sqrt\n");
    return 0;
}

double pow(double x, double y)
{
    printf("pow\n");
    return 0;
}

//memory
void* calloc(size_t num, size_t size)
{
    /*
    Allocates memory for an array of num objects of size size and initializes all bytes in the allocated storage to zero.
    If allocation succeeds, returns a pointer to the lowest (first) byte in the allocated memory block that is suitably aligned for any object type.
    If size is zero, the behavior is implementation defined (null pointer may be returned, or some non-null pointer may be returned that may not be used to access storage)
    */
    return malloc(num*size);
}

int memcmp(const void* lhs, const void* rhs, size_t count)
{
    /*
    http://en.cppreference.com/w/c/string/byte/memcmp
    Compares the first count characters of the objects pointed to by lhs and rhs. The comparison is done lexicographically.
    The sign of the result is the sign of the difference between the values of the first pair of bytes (both interpreted as unsigned char) that differ in the objects being compared.
    The behavior is undefined if access occurs beyond the end of either object pointed to by lhs and rhs. The behavior is undefined if either lhs or rhs is a null pointer.
    */

    int n;
    const char* left;
    const char* right;
    left = lhs;
    right = rhs;
    for (n = 0; n < count; n++)
    {
        if (left[n] != right[n])
        {
            return left[n] - right[n];
        }
    }

    return 0;
}

void* realloc(void *ptr, size_t new_size)
{
    /*
    Reallocates the given area of memory. It must be previously allocated by malloc(), calloc() or realloc() and not yet freed with a call to free or realloc. Otherwise, the results are undefined.
    The reallocation is done by either:
    a) expanding or contracting the existing area pointed to by ptr, if possible. The contents of the area remain unchanged up to the lesser of the new and old sizes. If the area is expanded, the contents of the new part of the array are undefined.
    b) allocating a new memory block of size new_size bytes, copying memory area with size equal the lesser of the new and the old sizes, and freeing the old block.
    If there is not enough memory, the old memory block is not freed and null pointer is returned.
    If ptr is NULL, the behavior is the same as calling malloc(new_size).
    If new_size is zero, the behavior is implementation defined (null pointer may be returned (in which case the old memory block may or may not be freed), or some non-null pointer may be returned that may not be used to access storage).
    */
    if (ptr)
    {
        printf("realloc new_size: %d \n", new_size);
        char* src = ptr;
        char* dest = (char*)malloc(new_size);
        int n;
        for (n = 0; n < new_size; n++) //not proper but hopefully works in most cases. Unsure how we can get the old pointer's size to do proper copy. if String it's easy, if it's not we don't know where say an int or float stops/begins
        {
            dest[n] = src[n];
        }
        dest[new_size - 1] = '\0'; //make sure the last thing is a null char
        free(src); //free the old memory and point both to nothing aka NULL
        ptr = NULL;
        src = NULL;
        return (void*)dest;
    }
    else //if null it acts like malloc(new_size)
    {
        return malloc(new_size);
    }
}

void* memcpy(void* destination, const void* source, size_t num)
{
    /*
    http://en.cppreference.com/w/c/string/byte/memcpy
    1) Copies count characters from the object pointed to by src to the object pointed to by dest. Both objects are interpreted as arrays of unsigned char.
    The behavior is undefined if access occurs beyond the end of the dest array. If the objects overlap (which is a violation of the restrict contract) (since C99), the behavior is undefined. The behavior is undefined if either dest or src is a null pointer.
    The behavior is undefined if the size of the character array pointed to by dest < count <= destsz; in other words, an erroneous value of destsz does not expose the impending buffer overflow.
    As all bounds-checked functions, memcpy_s is only guaranteed to be available if __STDC_LIB_EXT1__ is defined by the implementation and if the user defines __STDC_WANT_LIB_EXT1__ to the integer constant 1 before including string.h.
    Returns a copy of dest
    */

    int n;
    const char* so; 
    char* de;
    so = (char*)source;
    de = (char*)destination;
    for (n = 0; n < num; n++)
    {
        de[n] = so[n];
    }

    return destination;
}

//files
static FILE lua_file;

int fgetc(FILE *stream)
{
    //printf("fgetc\n");
    char buffer = EOF; //assume it's end of file until we find out it isn't
    if (lua_file.is_open)
    {
        int count = read(lua_file.file, &buffer, sizeof(buffer));
        if (count != 1) //if count is 0 that means we are at EOF but we don't know for sure what is in buffer so assign it EOF just to be safe
        {
            buffer = EOF;
        }
    }
    return buffer;
}

FILE *fopen(const char *filename, const char *mode)
{
    //printf("fopen\n");
    strcpy(lua_file.name, filename);
    lua_file.is_open = 1;
    lua_file.file = open(lua_file.name, O_CREATE | O_RDWR);
    return &lua_file;
}

int fclose(FILE *stream)
{
    //returns 0 on success, EOF others
    //printf("fclose\n");
    close(lua_file.file);
    lua_file.is_open = 0;
    return 0; //close should not fail, and if it does then we probably have bigger problems than a dangling file
}

int fprintf(FILE *stream, const char *format, ...)
{
    //TODO:change this to forward to sprintf and have printf and this function shove arguments to it. Pick an unused %char set that marks it as a forwarded so we can handle that in sprintf
    if (stream == stdin || stream == stdout || stream == stderr)
    {
        uint *ap = ((uint*)(void*)&format) + 1;
        printf("fprintf called\n");
        printf(format, *ap); //give what it points to, not the pointer itself when passing down Variadic Functions
    }
    else
    {
        printf("fprintf unknown file\n");
    }
    return 0;
}

static int sprintint(char* buffer, int num, int start)
{
    //modified from printf.c version
    static char digits[] = "0123456789";
    char buf[16];
    int i, neg, base;
    uint x;

    neg = 0;
    base = 10;
    if (num < 0)
    {
        neg = 1;
        x = -num;
    }
    else 
    {
        x = num;
    }

    i = 0;
    do 
    {
        buf[i++] = digits[x % 10];
    } while ((x /= base) != 0); //adds in reverse order so 100 becomes 001
    if (neg)
    {
        buf[i++] = '-';
    }
    neg = i; //reuse neg to save how many things we have written
    base = 0;
    while (--i >= 0) //puts in reverse order
    {
        buffer[start+base] = buf[i];
        base++;
    }
    return neg; //how many things we put in there
}

//assumes string contains only numbers
static int str_to_num(const char* string)
{
    //printf("string: %s\n", string);
    int n;
    int count = strlen(string);
    int number = 0;
    int i;
    int digit_multiplier = 1;
    for (i = 0; i < count - 1; i++) //quick and dirty exponent
    {
        digit_multiplier *=  10;
    }

    for (i = 0; i < count; i++)
    {
        switch (string[i])
        {
        case '0':
            n = 0;
            break;
        case '1':
            n = 1;
            break;
        case '2':
            n = 2;
            break;
        case '3':
            n = 3;
            break;
        case '4':
            n = 4;
            break;
        case '5':
            n = 5;
            break;
        case '6':
            n = 6;
            break;
        case '7':
            n = 7;
            break;
        case '8':
            n = 8;
            break;
        case '9':
            n = 9;
            break;
        default:
            printf("str_to_num unknown number: %c\n", string[i]);
            n = 0;
            break;
        }
        number += n * digit_multiplier;
        digit_multiplier /= 10;
    }

    return number;
}

static int extract_width_prec(const char* string, int* width, int* precision, int start)
{
    //width is a min, precision is a max min means left padded with a space if given string is not large enough
    char first[10];
    char second[10];
    memset(first, 0, 10);
    memset(second, 0, 10);
    int count = 0;
    int count2 = 0; //for indexing the second char array when we write to it
    int period = 0;

    while (isdigit(string[count + start]) || string[count + start] == '.') //keep looping until we find something that's not a number or a period
    {
        if (period == 0) //period not found, assuming there is one
        {
            if (string[count + start] == '.')
            {
                period = 1;
            }
            else
            {
                first[count] = string[count + start];
            }
            
        }
        else
        {
            second[count2] = string[count + start];
            count2++;
        }
        count++;
    }

    //use atoi instead
    *width = str_to_num(first);
    *precision = str_to_num(second);

    return count;
}

void memdumpi(int* ptr, int count)
{
    int i;
    for (i = 0; i < count; i++)
    {
        printf("adr: %p, val: %d\n", ptr, *ptr);
        ptr++;
    }
}


void memdumpf(double* ptr, int count)
{
    int i;
    for (i = 0; i < count; i++)
    {
        printf("adr: %p, val: %d\n", ptr, (int)*ptr);
        ptr++;
    }
}

int sprintf(char *buffer, const char *format, ...)
{
    //%[flags][width][.precision][length]specifier 
    //A % followed by another % character will write a single % to the stream.
    //sprintf(sn, "String: %10.10s...", s); string 10 min and 10 max. width adds padding if too small
    //sprintf(s, "%g", nvalue(obj));
    //sprintf(f, "%%%ds", m);
    //%f, %c

    double* apf;
    char *s;
    int c, i, state, written, count, width, precision; //written keeps track of our current spot within the buffer
    uint *ap;

    state = 0;
    written = 0;
    count = 0;
    width = 0;
    precision = 0;

    ap = (uint*)(void*)&format + 1;
    //printf("&ap: %p \n", &ap);
    //printf("&buffer: %p \n", &buffer);
    //printf("&format: %p \n", &format);
    //printf("ap: %p \n", ap);
    //printf("\n");
    for (i = 0; format[i]; i++)
    {
        c = format[i] & 0xff;
        if (state == 0) 
        {
            if (c == '%')
            {
                state = '%';
            }
            else 
            {
                buffer[written] = (char)c;
                written++;
            }
        }
        else if (state == '%')
        {
            if (isdigit(c) || c == '.') //keep going until we hit something else
            {
                count = extract_width_prec(format, &width, &precision, i);
                //printf("count: %d\n", count);
                //printf("width: %d\n", width);
                //printf("precision: %d\n", precision);
                i += count;
                c = format[i] & 0xff;
                //printf("current char: %c\n", c);
            }

            if (c == 'd' || c == 'x' || c == 'p')
            {
                //printf("ap: %p \n", ap);
                //printf("ap: %d \n", *ap);
                written += sprintint(buffer, *ap, written); //just print everything as DEC for simplicity
                ap++;
            }
            else if (c == 's') 
            {
                s = (char*) *ap;
                ap++;
                if (s == 0)
                {
                    s = "(null)";
                }

                if (count == 0) //just do regular print for %s.
                {
                    while (*s != 0)
                    {
                        buffer[written] = *s;
                        s++;
                        written++;
                    }
                }
                else //print with min/max and so on width = min, pad if needed. precision is max, cut off extra part as needed
                {
                    int str_count = strlen(s);
                    
                    int n, max;
                    max = precision;
                    if (max == 0 || max > str_count)
                    {
                        max = str_count;
                    }
                    int padding = width - str_count;
                    if (str_count > max) //more to write than there is room. so no room for padding
                    {
                        padding = 0;
                    }

                    //printf("str_count: %d\n", str_count);
                    //printf("padding: %d\n", padding);
                    //printf("max: %d\n", max);
                    for (n = 0; n < padding; n++)
                    {
                        buffer[written] = ' ';
                        written++;
                    }
                    for (n = 0; n < max; n++) //finally write the remaining characters or at least until we hit the max
                    {
                        buffer[written] = *s;
                        s++;
                        written++;
                    }
                }
                
                //reset them
                count = 0;
                width = 0;
                precision = 0;
            }
            else if (c == 'c')
            {
                buffer[written] = (char)*ap;
                ap++;
                written++;
            }
            else if (c == '%')
            {
                buffer[written] = '%';
                written++;
            }
            else if (c == 'g' || c == 'f') //treat as INT for now, even though they are floats
            {
                /*
                Found out the problem thanks to https://stackoverflow.com/questions/1688942/variable-arguments-in-c-how-to-get-values-with-a-generic-type
                Floats are automatically promoted to doubles in va_arg which we don't have access to
                */
                //printf("g or h mode \n");
                apf = (double*)(void*)ap;
                printf("apf adr: %p\n", apf);
                //printf("written: %d \n", written);
                written += sprintint(buffer, (int)*apf, written); //just print everything as DEC for simplicity
                //printf("written: %d \n", written);
                //written += sprintint(buffer, *ap, written);
                ap++;

                apf--;
                apf--;
                apf--;
                apf--;
                apf--;
                memdumpf(apf, 15);
                printf("\n");
                ap = (int*)(void*)apf;
                memdumpi(ap, 15);
                printf("\n");
            }
            else
            {
                buffer[written] = '?';
                written++;
            }
            state = 0;
        }
    }

    buffer[written] = '\0';
    return written;
}

//files
int fscanf(FILE *stream, const char *format, ...)
{
    printf("fscanf\n");
    return 0;
}

int ungetc(int character, FILE * stream)
{
    printf("ungetc\n");
    return 0;
}

char* fgets(char *str, int count, FILE *stream)
{
    printf("fgets\n");
    return NULL;
}

int sscanf(const char * s, const char * format, ...)
{
    printf("sscanf\n");
    return 0;
}

//strings
double strtod(const char* str, char** endptr)
{
    //printf("strtod\n");
    int size = strlen(str); //entire size including the . and after it
    int isize = 0; //size of just the integer part
    int i = 0;
    while (isdigit(str[i]))
    {
        isize++;
        i++;
    }
    if (str[i] == '.')
    {
        i++;
    }

    //printf("isize: %d \n", isize);
    //at this point we found the decimal if one existed
    while (isdigit(str[i])) //so keep going until we hit a non digit character
    {
        i++;
    }
    //found end of the number
    //printf("i: %d \n", i);
    if (endptr)
    {
        *endptr = &str[i]; //point the endptr to the first character after the number if it's not null
    }

    //convert the first part into a number
    char temp_str[15];
    for (i = 0; i < isize; i++) //copy from actual str to temp and use that to get the integer part
    {
        temp_str[i] = str[i];
    }
    temp_str[isize] = '\0';
    //printf("temp_str: %s \n", temp_str);
    int num = atoi(temp_str);
    //printf("num: %d \n", num);

    return (double)num;
}

int isspace(int c)
{
    //http://www.cplusplus.com/reference/cctype/isspace/
    //Checks whether c is a white-space character.
    char s = (char)c;
    if (s == ' ')
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int isdigit(int c)
{
    /*
    0: 48
    9: 57
    */
    if (c >= '0' && c <= '9')
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int tolower(int c)
{
    printf("tolower\n");
    return 0;
}

int isalnum(int c)
{
    if (isdigit(c) || isalpha(c))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

char* strcat(char* destination, const char* source)
{
    //printf("strcat\n");
    int dest_size = strlen(destination);
    int source_size = strlen(source) + 1; //+1 because we want to include the null char at the end
    int current, i;
    current = dest_size; //overrite the null char of destination
    for (i = 0; i < source_size; i++)
    {
        destination[current+i] = source[i];
    }
    return destination;
}

char * strdup(const char *str1)
{
    uint size = strlen(str1) + 1; //doesn't count the null char
    char* str2 = (char*)malloc(size);
    strcpy(str2, str1);
    return str2;
}

int toupper(int c)
{
    printf("toupper\n");
    return 0;
}

char* strstr(const char * str1, const char * str2)
{
    printf("strstr\n");
    return NULL;
}

int isalpha(int c)
{
    /*
    a: 97
    z: 122
    A: 65
    Z: 90
    */
    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

//system
int system(const char* command)
{
    printf("system\n");
    return 0;
}

int remove(const char * filename)
{
    printf("remove\n");
    return 0;
}

