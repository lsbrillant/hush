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
	FILE *readfrom = stdin;
    
	import_env(); 
    
    do_opts( argc, argv );
     
    if( forced_interactive ||
        ( 
            isatty(fileno(stdin)) 
            &&
            isatty(fileno(stdout))
        )
    ) interactive = 1;

    /* running a file in the form hush FILE */
    if( optind < argc ) {
        interactive = 0;
        if( (readfrom = fopen(argv[optind],"r")) == NULL ) {
            perror("fopen");
            exit(EXIT_FAILURE);
        }
    }
    
	while (1) {
        if ( interactive ) 
            printf( "hushv%d> ",HUSH_VERSION );
		if ( getline( &line, &len, readfrom ) == -1 ) {
			if ( interactive )
                printf("\n");
            break;
        }
        if ( strcmp(line, QUIT_STRING) == 0 ) {
            break;
        } else if ( strcmp(line, "\n") == 0 ) {
            continue;
        }
        if (wipe_comments(line) != 1) {
            if ( (pid = fork()) == 0 ){
                run_command( line );
            }
            waitpid(pid, &status, WUNTRACED | WCONTINUED);
        }
	}
    if ( interactive )
        printf("bye\n");
	exit(EXIT_SUCCESS);
}
