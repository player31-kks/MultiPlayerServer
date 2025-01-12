#include "UDPSocket.h"
#include "SocketUtil.h"

UDPSocket::~UDPSocket()
{
#if _WIN32
	closesocket(mSocket);
#else
	close(mSocket);
#endif
}

auto UDPSocket::Bind(const SocketAddress& inToAddress)-> int
{
	int error = bind(
		mSocket,inToAddress.GetAsSockAddr(),
		static_cast<int>(inToAddress.GetSize())
	);

	if (error != 0) {
		SocketUtil::ReportError("UDPSOCKET::Bind Error");
		return SocketUtil::GetLastError();
	}

	return NO_ERROR;
}

auto UDPSocket::SetNonBlockingMode(bool inShouldBeNonBlocking) -> int
{
#if _WIN32
	u_long arg = inShouldBeNonBlocking ? 1 : 0;
	int result = ioctlsocket(mSocket, FIONBIO, &arg);
#else
	int flags = fcntl(mSocket, F_GETFL, 0);
	flags = inShouldBeNonBlocking ? (flags | O_NONBLOCK) : (flags & ~O_NONBLOCK);
	int result = fcntl(mSocket, F_SETFL, flags);
#endif

	if (result == SOCKET_ERROR)
	{
		SocketUtil::ReportError("UDPSocket::SetNonBlockingMode");
		return SocketUtil::GetLastError();
	}
	else
	{
		return NO_ERROR;
	}
}
