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

