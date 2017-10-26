#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <pthread.h>

#include "simple-iot.h"
#include "load-switch.h"

#include "target_priv.h"
#include "target_pub.h"

static pthread_t tid_rx;
static pthread_t tid_tx;

static int parse_status(char *message)
{
	char *str;
	char *strp;
	int ch;
	int level;

	for (str=strtok_r(message," ", &strp); str; str=strtok_r(NULL, " ", &strp)) {
		if (strstr(str, "channel")) {
			sscanf(str, "channel=%d", &ch);
			continue;
		}
		if (strstr(str, "level")) {
			sscanf(str, "level=%d", &level);
			continue;
		}
	}

	if (level) {
		printf("led %d on\n", ch);
	} else {
		printf("led %d off\n", ch);
	}
}

static void *thread_rx(void *arg)
{
	int status;
	char *message;

	while (1) {
		status = simple_transport_recv(&message);

		if (strstr(message, "/load/switch/status")) {
			parse_status(message);
		}
	}
}

/* This thread simulates a user pushing switch 1 button every 2 seconds.. */
static void *thread_tx(void *arg)
{
	int status;
	char message[50];
	int ch=1;
	int level=1;

	while (1) {
		printf("requesting channel %d %s\n", ch, level?"on":"off");
		status=sprintf(message, "/load/switch/set channel=%d level=%d", ch, level);
		simple_transport_send(message, status+1);
		sleep(2);
		level = !level;
	}
}

int main(int argc, char* argv[])
{
	int status;
	size_t privkey_size = (size_t)(&_binary_target_priv_pem_end-&_binary_target_priv_pem_start);
	size_t pubkey_size = (size_t)(&_binary_target_pub_pem_end-&_binary_target_pub_pem_start);

	simple_transport_init(MC_IP, MC_PORT);
	simple_security_init(&_binary_target_priv_pem_start, privkey_size, &_binary_target_pub_pem_start, pubkey_size);

	status = pthread_create(&tid_rx, NULL, thread_rx, NULL);
	status = pthread_create(&tid_tx, NULL, thread_tx, NULL);

	while(1) {
		sleep(10);
	}
}
