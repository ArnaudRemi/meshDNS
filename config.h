//meshDNS by rémi arnaud

#ifndef __CONFIGH__
#define __CONFIGH__

#include <netinet/in.h>
#include "meshDNS.h"

typedef struct snamelist{
  struct sname		*name;
  struct snamelist	*next;
} lname;

typedef struct skey {
  struct sockaddr_in	ip;
  char   key[128];
  struct snamelist	*lName;
  struct skey		*next;
} key;

typedef struct skeylist {
  struct skey		*key;
  struct skeyname	*next;
} lkey;

typedef struct sname {
  char			name[128];
  struct skeylist	*lKey;
  struct sname		*next;
} name;

typedef struct localinfo {
  key     *me;
  name    *names;
  key     *keys;
} linfo;

char parseFile(linfo *);

#endif
