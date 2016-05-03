#ifndef ENV_H
#define ENV_H

extern char **environ;

int import_env( ); 

int search_path( char * cmd, char ** prefix );

#endif
