#include <stdio.h>
#include <stdlib.h>
#include <string.h> // strtok

int main() {
    //char str[] = "Hello world, nice to meet you";
    //1) create homework2.c rwr---
    //2) read homework2.c
    //3) write homework2.c o/a
    //4) mode homework2.c rw---- 

    char str[128] ;
    //scanf("%s",&str);
    fgets(str,128,stdin);
    const char* d = "  ";
    char *p;
    char *instr;
    char *filename;
    char *instr_flag;
    printf("str:%s",str);
    instr = strtok(str, d);
    filename = strtok(NULL, d);
    instr_flag = strtok(NULL, d);

    
    /*while (p != NULL) {
        printf("%s\n", p);
        p = strtok(NULL, d);		   
    }*/
    printf("instr:%s\n",instr);
    printf("filename:%s\n",filename);
    printf("instr_flag:%s\n",instr_flag);
    

    printf("%s",str);
    
    return 0;
}