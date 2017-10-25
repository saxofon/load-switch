#ifndef __SIMPLE_IOT_TRANSPORT_H__
#define __SIMPLE_IOT_TRANSPORT_H__

extern int simple_transport_init(char *ip, short port);
extern int simple_transport_send(char* buf, int len);
extern int simple_transport_recv(char** buf);

#endif
