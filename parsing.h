//meshDNS by rémi Arnaud

#ifndef __PARSINGH__
#define __PARSINGH__

#include "meshDNS.h"

mdns *createReq(char* str, enum etype t);
void parseResponse(mdns *req, linfo *infos);
mdns *parseBuf(char *buf);
resp *parseReq(mdns *req, linfo *infos);

#endif
