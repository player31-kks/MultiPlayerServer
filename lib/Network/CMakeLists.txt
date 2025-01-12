add_library(Network 
	STATIC 
	"include/SocketAddress.h" 
	"src/SocketAddress.cpp" "src/UDPSocket.cpp" "include/SocketUtil.h" "src/SocketUtil.cpp")

target_include_directories(Network PUBLIC "./include")
target_precompile_headers(Network PRIVATE "include/PCH.h")


