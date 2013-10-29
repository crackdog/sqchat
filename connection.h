#ifndef _CONNECTION_H_
#define _CONNECTION_H_

#include "sqserver.h"
#include "encryption.h"

class Connection
{
  private:
    Encryption crypt;
  public:
    Connection();
    ~Connection();
    
    void start(Encryption c);
};

#endif
