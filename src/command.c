#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>

#define FFLAG (O_WRONLY | O_CREAT | O_TRUNC)
#define FMODE (S_IRUSR | S_IWUSR)

#include "command.h"
#include "env.h"
#include "argv.h"

static void perror_exit( char*s ) {
    perror(s);
    exit(EXIT_FAILURE);
}

int run_command( char *line ) {
    int child, count, i;
    int fds[2];
    char ** pipelist;

    count = makeargv(line, "|", &pipelist);
    if ( count < 0) {
        fprintf(stderr, "Fail to create pipelist\n");
        exit(EXIT_FAILURE);
    }
    for( i=0; i < count - 1; i++ ) {
        if (pipe(fds) == -1)
            perror_exit("Failed to find any commands\n");
        else if ( (child = fork()) == -1 )
            perror_exit("Failed to create process to run command");
        else if (child) {
            if (dup2(fds[1], STDOUT_FILENO) == -1)
                perror_exit("Failed to connect pipeline");
            if (close(fds[0]) || close(fds[1]))
                perror_exit("Failed to close needed files");
            execute_redirect(pipelist[i], i==0,0);
            exit(EXIT_FAILURE);
        }
        if (dup2(fds[0], STDIN_FILENO) == -1)
            perror_exit("Failed to connect the last component");
        if (close(fds[0]) || close(fds[1]))
            perror_exit("Failed to do final close");
    }
    execute_redirect(pipelist[i], i==0,1);    
    exit(EXIT_FAILURE); 
}

int execute_redirect( char *line , int in, int out ) {
	char delim[] = " \t\n";
	char ** argv; 
    char * cmd_location, pin, pout;
    
    if ( in && ((pin = strchr(line,'<')) != NULL) && 
           out && ((pout = strchr(line,'>')) != NULL ) && (pin > pout) ) {
        if (parse_and_redirect_in(line) == -1) {
            perror("Failed to redirect input");
            return;
        }
        in = 0; 
    }
    if( out && (parse_and_redirect_out(line) == -1) )
        perror("Failed to redirect output");
    else if ( in && (parse_and_redirect_in(line) == -1) )
        perror("Failed to redirect input");
    else if (makeargv(line, delim, &argv) == -1)
        fprintf(stderr, "Failed to construct an argument array for %s\n", line);
    else if( search_path( argv[0], &cmd_location ) == -1 )
        fprintf(stderr, "No such command : %s\n", argv[0]);
    else { 
        execvp( cmd_location, argv );
        perror("execvp");
    }
    exit(EXIT_FAILURE);
}

int parse_and_redirect_in( char *cmd ) {
    int error;
    int infd;
    char *infile;

    if ( (infile = strchr(cmd, '<')) == NULL )
        return 0;
    *infile = 0;
    infile = strtok(infile + 1, " \t\n");
    if ( infile == NULL )
        return 0;
    if ( (infd = open(infile, O_RDONLY)) == -1 )
        return -1;
    if ( dup2(infd, STDIN_FILENO) == -1 ) {
        error = errno;
        close(infd);
        errno = error;
        return -1;
    }
    return close(infd);
}

int parse_and_redirect_out( char *cmd ) {
    int error;
    int outfd;
    char *outfile;

    if ( (outfile = strchr(cmd, '>')) == NULL )
        return 0;
    *outfile = 0;
    outfile = strtok(outfile + 1, " \t\n");
    if ( outfile == NULL )
        return 0;
    if ( (outfd = open(outfile, FFLAG, FMODE)) == -1 )
        return -1;
    if ( dup2(outfd, STDOUT_FILENO) == -1 ) {
        error = errno;
        close(outfd);
        errno = error;
        return -1;
    }
    return close(outfd);
}
