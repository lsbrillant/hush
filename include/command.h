#ifndef COMMAND_H
#define COMMAND_H

/* does fork exec and searches path */
int run_command( char *line );
int execute_redirect( char *s , int in, int out );

int parse_and_redirect_in(char *s);
int parse_and_redirect_out(char *s);

enum builtins {
    change_dir
};

#endif
