#include <iostream>
#include <signal.h>
#include "TCPClient.h"
#include <sstream>
TCPClient tcp;

void sig_exit(int s)
{
	tcp.exit();
	exit(0);
}

int main(int argc, char *argv[])
{
	signal(SIGINT, sig_exit);

	tcp.setup("127.0.0.1",11999);
	int i=0;
	while(1)
	{
		srand(time(NULL));
		string fileSizeStr=tcp.receive();
		tcp.Send("file_size_ack");
		stringstream stream(fileSizeStr);
		int fileSize=0;
		stream>>fileSize;
		tcp.receiveFile(fileSize,i++);
		//发送接收完确认
		tcp.Send("over");
		sleep(1);
	}
	return 0;
}
