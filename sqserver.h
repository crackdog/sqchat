#ifndef _SQSERVER_H_
#define _SQSERVER_H_

using namespace std;

//------------------------
//includes:

//------------------------

//************************
//defines:
#define TRUE            1
#define FALSE           0
#define BUF_SIZE        4096
#define TS3_SERVER_PORT 10011
#define SERVER_PORT     6666
#define CLOGINFILENAME  "clientlogin.conf"
//************************

//++++++++++++++++++++++++
//typedefs:
//typedef int boolean;
typedef enum logfiletype 
  { SERVER_LOG, CLIENT_LOG }
  logtype;
//++++++++++++++++++++++++

#endif
