#include<stdio.h>
#include<string.h>	//strlen
#include<stdlib.h>	//strlen
#include<sys/socket.h>
#include<arpa/inet.h>	//inet_addr 
#include<unistd.h>	//write
#include<pthread.h> //for threading , link with lpthread
#include<netinet/in.h> /* INADDR_ANY sockaddr_in */
#include<time.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<ctype.h>
struct client
{
    int fd;    // file description
    int leave; // exit or not
    char name[10];
    char group[15];
};

struct capability_list
{
    char fname[16];
    char owner[10];
    char access_right[6];   //|rw|rw|rw|
    char group[15];
    int opFlag;             // available=1 writing=2 reading=0
    int anyUserReading;     // decide whether should set opFlag to available when an user finsh read 
};

void main () {
    char message[128];
    char *instr;
    char *filename;
    char *instr_flag;
    char instr_array[16];
    char filename_array[16];
    char instr_flag_array[6];
    const char* d = "  ";

    struct capability_list file[256];
    int fileNum = 0;
    int available;

    printf("input command:");
    fgets(message ,128,stdin);
    printf("command get\n");
    instr = strtok(message , d);
    filename = strtok(NULL, d);
    instr_flag = strtok(NULL, d);
    strcpy(instr_array,instr);
    strcpy(filename_array,filename);
    strcpy(instr_flag_array,instr_flag);

    
    FILE * fp;
    printf("instr:%s\n",instr_array);
    printf("filename:%s\n",filename_array);
    printf("instr_flag:%c\n",instr_flag_array[0]);
    /*switch (*instr) {
    case 'create' : if (fp = fopen(filename, "r")) {printf("file has exist");} 
                    else {fopen( filename,"w" );}
                    break;
    case 'write' : if (fp = fopen(filename, "r")) {
                      if (instr_flag == 'o') { //overwrite
                       fopen( filename,"w" );  
                      }
                      else if (instr_flag == 'a') { //append
                       fopen( filename,"a" );
                      }
                      else {printf("Undefined flag");}
                    }
                    else {printf("file didn't exist");}
                    break;
                      
                    
    case 'read' : if (fp = fopen(filename, "r")) {printf("file has exist");} 
                    else {printf("file didn't exist");}
                break;
    case 'mode' :break;
    }*/
    if (strcmp(instr_array,"create") == 0) {
        printf("into create\n");
      if (fp = fopen(filename_array, "r")) {printf("file has exist");} 
      
      else {
                    fopen( filename_array,"w" );
                    strcpy(file[fileNum].fname, filename_array);
                    strcpy(file[fileNum].access_right, instr_flag_array);
                    strcpy(file[fileNum].owner, name);
                    strcpy(file[fileNum].group, group);
                    file[fileNum].opFlag = available;
                    fileNum++;
                    fclose(fp);
                    printf("create new file %s\n", filename_array);
                    //send(cli_fd,"creation successed!", sizes, 0);
            }  
    }
    else if (strcmp(instr_array,"write") == 0) {
        printf("into write\n");
        if (fp = fopen(filename_array, "r")) {
                      if (instr_flag_array == "o") { //overwrite
                       
                       fopen( filename_array,"w" );  
                      }
                      else if (instr_flag_array == "a") { //append
                       fopen( filename_array,"a" );
                      }
                      else {printf("Undefined flag\n");}
                    }
                    else {printf("file didn't exist\n");}
    }
    else if (strcmp(instr_array,"read") == 0) {
        printf("into read");
        if (fp = fopen(filename_array, "r")) {printf("file has exist");} 
                    else {printf("file didn't exist");}
    }
    else if (strcmp(instr_array,"mode") == 0) {
        printf("into mode\n");
        printf("mode change\n");
    }
    else printf("all fail\n");
    
}