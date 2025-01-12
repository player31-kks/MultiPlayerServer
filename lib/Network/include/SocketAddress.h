#pragma once
#include "PCH.h"

class SocketAddress{
public:
	explicit SocketAddress();
	SocketAddress(uint32_t inAddress, uint16_t inPort);
	SocketAddress(const std::string& IPAddress, uint16_t inPort);
	explicit SocketAddress(const sockaddr& inSocketAddress);
	[[nodiscard]] auto GetSize() const noexcept -> size_t;
	[[nodiscard]] auto GetAsSockAddr() const noexcept -> const sockaddr*;
	[[nodiscard]] auto GetAsSockAddr() noexcept -> sockaddr*;

	bool operator==(const SocketAddress& inOther) const
	{
		return (mSockAddr.sa_family == AF_INET &&
			GetAsSockAddrIn()->sin_port == inOther.GetAsSockAddrIn()->sin_port) &&
			(GetIP4Ref() == inOther.GetIP4Ref());
	}

	size_t GetHash() const
	{
		return (GetIP4Ref()) |
			((static_cast<size_t>(static_cast<uint32_t>(GetAsSockAddrIn()->sin_port))) << 13) |
			mSockAddr.sa_family;
	}


private:
	sockaddr mSockAddr;
	[[nodiscard]] auto GetAsSockAddrIn() noexcept -> sockaddr_in*;
	[[nodiscard]] auto GetAsSockAddrIn() const noexcept -> const sockaddr_in*;

#if _WIN32
	uint32_t& GetIP4Ref() { return *reinterpret_cast<uint32_t*>(&GetAsSockAddrIn()->sin_addr.S_un.S_addr); }
	const uint32_t& GetIP4Ref()			const { return *reinterpret_cast<const uint32_t*>(&GetAsSockAddrIn()->sin_addr.S_un.S_addr); }
#else
	uint32_t& GetIP4Ref() { return GetAsSockAddrIn()->sin_addr.s_addr; }
	const uint32_t& GetIP4Ref()			const { return GetAsSockAddrIn()->sin_addr.s_addr; }
#endif

};

using SocketAddressPtr = std::shared_ptr<SocketAddress>;

namespace std
{
	template<> struct hash< SocketAddress >
	{
		size_t operator()(const SocketAddress& inAddress) const
		{
			return inAddress.GetHash();
		}
	};
}

