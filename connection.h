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
    
    int conditions(void); //0 = false, 1 = true
    void closeConnections(void);
    int msgforward(int recvFromSocket, int sendToSocket, int fromClient);
    int isAllowedMsg(const char * msg);
    size_t stringlen(const char * str);
    
  public:
    Connection();
    ~Connection();
    
    void setEncryption(Encryption c);
    //void setTsConnection(int tsSocket);
    int connectToTs(unsigned int port);
    void sendTextFileCommands(int socket, const char * loginfile);
    void setClientConnection(int client_socket);
    
    void sigterm(void);
    
    int startServer(void); //returns 0 for no error...
};

#endif
