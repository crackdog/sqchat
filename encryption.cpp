#include "sqserver.h"
#include "encryption.h"

static binarydata key = {NULL, 0};

const char base64chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

static const unsigned char decodeb64[] = {
    66,66,66,66,66,66,66,66,66,64,64,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
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
}

Encryption::~Encryption()
{
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
    nextByte = decodeb64[databuf[dataindex]];
    
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
  resultlen = (((datalen + (datalen % 3)) / 3) * 4) + 1;
  
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

