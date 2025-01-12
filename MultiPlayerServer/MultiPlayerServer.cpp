// MultiPlayerServer.cpp : 애플리케이션의 진입점을 정의합니다.
//

#include "MultiPlayerServer.h"
#include "SocketUtil.h"
#include <array>

using namespace std;



int main()
{
	SocketUtil::StaticInit();

	UDPSocketPtr serverSocket = SocketUtil::CreateUdpSocket();
	SocketAddress localAddress{ "192.168.45.154" ,3000};
	serverSocket->Bind(localAddress);
	serverSocket->SetNonBlockingMode(true);
	std::cout << "Server Start!!!" << std::endl;

	while (true) {
		std::array<char, 1500> data{};
		data.fill(0);
		SocketAddress receviedAddress;
		int bytesReceived =serverSocket->ReceiveFrom(data, receviedAddress);
		std::cout << "Loop!!!" << std::endl;
	}
	SocketUtil::CleanUp();
	return 0;
}
