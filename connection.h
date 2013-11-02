#ifndef _CONNECTION_H_
#define _CONNECTION_H_

#include "sqserver.h"
#include "encryption.h"

class Connection
{
  private:
    Encryption crypt;
    int ts3Socket;
    int clientSocket;
    int encrypt;
    
    int conditions(void); //0 = false, 1 = true
    void closeConnections(void);
    int msgforward(int recvFromSocket, int sendToSocket, int fromClient);
    int isAllowedMsg(const char * msg);
    size_t stringlen(const char * str);
    
  public:
    Connection();
    ~Connection();
    
    void setEncryption(Encryption c);
    int connectToTs(unsigned int port);
    void sendTextFileCommands(int socket, const char * loginfile);
    void setClientConnection(int client_socket);
    void setEncryption(int on);
    
    void sigterm(void);
    
    int startServer(void); //returns 0 for no error...
};

#endif
