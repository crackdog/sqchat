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

