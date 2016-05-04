#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#include "argv.h"
#include "command.h"
#include "env.h"
#include "opts.h"

#define HUSH_VERSION 1

#define QUIT_STRING "exit\n"

int main ( int argc, char ** argv ) 
{	
	char delim[] = " \t\n";
	char * line;
	size_t len = 0;
	pid_t pid;
    int status;
	
	import_env(); 
    
    do_opts( argc, argv );	

	while (1) {
        if ( interactive ) 
            printf( "hushv%d> ",HUSH_VERSION );
		if ( getline( &line, &len, stdin ) == -1 ) {
			if ( interactive )
                printf("\n");
            break;
        }
        if ( strcmp(line, QUIT_STRING) == 0 ) {
            break;
        } else if ( strcmp(line, "\n") == 0 ) {
            continue;
        }
        if ( (pid = fork()) == 0 ){
            run_command( line );
        }
        waitpid(pid, &status,WCONTINUED);
	}
    if ( interactive )
        printf("bye\n");
	exit(EXIT_SUCCESS);
}
