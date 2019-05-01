#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<pthread.h>
 
//the thread function
void *connection_handler(void *);
 
/* error: wrapper for perror */
void error(char *msg){
    perror(msg);
}

int fak (int a){
	if (a==0)
		return 1;
	else
		return (a * fak(a-1));
}

/* unused function to be used to demonstrate overflow vulnerability */
void func(){
    puts("SMASH");
}

int main(int argc , char *argv[])
{
    int socket_desc , client_sock , c , *new_sock;
    struct sockaddr_in server , client;
     
    //Create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1)
    {
        printf("[-] Could not create socket");
    }

     
    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( 8888 );
     
    //Bind
    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        //print the error message
        perror("[-] bind failed. Error");
        return 1;
    }
     
    //Listen
    listen(socket_desc , 3);
     
    //Accept and incoming connection
    puts("[*] Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);

    while( (client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c)) )
    {
        puts("[+] Connection accepted");
         
        pthread_t sniffer_thread;
        new_sock = malloc(1);
        *new_sock = client_sock;
         
        if( pthread_create( &sniffer_thread , NULL ,  connection_handler , (void*) new_sock) < 0)
        {
            perror("[-] could not create thread");
            return 1;
        }
         
        //Now join the thread , so that we dont terminate before the thread
        //pthread_join( sniffer_thread , NULL);
    }
     
    if (client_sock < 0)
    {
        perror("accept failed");
        return 1;
    }
     
    return 0;
}
 
/*
 * This will handle connection for each client
 * */
void *connection_handler(void *socket_desc)
{
    //Get the socket descriptor
    int sock = *(int*)socket_desc;
    int read_size;
    int bufsize = (fak(8));
    char fish[] = "><(((('> ";
    char echo_message[bufsize/2];
    char client_message[bufsize];

    
     
    //Receive a message from client
    while( (read_size = recv(sock , client_message , bufsize , 0)) > 0 )
    {
	printf("[+] Received: %s", client_message);
        strcpy(echo_message, client_message);
	write(sock , fish , strlen(fish));
        write(sock , echo_message , strlen(echo_message));
    }
     
    if(read_size == 0)
    {
        puts("[-] Client disconnected");
        fflush(stdout);
    }
    else if(read_size == -1)
    {
        perror("[-]recv failed");
    }
         
    //Free the socket pointer
    free(socket_desc);
     
    return 0;
}
