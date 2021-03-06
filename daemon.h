#ifndef _DAEMON_H_
#define _DAEMON_H_

#include "connection.h"

/****************************************
 * Description:
 * - deamonizes the server
 * - installs the sigchld_handler for
 *   forks
 * - maybe logging functions
 *
 ***************************************/

extern int daemonize(void);
extern void setSigConnection(Connection * con);
extern void sigchld_handler(int sig);
extern void sigterm_handler(int sig);
extern void set_sigchld_handler(void);

#endif

