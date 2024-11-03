#include <stdio.h>      //printf
#include <string.h>     //strlen
#include <sys/socket.h> //socket
#include <arpa/inet.h>  //inet_addr

#define portNum 1314
#define sizes 256

int main(int argc, char *argv[])
{
    int sock;
    struct sockaddr_in server; // socket address -> fixed

    if (argc != 1)
    {
        // argc is int
        // argc is the numbers of commands starting this program
        // argc is bigger then 1 (this project's name)
        puts("Usage : ./client \n");
        return 1;
    }
    //int portNum = 8787;

    // create socket
    // domain send message through ipv4, stream type TCP, protocol usually set 0
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1)
    {
        puts("failed to create a socket");
    }
    puts("Socket created");

    server.sin_addr.s_addr = inet_addr("127.0.0.87");
    server.sin_family = AF_INET;
    server.sin_port = htons(portNum);

    // connect to remote server
    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        perror("connect failed. Error");
        return 1;
    }
    puts("connected");

    //pthread_create(&tid[i], NULL, cientThread, NULL) != 0

    // wait for user to choose user
    char serverMsg[sizes], name[10], instruction[10], command[sizes], content[sizes];
    recv(sock, serverMsg, sizes, 0); //info: pls choose user

    printf("%s\n", serverMsg);
    scanf("%s", name); // wait for typing name
    while (strcmp(name, "A") != 0 && strcmp(name, "B") != 0 && strcmp(name, "C") != 0 && strcmp(name, "D") != 0 && strcmp(name, "E") != 0 && strcmp(name, "F"))
    {
        /* check if client type the correct name */
        printf("Input error, again : ");
        scanf("%s", name);              //scanf until the client choose valid user.
    }
    bzero(serverMsg, sizes);
    send(sock, name, 10, 0); // send name to server
    recv(sock, serverMsg, sizes, 0); //Info: choice check
    printf("%s\n", serverMsg);
    bzero(serverMsg, sizes);
    // type

    /*for typeing texts in*/
    // char ch;
    // scanf("%c",&ch);


    while (1)
    {
        //recv(sock,serverMsg, sizes, 0); //info: pls input command
        printf("Please Input your command :\n1.create filename access\n2.read filename\n3.write filename a/o\n4.modify filename access\n5.exit\n");
        //printf("%s\n",serverMsg);
        //bzero(serverMsg,sizes);

        
        gets(command); //怕後面還沒切完，就被覆蓋
                       // serverMsg 初始化
        
        //fgets(command, sizes, stdin); //let user input command (使用上比 gets 安全, 但他媽的會多一個 "\n" 在字串裡)

        send(sock, command, sizes, 0);

        if (strcmp(command, "exit") == 0){
            break; // leave while loop
        }
        else{
            /***split_command line***/
            char split[3][20];
            char *space = " ";
            char *sentence;
            sentence = strtok(command, space);
            int sent_pointer = 0;
            while (sentence != NULL)
            {
                strcpy(split[sent_pointer], sentence);
                sentence = strtok(NULL, space);
                sent_pointer++;
            }

            recv(sock, instruction, 11, 0); // get create/read/write/changemode from server
            // received create
            if (strcmp(instruction, "create") == 0)
            {
                //send(sock, "got create", 20, 0);
                // receave file exist / create successed
                bzero(serverMsg, sizes);
                recv(sock, serverMsg, sizes, 0); //info: creation successed! or file has exist.
                printf("%s \n", serverMsg);
            }
            else if (strcmp(instruction, "read") == 0)
            {
                // send(sock, "reading", 20, 0);//to cli_waiting
                //   r w  |  r w  | r w
                //  owner | group | other
                recv(sock, serverMsg, sizes, 0); // permitted or file_not_exist or someone_writing
                if (strcmp(serverMsg, "permitted") == 0)
                {
                    bzero(serverMsg,sizes);
                    puts("Permitted to read the file");
                    puts("Start dowloading...");
                    //bzero(command, sizes);
                    recv(sock, content, sizes, 0); // receave file content
                    // printf("%s", serverMsg);
                    FILE *fp = fopen(split[1], "w+"); // write file content
                    fprintf(fp, "%s", content);
                    fclose(fp);
                    recv(sock, serverMsg, sizes, 0); // done reading
                    printf("%s\n", content);         // print file text
                    printf("%s\n", serverMsg);       // Done reading
                }
                else
                {
                    printf("%s\n", serverMsg);
                }
            }
            /*write*/

            else if (strcmp(instruction, "write") == 0)
            {
                // char ch;
                //puts("writing");
                recv(sock, serverMsg, sizes, 0); // permitted or file_not_exist or someone_reading
                if (strcmp(serverMsg, "permitted") == 0)
                {
                    // permitted
                    puts("Permmitted to write the file. Please type in ...");
                    char inputText[sizes];
                    //gets(inputText);
                    fgets(inputText, sizes, stdin); //let user input content
                    send(sock, inputText, sizes, 0); // send text to server
                    recv(sock, serverMsg, sizes, 0); // start uploading file
                    printf("%s\n", serverMsg);
                    bzero(serverMsg,sizes);
                    recv(sock, serverMsg, sizes, 0); // uploading completed
                    printf("%s\n", serverMsg);
                }
                else
                {
                    // file_not_exist or someone_writing
                    printf("%s\n", serverMsg);
                }
            }
            /*modify*/
            else if (strcmp(instruction, "changemode") == 0)
            {
                puts("chmode test");            
                bzero(serverMsg,sizes);
                recv(sock, serverMsg, sizes, 0); // permitted, etc
                //puts("In modify");
                if(strcmp(serverMsg, "permitted") == 0){
                    bzero(serverMsg,sizes);
                    recv(sock, serverMsg, sizes, 0); // changed
                    printf("%s\n", serverMsg);
                    //printf("chmode test\n");
                    bzero(serverMsg,sizes);
                }
                else
                {
                    printf("%s\n", serverMsg);
                }

            }
            else
            {

                recv(sock, serverMsg, sizes, 0);
                //perror("%s",serverMsg)
                //puts("ErrorInput");
                printf("%s\n", serverMsg);

            }
        }
    }
    close(sock);
    return 0;
}

// void *connection_handler(void *sock){
// // wait for user to choose user
//     char serverMsg[sizes], name[10], instruction[10], command[sizes], content[sizes];
//     recv(sock, serverMsg, sizes, 0); //info: pls choose user

//     printf("%s\n", serverMsg);
//     scanf("%s", name); // wait for typing name
//     while (strcmp(name, "A") != 0 && strcmp(name, "B") != 0 && strcmp(name, "C") != 0 && strcmp(name, "D") != 0 && strcmp(name, "E") != 0 && strcmp(name, "F"))
//     {
//         /* check if client type the correct name */
//         printf("Input error, again : ");
//         scanf("%s", name);              //scanf until the client choose valid user.
//     }
//     bzero(serverMsg, sizes);
//     send(sock, name, 10, 0); // send name to server
//     recv(sock, serverMsg, sizes, 0); //Info: choice check
//     printf("%s\n", serverMsg);
//     bzero(serverMsg, sizes);
//     // type

//     /*for typeing texts in*/
//     // char ch;
//     // scanf("%c",&ch);
    

//     while (1)
//     {
//         //recv(sock,serverMsg, sizes, 0); //info: pls input command
//         printf("Please Input your command :\n1.create filename access\n2.read filename\n3.write filename a/o\n4.modify filename access\n5.exit\n");
//         printf("%s",serverMsg);
//         //bzero(serverMsg,sizes);

        
//         gets(command); //怕後面還沒切完，就被覆蓋
//                        // serverMsg 初始化
        
//         //fgets(command, sizes, stdin); //let user input command (使用上比 gets 安全, 但他媽的會多一個 "\n" 在字串裡)

//         send(sock, command, sizes, 0);

//         if (strcmp(command, "exit") == 0){
//             break; // leave while loop
//         }
//         else{
//             /***split_command line***/
//             char split[3][20];
//             char *space = " ";
//             char *sentence;
//             sentence = strtok(command, space);
//             int sent_pointer = 0;
//             while (sentence != NULL)
//             {
//                 strcpy(split[sent_pointer], sentence);
//                 sentence = strtok(NULL, space);
//                 sent_pointer++;
//             }

//             recv(sock, instruction, 10, 0); // get create/read/write/changemode from server
//             // received create
//             if (strcmp(instruction, "create") == 0)
//             {
//                 //send(sock, "got create", 20, 0);
//                 // receave file exist / create successed
//                 bzero(serverMsg, sizes);
//                 recv(sock, serverMsg, sizes, 0); //info: creation successed! or file has exist.
//                 printf("%s \n", serverMsg);
//             }
//             else if (strcmp(instruction, "read") == 0)
//             {
//                 // send(sock, "reading", 20, 0);//to cli_waiting
//                 //   r w  |  r w  | r w
//                 //  owner | group | other
//                 recv(sock, serverMsg, sizes, 0); // permitted or file_not_exist or someone_writing
//                 if (strcmp(serverMsg, "permitted") == 0)
//                 {
//                     puts("Permitted to read the file");
//                     puts("Start dowloading...");
//                     //bzero(command, sizes);
//                     recv(sock, content, sizes, 0); // receave file content
//                     // printf("%s", serverMsg);
//                     FILE *fp = fopen(split[1], "w+"); // write file content
//                     fprintf(fp, "%s", content);
//                     fclose(fp);
//                     recv(sock, serverMsg, sizes, 0); // done reading
//                     printf("%s\n", content);         // print file text
//                     printf("%s\n", serverMsg);       // Done reading
//                 }
//                 else
//                 {
//                     printf("%s\n", serverMsg);
//                 }
//             }
//             /*write*/

//             else if (strcmp(instruction, "write") == 0)
//             {
//                 // char ch;
//                 //puts("writing");
//                 recv(sock, serverMsg, sizes, 0); // permitted or file_not_exist or someone_reading
//                 if (strcmp(serverMsg, "permitted") == 0)
//                 {
//                     // permitted
//                     puts("Permmitted to write the file. Please type in ...");
//                     char inputText[sizes];
//                     //gets(inputText);
//                     fgets(inputText, sizes, stdin); //let user input content
//                     send(sock, inputText, sizes, 0); // send text to server
//                     recv(sock, serverMsg, sizes, 0); // start uploading file
//                     printf("%s\n", serverMsg);
//                     bzero(serverMsg,sizes);
//                     recv(sock, serverMsg, sizes, 0); // uploading completed
//                     printf("%s\n", serverMsg);
//                 }
//                 else
//                 {
//                     // file_not_exist or someone_writing
//                     printf("%s\n", serverMsg);
//                 }
//             }
//             /*modify*/
//             else if (strcmp(instruction, "changemode") == 0)
//             {
//                 //puts("In modify");
//                 recv(sock, serverMsg, sizes, 0); // change done
//                 printf("%s\n", serverMsg);
//             }
//             else
//             {

//                 recv(sock, serverMsg, sizes, 0);
//                 //perror("%s",serverMsg)
//                 //puts("ErrorInput");
//                 printf("%s\n", serverMsg);

//             }
//         }
//     }
//     close(sock);
//     return 0;
// }