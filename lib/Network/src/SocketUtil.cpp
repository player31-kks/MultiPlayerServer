#include "SocketUtil.h"

auto SocketUtil::StaticInit() -> bool
{
#if _WIN32
	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != NO_ERROR)
	{
		ReportError("Starting Up");
		return false;
	}
#endif
	return true;
}

auto SocketUtil::CleanUp() -> void
{
#if _WIN32
	WSACleanup();
#endif
}

auto SocketUtil::ReportError(const std::string& inOperationDesc) -> void
{
#if _WIN32
	LPVOID lpMsgBuf;
	DWORD errorNum = GetLastError();

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		errorNum,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf,
		0, NULL);


	printf_s("Error %s: %d- %s\n", inOperationDesc.c_str(), errorNum, lpMsgBuf);
#else
	printf("Error: %hs\n", inOperationDesc);
#endif
}

int SocketUtil::GetLastError()
{
#if _WIN32
	return WSAGetLastError();
#else
	return errno;
#endif
}

auto SocketUtil::CreateUdpSocket() -> UDPSocketPtr
{
	SOCKET s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	if (s != INVALID_SOCKET)
	{
		return UDPSocketPtr(new UDPSocket{ s });
	}
	else
	{
		ReportError("SocketUtil::CreateUDPSocket");
		return nullptr;
	}
}
