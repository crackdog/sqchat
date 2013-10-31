#include "sqserver.h"
#include "connection.h"
#include "encryption.h"

Connection::Connection()
{
  this->clientSocket = -1;
  this->ts3Socket = -1;
  
  printf("created...\n");
}

Connection::~Connection()
{
  cout << "destruct..." << endl;
  closeConnections();
}

void Connection::closeConnections()
{
  if(this->clientSocket != -1)
  {
    close(clientSocket);
  }
  if(this->ts3Socket!= -1)
  {
    close(ts3Socket);
  }
}

int Connection::conditions()
{
  int ret;
  
  ret = this->clientSocket != -1;
  ret = ret && this->ts3Socket != -1;
  
  return ret;
}

void Connection::setEncryption(Encryption c)
{
  cout << "setting encryption..." << endl;
  this->crypt = c;
  
  //cout << this->crypt.getKey() << endl;
}

int Connection::connectToTs(unsigned int port)
{
  int s;
  struct hostent *host;
  struct sockaddr_in addr;

  if (!inet_aton("localhost", &addr.sin_addr))
  {
    host = gethostbyname("localhost");
    if (!host)
    {
      return FALSE;
    }
    addr.sin_addr = *(struct in_addr*)host->h_addr;
  }

  s = socket(PF_INET, SOCK_STREAM, 0);
  if (s == -1)
  {
    return FALSE;
  }

  addr.sin_port = htons(port);
  addr.sin_family = AF_INET;

  if (connect(s, (struct sockaddr*)&addr, sizeof(addr)) == -1)
  {
    return FALSE;
  }
  
  this->ts3Socket = s;

  return TRUE;
}

void Connection::setClientConnection(int client_socket)
{
  this->clientSocket = client_socket;
}

void Connection::sendTextFileCommands(int socket, const char * loginfile)
{
  FILE * loginf;
  char * buffer = new char[BUF_SIZE];
  
  loginf = fopen(loginfile, "r");
  
  if(socket != -1 && loginf != NULL)
  {  
    while(!feof(loginf))
    {
      if(fgets(buffer, BUF_SIZE, loginf) != NULL)
      {
        send(socket, buffer, strlen(buffer), 0);
      }
    }
  }
}

int Connection::startServer()
{
  const char * msg;
  int bytes, max;
  char * msgbuffer = new char[BUF_SIZE];
  fd_set fds;
  struct timeval timer;
  
  if(conditions())
  {
    //sending test messagt to client
    msg = crypt.encrypt_msg("connected");
    send(clientSocket, msg, strlen(msg), 0);
    
    //ts3 server login...
    sendTextFileCommands(ts3Socket, CLOGINFILENAME);
    
    while(conditions())
    {
      //building select structures
      FD_ZERO(&fds);
      FD_SET(clientSocket, &fds);
      FD_SET(ts3Socket, &fds);
      max = clientSocket > ts3Socket ? clientSocket : ts3Socket;
      
      //creating timer structure
      timer.tv_sec = 2;
      timer.tv_usec = 0;
      
      //clearing buffer
      memset(msgbuffer, '\0', BUF_SIZE);
      
      //select...
      if(select(max + 1, &fds, NULL, NULL, &timer) == -1)
      {
        //error...
        cout << "error at select" << endl;
        closeConnections();
      }
      
      if(FD_ISSET(clientSocket, &fds)) //if new msg at clientSocket
      {
        
      }
      else if(FD_ISSET(ts3Socket, &fds)) //if new msg at ts3Socket
      {
        
      }
      else //else timer...
      {
        const char * clientlist = "clientlist\n";
        
        bytes = send(ts3Socket, clientlist, strlen(clientlist), 0);
        if(bytes == -1)
        {
          //error
          cout << "error at timer send..." << endl;
          closeConnections();
        }
      }
      break;
    }
  }
  
  //destructing...
  closeConnections();
  delete msgbuffer;
  
  return 0;
}

