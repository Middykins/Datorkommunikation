#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
/* You will to add includes here */

// Enable if you want debugging to be printed, see examble below.
// Alternative, pass 
#define DEBUG

const int MAXDATASIZE = 256;
// Included to get the support library
#include <calcLib.h>

int main(int argc, char *argv[])
{

  /*
    Read first input, assumes <ip>:<port> syntax, convert into one string (Desthost) and one integer (port). 
     Atm, works only on dotted notation, i.e. IPv4 and DNS. IPv6 does not work if its using ':'. 
  */
  char delim[]=":";
  char *Desthost=strtok(argv[1],delim);
  char *Destport=strtok(NULL,delim);
  // *Desthost now points to a sting holding whatever came before the delimiter, ':'.
  // *Dstport points to whatever string came after the delimiter. 
  int sockfd;
	struct addrinfo hints, *servinfo, *p;
	int rv;
	int numbytes;
	char buf[MAXDATASIZE];

	if (argc != 2) 
  {
	  printf("Try again, enter host and port\n");
	  exit(1);
	}

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	if ((rv = getaddrinfo(Desthost, Destport, &hints, &servinfo)) != 0) 
  {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}

	// loop through all the results and make a socket
	for(p = servinfo; p != NULL; p = p->ai_next) 
  {
		if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) 
    {
			perror("talker: socket");
			continue;
		}

    if (connect(sockfd,p->ai_addr, p->ai_addrlen) < 0 ) 
    {
	  perror("talker2: connect .\n");
    close (sockfd);
    continue;

	  }
    

		break;
	}

  printf("Managed to create socket\n");

	if (p == NULL) 
  {
		fprintf(stderr, "talker: failed to create socket\n");
		return 2;
	}

  initCalcLib();
  char *ptr;
  ptr=randomType(); // Get a random arithemtic operator. 

  double f1,f2,fresult;
  int i1,i2,iresult;

  char *lineBuffer=NULL;
  size_t lenBuffer=0;
  ssize_t recv(int sockfd, char &lineBuffer, size_t len, int flags);
  printf("Print a command: ");
  //=getline(&lineBuffer,&lenBuffer,stdin);
  
  printf("got:> %s \n",lineBuffer);

  char command[10];

  
  rv=sscanf(lineBuffer,"%s",command);

  printf("Command: |%s|\n",command);
  
  if(command[0]=='f'){
    printf("Float\t");
    rv=sscanf(lineBuffer,"%s %lg %lg",command,&f1,&f2);
    if(strcmp(command,"fadd")==0){
      fresult=f1+f2;
    } else if (strcmp(command, "fsub")==0){
      fresult=f1-f2;
    } else if (strcmp(command, "fmul")==0){
      fresult=f1*f2;
    } else if (strcmp(command, "fdiv")==0){
      fresult=f1/f2;
    }
    printf("%s %8.8g %8.8g = %8.8g\n",command,f1,f2,fresult);
  } else {
    printf("Int\t");
    rv=sscanf(lineBuffer,"%s %d %d",command,&i1,&i2);
    if(strcmp(command,"add")==0){
      iresult=i1+i2;
    } else if (strcmp(command, "sub")==0){
      iresult=i1-i2;
      printf("[%s %d %d = %d ]\n",command,i1,i2,iresult);
    } else if (strcmp(command, "mul")==0){
      iresult=i1*i2;
    } else if (strcmp(command, "div")==0){
      
      iresult=i1/i2;
    } else {
      printf("No match\n");
    }

    printf("%s %d %d = %d \n",command,i1,i2,iresult);

  /* Do magic */
  int port=atoi(Destport);
#ifdef DEBUG 
  printf("Host %s, and port %d.\n",Desthost,port);
#endif

  
}
}