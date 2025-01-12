#include "SocketAddress.h"

SocketAddress::SocketAddress()
{
	auto* addr = GetAsSockAddrIn();
	addr->sin_family = AF_INET;
	addr->sin_port = 0;
	GetIP4Ref() = INADDR_ANY;

}

SocketAddress::SocketAddress(uint32_t inAddress, uint16_t inPort)
	:mSockAddr{}
{
	auto* addr = GetAsSockAddrIn();
	addr->sin_family = AF_INET;
	addr->sin_port = htons(inPort);
	GetIP4Ref() = htonl(inAddress);
}

SocketAddress::SocketAddress(const std::string& iPAddress, uint16_t inPort)
{
	auto* addr = GetAsSockAddrIn();
	addr->sin_family = AF_INET;
	addr->sin_port = htons(inPort);
	inet_pton(AF_INET, iPAddress.c_str(), &(addr->sin_addr));
}

SocketAddress::SocketAddress(const sockaddr& inSocketAddress)
	:mSockAddr{ inSocketAddress }
{
}

auto SocketAddress::GetSize() const noexcept ->size_t
{
	return sizeof(sockaddr);
}

auto SocketAddress::GetAsSockAddr() const noexcept -> const sockaddr*
{
	return &mSockAddr;
}

auto SocketAddress::GetAsSockAddr() noexcept -> sockaddr*
{
	return &mSockAddr;
}

auto SocketAddress::GetAsSockAddrIn() noexcept -> sockaddr_in*
{
	return std::launder(reinterpret_cast<sockaddr_in*>(&mSockAddr));
}

auto SocketAddress::GetAsSockAddrIn() const noexcept -> const sockaddr_in*
{
	return std::launder(reinterpret_cast<const sockaddr_in*>(&mSockAddr));
}
