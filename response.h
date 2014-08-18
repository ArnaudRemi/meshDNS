//meshDNS by rémi arnaud

#ifndef __RESPONSEH__
#define __RESPONSEH__

void responseIpToKey(mdns *req, linfo *infos);
void responseIpToName(mdns *req, linfo *infos);
void responseKey(mdns *req, linfo *infos);
void responseName(mdns *req, linfo *infos);
void addKeyToInfo(key *k, linfo *infos);
void addNameToInfo(name *n, linfo *infos);


#endif
