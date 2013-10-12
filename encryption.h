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
    binarydata key;
    
    static binarydata base64decodebin(const char * databuf);
    static const char * base64encodebin(const void * databuf, size_t datalen);
    static binarydata keygen(size_t length = 32);
  public:
    Encryption(); //constructor to create a standard key
    Encryption(const char * base64key); //constructor to set a custom key
    Encryption(size_t keylength); //constructor to generate a key with custom length
    ~Encryption(); //destructor
    
    static const char * base64encode(const char * databuf);
    static const char * base64decode(const char * databuf);
    
    const char * getKey(void);
    void setKey(const char * base64key);
    static const char * generateKey(size_t length = 32);
};

#endif
