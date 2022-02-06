// Program: filecopy.c
// Author: 	Leah Healy
// Date: 		Feb 6th, 2022
// OS - Project 1 


#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

void usage(const char *program) {
    fprintf(stderr, "Usage: %s source target\n", program);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
    	usage(argv[0]);
    	return EXIT_FAILURE;
    }

    // Command line arguments
    const char *source_path = argv[1];							// First is the source
    const char *target_path = argv[2];							// Second is the target

    // Open file for reading
    int source_fd = open(source_path, O_RDONLY);    // Open the source
    if (source_fd < 0) {			 
    	fprintf(stderr, "filecopy: open: %s\n", strerror(errno));		// Error Message
    	return EXIT_FAILURE;
    }

    FILE *source_file = fdopen(source_fd, "r");	    // Open source file
    if (!source_file) {															// Not able to open the source file
    	fprintf(stderr, "filecopy: fopen: %s\n", strerror(errno));	// Error Message
    	return EXIT_FAILURE;
    }

    // Open file for writing
    FILE *target_file = fopen(target_path, "w");  	// Open target file
    if (!target_file) {															// Not able to write to target file
    	fprintf(stderr, "filecopy: fopen: %s\n", strerror(errno));	// Error Message
    	return EXIT_FAILURE;
    }

		int bytes = 0;
    // Copy from source file to target file
    char   buffer[BUFSIZ];
    size_t nread;
    while ((nread = fread(buffer, 1, BUFSIZ, source_file)) > 0) {
    	fwrite(buffer, 1, nread, target_file);	    // fwrite to target file
			bytes += nread;															// Add to bytes
    }

    // Close files
    fclose(source_file);			    								// Close the source file
    fclose(target_file);													// Close the target file 
		printf("filecopy: copied %d bytes from %s to %s\n", bytes, source_path, target_path);

    return EXIT_SUCCESS;
}
