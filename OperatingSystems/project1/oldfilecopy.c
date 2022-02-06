// Program to copy a single file from one location to another as specified by the user
//


#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


#define BUFFER_SIZE 4096
int size = 0;			// define global var for bytes
char *source, *target, *prog;


int copy(int const sfd, int const tfd){
	char buffer[BUFFER_SIZE];		// define buffer string for copying
	int nread;			// define size of read string var

	while((nread = read(sfd, buffer, BUFFER_SIZE)) > 0){		// while not at end of file
		
		if(nread < 0){		// check read failure
			fprintf(strerr, "%s: Unable to read from %s: %s\n", prog, source, strerror(errno));
			close(sfd);
			close(tfd);
			return 1;
		}

		if((write(tfd, buffer, nread)) != nread){		// check write failure
			fprintf(strerr, "%s: Unable to read from %s: %s\n", prog, target, strerror(errno));
			close(sfd);
			close(tfd);
			return 1;

		}

		size += nread*sizeof(*buffer);

	}

	return 0; 			// success

}



int main(int argc, char *argv[]){
	//char *source, *target;		//init source and target file
	//char buffer[BUFFER_SIZE];	//define buffer string for copying
	//int nread;								//define size of read string var
	//int size = 0;							//init file size


	if(argc != 3){			// exit of improper user input
		printf("Usage: %s SOURCE TARGET\n", argv[0]);
		return EXIT_FAILURE;
	}
	prog		= argv[0];		// assign program name var
	source  = argv[1];		// assign source var
	target  = argv[2];		// assign target var

	struct stat info;
	if(stat(target, &info) == 0){		// target file already exists
		fprintf(stderr, "%s:Target file %s already exists\n", prog, target);







	



	int tfd = open


	}

























}
