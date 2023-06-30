#ifndef ENIGMA_MEMORY
#define ENIGMA_MEMORY

/*
This module itself is allowed to stop execution completely without the VM knowing.
It can abort or exit the program entirely.
It will display necessary error messages before exiting.
*/

#include <vector>
#include <cstdint>
// #include <string>
#include <iostream> //temporary: remove later
#include <cstdlib>

typedef std::uint8_t byte;
typedef std::uint16_t word;
typedef std::uint32_t dword;
typedef std::uint64_t qword;

#ifndef MEM_START // can explicitly declare if the memory needs to start from some address
#define MEM_START 0x0
#endif

#ifndef MEM_SIZE // can explicitly declare if the memory needs to be of some size
#define MEM_SIZE 1024
#endif

#define GET_BYTE 255

#define BIN_MAX 0b1111111111111111111111111111111111111111111111111111111111111111

// the maximum possible memory size
// this value can be changed by the user, however, user discretion is required
// since we support upto 64 bits memory addressing, this value can reach as much as 2^64 -1 which is impossible in real world since, the memory are not that large
static std::uint64_t max_memory_length = 524288;

class Memory
{
public:
  // Only default constructor allowed, everything else is not
  Memory();

  // method for writing to memory
  // we need a address to write, as well as a value to write. We also need the number of bytes to write.
  //  the function will take a 64 bits address and 64 bits value even if that is not the case for the value
  void mem_write64(std::uint64_t address, std::uint64_t value);
  void mem_write32(qword address, qword value);
  void mem_write16(qword address, qword vaue);
  void mem_write8(qword address, qword value);

  // method for reading from memory, takes only the address
  std::uint64_t mem_read64(std::uint64_t address);
  std::uint64_t mem_read32(std::uint64_t address);
  qword mem_read16(qword address);
  qword mem_read8(qword address);

  void resize(word __new_size);

  void pointer_limit_increase(word __increase_by);

  void increase_upper_limit(qword __increase_by);

  std::size_t current_size() { return pointer_limit; }

  void add_size(qword size_to_add);

private:
  std::vector<std::uint8_t> memory;

  std::uint64_t pointer_limit;
};

// the constructor
Memory::Memory()
{
  // the starting size of the memory will be 1024 bytes or 1MB
  memory.resize(1024);
  pointer_limit = 1024; // for now
}

// the mem_write function
void Memory::mem_write64(std::uint64_t address, std::uint64_t value)
{
  // the memory will be an array of bytes
  // first we break the value to individual bytes and put those values from the address up to length
  // the length can only be either 1, 2, 4 or 8. It cannot be 16 or 64
  // is the memory access out of bounds?
  if (address + 7 >= pointer_limit)
  {
    std::cerr << "Segmentation fault. Accessing out of bounds memory." << std::endl;
    exit(-1);
  }
  std::uint32_t shift_by = 56;
  for (std::uint32_t i = 0; i < 8; i++)
  {
    memory[i + address] = (value >> shift_by) & GET_BYTE;
    shift_by -= 8;
  }
}

// the mem_write function
void Memory::mem_write32(qword address, qword value)
{
  // the memory will be an array of bytes
  // first we break the value to individual bytes and put those values from the address up to length
  // the length can only be either 1, 2, 4 or 8. It cannot be 16 or 64
  // is the memory access out of bounds?
  if (address + 3 >= pointer_limit)
  {
    std::cerr << "Segmentation fault. Accessing out of bounds memory." << std::endl;
    exit(-1);
  }
  std::uint32_t shift_by = 24;
  for (std::uint32_t i = 0; i < 4; i++)
  {
    memory[i + address] = (value >> shift_by) & GET_BYTE;
    shift_by -= 8;
  }
}

// the mem_write function
void Memory::mem_write16(qword address, qword value)
{
  // the memory will be an array of bytes
  // first we break the value to individual bytes and put those values from the address up to length
  // the length can only be either 1, 2, 4 or 8. It cannot be 16 or 64
  // is the memory access out of bounds?
  if (address + 1 >= pointer_limit)
  {
    std::cerr << "Segmentation fault. Accessing out of bounds memory." << std::endl;
    exit(-1);
  }
  std::uint32_t shift_by = 8;
  for (std::uint32_t i = 0; i < 2; i++)
  {
    memory[i + address] = (value >> shift_by) & GET_BYTE;
    shift_by -= 8;
  }
}

// the mem_write function
void Memory::mem_write8(qword address, qword value)
{
  // the memory will be an array of bytes
  // first we break the value to individual bytes and put those values from the address up to length
  // the length can only be either 1, 2, 4 or 8. It cannot be 16 or 64
  // is the memory access out of bounds?
  if (address + 1 >= pointer_limit)
  {
    std::cerr << "Segmentation fault. Accessing out of bounds memory." << std::endl;
    exit(-1);
  }
  memory[address] = (value & 255);
}

std::uint64_t Memory::mem_read64(std::uint64_t address)
{
  // this will read from the memory and hence return the value in it.
  // it also takes length as the input and sends the data of that range only
  // the length can only be either 1, 2, 4 or 8. It cannot be 16 or 64
  // is the memory access out of bounds?
  if (address + 8 >= pointer_limit)
  {
    std::cerr << "Segmentation fault. Accessing out of bounds memory." << std::endl;
    exit(-1);
  }
  std::uint64_t output = 0;
  for (std::uint32_t i = 0; i < 8; i++)
  {
    output = (output << 8) | memory[i + address];
  }
  return output;
}

std::uint64_t Memory::mem_read32(std::uint64_t address)
{
  // this will read from the memory and hence return the value in it.
  // it also takes length as the input and sends the data of that range only
  // the length can only be either 1, 2, 4 or 8. It cannot be 16 or 64
  // is the memory access out of bounds?
  if (address + 3 >= pointer_limit)
  {
    std::cerr << "Segmentation fault. Accessing out of bounds memory." << std::endl;
    exit(-1);
  }
  std::uint64_t output = 0;
  for (std::uint32_t i = 0; i < 4; i++)
  {
    output = (output << 8) | memory[i + address];
  }
  return output;
}

qword Memory::mem_read16(qword address)
{
  // this will read from the memory and hence return the value in it.
  // it also takes length as the input and sends the data of that range only
  // the length can only be either 1, 2, 4 or 8. It cannot be 16 or 64
  // is the memory access out of bounds?
  if (address + 1 >= pointer_limit)
  {
    std::cerr << "Segmentation fault. Accessing out of bounds memory." << std::endl;
    exit(-1);
  }
  std::uint64_t output;
  for (std::uint32_t i = 0; i < 2; i++)
  {
    output = (output << 8) | memory[i + address];
  }
  return output;
}

qword Memory::mem_read8(qword address)
{
  // this will read from the memory and hence return the value in it.
  // it also takes length as the input and sends the data of that range only
  // the length can only be either 1, 2, 4 or 8. It cannot be 16 or 64
  // is the memory access out of bounds?
  if (address >= pointer_limit)
  {
    std::cerr << "Segmentation fault. Accessing out of bounds memory." << std::endl;
    exit(-1);
  }
  std::uint64_t output = memory[address];
  return output;
}

void Memory::resize(word __new_size)
{
  if (__new_size > max_memory_length)
  {
    std::cerr << "Memory expansion requested exceeding the upper limit" << std::endl;
    exit(-1);
  }
  memory.resize(__new_size);
}

void Memory::pointer_limit_increase(word __increase_by)
{
  if (pointer_limit + __increase_by > max_memory_length)
  {
    std::cerr << "Error increasing the pointer limit. max_memory_length is " << max_memory_length << std::endl;
    exit(-1);
  }
  pointer_limit += __increase_by;
}

void Memory::increase_upper_limit(qword __increase_by)
{
  if (max_memory_length + __increase_by > 1073741824)
  {
    std::cerr << "Increasing memory by leaps and bounds" << std::endl;
    exit(-1);
  }
  max_memory_length += __increase_by;
}

void Memory::add_size(qword size_to_add)
{
  if (pointer_limit + size_to_add > max_memory_length)
  {
    std::cerr << "Error increasing the pointer limit. max_memory_length is " << max_memory_length << std::endl;
    exit(-1);
  }
  pointer_limit += size_to_add;
  memory.resize(pointer_limit);
}

#endif