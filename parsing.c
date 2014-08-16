//meshDNS by rémi Arnaud

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/in.h>

#include "parsing.h"
#include "meshDNS.h"

mdns *createReq(char* str, enum etype t){
  printf("generate request\n");
  mdns	*req;
  int	i = 0;

  req = malloc(sizeof(mdns));
  if (req == NULL){
    printf("Error : malloc fail\n");
    return NULL;
  }

  req->infos = IPV6 | IPV4;
  req->type = t;
  while(str[i]){
    req->request[i] = str[i];
    ++i;
  }
  req->request[i] = '\0';
  //ret-> hash = md5()

  return req;
}

mdns *parseBuf(char *buf){
  int	i = 0;
  char	*split;
  mdns	*ret;

  if ((split = strchr(buf, ':')) == NULL){
    printf("Error : invalid syntax\n");
    return NULL;
  }
  ++split;
  split[strlen(split) - 1] = '\0';
  printf("%s\n", split);
  
  if (strncmp(buf, "ip", 2) == 0)
    ret = createReq(split, IPTOKEY);
  else if (strncmp(buf, "key", 3) == 0)
    ret = createReq(split, KEY);
  else if (strncmp(buf, "name", 4) == 0)
    ret = createReq(split, NAME);
  else{
    printf("Error : invalid syntax\n");
    return NULL;
  }
  return ret;
}

void parseResponse(mdns *req, linfo *infos){
  
  if (req->type == NAME)
    responseName(req, infos);
  else if (req->type == KEY)
    responseKey(req, infos);
  else if (req->type == IPTOKEY)
    responseIpToKey(req, infos);
  else if (req->type == IPTONAME)
    responseIpToName(req, infos);  

  return NULL;
}

resp *parseReq(mdns *req, linfo *infos){
  
  //hash verification

  if (req->infos & RESPOND){
    parseResponse(req, infos);
    return NULL;
  }

  if (req->type == NAME)
    return requestName(req, infos);
  else if (req->type == KEY)
    return requestKey(req, infos);
  else if (req->type == IPTOKEY)
    return requestIpToKey(req, infos);
  else if (req->type == IPTONAME)
    return requestIpToName(req, infos);

  return NULL;
}
