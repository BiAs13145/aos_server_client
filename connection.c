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

#include "connection.h"

#define sizes 256

extern struct client cli[sizes];
extern struct capability_list file[sizes];
extern int clientNum;
extern int fileNum;
extern int available;
extern int writing;
extern int reading;

void *connection_handler(void *socketfd)
{

    int cli_fd = *(int *)socketfd; // file description for client
    char name[10], group[15], cli_msg[sizes], command[sizes];
    int client_index;

    puts("Waiting for user choose user...");
    send(cli_fd, "Please choose user(AOS-students: A, B, C or CSE-students: D, E, F)", sizes, 0);
    recv(cli_fd, name, 10, 0);
    printf("%s is connect \n", name);

    // check id with client
    if (strcmp(name, "A") == 0)
    {
        send(cli_fd, "Choose A as user. In the AOS-students group", sizes, 0);
        strcpy(group, "AOS-students");
    }
    else if (strcmp(name, "B") == 0)
    {
        send(cli_fd, "Choose B as user. In the AOS-students group", sizes, 0);
        strcpy(group, "AOS-students");
    }
    else if (strcmp(name, "C") == 0)
    {
        send(cli_fd, "Choose C as user. In the AOS-students group", sizes, 0);
        strcpy(group, "AOS-students");
    }
    else if (strcmp(name, "D") == 0)
    {
        send(cli_fd, "Choose D as user. In the CSE-students group", sizes, 0);
        strcpy(group, "CSE-students");
    }
    else if (strcmp(name, "E") == 0)
    {
        send(cli_fd, "Choose E as user. In the CSE-students group", sizes, 0);
        strcpy(group, "CSE-students");
    }
    else if (strcmp(name, "F") == 0)
    {
        send(cli_fd, "Choose F as user. In the CSE-students group", sizes, 0);
        strcpy(group, "CSE-students");
    }

    for (int i = 0; i < clientNum; i++){
        if (cli_fd == cli[i].fd){
            client_index = i;
            strcpy(cli[i].name, name);
            strcpy(cli[i].group, group);
        }
    }

    while (1){
        puts("Waiting for user type command...");
        //send(cli_fd, "Please Input your command :\n1.create filename access\n2.read filename\n3.write filename a/o\n4.modify filename access\n5.exit\n", sizes, 0);

        
        recv(cli_fd,cli_msg,sizes, 0); // get command
        //sleep(100000);
        //while(strcmp(cli_msg, NULL) == 0){}
        strcpy(command, cli_msg);
        //exit? return:continue

        //printf("%s",command);


        if(strcmp(command, "exit") == 0){
            cli[client_index].leave = 1;
            printf("User: %s exited\n", cli[client_index].name);
            break;
        }
        else{
            // split command line
            char split[3][20];
            char *space = " ";
            char *sentence;
            int split_index = 0;

            sentence = strtok(command, space);
            while (sentence != NULL){
                strcpy(split[split_index], sentence);
                sentence = strtok(NULL, space);
                split_index++;
            }

            // create
            if (strcmp(split[0], "create") == 0){
                send(cli_fd, "create", 10, 0);
                puts("create");
                // recv(clifd, cli_waiting, 20, 0);
                // printf("%s\n", cli_waiting);
                int isFileExist = 0;
                // check if file exist
                for (int i = 0; i < fileNum; i++){
                    if (strcmp(file[i].fname, split[1]) == 0){
                        isFileExist = 1;
                    }
                }
                if (isFileExist){
                    send(cli_fd, "file has exist.\n", sizes, 0);
                    continue;
                }
                else{
                    // create file
                    FILE *fp = fopen(split[1], "w");
                    strcpy(file[fileNum].fname, split[1]);
                    strcpy(file[fileNum].access_right, split[2]);
                    strcpy(file[fileNum].owner, name);
                    strcpy(file[fileNum].group, group);
                    file[fileNum].opFlag = available;
                    fileNum++;
                    fclose(fp);
                    printf("create new file %s\n", split[1]);
                    send(cli_fd,"creation successed!", sizes, 0);
                }
            }
            //read
            else if(strcmp(split[0], "read") == 0){
                char readingBuffer[sizes], readResult[sizes];

                send(cli_fd,"read", 10, 0);
                puts("read");
                int isFileExist = 0;
                int fileCount = 0;
                int permission = 0;

                // check if file exist
                for (int i = 0; i < fileNum; i++){
                    if (strcmp(file[i].fname, split[1]) == 0){
                        isFileExist = 1;
                        fileCount = i;
                    }
                }
                //file dosen't exist
                if(!(isFileExist)){
                    send(cli_fd, "File dose not exist.\nyou should create first.\n", sizes, 0);
                    continue;
                }else{
                    //file exist
                    if(strcmp(file[fileCount].owner, name) == 0){
                        if(file[fileCount].access_right[0] == 'r')
                            permission = 1;
                    
                    }else if(strcmp(file[fileCount].group, group) == 0){
                        if(file[fileCount].access_right[2] == 'r')
                            permission = 1;

                    }else{
                        if(file[fileCount].access_right[4] == 'r')
                            permission = 1;
                    }
                }
                //Permission deny
                if(!(permission)){
                    puts("Permission deny");
                    send(cli_fd,"File exist.\nBut you don't have permission.\n", sizes, 0);
                    continue;
                }else{
                    printf("%s has the permission\n", name);
                    if((file[fileCount].opFlag == reading) || (file[fileCount].opFlag == available)){
                        puts("allowed to read");
                        file[fileCount].anyUserReading++;
                        bzero(readingBuffer, sizes);
                        bzero(readResult, sizes);
                        send(cli_fd, "permitted", sizes, 0);
                        file[fileCount].opFlag = reading;
                        FILE *fp = fopen(split[1], "r");
                        while(fgets(readingBuffer, sizes, fp)){
                            strcat(readResult, readingBuffer);
                        }
                        //sent readResult
                        send(cli_fd, readResult, sizes, 0);
                        sleep(10);
                        file[fileCount].anyUserReading--;
                        //file[fileCount].opFlag = available;
                        if(file[fileCount].anyUserReading == 0){ //last one finsh read
                            file[fileCount].opFlag = available;
                        }

                        send(cli_fd, "Done reading", sizes, 0);
                        puts("Download Finished");
                    }else{ //There is user writing
                        send(cli_fd, "Accessed denied, someone is accessing the file\n", sizes, 0);
                    }
                }
            }
            //write
            else if(strcmp(split[0], "write") == 0){
                send(cli_fd,"write", 10, 0);
                puts("writing");
                int isFileExist = 0;
                int fileCount = 0;
                int permission = 0;

                // check if file exist
                for (int i = 0; i < fileNum; i++){
                    if (strcmp(file[i].fname, split[1]) == 0){
                        isFileExist = 1;
                        fileCount = i;
                    }
                }
                //file dosen't exist
                // 應該可改
                if(!(isFileExist)){
                    send(cli_fd, "File dose not exist.\nyou should create first.\n", sizes, 0);
                    continue;
                }else{
                    //file exist
                    if(strcmp(file[fileCount].owner, name) == 0){
                        if(file[fileCount].access_right[1] == 'w')
                            permission = 1;
                    
                    }else if(strcmp(file[fileCount].group, group) == 0){
                        if(file[fileCount].access_right[3] == 'w')
                            permission = 1;

                    }else{
                        if(file[fileCount].access_right[5] == 'w')
                            permission = 1;
                    }
                }
                //Permission deny
                if(!(permission)){
                    puts("Permission deny");
                    send(cli_fd,"File exist.\nBut you don't have permission.\n", sizes, 0);
                    continue;
                }else{
                    printf("%s has the permission\n", name);
                    if(file[fileCount].opFlag == available){
                        puts("allowed to write");
                        send(cli_fd, "permitted", sizes, 0);
                        file[fileCount].opFlag = writing;
                        if(strcmp(split[2], "a")){
                            FILE *fp = fopen(split[1], "a");
                            recv(cli_fd,cli_msg, sizes, 0); //writing content
                            fprintf(fp, "%s", cli_msg);
                            fclose(fp);

                        }else if(strcmp(split[2], "w")){
                            FILE *fp = fopen(split[1], "w"); 
                            recv(cli_fd,cli_msg, sizes, 0); //writing content
                            fprintf(fp, "%s", cli_msg);
                            fclose(fp);                            
                        }
                        send(cli_fd, "Start uploading the file. Please wait ... \n", sizes, 0);
                        
                        sleep(10);
                        send(cli_fd, "upload completed", sizes, 0);
                        file[fileCount].opFlag = available;
                    }else{ //There is user writing
                        send(cli_fd, "Accessed denied, someone is accessing the file\n", sizes, 0);
                    }
                }                
            }
            //changemode
            else if(strcmp(split[0], "changemode") == 0){
                //puts("changing mode");
                send(cli_fd,"changemode", sizes, 0);
                int isFileExist = 0;
                int fileCount = 0;
                int permission = 0;

                // check if file exist
                for (int i = 0; i < fileNum; i++){
                    if (strcmp(file[i].fname, split[1]) == 0){
                        isFileExist = 1;
                        fileCount = i;
                    }
                }

                if(!(isFileExist)){
                    //puts("File dosen't exist.");
                    send(cli_fd,"File dosen't exist\n", sizes, 0);
                    continue;
                }else{
                    //file exist
                    if(strcmp(file[fileCount].owner, name) == 0)
                        permission = 1;
                }
                //Permission deny
                if(!(permission)){
                    puts("Permission deny");
                    send(cli_fd,"File exist.\nBut you don't have permission.\n", sizes, 0);
                    continue;
                }else{
                    puts("changed");
                    send(cli_fd,"permitted", sizes, 0);

                    printf("Befor change: ");
                    printf("file : %s\n",split[1]); //print this file's current access right
                    printf("owner : %c %c\n",file[fileCount].access_right[0],file[fileCount].access_right[1]);
                    printf("group : %c %c\n",file[fileCount].access_right[2],file[fileCount].access_right[3]);
                    printf("others : %c %c\n",file[fileCount].access_right[4],file[fileCount].access_right[5]);  

                    strcpy(file[fileCount].access_right, split[2]);
                    send(cli_fd,"changed", sizes, 0);

                    printf("After change: ");
                    printf("file : %s\n",split[1]); //print this file's access right
                    printf("owner : %c %c\n",file[fileCount].access_right[0],file[fileCount].access_right[1]);
                    printf("group : %c %c\n",file[fileCount].access_right[2],file[fileCount].access_right[3]);
                    printf("others : %c %c\n",file[fileCount].access_right[4],file[fileCount].access_right[5]);                    

                }
            }else{
                puts("ErrorInput");
                send(cli_fd, "ErrorInput", sizes , 0);
                //continue;
                //break;
            }
        }
    }
    free(socketfd);
    pthread_exit(NULL);
}