//meshDNS by rémi arnaud

#ifndef __MESHDNSH__
#define __MESHDNSH__

#define IPV6	 0b00000001
#define IPV4    0b00000010
#define RESPOND	0b10000000

#define REQUESTSIZE 512
#define REQUEST 0
#define RESPONSE 256

#include "config.h"

typedef struct smdns {
  char	infos;
  char	hash[16];
  char	type;
  char  request[REQUESTSIZE];
} mdns;

enum etype {
  NAME,
  KEY,
  IPTOKEY,
  IPTONAME
} etype; 

typedef struct sresponse {
  struct smdns *req;
  struct sresponse *next;
} resp;

#endif
