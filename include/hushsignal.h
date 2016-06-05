#ifndef HUSHSIGNAL_H
#define HUSHSIGNAL_H

#include <signal.h>

int signal_setup( struct sigaction *def, sigset_t *mask, void (*handler)(int) );

#endif
