#include "sqserver.h"
#include "encryption.h"

using namespace std;

const char * param_arr[] = {
  "-g",
  "-d",
  "-e",
  "-bd",
  "-be",
  "-s",
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
    default:
      cout << "usage: " << argv[0] << " <options> [<optional arguments>]" << endl;
      break;
  }

  return 0;
}

