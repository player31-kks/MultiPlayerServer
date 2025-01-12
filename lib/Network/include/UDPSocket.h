#pragma once
#include "PCH.h"
#include "SocketAddress.h"

class UDPSocket{

public:
	~UDPSocket();
	[[nodiscard]] auto Bind(const SocketAddress& inToAddress)->int;
	
	template <std::contiguous_iterator It>
	[[nodiscard]] auto SendTo(std::span<It> data, const SocketAddress& inTo) -> int {
		if (data.empty()) {
			return 0;
		}

		const char* buffer = reinterpret_cast<const char*>(data.data());
		int dataSize = static_cast<int>(data.size_bytes());
		int byteSentCount = sendto(mSocket, 
			buffer, 
			dataSize, 
			0, 
			inTo.GetAsSockAddr(), 
			static_cast<int>(inTo.GetSize()));
		
		return byteSentCount == SOCKET_ERROR ? -1 : byteSentCount;
	}

	template <typename T,size_t N>
	[[nodiscard]] auto ReceiveFrom(std::array<T,N>& buffer, SocketAddress& outFrom) -> int {
		if (buffer.empty()) {
			return 0;  
		}
		
		char* recvBuffer = reinterpret_cast<char*>(buffer.data());
		int bufferSize = static_cast<int>(buffer.size());
		socklen_t fromLength = outFrom.GetSize();


		int bytesReceived = recvfrom(
			mSocket,
			recvBuffer,
			bufferSize,
			0,
			outFrom.GetAsSockAddr(),
			&fromLength);

		return bytesReceived == SOCKET_ERROR ? -1 : bytesReceived;
	}

	auto SetNonBlockingMode(bool inShouldBeNonBlocking)->int;

private:
	friend class SocketUtil;
	explicit UDPSocket(SOCKET inSocket) :mSocket{ inSocket } {};
	SOCKET mSocket;

public:
	UDPSocket(const UDPSocket&) = delete;
	UDPSocket& operator=(const UDPSocket&) = delete;
	UDPSocket(UDPSocket&&) noexcept = default;
	UDPSocket& operator=(UDPSocket&&) noexcept = default;
};

using UDPSocketPtr = std::shared_ptr<UDPSocket>;

