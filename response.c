//meshDNS by rémi arnaud
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "meshDNS.h"
#include "config.h"
#include "parsing.h"
#include "request.h"

void addNameToInfo(name *n, linfo *infos){
  name *tmp;

  if (infos->names == NULL)
    infos->names = malloc(sizeof(name));

  tmp = infos->names;
  while(tmp->next)
    tmp = tmp->next;
  tmp->next = n;
}

void addKeyToInfo(key *k, linfo *infos){
  key *tmp;

  if (infos->keys == NULL)
    infos->keys = malloc(sizeof(key));

  tmp = infos->keys;
  while(tmp->next)
    tmp = tmp->next;
  tmp->next = k;
}

void responseName(mdns *req, linfo *infos){
  name *n;
  key *k;
  struct sockaddr_in ip;
  char found;

  printf("response: %s\n", &(req->request[RESPONSE]));

  // memset ip 0
  n = infos->names;
  k = infos->keys;
  found = 0;
  while (!found && n){
    if (strcmp(&(n->name[0]), &(req->request[REQUEST])) == 0)
      found = 1;
    else
      n = n->next;    
  }
  found = 0;
  while (!found && k){
    if (strcmp(&(k->key[0]), &(req->request[RESPONSE])) == 0)
      found = 1;
    else
      k = k->next;    
  }

  if (n == NULL){
    n = newName(&(req->request[REQUEST]), NULL);
    addNameToInfo(n, infos);
  }
  if (k == NULL){
    k = newKey(ip, &(req->request[RESPONSE]), NULL);
    addKeyToInfo(k, infos);
  }
    addKeyToName(k, n);
    addNameToKey(n, k);
}

void responseKey(mdns *req, linfo *infos){
  key *k;
  key *kbis;
  struct sockaddr_in ip;
  char found;

  k = infos->keys;
  kbis = infos->keys;
  found = 0;
  while (!found && k){
    if (strcmp(&(k->key[0]), &(req->request[REQUEST])))
      found = 1;
    else
      k = k->next;    
  }
  found = 0;
  while (!found && kbis){
    if (memcmp(&(kbis->ip), &(req->request[RESPONSE]), sizeof(struct sockaddr_in)) == 0)
      found = 1;
    else
      kbis = kbis->next;    
  }

  if (k && !kbis)
    memcpy(&(k->ip), &(req->request[RESPONSE]), sizeof(struct sockaddr_in));
  else if (!k && kbis)
    strcpy(&(kbis->key[0]), &(req->request[REQUEST]));
  else if (!k && !kbis){
    memcpy(&(ip), &(req->request[RESPONSE]), sizeof(struct sockaddr_in));
    k = newKey(ip, &(req->request[REQUEST]), NULL);
    addKeyToInfo(k, infos);
  }

}

void responseIpToName(mdns *req, linfo *infos){
  name *n;
  key *k;
  struct sockaddr_in ip;
  char found;

  // memset ip 0
  n = infos->names;
  k = infos->keys;
  found = 0;
  while (!found && n){
    if (strcmp(&(n->name[0]), &(req->request[RESPONSE])) == 0)
      found = 1;
    else
      n = n->next;    
  }
  found = 0;
  while (!found && k){
    if (memcmp(&(k->ip), &(req->request[REQUEST]), sizeof(struct sockaddr_in)) == 0)
      found = 1;
    else
      k = k->next;    
  }

  if (n == NULL){
    n = newName(&(req->request[RESPONSE]), NULL);
    addNameToInfo(n, infos);
  }
  if (k == NULL){
    memcpy(&(ip), &(req->request[REQUEST]), sizeof(struct sockaddr_in));
    k = newKey(ip, NULL, NULL);
    addKeyToInfo(k, infos);
  }
    addKeyToName(k, n);
    addNameToKey(n, k);
}

void responseIpToKey(mdns *req, linfo *infos){
  key *k;
  key *kbis;
  struct sockaddr_in ip;
  char found;

  k = infos->keys;
  kbis = infos->keys;
  found = 0;
  while (!found && k){
    if (strcmp(&(k->key[0]), &(req->request[RESPONSE])))
      found = 1;
    else
      k = k->next;    
  }
  found = 0;
  while (!found && kbis){
    if (memcmp(&(kbis->ip), &(req->request[REQUEST]), sizeof(struct sockaddr_in)) == 0)
      found = 1;
    else
      kbis = kbis->next;    
  }

  if (k && !kbis)
    memcpy(&(k->ip), &(req->request[REQUEST]), sizeof(struct sockaddr_in));
  else if (!k && kbis)
    strcpy(&(kbis->key[0]), &(req->request[RESPONSE]));
  else if (!k && !kbis){
    memcpy(&(ip), &(req->request[REQUEST]), sizeof(struct sockaddr_in));
    k = newKey(ip, &(req->request[RESPONSE]), NULL);
    addKeyToInfo(k, infos);
  }
}