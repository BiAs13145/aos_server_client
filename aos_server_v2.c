#include<stdio.h>dex
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

struct capability_list file[256];
struct client cli[256];
int available;
int clientNum, fileNum = 0;
int available = 1;
int writing = 2;
int reading = 0;

int *connection_handler(void *socketfd)

{
    //socket的建立
    int forClientSockfd= *(int *)socketfd; 
    int client_index;
    char inputBuffer[256] = {};
    char message[128];
    strcpy(message,"Hi,this is server.\n Group (A for AOS-members,C for CSE-members):");
    //int sockfd = 0,forClientSockfd = 0;
    //sockfd = socket(AF_INET , SOCK_STREAM , 0);
    char name[10], group[15], cli_msg[128], command[16];
    char YN[1];


    //socket的連線
    struct sockaddr_in serverInfo,clientInfo;
    int addrlen = sizeof(clientInfo);
    //bzero(&serverInfo,sizeof(serverInfo));

    //serverInfo.sin_family = PF_INET;
    //serverInfo.sin_addr.s_addr = INADDR_ANY;
    //serverInfo.sin_port = htons(8700);
    //bind(sockfd,(struct sockaddr *)&serverInfo,sizeof(serverInfo));
    //listen(sockfd,6);

    //forClientSockfd = accept(sockfd,(struct sockaddr*) &clientInfo, &addrlen);
    send(forClientSockfd,message,sizeof(message),0);
    recv(forClientSockfd,group,sizeof(group),0); // group 53
    printf("%s group \n", group); 
    memset(message,0,128);
    strcpy(message, "Enter name:");
    printf("content in message %s\n",message);
    send(forClientSockfd,message,sizeof(message),0);
    recv(forClientSockfd,name,sizeof(name),0);//name 64
    /*if (strcmp( group, "A") == 0) {
           strcpy( message, "Enter name:");
           send(forClientSockfd,message,sizeof(message),0);
           printf("send owner name reqest");
        }*/
    printf("name:%s\n",name);
    for (int i = 0; i < clientNum; i++){
        if ( cli[i].name == name){
            client_index = i;
            strcpy(cli[i].name, name);
            strcpy(cli[i].group, group);
        }
        else {
            strcpy(cli[clientNum].name, name);
            strcpy(cli[clientNum].group, group);
            clientNum ++;
            client_index = clientNum;
        }
    }
    
    char *instr;
    char *filename;
    char *instr_flag;
    char instr_array[16];
    char filename_array[16];
    char instr_flag_array[16];
    const char* d = "  ";

    
    

    while(1){
        //forClientSockfd = accept(sockfd,(struct sockaddr*) &clientInfo, &addrlen);
        memset(message,0,128);
        memset(inputBuffer,0,sizeof(inputBuffer));
        //strcpy( message, "user:");
        //send(forClientSockfd,message,sizeof(message),0);

        //strcpy( message, name);
        //send(forClientSockfd,message,sizeof(message),0);
        strcpy( message, "Enter request:");
        printf("REQ content in message %s\n",message);
        send(forClientSockfd,message,sizeof(message),0);
        recv(forClientSockfd,inputBuffer,sizeof(inputBuffer),0); //89
        printf("Get:%s\n",inputBuffer);

        instr = strtok(inputBuffer , d);
        filename = strtok(NULL, d);
        instr_flag = strtok(NULL, d);
        
        strcpy(instr_array,instr);
        strcpy(filename_array,filename);
        if (!(strcmp(instr_array,"read") == 0)) {
        strcpy(instr_flag_array,instr_flag);}
        printf("instr:%s\n",instr_array);
        printf("filename:%s\n",filename_array);
        printf("instr_flag:%c\n",instr_flag_array[0]);
        printf("instr_flag:%c\n",instr_flag_array[1]);
        printf("instr_flag:%c\n",instr_flag_array[2]);
        printf("instr_flag:%c\n",instr_flag_array[3]);
        printf("instr_flag:%c\n",instr_flag_array[4]);
        printf("instr_flag:%c\n",instr_flag_array[5]);

        
        strcpy( message, "Processing request......\n");
        //printf("WAIT content in message %s\n",message);
        send(forClientSockfd,message,sizeof(message),0);
        //printf("Get:%s\n",inputBuffer);
        
//-------------------------------------------------------------create
        if (strcmp(instr_array,"create") == 0) {
            FILE * fp;
        printf("into create\n");
            if (fp = fopen(filename_array, "r")) {
                                                 printf("file has exist\n");
                                                 strcpy( message, "file has exist\n");
                                                 } 
      
            else {
                    fopen( filename_array,"w" );
                    strcpy(file[fileNum].fname, filename_array);
                    strcpy(file[fileNum].access_right, instr_flag_array);
                    strcpy(file[fileNum].owner, name);
                    strcpy(file[fileNum].group, group);
                    file[fileNum].opFlag = available;
                    fileNum++;
                    
                    printf("create new file %s\n", filename_array);
                    //send(forClientSockfd,"creation successed!", sizes, 0);
                    //fclose(fp);
                    printf("finished create\n");
                    strcpy( message, "Finish creation\n");
                }
                
                send(forClientSockfd,message,sizeof(message),0);
                fclose(fp);  
         }
//----------------------------------------------------------------------write
        else if (strcmp(instr_array,"write") == 0) {
            FILE * fp;
            printf("into write\n");
            if (fp = fopen(filename_array, "r")) {
                       
                      if ( instr_flag_array[0] == 'o') { //overwrite
                       
                       fopen( filename_array,"w" );
                       strcpy( message, "Continue writing? (Y/N):");  
                       send(forClientSockfd,message,sizeof(message),0);
                       printf("send yn\n");
                       //memset(inputBuffer,0,sizeof(inputBuffer));
                       recv(forClientSockfd,inputBuffer,sizeof(inputBuffer),0); //YN 108
                       printf("continue?%s",inputBuffer);
                       YN[0] = inputBuffer[0];
                       while (YN[0]!='N') {
                        strcpy( message, "Continue writing? (Y/N):");  
                        send(forClientSockfd,message,sizeof(message),0);
                        memset(inputBuffer,0,sizeof(inputBuffer));
                        recv(forClientSockfd,inputBuffer,sizeof(inputBuffer),0); //YN 117
                        printf("continue?%s",inputBuffer);
                        printf("yn from cli %s",inputBuffer);
                        YN[0] = inputBuffer[0];
                        

                       }
                      }
            
                      else if (instr_flag_array[0] == 'a') { //append
                       fopen( filename_array,"a" );
                       strcpy( message, "Continue writing? (Y/N):");  
                       send(forClientSockfd,message,sizeof(message),0);
                       printf("send yn\n");
                       //memset(inputBuffer,0,sizeof(inputBuffer));
                       recv(forClientSockfd,inputBuffer,sizeof(inputBuffer),0); //YN 108
                       printf("continue?%s",inputBuffer);
                       YN[0] = inputBuffer[0];
                       while (YN[0]!='N') {
                        strcpy( message, "Continue writing? (Y/N):");  
                        send(forClientSockfd,message,sizeof(message),0);
                        memset(inputBuffer,0,sizeof(inputBuffer));
                        recv(forClientSockfd,inputBuffer,sizeof(inputBuffer),0); //YN 117
                        printf("continue?%s",inputBuffer);
                        printf("yn from cli %s",inputBuffer);
                        YN[0] = inputBuffer[0];
                        

                      }
                    }
                      else {printf("Undefined flag\n");}
                      fclose(fp);
                    }
                    else {printf("file didn't exist\n");}
                    
            }
//------------------------------------------------------------------------read
        else if (strcmp(instr_array,"read") == 0) {
            FILE * fp;
            printf("into read");
            if (fp = fopen(filename_array, "r")) {
                                                   printf("file has exist\n");
                                                   strcpy( message, "File downloading\nFinish\n");
                                                   fclose(fp);
                                                   } 
                    else {
                          printf("file didn't exist\n");
                          strcpy( message, "file didn't exist\n");
                          }
            send(forClientSockfd,message,sizeof(message),0);              
            
            }
//-------------------------------------------------------------------mode
        else if (strcmp(instr_array,"mode") == 0) {
            FILE * fp;
            printf("into mode\n");
            printf("mode change\n");

            if (fp = fopen(filename_array, "r")) {
            for (int i = 0; i < fileNum; i++)
            {
                if (strcmp( file[i].fname,filename_array) == 0) {
                    for (int j= 0 ;j<6;j++) {
                    file[i].opFlag = instr_flag[i];
                    }
                }
                strcpy( message, "mode change\n");
            }
            fclose(fp);
            }
            else {
                printf("file didn't exist\n");
                strcpy( message, "file didn't exist\n");
            }
            

            
            end(forClientSockfd,message,sizeof(message),0); 
           }     
        else {
            printf("all fail\n");
            cli[client_index].leave = 1;
            break;
            }


        memset(inputBuffer,0,sizeof(inputBuffer));
        memset(message,0,128);
        
    }
    free(socketfd);
    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    
    int socketfd, cli_socket, cli_len, *create_socket;
    struct sockaddr_in server, client;

    if (argc != 1)
    {
        printf("Usage: ./server \n");
        return 1;
    }

    // create TCP socket
    socketfd = socket(AF_INET, SOCK_STREAM, 0); // IPV4

    if (socketfd == -1)
    {
        // print error
        perror("Error: socket creation is fail.");
    }

    // prepare socketaddress_in structure
    server.sin_family = AF_INET;           // IPV4
    // server.sin_addr.s_addr = INADDR_ANY; // flexible addr
    server.sin_addr.s_addr = inet_addr("127.0.0.87");
    server.sin_port = htons(portNum);    // specific port 1314 transfer local string(endian) translate to internet's string

    // Bind
    if (bind(socketfd, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        // print error
        perror("Error: bind failed.");
        return 1;
    }

    // Listen
    listen(socketfd, 5);

    // Accept incoming connection
    puts("Waiting for incoming connections...");
    pthread_t p_thread[5];
    int i = 0;
    cli_len = sizeof(client);

    while (1)
    {
        cli[clientNum].fd = accept(socketfd, (struct sockaddr *)&client, (socklen_t *)&cli_len);
        if (cli[clientNum].fd)
        {
            puts("Connection accepted!");

            // pthread to handle each client
            // pthread_t p_thread;

            //pthread_t p_thread[5];
            create_socket = malloc(1);
            *create_socket = cli[clientNum].fd; // parameter of handler
            cli[clientNum].leave = 0;           // login
            clientNum++;
            // if (pthread_create(&p_thread, NULL, connection_handler, (void *)create_socket) < 0)
            // {
            //     perror("pthread creation is fail.");
            //     return 1;
            // }

            if (pthread_create(&p_thread[i], NULL, connection_handler, (void *)create_socket) < 0)
            {
                perror("pthread creation is fail.");
                return 1;
            }
            i++;
            puts("Handler assigned!");

        // while(i <= 5){
        //     pthread_join(p_thread[i++], NULL);
        //     printf("%d:\n",i);           
        // }

        }else if(create_socket < 0){
            perror("Accept failed.");
            return 1;
        }
        //pthread_join(p_thread, NULL);
    }
    
    while(i <= 5)
    {
        pthread_join(p_thread[i++], NULL);
        printf("%d:\n",i);           
    }

    return 0;
}

