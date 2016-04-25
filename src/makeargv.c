#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include "argv.h"

int makeargv(const char *s, const char *delimiters, char ***argvp) {
	int error;
	int i;
	int numtokens;
	/*
	 * Note that const char * says the data is const, not the pointer.
	 */
	const char *snew;
	char *t;

	if ((s == NULL) || (delimiters == NULL) || argvp == NULL) {
		errno = EINVAL;
		return -1;
	}

	*argvp = NULL;
	/* skip any leading delimiters */
	snew = s + strspn(s, delimiters);
	if ((t = malloc(strlen(snew) + 1)) == NULL)
		return -1;
	strcpy(t, snew);
	numtokens = 0;
	if (strtok(t, delimiters) != NULL)
	for(numtokens = 1;  strtok(NULL, delimiters) != NULL; numtokens++)
			;
	if ((*argvp = malloc((numtokens+1)*sizeof(char *))) == NULL) {
		error = errno;
		free(t);
		errno = error;
		return -1;
	}

	if (numtokens == 0)
		free(t);
	else {
		strcpy(t, snew);
		**argvp = strtok(t, delimiters);
		for(i=1; i<numtokens; i++)
			*((*argvp)+i) = strtok(NULL, delimiters);
	}
	*((*argvp)+numtokens) = NULL;
	return numtokens;
}
