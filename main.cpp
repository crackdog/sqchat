#include "sqserver.h"
#include "encryption.h"
#include "connection.h"
#include "daemon.h"

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

int init_server_socket(int local_port)
{
  cout << "initialize server socket on port '" << local_port << "'" << endl;
  
  int s;
  struct sockaddr_in addr;

  s = socket(PF_INET, SOCK_STREAM, 0);
  if (s == -1)
  {
    cout << "socket() failed" << endl;
    return -1;
  }

  addr.sin_addr.s_addr = INADDR_ANY;
  addr.sin_port = htons(local_port);
  addr.sin_family = AF_INET;

  if(bind(s, (struct sockaddr*)&addr, sizeof(addr)) == -1)
  {
    cout << "bind() failed" << endl;
    return -1;
  }

  if(listen(s, 3) == -1)
  {
    cout << "listen() failed" << endl;
    return -1;
  }

  return s;
}

int main(int argc, char * argv[])
{
  const char * keyfile = "test.key";

  if(argc < 2 || strncmp(argv[1], "-h", 2) == 0)
  {
    cout << "usage: " << argv[0] << " <options> [<optional arguments>]" << endl;
    cout << "the optional arguments are:" << endl;
    for(int i = 0; param_arr[i] != NULL; i++)
    {
      cout << param_arr[i];
      if(param_arr[i+1] == NULL)
      {
        cout << endl;
      }
      else
      {
        cout << ", ";
      }
    }
    cout << "the default keyfile for encryption is 'test.key'" << endl;
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
    case 1: //decrypt
    {
      Encryption * e = new Encryption();
      e->loadKeyFile(keyfile);
      
      if(argc > 2)
      {
        cout << e->decrypt_msg(argv[2]) << endl;
      }
      else
      {
        char buffer[BUF_SIZE];
        while(!feof(stdin))
        {
          if(fgets(buffer, BUF_SIZE, stdin) == 0)
          {
            break;
          }
          cout << e->decrypt_msg(buffer) << endl;
        }
      }
      break;
    }
    case 2: //encrypt
    {
      Encryption * e = new Encryption();
      e->loadKeyFile(keyfile);
        
      if(argc > 2)
      {
        cout << e->encrypt_msg(argv[2]) << endl;
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
          cout << e->encrypt_msg(buffer) << endl;
        }
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
    case 5: //starting server functions...
    {
      int loop = TRUE;
      int serverSocket, clientSocket;
      socklen_t client_addr_len;
      struct sockaddr_in client_addr;
      
      //creating server socket
      serverSocket = init_server_socket(SERVER_PORT);
      if(serverSocket == -1)
      {
        cerr << "error at 'init_server_socket()'" << endl;
        return 5;
      }
      
      //setting sig handler
      set_sigchld_handler();
      
      while(loop)
      {      
        //waiting for connection
        clientSocket = accept(serverSocket, (struct sockaddr *) &client_addr, &client_addr_len);
        if(clientSocket == -1)
        {
          cerr << "error at accept()" << endl;
          continue;
        }
      
        //create new fork and handle the client
        pid_t pid;
        
        pid = fork();
        
        if(pid == 0) //child
        {
          Encryption e;
          Connection c;
          
          close(serverSocket);
          
          setSigConnection(&c);
          
          e.loadKeyFile(keyfile);
          c.setEncryption(e);
          
          c.setEncryption(FALSE);
          
          c.setClientConnection(clientSocket);
          
          if(!c.connectToTs(TS3_SERVER_PORT))
          {
            cout << "couldn't connect to ts3 server" << endl;
          }
          else
          {         
            c.startServer();
          }
          
          cout << "quit client handle" << endl;
          
          loop = FALSE;
        }
        else //parent
        {
          close(clientSocket);
          
          cout << "child pid: " << pid << endl;
          //sleep(10);
          //loop = FALSE;
        }
      }
      close(serverSocket);
      break;
    }
    default:
      cout << "usage: " << argv[0] << " <options> [<optional arguments>]" << endl;
      break;
  }

  return 0;
}

