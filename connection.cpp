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

