#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

#include "env.h"
#include "argv.h"


int import_env( char ***env ) { 
	if(  ( *env = environ) == NULL ) {
        return -1;
    }
    putenv("SHELL=/home/lucien/Documents/SOS/shellme/hush");
    //*env[1] = "SHELL=hush";
    //(void)env;
    return 0;
}

int search_path( char * cmd, char ** prefix ) {
    char * p;
    char path[1024];
    char delim[] = ":";
    char ** pathargs;
    int numargs, i, error;
    struct stat sb;
     
    p = (char*)getenv("PATH");
    numargs = makeargv( p, delim, &pathargs);
    
    for( i=0; i < numargs; i++ ) {
        strcpy(path,pathargs[i]);
        strcat(path,"/");
        strcat(path,cmd);
        
        if( -1 != stat(path, &sb) ) {
	        if ((*prefix = malloc(strlen(path))) == NULL) {
		        error = errno;
		        free(pathargs);
		        errno = error;
		        return -1;
	        }
            strcpy(*prefix,path);
            return 0;
        }
    } 
    return -1;
}

