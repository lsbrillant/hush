#ifndef OPTS_H
#define OPTS_H

#include <unistd.h> 

extern char *optarg;

extern int interactive;
extern int forced_interactive;

int do_opts( int argc , char **argv );

void print_usage( char *s );

#endif
