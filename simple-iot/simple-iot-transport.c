#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>

#include "simple-iot.h"

static int sock;
static struct sockaddr_in toaddr;
static int toaddrlen;
static struct sockaddr_in fromaddr;
static int fromaddrlen;

int simple_transport_init(char *ip, short port)
{
	struct ip_mreq mreq;
	int flag_on = 1;

	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock < 0) {
		perror("failed to create socket");
		return(errno);
	}

	if ((setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &flag_on, sizeof(flag_on))) < 0) {
		perror("failed to reuse addr");
		close(sock);
		return(errno);
	}

	toaddrlen = sizeof(toaddr);
	bzero((char *)&toaddr, toaddrlen);
	toaddr.sin_family = AF_INET;
	toaddr.sin_addr.s_addr = inet_addr(ip);
	toaddr.sin_port = htons(port);

	fromaddrlen = sizeof(fromaddr);
	fromaddr.sin_family = AF_INET;
	fromaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	fromaddr.sin_port = htons(port);

	if (bind(sock, (struct sockaddr *)&fromaddr, fromaddrlen) < 0) {
		perror("failed to bind on socket");
		close(sock);
		return(errno);
	}

	mreq.imr_multiaddr.s_addr = inet_addr(ip);
	mreq.imr_interface.s_addr = htonl(INADDR_ANY);
	if (setsockopt(sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq)) < 0) {
		perror("failed to become multicast member");
		close(sock);
		return(errno);
	}
}

int simple_transport_send(char *buf, int len)
{
	int status;
	int cnt;
	char *tmpbuf;
	size_t tmplen;

	tmpbuf = malloc(1024);

	status = simple_encrypt_blob(&pkpub, buf, len, &tmpbuf, &tmplen);

	cnt = sendto(sock, tmpbuf, tmplen, 0, (struct sockaddr *)&toaddr, toaddrlen);
	if (cnt < 0) {
		perror("failed to send mc msg");
		return (0);
	}

	return (len);
}

int simple_transport_recv(char** buf)
{
	int status;
	int cnt;
	char *tmpbuf;
	size_t tmplen;
	size_t len;

	tmpbuf = malloc(1024);

	status = recvfrom(sock, tmpbuf, 1024, 0, (struct sockaddr *)&fromaddr, &fromaddrlen);
	if (status < 0) {
		perror("failed to get mc msg");
		return (0);
	} else if (status == 0) {
		return (0);
	}

	status = simple_decrypt_blob(&pkpriv, tmpbuf, status, buf, &len);

	return (len);
}
