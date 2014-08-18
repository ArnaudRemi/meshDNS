//meshDNS by rémi arnaud

#ifndef __REQUESTH__
#define __REQUESTH__


resp *requestName(mdns *req, linfo *infos);
resp *requestKey(mdns *req, linfo *infos);
resp *requestIpToKey(mdns *req, linfo *infos);
resp *requestIpToName(mdns *req, linfo *infos);
void freeResp(resp *r);


#endif