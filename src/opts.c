#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include "opts.h"
#include "command.h"

int interactive;

int do_opts( int argc , char **argv ) {
    int pusage = 0, rcmd = 0;
    int c;
    
    char *c_cmd;

    while(1)
    {
        c = getopt(argc, argv, "ic:?h"); 
        if( c == EOF )
            break;

        switch(c)
        {
            case 'i':
                interactive = 1;
                break;
            case 'c':
                interactive = 0;
                rcmd = 1;
                c_cmd = optarg;
                /* also run the comand */
                break;
            case 'h':
            case '?':
            defualt:
                pusage = 1;
        }
    }
    if ( pusage ) {
        print_usage(argv[0]);
        exit(EXIT_FAILURE);
    }
    if ( rcmd && c_cmd != NULL ) {
        run_command(c_cmd);
    }
    return 0;
}

void print_usage( char *s ) {
    fprintf(stderr, "Usage: %s [options]\n" ,s );
    fprintf(stderr, "\toptions:\n");
    fprintf(stderr, "\t\t-c [command string]\n");
    fprintf(stderr, "\t\t-i (interactive mode)\n");
    fprintf(stderr, "\n");
}
