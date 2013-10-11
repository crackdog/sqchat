#ifndef _ENCRYPTION_H_
#define _ENCRYPTION_H_

//typedefs
typedef struct {
  uint8_t * data;
  size_t len;
} binarydata;

class Encryption
{
  private:
    binarydata base64decodebin(const char * databuf);
    const char * base64encodebin(const void * databuf, size_t datalen);
  public:
    Encryption(); //constructor
    ~Encryption(); //destructor
    
    const char * base64encode(const char * databuf);
    const char * base64decode(const char * databuf);
};

#endif
