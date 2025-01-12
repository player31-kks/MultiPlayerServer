#pragma once

#include "PCH.h"
#include "UDPSocket.h"

class SocketUtil{
public:
	static auto StaticInit()->bool;
	static auto CleanUp() -> void;
	static auto ReportError(const std::string& inOperationDesc)->void;
	static int GetLastError();
	static auto CreateUdpSocket()->UDPSocketPtr;
};
