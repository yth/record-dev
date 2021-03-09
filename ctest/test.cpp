#include <iostream>
#include <cassert>

#include "helper.cpp"

int main() {
	std::cout << "Hello, World\n";

  FILE* file = fopen("test_write_size_t.txt", "w+");

  char c = 'c';
  fwrite(&c, 1, 1, file);

  size_t offset = 1;
  size_t size = sizeof(size_t);

  size_t input = 42;
  write_size_t(file, input);

  offset += size;

  assert(read_size_t(file, offset, 1) == 42);

  write_size_t(file, 1);
  offset += size;

  write_size_t(file, 2);
  offset += size;

  write_size_t(file, 3);
  offset += size;

  assert(read_size_t(file, offset, size*2 + 1) == 2);

  write_size_t(file, 0xFFFFFFFFFFFFFFFF);
  std::cout << "Last test\n";
  if ((size_t)*read_n(file, offset, size*4 + 1, 8) != 0xFFFFFFFFFFFFFFFF) {
	std::cout << "Failed test\n";
  }

  return 0;
}
