#include "sqserver.h"
#include "encryption.h"
#include <time.h>

const char base64chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

static const unsigned char decodeb64[] = {
    66,66,66,66,66,66,66,66,66,64,64,66,66,64,66,66,66,66,66,66,66,66,66,66,66, //0xd = carriage return
    66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,62,66,66,66,63,52,53,
    54,55,56,57,58,59,60,61,66,66,66,65,66,66,66, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
    10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,66,66,66,66,66,66,26,27,28,
    29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,66,66,
    66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
    66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
    66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
    66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
    66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
    66,66,66,66,66,66
};

#define WHITESPACE 64
#define EQUAL      65
#define INVALID    66

Encryption::Encryption()
{
  key = keygen();
}

Encryption::Encryption(const char * base64key)
{
  key = base64decodebin(base64key);
}

Encryption::Encryption(size_t keylength)
{
  key = keygen(keylength);
}

Encryption::~Encryption()
{
  key.data = NULL; //created an error when using delete key.data...
  key.len = 0;
}

binarydata Encryption::keygen(size_t length)
{
  binarydata ret;
  ret.data = new uint8_t[length];
  ret.len = length;
  
  srand(time(NULL));
  
  for(size_t i = 0; i < length; i++)
  {
    ret.data[i] = rand() % 0xff;
  }
  
  return ret;
}

binarydata Encryption::base64decodebin(const char * databuf)
{
  binarydata result;
  uint8_t * buffer;
  size_t datalen, padcount;
  size_t dataindex, bufferindex;
  uint32_t x;
  uint8_t nextByte;
  int i;
  
  datalen = strlen(databuf);
  
  buffer = (uint8_t *) malloc(datalen);
  
  padcount = 0;
  
  for(dataindex = 0, i = 0, bufferindex = 0, x = 0; dataindex < datalen; dataindex++)
  {
    nextByte = decodeb64[(int) databuf[dataindex]]; //int cast only to avoid a warning
    
    if(nextByte == INVALID)
    {
      //printf("invalid: '%c' (%i)\n", databuf[dataindex], databuf[dataindex]);
      //continue;
      exit(1337);
    }
    
    if(nextByte != WHITESPACE)
    {
      if(nextByte == EQUAL || databuf[dataindex] == '=')
      {
        nextByte = 0x0;
        padcount++;
      }
      
      x <<= 6;
      x |= nextByte;
      
      i++;
      
      if(i >= 4)
      {
        buffer[bufferindex++] = (x >> 16) & 0xff;
        buffer[bufferindex++] = (x >> 8) & 0xff;
        buffer[bufferindex++] = x & 0xff;
        
        i = 0;
        x = 0;
      }
    }
  }
  
  bufferindex -= padcount;
  
  buffer[bufferindex] = '\0';
  
  result.len = bufferindex;
  result.data = new uint8_t[result.len];
  memcpy(result.data, buffer, result.len);
  
  free(buffer);
  
  return result;
}

const char * Encryption::base64encodebin(const void * databuf, size_t datalen)
{
  const uint8_t * data;
  char * result;
  size_t resultlen;
  size_t i, j;
  uint8_t x0, x1, x2, x3;
  uint32_t x;
  
  //datalen--;
  
  data = (const uint8_t *) databuf;
  resultlen = (((datalen + (datalen % 3)) / 3) * 4) + 2;
  
  result = (char *) malloc(resultlen * sizeof(char));
  
  for(i=0, j=0; i < datalen; i+=3)
  {
    //saving the 3 bytes into an 32 bit number
    x = data[i] << 16;
    
    if(i+1 < datalen)
    {
      x += data[i+1] << 8;
    }
    if(i+2 < datalen)
    {
      x += data[i+2];
    }
    
    //seperate into 4 6bit numbers
    x0 = (uint8_t) (x >> 18) & 63;
    x1 = (uint8_t) (x >> 12) & 63;
    x2 = (uint8_t) (x >> 6) & 63;
    x3 = (uint8_t) x & 63;
    
    result[j++] = base64chars[x0];
    result[j++] = base64chars[x1];
    
    if(i+1 < datalen)
    {
      result[j++] = base64chars[x2];
    }
    else
    {
      result[j++] = '=';
    }
    
    if(i+2 < datalen)
    {
      result[j++] = base64chars[x3];
    }
    else
    {
      result[j++] = '=';
    }
  }
  
  result[j++] = '\n';
  result[j] = '\0';
  return result;
}

const char * Encryption::base64decode(const char * databuf)
{
  return (const char *) base64decodebin(databuf).data;
}

const char * Encryption::base64encode(const char * databuf)
{
  return base64encodebin(databuf, strlen(databuf) + 1);
}

const char * Encryption::getKey()
{
  return this->base64encodebin(key.data, key.len);
}

void Encryption::setKey(const char * base64key)
{
  this->key = base64decodebin(base64key);
}

const char * Encryption::generateKey(size_t length)
{
  binarydata newkey;
  
  newkey = keygen(length);
  
  return base64encodebin(newkey.data, newkey.len);
}

void Encryption::xor_crypt(void * buffer, size_t length)
{
  size_t i, j;
  uint8_t * buf = (uint8_t *) buffer;
  for(i = 0, j = 0; i < length; i++)
  {
    if(j >= key.len)
    {
      j = 0;
    }
    
    buf[i] ^= key.data[j++];
  }
}

const char * Encryption::encrypt_msg(const char * msg)
{
  size_t msglen;
  uint8_t * buffer;
  const char * res;
  
  msglen = strlen(msg) + 1;
  
  buffer = new uint8_t[msglen + 1]; //maybe without +1
  memcpy(buffer, msg, msglen);
  
  xor_crypt(buffer, msglen);
  res = base64encodebin(buffer, msglen);
  delete buffer;
  
  return res;
}

const char * Encryption::decrypt_msg(const char * encrypted_msg)
{
  binarydata msgbuf;
  
  msgbuf = this->base64decodebin(encrypted_msg);
  this->xor_crypt(msgbuf.data, msgbuf.len);
  
  return (const char *) msgbuf.data;
}

int Encryption::loadKeyFile(const char * filename)
{
  FILE * keyfile;
  char * buffer = new char[BUF_SIZE];
    
  memset(buffer, 0, BUF_SIZE);
  //user buffer for the complete path filename...
  //only do this if ther is no '/' in "filename".
  if(strstr(filename, "/") == NULL)
  {
      if(getcwd(buffer, BUF_SIZE) != NULL)
      {
          strncat(buffer, "/", 1);
          strncat(buffer, filename, BUF_SIZE);
      }
      else
      {
          strncpy(buffer, filename, BUF_SIZE);
      }
  }

  keyfile = fopen(buffer, "r");
  
  if(keyfile != NULL)
  {
    if(fgets(buffer, BUF_SIZE, keyfile) != NULL)
    {
      this->key = base64decodebin(buffer);
    }
    fclose(keyfile);
    
    return TRUE;
  }
  else
  {
    key.data = NULL;
    key.len = 0;
    return FALSE;
  }
}



