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
#include <string>
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

  if (Desthost==NULL)
  {
    Desthost = (char*)"0.0.0.0";
  }
  if (Destport==NULL)
  {
    Destport = (char*)"0";
  }
  
  
  int sockfd;
	struct addrinfo hints, *servinfo, *p;
	int rv;
	char buf[MAXDATASIZE];
  char e621[] = "OK\n";

	if (argc != 2) 
  {
	  printf("Try again, enter host and port\n");
	  exit(1);
	}
  else
  {
    int port=atoi(Destport);
    #ifdef DEBUG 
    printf("Host %s, and port %d.\n",Desthost,port);
    #endif
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

  if(recv(sockfd, buf,sizeof(buf),0) == -1)
  {
    perror("RECV");
    exit(1);
  }

  printf("%s\n", buf);
  if (strcmp(buf, "TEXT TCP 1.0\n\n") == 0 || strcmp(buf, "TEXT TCP 1.1\n\n") == 0)
  {
    printf("Matching messages");
  }
  else
  {
    perror("Not matching\n");
    exit(1);
  }

  if (send(sockfd, e621, strlen(e621), 0) == -1)
  {
    perror("Message not sent");
    exit(1);
  }
  
  double f1,f2,flotRes;
  int i1,i2,intRes;

  char lineBuffer[MAXDATASIZE];
  if(recv(sockfd, lineBuffer, sizeof(lineBuffer), 0) == -1)
  {
    perror("Error obtaining data");
  }
  else
  {
    printf("Data obtained correctly: %s", lineBuffer);
  }
  printf("Print a command: ");
  
  printf("%s \n",lineBuffer);

  char command[10];

  
  rv=sscanf(lineBuffer,"%s",command);

  printf("Command: |%s|\n",command);
  
  if(command[0]=='f')
  {
    printf("Float\t");
    rv=sscanf(lineBuffer,"%s %lg %lg",command,&f1,&f2);
    if(strcmp(command,"fadd")==0){
      flotRes=f1+f2;
    } 
    else if (strcmp(command, "fsub")==0)
    {
      flotRes=f1-f2;
    } 
    else if (strcmp(command, "fmul")==0)
    {
      flotRes=f1*f2;
    } 
    else if (strcmp(command, "fdiv")==0)
    {
      flotRes=f1/f2;
    }
    printf("%s %8.8g %8.8g = %8.8g\n",command,f1,f2,flotRes);
  } 
  else 
  {
    printf("Int\t");
    rv=sscanf(lineBuffer,"%s %d %d",command,&i1,&i2);
    if(strcmp(command,"add")==0)
    {
      intRes=i1+i2;
    } 
    else if (strcmp(command, "sub")==0)
    {
      intRes=i1-i2;
      
    } 
    else if (strcmp(command, "mul")==0)
    {
      intRes=i1*i2;
    } 
    else if (strcmp(command, "div")==0)
    {
      
      intRes=i1/i2;
    } 
    else 
    {
      printf("No match\n");
    }

    printf("%s %d %d = %d\n",command,i1,i2,intRes);
  }
    if (command[0] == 'f')
    {
      char floatSenderValue[MAXDATASIZE];
      sprintf(floatSenderValue,"%8.8g\n",flotRes);
      if (send(sockfd, floatSenderValue, strlen(floatSenderValue), 0) == -1)
      {
        perror("Error sending\n");
        exit(1);
      }
      else
      {
        printf("Sent: %s\n", floatSenderValue);
      }
    }

    else
    {
      char intSenderValue[MAXDATASIZE];
      sprintf(intSenderValue,"%d\n", intRes);
      if (send(sockfd, intSenderValue, strlen(intSenderValue), 0) == -1)
      {
        perror("Error sending\n");
        exit(1);
      }
      else
      {
        printf("Sent: %s\n", intSenderValue);
      }
    }

    if (recv(sockfd,buf,sizeof(buf),0) != 0)
    {
      perror("Failed to recieve\n");
    }
    else
    {
      printf("%s\n", buf);
    }
    
    

  /* Do magic */

  
}