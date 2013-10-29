#include "sqserver.h"
#include "connection.h"
#include "encryption.h"

Connection::Connection()
{
  printf("created...\n");
}

Connection::~Connection()
{
  cout << "destruct..." << endl;
}

void Connection::start(Encryption c)
{
  cout << "starting server..." << endl;
  this->crypt = c;
}

