//meshDNS by rémi arnaud

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>

#include "meshDNS.h"
#include "config.h"
#include "parsing.h"
#include "request.h"

resp *requestName(mdns *req, linfo *infos){
  // envoi d'un name, reponse une/key des

  printf("receive name request\n");

  resp *r = NULL;
  resp *rbis;
  char found = 0;
  name *n;
  lkey *lk;

  n = infos->names;
  if (n == NULL)
    printf("eez\n");
  while (!found && n){
    printf("test name: %s\n", &(n->name[0]));
    if (strcmp(&(n->name[0]), &(req->request[0])) == 0)
      found = 1;
    else
      n = n->next;
  }
  if (!found){
    printf("not found \n");
    return NULL;
  }
  printf("name %s found\n", &(n->name[0]));

  if (n->lKey == NULL)
    return NULL;

  r = malloc(sizeof(resp));
  // error malloc
  rbis = r;
  lk = n->lKey;
  while (lk){
    rbis->req = malloc(sizeof(mdns));
    // error malloc
    memset(rbis->req, 0, sizeof(mdns));
    rbis->req->infos = req->infos | RESPOND;
    rbis->req->type = req->type;
    strcpy(&(rbis->req->request[REQUEST]), &(req->request[0]));    
    strcpy(&(rbis->req->request[RESPONSE]), &(lk->key->key[0]));
    printf("will send response: %s\n", &(lk->key->key[0]));
    //generateHash();
    lk = lk->next;
    if (lk){
      rbis->next = malloc(sizeof(resp));
      // error malloc
      rbis = rbis->next;
    }
    else
      rbis->next = NULL;
  }
  return r;
}

resp *requestKey(mdns *req, linfo *infos){
  //envoi d'une key, réponse ip
  resp *r = NULL;
  char found = 0;
  key *keys;

  keys = infos->keys;
  while (!found && keys){
    if (strcmp(&(keys->key[0]), &(req->request[0])) == 0)
      found = 1;
    else
      keys = keys->next;
  }
  if (!found)
    return NULL;

  r = malloc(sizeof(resp));
  r->next = NULL;
  r->req = malloc(sizeof(mdns));
  memset(r->req, 0, sizeof(mdns));
  r->req->infos = req->infos | RESPOND;
  r->req->type = req->type;
  strcpy(&(r->req->request[REQUEST]), &(req->request[0]));
  memcpy(&(r->req->request[RESPONSE]), &(keys->ip), sizeof(struct sockaddr_in));
  return r;
}

resp *requestIpToKey(mdns *req, linfo *infos){
  resp *r = NULL;
  char found = 0;
  key *keys;

  keys = infos->keys;
  while (!found && keys){
    if (memcmp(&(keys->ip), &(req->request[0]), sizeof(struct sockaddr_in)) == 0)
      found = 1;
    else
      keys = keys->next;
  }
  if (!found)
    return NULL;

  r = malloc(sizeof(resp));
  r->next = NULL;
  r->req = malloc(sizeof(mdns));
  memset(r->req, 0, sizeof(mdns));
  r->req->infos = req->infos | RESPOND;
  r->req->type = req->type;
  memcpy(&(r->req->request[REQUEST]), &(req->request[0]), sizeof(struct sockaddr_in));
  strcpy(&(r->req->request[RESPONSE]), &(keys->key[0]));
  return r;
}

resp *requestIpToName(mdns *req, linfo *infos){
  resp *r = NULL;
  resp *rbis;
  char found = 0;
  key *keys;
  lname *ln;

  keys = infos->keys;
  while (!found && keys){
    if (memcmp(&(keys->ip), &(req->request[0]), sizeof(struct sockaddr_in)) == 0)
      found = 1;
    else
      keys = keys->next;
  }
  if (!found)
    return NULL;

  r = malloc(sizeof(resp));
  // error malloc
  rbis = r;
  ln = keys->lName;
  while (ln){
    rbis->req = malloc(sizeof(mdns));
    // error malloc
    memset(rbis->req, 0, sizeof(mdns));
    rbis->req->infos = req->infos | RESPOND;
    rbis->req->type = req->type;
    memcpy(&(rbis->req->request[REQUEST]), &(req->request[0]), sizeof(struct sockaddr_in));
    strcpy(&(rbis->req->request[RESPONSE]), &(ln->name->name[0]));
    //generateHash();
    ln = ln->next;
    if (ln){
      rbis->next = malloc(sizeof(resp));
      // error malloc
      rbis = rbis->next;
    }
    else
      rbis->next = NULL;
  }
  return r;
}

void freeResp(resp *r){
  if (r == NULL)
    return;

  freeResp(r->next);

  free(r->req);
  free(r);
}