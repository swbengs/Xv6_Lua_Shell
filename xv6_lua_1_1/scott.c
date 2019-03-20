/*
Scott Bengs
CSIS 430
Final
*/

#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

void am_i_crazy()
{
    float test = 1000.1f;
    int* ap = &test;
    float* apf = (float*)(void*)ap;
    float tempf = *apf;
    int temp = (int)tempf;
    printf("s1: %x\n", test);
    printf("s2: %d\n", temp);
}

void strtod_test()
{
    char* str = "1998.9 and more stuff";
    char* end;
    float result = (float)strtod(str, &end);
    //float result = (float)strtod(str, NULL);
    //float result = 1000.0f;
    //sizeof(result);
    char* s1 = (char*)malloc(50);
    char* s2 = (char*)malloc(50);
    char* s3 = (char*)malloc(50);
    char* s4 = (char*)malloc(50);
    char* s5 = (char*)malloc(50);

    int junk = 88888;

    //printf("result before conv: %d \n", (int)result);
    sprintf(s1, "str: %s", str);
    sprintf(s2, "end: %s", end);
    sprintf(s3, "result: %g", result);
    //sprintf(s3, "result int g: %g", (int)result, junk);
    //sprintf(s4, "result after convx: %x", (int)result);
    sprintf(s5, "result int: %d", (int)result);

    printf("s1: %s\n", s1);
    printf("s2: %s\n", s2);
    printf("s3: %s\n", s3);
    //printf("s4: %s\n", s4);
    printf("s5: %s\n", s5);
    //printf("s6: %x\n", 1000.1f);

    free(s1);
    free(s2);
    free(s3);
    free(s4);
    free(s5);
}

void test_sprintf_limits()
{
    printf("limit test\n");
    char* x = "012345678901234";
    char* x2 = "012";
    char* s1 = (char*)malloc(100);
    char* s2 = (char*)malloc(100);
    char* s3 = (char*)malloc(100);

    sprintf(s1, "String: %8.8s...", x);
    sprintf(s2, "String: %10.10s...", x);
    sprintf(s3, "String: %5s...", x);

    printf("s1: %s\n", s1);
    printf("s2: %s\n", s2);
    printf("s3: %s\n", s3);

    free(s1);
    free(s2);
    free(s3);
}

void test_sprintf()
{
    char* format = "int: %d char: %c string: %s \n";
    int the_int = 100;
    char the_char = 'a';
    char* the_string = "hi there";
    //sprintf(f, "%%%ds", m);
    //sprintf(sn, "String: %10.10s...", s);

    char* buffer = (char*)malloc(100); //make sure this is large enough for the entire string or else buffer overflow will happen

    printf("printf result\n");
    printf(format, the_int, the_char, the_string);
    sprintf(buffer, format, the_int, the_char, the_string);
    printf("sprintf result\n");
    printf("%s \n", buffer);
    free(buffer);

    char* s = "suuup";
    char* string1 = (char*)malloc(100);
    char* string2 = (char*)malloc(100);
    int m = 123;
    sprintf(string1, "%%%ds", m);
    sprintf(string2, "String: %10.10s...", s);
    printf("string1: %s\n", string1);
    printf("string2: %s\n", string2);
    free(string1);
    free(string2);
    printf("\n\n");
}

int main(int argc, char *argv[])
{
    //test_sprintf();
    //test_sprintf_limits();
    strtod_test();
    //am_i_crazy();

    exit();
}

