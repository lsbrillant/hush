#ifndef COMMAND_H
#define COMMAND_H

/* does fork exec and searches path */
int run_command( char ** args, char ** env );

enum builtins {
    change_dir
};

#endif
