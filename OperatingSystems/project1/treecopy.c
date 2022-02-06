// Program: treecopy.c
// Author: 	Leah Healy
// Date: 		Feb 6th, 2022
// OS - Project 1 


#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>

// Variable and Function declarations
#define BUFFER_SIZE 4096
#define PATH_SIZE   1024

// Init globals variables for num of bytes, directory, and files
int size, directory_count, file_count = 0;
char *source, *target, *prog;

// Function prototypes
int file_copy(char *src, char *tgt);
int dirtectory_copy(char *src, char *tgt);
int find_copy_type(char *src, char *tgt);


// Function: file_copy		copys an exisitng file to a new file (a targ file that does not already exist)
int file_copy(char *src, char *tgt){

	char buffer[BUFFER_SIZE];					// Buffer string for copying
	int nread;												// Size of read string var

	int sfd = open(src, O_RDONLY);		// Open the source file with read permissions
	if(sfd < 0){											// If file does not open --> exit
		fprintf(stderr, "%s: Unable to open %s: %s\n", prog, source, strerror(errno));
		return EXIT_FAILURE;
	}

	int tfd = open(tgt, O_CREAT|O_WRONLY, 0666);		// open target file with write permissions
	if(tfd < 0){											// If file does not open --> exit
		fprintf(stderr, "%s: Unable to open %s: %s\n", prog, target, strerror(errno));
		close(sfd);
		return EXIT_FAILURE;
	}

	while((nread = read(sfd, buffer, BUFFER_SIZE)) > 0){		// while not at end of file

		if(nread < 0){									// Check read failure
			fprintf(stderr, "%s: Unable to read from %s: %s\n", prog, source, strerror(errno));
			close(sfd);										// Close source
			close(tfd);										// Close target 
			return 1;
		}

		if((write(tfd, buffer, nread)) != nread){							// check write failure
			fprintf(stderr, "%s: Unable to write to %s: %s\n", prog, target, strerror(errno));
			close(sfd);										// Close source
			close(tfd);										// Close target
			return 1;
		}

		size += nread*sizeof(*buffer);
	}

	printf("%s  -> %s\n", src, tgt);	// Printing the source to target
	return 0;													// Success

}


// Function: directory_copy		copys an exisitng directory to a new directory (a targ directory that does not already exist)
int directory_copy(char *src, char *tgt){

	DIR *src_dir = opendir(src);			// Create a directory
	if(!src_dir){											// Not able to open the directory --> error message
		fprintf(stderr, "%s: Unable to open directory %s: %s\n", prog, src, strerror(errno));
		return 1;
	}

	int result = mkdir(tgt, 0777);		// Make new directory in target
	if(result < 0){										// Error check mkdir --> error message
		fprintf(stderr, "%s: Unable to create directory %s: %s\n", prog, tgt, strerror(errno));
		return 1;
	}

	printf("%s  -> %s\n", src, tgt);	// Print the source to target

	struct dirent *d;
	while((d = readdir(src_dir))){		// Read in directory
		if(strcmp(d->d_name, ".") == 0 || strcmp(d->d_name, "..") == 0){
			continue;
		}

		char new_source[PATH_SIZE];
		char new_target[PATH_SIZE];

		snprintf(new_source, PATH_SIZE, "%s/%s", src, d->d_name);		// Concatenate source path
		snprintf(new_target, PATH_SIZE, "%s/%s", tgt, d->d_name);		// Concatenate path

		find_copy_type(new_source, new_target);
	}

	closedir(src_dir);
	return 0;
}


// Function: find_copy_type		Calls other funcs in it. directory_copy copies a directory and file_copy copies a file

int find_copy_type(char *src, char *tgt){

	struct stat info;
	if(stat(src, &info)){							// Target file that already exists
		fprintf(stderr, "%s: Unable to copy %s\n", prog, src);			// Error message
		return EXIT_FAILURE;
	}
	
	if(S_ISDIR(info.st_mode)){
		directory_count++;							// Increment count of directories
		return directory_copy(src, tgt);// Call dir copy func	and return 
	}else if(S_ISREG(info.st_mode)){
		file_count++;										// Increment count of files
		return file_copy(src, tgt);			// Call file copy func and return 
	}else{
		fprintf(stderr, "%s: Unable to copy file %s\n", prog, source);
		return 1;
	}
}


// Main Function:
int main(int argc, char *argv[]){

	if(argc != 3){										// Improper user inout --> exit
		printf("Usage: %s SOURCE TARGET\n", argv[0]);
		return EXIT_FAILURE;
	}

	struct stat info;
	if(stat(target, &info) == 0){			// Target file that already exists
		fprintf(stderr, "%s:Target %s already exists\n", prog, target);
		return EXIT_FAILURE;
	}

	prog	 = argv[0];		// assign program name var
	source = argv[1];		// assign source var
	target = argv[2];		// assign target var

	if(find_copy_type(source, target))
		return EXIT_FAILURE;

	printf("%s copied %i directories, %i files, and %i bytes from %s to %s\n", prog, directory_count, file_count, size, source, target);
	return EXIT_SUCCESS;
}
