#include "TCPClient.h"
#define BUFFER_SIZE 1024
TCPClient::TCPClient()
{
	sock = -1;
	port = 0;
	address = "";
}

bool TCPClient::setup(string address , int port)
{
  	if(sock == -1)
	{
		sock = socket(AF_INET , SOCK_STREAM , 0);
		if (sock == -1)
		{
      			cout << "Could not create socket" << endl;
    		}
        }
  	if(inet_addr(address.c_str()) == -1)
  	{
    		struct hostent *he;
    		struct in_addr **addr_list;
    		if ( (he = gethostbyname( address.c_str() ) ) == NULL)
    		{
		      herror("gethostbyname");
      		      cout<<"Failed to resolve hostname\n";
		      return false;
    		}
	   	addr_list = (struct in_addr **) he->h_addr_list;
    		for(int i = 0; addr_list[i] != NULL; i++)
    		{
      		      server.sin_addr = *addr_list[i];
		      break;
    		}
  	}
  	else
  	{
    		server.sin_addr.s_addr = inet_addr( address.c_str() );
  	}
  	server.sin_family = AF_INET;
  	server.sin_port = htons( port );
  	if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
  	{
    		perror("connect failed. Error");
    		return false;
  	}
  	return true;
}

bool TCPClient::Send(string data)
{
	if(sock != -1)
	{
		if( send(sock , data.c_str() , strlen( data.c_str() ) , 0) < 0)
		{
			cout << "Send failed : " << data << endl;
			return false;
		}
	}
	else
		return false;
	return true;
}

string TCPClient::receive(int size)
{
  	char buffer[size];
	memset(&buffer[0], 0, sizeof(buffer));

  	string reply;
	if( recv(sock , buffer , size, 0) < 0)
  	{
	    	cout << "receive failed!" << endl;
		return nullptr;
  	}
  	cout<<"buffer length=="<<strlen(buffer)<<endl;
	buffer[size-1]='\0';
	for(int i=0;i<size-1;i++){
	cout<<buffer[i];}
  	reply = buffer;
  	return reply;
}
void TCPClient::receiveFile(int fileSize,int i)
{
char b[10];
sprintf(b,"%d.jpg",i);
  	const char *file_name = (const char*)&b;

	// 打开文件，准备写入
	FILE *fp = fopen(file_name, "w");
	if (NULL == fp)
	{
		printf("File:\t%s Can Not Open To Write\n", file_name);
		return;
	}

	// 从服务器接收数据到buffer中
	// 每接收一段数据，便将其写入文件中，循环直到文件接收完并写完为止
	char buffer[BUFFER_SIZE];
	bzero(buffer, BUFFER_SIZE);
	int length = 0;
	cout<<"receive file----"<<endl;
	int rest=fileSize;
	while ((rest) > 0)
	{
	length = recv(sock, buffer, BUFFER_SIZE, 0);
		if (fwrite(buffer, sizeof(char), length, fp) < length)
		{
			printf("File:\t%s Write Failed\n", file_name);
			break;
		}
		bzero(buffer, BUFFER_SIZE);
		rest-=length;
	}
	// 接收成功后，关闭文件，关闭socket
	printf("Receive File:\t%s From Server IP Successful!\n", file_name);
}


string TCPClient::read()
{
  	char buffer[1] = {};
  	string reply;
  	while (buffer[0] != '\n') {
    		if( recv(sock , buffer , sizeof(buffer) , 0) < 0)
    		{
      			cout << "receive failed!" << endl;
			return nullptr;
    		}
		reply += buffer[0];
	}
	return reply;
}

void TCPClient::exit()
{
    close( sock );
}
