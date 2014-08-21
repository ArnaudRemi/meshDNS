//meshDNS by rémi arnaud

#include <stdio.h> 
#include <string.h> 
#include <stdlib.h> 
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include <unistd.h>
#include <fcntl.h>

#include "meshDNS.h"
#include "parsing.h"
#include "config.h"
#include "request.h"
#include "response.h"

#define PORT 8173
#define BUFSIZE 512

char *getPublicKey(){
  char *key;
  char c;
  int fd;
  int i = 0;

  srand(time(NULL));
  key = malloc(101);
  key[100] = '\0';

  if ((fd = open(".pkey", O_RDONLY)) != -1)
    read(fd, key, 100);
  else{
    fd = open(".pkey", O_WRONLY | O_CREAT);
    while (i < 100){
      c = (rand() % 254) + 1; 
      key[i] = c;
      write(fd, &c, 1);
      ++i;
    }
  }
  return key;
}

int main(int ac, char **av){
  struct sockaddr_in	si_me;
  struct sockaddr_in	si_other;
  struct sockaddr_in	*si_bcast;
  int			sock;
  char			*buf;
  mdns			*req;
  resp      *response;
  int			recv_len;
  int			slen;
  struct ifreq		ifr;
  struct ifreq    ifr2;
  fd_set		rfds;
  int			retval;
  linfo			infos;
  char			*publickey;
  resp *tmp;


  slen = sizeof(si_other);
  if ((sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0){
    printf("Error : create UDP socket fail\n");
    exit(1);
	  }
  ifr.ifr_addr.sa_family = AF_INET;
  strncpy(ifr.ifr_name, "wlan0", IFNAMSIZ-1);
  ioctl(sock, SIOCGIFBRDADDR, &ifr); 
  si_bcast = ((struct sockaddr_in *) &ifr.ifr_broadaddr); 
  si_bcast->sin_family = AF_INET;
  si_bcast->sin_port = htons(PORT);
  
//  strncpy(ifr.ifr_name, "wlan0", IFNAMSIZ-1);
  ifr2.ifr_addr.sa_family = AF_INET;
  strncpy(ifr2.ifr_name, "wlan0", IFNAMSIZ-1);
  ioctl(sock, SIOCGIFADDR, &ifr2);

  int broadcastEnable=1;
  int ret=setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &broadcastEnable, sizeof(broadcastEnable));

  buf = (char *) malloc(BUFSIZE + 1);
  req = (mdns *) malloc(sizeof(mdns) + 1);
  if (req == NULL || buf == NULL){
    printf("Error : malloc failed\n");
    exit(1);
  }

  publickey = getPublicKey();

  memset((char *) &si_me, 0, sizeof(si_me));
  
  si_me.sin_family = AF_INET;
  si_me.sin_port = htons(PORT);
  si_me.sin_addr.s_addr = ((struct sockaddr_in*)&ifr2.ifr_addr)->sin_addr.s_addr;

  printf("ip address: %s\n", inet_ntoa(si_me.sin_addr));

  infos.me = newKey(si_me, publickey, NULL);
  infos.names = NULL;
  infos.keys = infos.me;

  si_me.sin_addr.s_addr = htonl(INADDR_ANY);

  if (parseFile(&infos) == -1){
    printf("Error : config file parsing failed\n");
    exit(1);
  }
  printf("parse config ok\n");

  if( bind(sock , (struct sockaddr*)&si_me, sizeof(si_me)) < 0)
    {
      printf("Error : bind socket fail\n");
      exit(1);
    }

  while(1)
    {
      FD_ZERO(&rfds);
      FD_SET(0, &rfds);
      FD_SET(sock, &rfds);
      
      printf("Waiting for data...\n");

      if (select(sock + 1, &rfds, NULL, NULL, NULL) < 0)
      	printf("select error\n");
      else {
      	if (FD_ISSET(0, &rfds)){
      	  read(0, buf, BUFSIZE);
      	  printf("Send Request\n");
      	  req = parseBuf(buf);
      	  if (req != NULL){            
      	    //send request to broadcast
      	    if (sendto(sock, req, sizeof(mdns), 0, (struct sockaddr*) si_bcast, slen) == -1){
              printf("Error : sendto fail\n");
            }
      	    free(req);
      	  }
      	}
      	else if(FD_ISSET(sock, &rfds)){
      	  if ((recv_len = recvfrom(sock, buf, sizeof(mdns), 0, (struct sockaddr *) &si_other, &slen)) < 0){
     	      printf("Error : recvfrom fail\n");
     	      exit(1);
     	    }
      	  printf("Receive Request: %s\n", &(((mdns *)buf)->request[0]));
      	  response = parseReq((mdns *)buf, &infos);
          tmp = response;
          while (tmp){
            sendto(sock, tmp->req, sizeof(mdns), 0, (struct sockaddr*) &si_other, slen);
            printf("a respond is send\n");
            tmp = tmp->next;
          }
          freeResp(response);
      	}
      }      
    }
 
    close(sock);
    return 0;
}
