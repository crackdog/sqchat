#include "sqserver.h"
#include "encryption.h"

using namespace std;

const char * param_arr[] = {
  "-g",   //0
  "-d",   //1
  "-e",   //2
  "-bd",  //3
  "-be",  //4
  "-s",   //5
  NULL };

int getparam(const char * paramv[], const char * arg)
{
  if(paramv == NULL || arg == NULL)
  {
    return -1;
  }
  
  int i = 0;
  while(paramv[i] != NULL && strncmp(arg, paramv[i], strlen(paramv[i])) != 0)
  {
    i++;
  }
  if(paramv[i] == NULL)
  {
    return -1;
  }
  else
  {
    return i;
  }
}

int main(int argc, char * argv[])
{
  if(argc < 2 || strncmp(argv[1], "-h", 2) == 0)
  {
    cout << "usage: " << argv[0] << " <options> [<optional arguments>]" << endl;
    for(int i = 0; param_arr[i] != NULL; i++)
    {
      cout << param_arr[i] << endl;
    }
    return 0;
  }
  
  switch(getparam(param_arr, argv[1]))
  {
    case 0: //generate key
    {
      if(argc < 3)
      {
        cout << Encryption::generateKey(KEYLENGTH) << endl;
      }
      else
      {
        cout << Encryption::generateKey(atoi(argv[2])) << endl;
      }
      break;
    }
    case 3: //base64decode
    {
      if(argc > 2) //take next argument as input
      {
        cout << Encryption::base64decode(argv[2]) << endl;
      }
      else //take stdin as input
      {
        char buffer[BUF_SIZE];
        while(!feof(stdin))
        {
          if(fgets(buffer, BUF_SIZE, stdin) == 0)
          {
            break;
          }
          cout << Encryption::base64decode(buffer) << endl;
        }
      }
      break;
    }
    case 4: //base64encode
    {
      if(argc > 2) //take next argument as input
      {
        cout << Encryption::base64encode(argv[2]) << endl;
      }
      else //take stdin as input
      {
        char buffer[BUF_SIZE];
        while(!feof(stdin))
        {
          if(fgets(buffer, BUF_SIZE, stdin) == 0)
          {
            break;
          }
          cout << Encryption::base64encode(buffer) << endl;
        }
      }
      break;
    }
    default:
      cout << "usage: " << argv[0] << " <options> [<optional arguments>]" << endl;
      break;
  }

  return 0;
}

