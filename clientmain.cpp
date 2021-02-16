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
  if (argc != 2) 
  {
    fprintf(stderr,"usage: %s host:port (%d)\n",argv[0],argc);
	  exit(1);
	}
  /*
    Read first input, assumes <ip>:<port> syntax, convert into one string (Desthost) and one integer (port). 
     Atm, works only on dotted notation, i.e. IPv4 and DNS. IPv6 does not work if its using ':'. 
  */

  char buf[MAXDATASIZE];
  char delim[]=":";
  char msg[] = "OK\n";
  char *Desthost=strtok(argv[1],delim);
  char *Destport=strtok(NULL,delim);
  double f1,f2,floatRes;
  int i1,i2,intRes,rv,sockfd;
  struct addrinfo hints, *servinfo, *p;
  
  if ((rv = getaddrinfo(Desthost, Destport, &hints, &servinfo)) != 0) 
  {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}

  if (Desthost==NULL)
  {
    Desthost = (char*)"0.0.0.0";
  }
  if (Destport==NULL)
  {
    Destport = (char*)"0";
  }

  int port=atoi(Destport);
  printf("Host %s, and port %d.\n",Desthost,port);

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

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

	if (p == NULL) 
  {
		perror("talker: failed to create socket\n");
		exit(1);
	}
  #ifdef DEBUG 
  printf("Connected to %s:%d.\n",Desthost,port);
  #endif

  if(recv(sockfd, buf,sizeof(buf),0) == -1)
  {
    perror("RECV");
    exit(1);
  }

  if (strcmp(buf, "TEXT TCP 1.0\n\n") != 0)
  {
    perror("Not matching\n");
    exit(1);
  }
  memset(buf, 0, sizeof(buf));

  if (send(sockfd, msg, strlen(msg), 0) == -1)
  {
    perror("Message not sent");
    exit(1);
  }
  if(recv(sockfd, buf, sizeof(buf), 0) == -1)
  {
    perror("Error obtaining data");
  }
 
  printf("ASSIGNMENT: ");
  
  printf("%s",buf);

  char command[10];

  rv=sscanf(buf,"%s",command);
  
  if(command[0]=='f')
  {
    rv=sscanf(buf,"%s %lg %lg",command,&f1,&f2);
    if(strcmp(command,"fadd")==0){
      floatRes=f1+f2;
    } 
    else if (strcmp(command, "fsub")==0)
    {
      floatRes=f1-f2;
    } 
    else if (strcmp(command, "fmul")==0)
    {
      floatRes=f1*f2;
    } 
    else if (strcmp(command, "fdiv")==0)
    {
      floatRes=f1/f2;
    }
    #ifdef DEBUG 
    printf("Calculated the result to %8.8g\n",floatRes);
    #endif
  } 
  else 
  {
    rv=sscanf(buf,"%s %d %d",command,&i1,&i2);
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
    #ifdef DEBUG 
    printf("Calculated the result to %d\n",intRes);
    #endif
  }
  memset(buf, 0, sizeof(buf));
  
    if (command[0] == 'f')
    {
      sprintf(buf,"%8.8g\n",floatRes);
      if (send(sockfd, buf, strlen(buf), 0) == -1)
      {
        perror("Error sending\n");
        exit(1);
      }
    }

    else
    {
      sprintf(buf,"%d\n", intRes);
      if (send(sockfd, buf, strlen(buf), 0) == -1)
      {
        perror("Error sending\n");
        exit(1);
      }
    }
    memset(buf, 0, sizeof(buf));

    if (recv(sockfd,buf,sizeof(buf),0) == -1)
    {
      perror("Failed to recieve\n");
    }
      printf("%s ", buf);
      if (command[0]=='f')
      {
        printf("(myresult=%8.8g)\n", floatRes);
      }
      else printf("(myresult=%d)\n", intRes);
      
    
    

  /* Do magic */

  
}