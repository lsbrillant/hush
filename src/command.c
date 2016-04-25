#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

#include "command.h"
#include "env.h"

int run_command( char ** argv, char ** env ) {
    int retval = 0;
    char * file_prefix;
    
    do {
 
        if( search_path( argv[0], &file_prefix ) == -1 ) {
            retval = -1;
            break;
        } 
        if (fork() == 0) {
            execve( file_prefix, argv, env );
            perror("fork()");
            exit(EXIT_FAILURE);
        }
    } while(0);

    return retval; 
}
