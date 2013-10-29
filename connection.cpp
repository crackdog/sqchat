#include "sqserver.h"
#include "connection.h"
#include "encryption.h"
#include <stdlib.h>

Connection::Connection()
{
  printf("created...\n");
}

Connection::~Connection()
{
  cout << "destruct..." << endl;
}

