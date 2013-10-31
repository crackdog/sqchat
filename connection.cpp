#include "sqserver.h"
#include "connection.h"
#include "encryption.h"

Connection::Connection()
{
  this->clientSock = -1;
  this->ts3Sock = -1;
  
  printf("created...\n");
}

Connection::~Connection()
{
  cout << "destruct..." << endl;
  if(this->clientSock != -1)
  {
    close(clientSock);
  }
  if(this->ts3Sock != -1)
  {
    close(ts3Sock);
  }
}

int Connection::conditions()
{
  int ret;
  
  ret = this->clientSock != -1;
  ret = ret && this->ts3Sock != -1;
  
  return ret;
}

void Connection::setEncryption(Encryption c)
{
  cout << "setting encryption..." << endl;
  this->crypt = c;
  
  //cout << this->crypt.getKey() << endl;
}

int Connection::connectToTs(unsigned int port)
{
  int s;
  struct hostent *host;
  struct sockaddr_in addr;

  if (!inet_aton("localhost", &addr.sin_addr))
  {
    host = gethostbyname("localhost");
    if (!host)
    {
      return FALSE;
    }
    addr.sin_addr = *(struct in_addr*)host->h_addr;
  }

  s = socket(PF_INET, SOCK_STREAM, 0);
  if (s == -1)
  {
    return FALSE;
  }

  addr.sin_port = htons(port);
  addr.sin_family = AF_INET;

  if (connect(s, (struct sockaddr*)&addr, sizeof(addr)) == -1)
  {
    return FALSE;
  }
  
  this->ts3Sock = s;

  return TRUE;
}

void Connection::setClientConnection(int clientSocket)
{
  this->clientSock = clientSocket;
}

void Connection::sendTextFileCommands(int socket, const char * loginfile)
{
  FILE * loginf;
  char * buffer = new char[BUF_SIZE];
  
  loginf = fopen(loginfile, "r");
  
  if(socket != -1 && loginf != NULL)
  {  
    while(!feof(loginf))
    {
      if(fgets(buffer, BUF_SIZE, loginf) != NULL)
      {
        send(socket, buffer, strlen(buffer), 0);
      }
    }
  }
}

int Connection::startServer()
{
  const char * msg;
  
  msg = crypt.encrypt_msg("connected");
  send(clientSock, msg, strlen(msg), 0);
  
  sendTextFileCommands(ts3Sock, CLOGINFILENAME);
  
  sleep(5);
  
  send(ts3Sock, "quit\n", strlen("quit\n"), 0);
  
  return 0;
}

