#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


int main(int argc , char *argv[])
{

    //socket的建立
    int sockfd = 0;
    sockfd = socket(AF_INET , SOCK_STREAM , 0);

    if (sockfd == -1){
        printf("Fail to create a socket.");
    }

    //socket的連線

    struct sockaddr_in info;
    bzero(&info,sizeof(info));
    info.sin_family = PF_INET;

    //localhost test
    info.sin_addr.s_addr = inet_addr("127.0.0.1");
    info.sin_port = htons(8700);


    int err = connect(sockfd,(struct sockaddr *)&info,sizeof(info));
    if(err==-1){
        printf("Connection error");
    }


    //Send a message to server
    char message[128];
    char *instr;
    char instr_array[16];
    char YN[1];
    const char* d = "  ";
    
    char name[10], group[15], cli_msg[128], command[16];
    char receiveMessage[128] = {};
    recv(sockfd,receiveMessage,sizeof(receiveMessage),0); //HELLO 54
    printf("%s",receiveMessage);
    memset(receiveMessage,0,100);
    
    //scanf("%s",&group);
    fgets(group,sizeof(group),stdin);
    send(sockfd,group,sizeof(group),0); //group
    printf("send group ok\n");
    recv(sockfd,receiveMessage,sizeof(receiveMessage),0); //NAME 60
    printf("%s",receiveMessage);
    memset(receiveMessage,0,128);
    //recv(sockfd,receiveMessage,sizeof(receiveMessage),0);  //req
    
    printf("%s\n",receiveMessage);
    //printf("SERVER ask for name\n");
    //scanf("%s",&name);
    fgets(name,sizeof(name),stdin);
    send(sockfd,name,sizeof(name),0); //name
    printf("send name ok\n");
    memset(message,0,128);
    
    
    
    /*recv(sockfd,receiveMessage,sizeof(receiveMessage),0);
    printf("%s",receiveMessage);
    memset(receiveMessage,0,100);*/
    
    
    
    while (!(strcmp(message,"exit") == 0))
    {
    
    memset(receiveMessage,0,128);
    recv(sockfd,receiveMessage,sizeof(receiveMessage),0);  //req 92
    printf("%s",receiveMessage);
    //memset(receiveMessage,0,100);
    
    memset(message,0,128);
    //scanf("%s",&message);
    fgets(message,128,stdin);
    printf("----\n");
    
    send(sockfd,message,sizeof(message),0); //command
    instr = strtok(message , d);
    printf("instr:%s\n",instr);
    strcpy(instr_array,instr);
    recv(sockfd,receiveMessage,sizeof(receiveMessage),0);  //PROCESSING 115
    printf("%s\n",receiveMessage);
    //--------------------------------------------------------------------------create
    if ((strcmp(instr_array,"create") == 0)) {
        recv(sockfd,receiveMessage,sizeof(receiveMessage),0);  //PROCESSING 115
        printf("%s\n",receiveMessage);

    }
    
    //--------------------------------------------------------------------------write
    else if ((strcmp(instr_array,"write") == 0)) {
        printf("into write\n");
        recv(sockfd,receiveMessage,sizeof(receiveMessage),0);  //continue? 156
        printf("%s\n",receiveMessage);
        fgets(message,128,stdin);
        send(sockfd,message,sizeof(message),0); //yes no
        YN[0] = message[0];
        while (YN[0]!='N') {
        memset(message,0,128);
        printf("-------------\n");
        recv(sockfd,receiveMessage,sizeof(receiveMessage),0);  //continue writing? 163
        printf("%s",receiveMessage);
        
        fgets(message,128,stdin);
        
        send(sockfd,message,sizeof(message),0); //yes no
        printf("send :%s",message);
        printf("rec yn\n");
        YN[0] = message[0];
        }
        printf("end up write\n");

    }
    //-------------------------------------------------------------------------read
    else if ((strcmp(instr_array,"read") == 0)) {
        recv(sockfd,receiveMessage,sizeof(receiveMessage),0);  //PROCESSING 115
        printf("%s\n",receiveMessage);

    }
    memset(receiveMessage,0,128);
    //recv(sockfd,receiveMessage,sizeof(receiveMessage),0); //result 139
    //printf("%s",receiveMessage);
    
    
    
    
    }
    

    //printf("%s",receiveMessage);
    printf("close Socket\n");
    close(sockfd);
    return 0;
}