#include <cassert>
#include <stdio.h>
#include <map>


#include "crc.h"


int main(int argc, char** argv) {
	// CRC32 Test
	assert(CRC32("test") == CRC32("test"));
	assert(CRC32("test1") != CRC32("test2"));

	// Map Test
	std::map<uint32_t, uint32_t> my_map;
	my_map[CRC32("test1")] = CRC32("test123");
	my_map[CRC32("test2")] = CRC32("test456");

	assert(my_map[CRC32("test1")] == CRC32("test123"));
	assert(my_map[CRC32("test2")] == CRC32("test456"));

	printf("Silence is golden.\n");
	return 0;
}