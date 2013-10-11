#include "sqserver.h"
#include "encryption.h"

using namespace std;

int main(int argc, char * argv[])
{
  Encryption * crypt = new Encryption();
  
  cout << "Hello World!" << endl;
  cout << crypt->base64encode("Hello World!") << endl;
  cout << crypt->base64decode(crypt->base64encode("Hello World!")) << endl;

  return 0;
}

