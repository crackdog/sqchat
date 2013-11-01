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
    this->clientSocket = -1;
  }
  if(this->ts3Socket!= -1)
  {
    close(ts3Socket);
    this->ts3Socket = -1;
  }
}

void Connection::sigterm()
{
  //when a term signal comes...
  closeConnections();
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

int Connection::msgforward(int recvFromSocket, int sendToSocket, int allMsgAllowed)
{
  int bytes, ret;
  size_t len;
  char * msgbuffer = new char[BUF_SIZE];
  
  ret = TRUE;
  memset(msgbuffer, '\0', BUF_SIZE);
  
  bytes = recv(recvFromSocket, msgbuffer, BUF_SIZE, 0);
  if(bytes == -1 || bytes == 0)
  {
    //error
    cout << "error at client recv..." << endl;
    ret = FALSE;
  }
  else
  {
    if(!allMsgAllowed)
    {
      if(!isAllowedMsg(msgbuffer))
      {
        memset(msgbuffer, '\0', BUF_SIZE);
        strncpy(msgbuffer, "clientlist\n", strlen("clientlist\n"));
      }
      //calculating string length
      len = stringlen(msgbuffer);
      cout << len << " ... " << stringlen(msgbuffer) << endl;
    }
    else
    {
      len = strlen(msgbuffer);
    }
    
    //send msgbuffer to ts3 server
    bytes = send(sendToSocket, msgbuffer, len, 0);
    if(bytes == -1 || bytes == 0)
    {
      //error
      cout << "error at client recv send..." << endl;
      ret = FALSE;
    }
  }
  
  return ret;
}

int Connection::isAllowedMsg(const char * msg)
{
  const char * allowedmsg[] = 
    {"clientlist",
    //"channellist", 
    "sendtextmessage",
    "pokeclient",
    "quit",
    NULL};
  
  int i, ret;
  ret = FALSE;
  i = 0;
  
  while(allowedmsg[i] != NULL)
  {
    if(strncmp(msg, allowedmsg[i], strlen(allowedmsg[i])) == 0)
    {
      ret = TRUE;
    }
    i++;
  }
  
  return ret;
}

size_t Connection::stringlen(const char * str)
{
  size_t ret = 0;
  while(str[ret] != '\0')
  {
    ret++;
    if(str[ret-1] == '\n')
    {
      break;
    }
  }
  return ret;
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
      timer.tv_sec = 20;
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
        if(!msgforward(clientSocket, ts3Socket, FALSE))
        {
          //error
          cout << "error at msgforward from clientSocket to ts3Socket" << endl;
          closeConnections();
        }
      }
      else if(FD_ISSET(ts3Socket, &fds)) //if new msg at ts3Socket
      {
        if(!msgforward(ts3Socket, clientSocket, TRUE))
        {
          //error
          cout << "error at msgforward from ts3Socket to clientSocket" << endl;
          closeConnections();
        }
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
    }
  }
  
  //destructing...
  closeConnections();
  delete msgbuffer;
  
  exit(0);
  
  return 0;
}

