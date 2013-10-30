#ifndef _CONNECTION_H_
#define _CONNECTION_H_

#include "sqserver.h"
#include "encryption.h"

class Connection
{
  private:
    Encryption crypt;
    int ts3Sock;
    int clientSock;
    
    int conditions(void); //0 = false, 1 = true
    
  public:
    Connection();
    ~Connection();
    
    void setEncryption(Encryption c);
    //void setTsConnection(int tsSocket);
    int connectToTs(unsigned int port);
    void sendTextFileCommands(int socket, const char * loginfile);
    void setClientConnection(int clientSocket);
    
    int startServer(void); //returns 0 for no error...
};

#endif
