// meshDNS by rémi Arnaud

#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <fcntl.h>

#include "config.h"
#include "meshDNS.h"

name *newName(char *str, lkey *keys){
  name *n;
  
  if ((n = malloc(sizeof(name))) == -1) {
    printf("Error: malloc fail\n");
    return NULL;
  }

  n->next = NULL;
  strcpy(&(n->name[0]), str);
  n->lKey = keys;
    
  return n;
}

key *newKey(struct sockaddr_in ip, char *str, lname *names){
  key *k;

  k = malloc(sizeof(key));
  //error malloc
  k->next = NULL;
  strcpy(&(k->key[0]), str);
  k->ip = ip;
  k->lName = names;

  return k;
}

lkey *newLkey(key *k){
  lkey *lk;

  lk = malloc(sizeof(lkey));
  lk->key = k;
  lk->next = NULL;
  
  return lk;
}

lname *newLname(name *n){
  lname *ln;

  ln = malloc(sizeof(lname));
  ln->name = n;
  ln->next = NULL;

  return ln;
}

void addKeyToName(key *k, name *n){
  lkey *lk;

  if (n->lKey == NULL)
    n->lKey = newLkey(k);
  else {
    lk = n->lKey;
    while (lk->next)
      lk = lk->next;
    lk->next = newLkey(k);
  }
}

void addNameToKey(name *n, key *k){
  lname *ln;

  if (k->lName == NULL)
    k->lName = newLname(n);
  else {
    ln = k->lName;
    while (ln->next)
      ln = ln->next;
    ln->next = newLname(n);
  }  
}

char parseFile(linfo *infos){
  FILE *fd;
  char *line = NULL;
  size_t  useless = 0;
  name *new;
  
  if ((fd = fopen("./names.conf", "r")) == NULL){
    printf("Error: open names.conf file fail\n");
    return -1;
  }

  while(getline(&line, &useless, fd) != -1){
    new = newName(line, NULL);
    addKeyToName(infos->me, new);
    addNameToKey(new, infos->me);
    free(line);
    line = NULL;
  }
  
  return 0;
}
