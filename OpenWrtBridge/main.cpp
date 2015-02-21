#include <stdio.h>      // standard input / output functions
#include <stdlib.h>
#include <string.h>     // string function definitions
#include <unistd.h>     // UNIX standard function definitions
#include <fcntl.h>      // File control definitions
#include <errno.h>      // Error number definitions
#include <termios.h>    // POSIX terminal control definitions

#include "Parameters.h"






int main(int argc, char *argv[]) {
	Parameters& parameters = Parameters.getInstance(getenv("QUERY_STRING"));
	Serial port;

	char* params = getenv("QUERY_STRING");
	if (params == NULL) params = (char*)"";
	printf("Content-Type: text/html\n\n");


	port.openPort(serial);

	printf("serial=%24s & data=%254s", &serial, &data);

	/* *** WRITE *** */


	unsigned char cmd[] = {0x7E,0xFA,0xFB,0xDF,0x06,0x00,0x00,0xFF,0x00,0x00,0x00,0xAB};
	int n_written = write(USB, cmd, sizeof(cmd));

	/* Allocate memory for read buffer */
	char buf[256];
	memset(&buf, '\0', sizeof buf);

	/* *** READ *** */
	int n = read(USB, &buf, sizeof buf);

	/* Error Handling */
	if (n < 0) {
	}

	/* Print what I read... */

	close(USB);
	return EXIT_SUCCESS;
}
