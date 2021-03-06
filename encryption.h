#ifndef _ENCRYPTION_H_
#define _ENCRYPTION_H_

//typedefs
typedef struct {
  uint8_t * data;
  size_t len;
} binarydata; //used to store binary data

class Encryption
{
  private:
    binarydata key; //contains the current key
    
    static binarydata keygen(size_t length = 32); //generates a new key
    void xor_crypt(void * buffer, size_t length); //crypts a buffer with the key
  public:
    Encryption(); //constructor to create a standard key
    Encryption(const char * base64key); //constructor to set a custom key
    Encryption(size_t keylength); //constructor to generate a key with custom length
    ~Encryption(); //destructor
    
    static const char * base64encode(const char * databuf); //encode a ascii string to a base64 string
    static const char * base64decode(const char * databuf); //decode a base64 string to a ascii string
    static binarydata base64decodebin(const char * databuf); //decode a base64 string to binarydata
    static const char * base64encodebin(const void * databuf, size_t datalen); //encode binarydata to a base64 string
    
    const char * getKey(void); //returns the current key
    void setKey(const char * base64key); //set a new key
    static const char * generateKey(size_t length = 32); //returns a new generated key
    
    const char * encrypt_msg(const char * msg); //returns a zero-terminated string
    const char * decrypt_msg(const char * encrypted_msg); //returns a zero-terminated string
    
    int loadKeyFile(const char * filename);
};

#endif
