add_library(Memory 
	STATIC 
	"include/MemoryStream.h"
	"src/MemoryStream.cpp"
)

target_include_directories(Memory PUBLIC "./include")
