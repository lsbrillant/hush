#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>

#include "argv.h"
#include "command.h"
#include "env.h"
#include "opts.h"
#include "hushsignal.h"

#define HUSH_VERSION 1

#define QUIT_STRING "exit\n"

int main ( int argc, char ** argv ) 
{	
    sigset_t blockmask;
    struct sigaction defaction;
	char * line;
	size_t len = 0;
	pid_t pid;
    int status;
	FILE *readfrom = stdin;
     
    do_opts( argc, argv );
     
    if( forced_interactive ||
        ( 
            isatty(fileno(stdin)) 
            &&
            isatty(fileno(stdout))
        )
    ) interactive = 1;

    if ( signal_setup(&defaction, &blockmask, SIG_IGN) == -1 ) {
        perror("Failed to set up shell signal handling");
        exit(EXIT_FAILURE);
    }
    if ( sigprocmask(SIG_BLOCK, &blockmask, NULL) == -1 ) {
        perror("Failed to block signals");
        exit(EXIT_FAILURE);
    }

    /* running a file in the form hush FILE */
    if( optind < argc ) {
        interactive = 0;
        if( (readfrom = fopen(argv[optind],"r")) == NULL ) {
            perror("fopen");
            exit(EXIT_FAILURE);
        }
    }
    
	while (1) {
        if ( ! interactive ) { 
            if ( getline( &line, &len, readfrom ) == -1 ) {
                printf("\n");
                break;
            }
        } else if ( ( line = readline("hush> ") ) == NULL ) {
            printf("\n");
            break;
        }
        if ( strcmp(line, QUIT_STRING) == 0 ) {
            break;
        } else if ( strcmp(line, "\n") == 0 ) {
            continue;
        }
        if (wipe_comments(line) != 1) {
            if ( (pid = fork()) == 0 ) {
                if ((sigaction(SIGINT,&defaction,NULL) == -1) ||
                    (sigaction(SIGQUIT,&defaction,NULL) == -1) ||
                    (sigprocmask(SIG_UNBLOCK,&blockmask,NULL) == -1)) {
                    perror("Failed to set signal handling for command");
                    exit(EXIT_FAILURE);
                }
                run_command( line );
            }
            waitpid(pid, &status, WUNTRACED | WCONTINUED);
        }
	}
    if ( interactive )
        printf("bye\n");
	exit(EXIT_SUCCESS);
}
