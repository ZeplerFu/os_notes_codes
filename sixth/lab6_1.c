#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h> 
#include <time.h>
#include <sched.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
   
#define PATH "lab6_1_socket" 

int main(int argc, char *argv[])
{
    int n = atoi(argv[1]);

    int ret,len;
    
    /*define client parameter*/
    int sockfd_clie;
    struct sockaddr_un clieaddr;  
    
    /*define server parameter*/
    int connfd,sockfd_serv;
    struct sockaddr_un servaddr;  
    
    /*fork a process*/
    pid_t pid;
    pid=fork();

    if(pid > 0) //parent process as client
    {
        
        /*socket: create a socket (client)*/
        sockfd_clie=socket(AF_UNIX,SOCK_STREAM,0);  
        if(sockfd_clie<0)  
        {  
            perror("socket failed");  
            return 1;
        }     
        
        servaddr.sun_family=AF_UNIX;  
        strcpy(servaddr.sun_path,PATH);  
        

        /*connet to the server*/
        while(connect(sockfd_clie,(struct sockaddr*)&servaddr,sizeof(servaddr))==-1) 
        {  
            usleep(100000);
            
        }
        
        /*read the buffer*/
        char msg[10];
        for(int i=0; i<n; i++)
        {
            int number=read(sockfd_clie,msg,sizeof(msg));
			msg[number]='\0';
			printf("%s  ",msg);
			fflush(stdout);
        }
        printf("\n");
        /*close all*/
        close(sockfd_clie);
        return 0;
        
    }
    else if(pid == 0) //child process as server
    {
        /*socket: create a socket(server)*/
        sockfd_serv=socket(AF_UNIX,SOCK_STREAM,0);
        if(sockfd_serv<0)  
        {  
            perror("socket failed");
	    return 1;
        }

        unlink(PATH); //delete the file created last time
        
        servaddr.sun_family=AF_UNIX;
        strcpy(servaddr.sun_path,PATH);

        /*bind: bind local ip and socket*/
        ret=bind(sockfd_serv,(struct sockaddr*)&servaddr,sizeof(servaddr));  
        if(ret==-1)
        {  
            perror("bind failed");  
            close(sockfd_serv);  
            return 1;
        }  
        
        /*listen: wait the client to connect and deal with the request*/
        usleep((rand()%5000000)+1000000);
        ret=listen(sockfd_serv,1);  
        if(ret==-1)  
        {  
            perror("listen failed");  
            close(sockfd_serv);
            return 1;
        }  
    
    	/*accept: receive a connection*/
        len=sizeof(clieaddr);
        connfd=accept(sockfd_serv,(struct sockaddr*)&clieaddr,&len);  
        if(connfd<0)
        {  
            perror("accept failed");  
            close(sockfd_serv);  
            return 1;
        }
    		
    	/*write a buffer*/
        for(int i=0;i<n;i++)
        {
            float num=0.25*i*i;
            char msg[10];
            sprintf(msg,"%.2f",num);
            write(connfd,msg,sizeof(msg));
            usleep(rand()%5000000);
        }
        /*close all*/
        close(sockfd_serv);
        close(connfd);
        return 0;
        
    }
    else
    {
        perror("fork failed");
        return 1;
    }
    
    return 0;
}


