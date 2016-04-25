#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#include "argv.h"
#include "command.h"
#include "env.h"

#define HUSH_VERSION 1

#define QUIT_STRING "exit\n"

int main ( int argc, char ** argv ) 
{	
	char delim[] = " \t\n";
	int i;
	char ** myargv; 
	char ** pathargs;
	int numtokens;
	char **newenviron;
	char * line; 
	size_t len = 0;
	pid_t pid;
	
	import_env( &newenviron ); 
	
	if ( argc == 2 ) {
		if ((numtokens = makeargv(argv[1], delim, &myargv)) == -1) {
			fprintf(stderr, "Failed to construct an argument array for %s\n", argv[1]);
			return 1;
		}
		
		printf("The argument array contains: \n");
		for(i=0; i<numtokens; i++) { 
			printf("%d: %s\n", i, myargv[i]);
		}
		if ((pid = fork()) == 0) {
			printf("I am the child: %u\n", getpid());
			execve( myargv[0], myargv, newenviron );
			perror("execve");	
			exit(EXIT_FAILURE);
		} else {
			printf("I am the parent: %u and my child is: %u\n", getpid(), pid);
			exit(EXIT_SUCCESS);
		}
	} else if ( argc >= 3 ) {
		fprintf( stderr, "Usage: %s string\n\t or run with no args to have an iteractive shell\n", argv[1] );
		return 1;
	}

	while (1) {
        printf( "hushv%d> ",HUSH_VERSION );
		if ( getline( &line, &len, stdin ) == -1 ) {
			printf("\n");
            break;
        }
        if ( strcmp(line, QUIT_STRING) == 0 ) {
            break;
        } else if ( strcmp(line, "\n") == 0 ) {
            continue;
        }
		if ((numtokens = makeargv(line, delim, &myargv)) == -1) {
			fprintf(stderr, "Failed to construct an argument array for %s\n", argv[1]);
			continue;
		}
        if ( run_command( myargv, newenviron ) < 0 ) {
            perror( myargv[0] );
        }
        wait(0);
	}
    printf("bye\n");
	exit(EXIT_SUCCESS);
}
