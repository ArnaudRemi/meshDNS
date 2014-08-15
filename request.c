//meshDNS by rémi arnaud

#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>

#include "meshDNS.h"
#include "config.h"
#include "parsing.h"
#include "request.h"

resp *requestName(mdns *req, linfo *infos){
  // envoi d'un name, reponse une/key des
  resp *r = NULL;
  resp *rbis;
  char found = 0;
  name *n;
  lkey *lk;

  n = infos->names;
  while (!found && n){
    if (strcmp(n->name, req->request) == 0)
      found = 1;
    else
      n = n->next;
  }
  if (!found)
    return NULL;

  r = malloc(sizeof(resp));
  // error malloc
  rbis = r;
  lk = n->lKey;
  while (lk){
    rbis->req = malloc(sizeof(mdns));
    // error malloc
    rbis->req->infos = req->infos & RESPOND;
    rbis->req->type = req->type;
    strcmp(&(rbis->req->request[0]), &(lk->key->key[0]));
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

mdns *requestKey(mdns *req, linfo *infos){
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
  r->req->infos = req->infos & RESPOND;
  r->req->type = req->type;
  memcpy(&(r->req->request[0]), &(keys->ip), sizeof(struct sockaddr_in));
  return r;
}

mdns *requestIpToName(mdns *req, linfo *infos){
  resp *r = NULL;
  char found = 0;
  key *keys;

  keys = infos->keys;
  while (!found && keys){
    if (memcmp(&(keys->key[0]), &(req->request[0]), sizeof(struct sockaddr_in)) == 0)
      found = 1;
    else
      keys = keys->next;
  }
  if (!found)
    return NULL;

  r = malloc(sizeof(resp));
  r->next = NULL;
  r->req = malloc(sizeof(mdns));
  r->req->infos = req->infos & RESPOND;
  r->req->type = req->type;
  strcpy(&(r->req->request[0]), &(keys->key[0]));
  return r;
}

mdns *requestIpToKey(mdns *req, linfo *infos){
  resp *r = NULL;
  resp *rbis;
  char found = 0;
  key *keys;
  lname *ln;

  keys = infos->keys;
  while (!found && keys){
    if (memcmp(&(keys->key[0]), &(req->request[0]), sizeof(struct sockaddr_in)) == 0)
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
    rbis->req->infos = req->infos & RESPOND;
    rbis->req->type = req->type;
    strcmp(&(rbis->req->request[0]), &(ln->name->name[0]));
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