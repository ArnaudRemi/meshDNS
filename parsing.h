//meshDNS by rémi Arnaud

#ifndef __PARSINGH__
#define __PARSINGH__

#include "meshDNS.h"

mdns *reqIp(char *str);
mdns *reqKey(char *str);
mdns *reqName(char *str);

mdns *parseBuf(char *buf);
mdns *parseReq(mdns *req);

#endif
