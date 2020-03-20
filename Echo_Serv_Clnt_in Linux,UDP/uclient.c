#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define BUF_SIZE 30
void error_handling(char *message);

int main(int argc,char *argv[]){
	int sock;
	char message[BUF_SIZE];
	int str_len;
	socklen_t adr_sz;
	
	struct sockaddr_in serv_adr,from_adr;
	if(argc !=3){
	/*  		EX)./userver 127.0.0.1 9190     		   */
		printf("Usage : %s <iIP <port> \n",argv[0]);
		exit(1);
	}
	/*		CREATE IPv4 DATAGRAM SOCKET		*/
    	sock=socket(PF_INET,SOCK_DGRAM,0);
	if(sock == -1){
		error_handling("UDP socket creation error");
	}

	memset(&serv_adr,0,sizeof(serv_adr));
	/*		CREATE IPv4 Address		*/
	serv_adr.sin_family=AF_INET;
	serv_adr.sin_addr.s_addr=inet_addr(argv[1]);
	serv_adr.sin_port=htons(atoi(argv[2]));
	
	while(1){
		fputs("Insert message(/q or /Q to quit)",stdout);
		fgets(message,sizeof(message),stdin);
		if(!strcmp(message,"/q\n") || !strcmp(message,"/Q\n"))
			break;

		sendto(sock,message,strlen(message),0,(struct sockaddr*)&serv_adr,sizeof(serv_adr));
		adr_sz=sizeof(from_adr);
		str_len=recvfrom(sock,message,BUF_SIZE,0,(struct sockaddr*)&from_adr,&adr_sz);
		message[str_len]=0;
		printf("Message from server: %s",message);	
	}
	close(sock);
	return 0;
}
void error_handling(char *message){
	fputs(message,stderr);
	fputc('\n',stderr);
	exit(1);
}