/*
	Xenomai demo - client application (no real time)
	Author    : M.Langer
	Copyright : taskit GmbH
	www.taskit.de

	All rights reserved.

	Redistribution and use in source and binary forms, with or without
	modification, are permitted provided that the following conditions are met:

		1. Redistributions of source code must retain the above copyright
		   notice, this list of conditions and the following disclaimer.
		2. Redistributions in binary form must reproduce the above copyright
		   notice, this list of conditions and the following disclaimer in the
		   documentation and/or other materials provided with the distribution.

	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT OWNER AND CONTRIBUTORS AS IS
	AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
	IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
	ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
	LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
	CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
	SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
	INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
	CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
	ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
	POSSIBILITY OF SUCH DAMAGE.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define ALLOC(p, n)  p = malloc(n); while(p == NULL) {sleep(1); p = malloc(n);};

/* main loop */
int main(int argc, char **argv)
{
	int rt_pipe_fd;
	char* str_buf;
	ssize_t n;

	/* ignore broken pipes */
	signal(SIGPIPE, SIG_IGN);

	/* open rt pipe */
	rt_pipe_fd = open("/proc/xenomai/registry/native/pipes/msg_pipe", O_RDONLY);

	/* allocate string buffer */
	ALLOC(str_buf, 256);

	/* loop while pipe is open */
	do
	{
		/* print string received through rt pipe */
		n = read(rt_pipe_fd, str_buf, sizeof(str_buf));
		if(n > 0)
			write(STDOUT_FILENO, str_buf, n);
	} while(n > 0);

	/* clean up */
	free(str_buf);
	close(rt_pipe_fd);

	return(0);
}

