#ifndef ENIGMA_MEMORY
#define ENIGMA_MEMORY

/*
This module itself is allowed to stop execution completely without the VM knowing.
It can abort or exit the program entirely.
It will display necessary error messages before exiting.
*/

#include <vector>
#include <cstdint>
#include <iostream>
#include <cstdlib>

typedef std::uint8_t byte;
typedef std::uint16_t word;
typedef std::uint32_t dword;
typedef std::uint64_t qword;

#ifndef MEM_START
#define MEM_START 0x0
#endif

#ifndef MEM_SIZE
#define MEM_SIZE 1024
#endif

#define GET_BYTE 255

#define BIN_MAX 0b1111111111111111111111111111111111111111111111111111111111111111

static qword max_memory_length = 524288;

class Memory
{
public:
  Memory();

  void mem_write64(qword address, qword value);
  void mem_write32(qword address, qword value);
  void mem_write16(qword address, qword vaue);
  void mem_write8(qword address, qword value);

  qword mem_read64(qword address);
  qword mem_read32(qword address);
  qword mem_read16(qword address);
  qword mem_read8(qword address);

  void resize(word __new_size);

  void pointer_limit_increase(word __increase_by);

  void increase_upper_limit(qword __increase_by);

  std::size_t current_size() { return pointer_limit; }

  void add_size(qword size_to_add);

private:
  std::vector<std::uint8_t> memory;

  qword pointer_limit;
};

Memory::Memory()
{
  memory.resize(MEM_SIZE);
  pointer_limit = MEM_SIZE;
}

void Memory::mem_write64(qword address, qword value)
{
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

void Memory::mem_write32(qword address, qword value)
{
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

void Memory::mem_write16(qword address, qword value)
{
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

void Memory::mem_write8(qword address, qword value)
{
  if (address + 1 >= pointer_limit)
  {
    std::cerr << "Segmentation fault. Accessing out of bounds memory." << std::endl;
    exit(-1);
  }
  memory[address] = (value & 255);
}

qword Memory::mem_read64(qword address)
{
  if (address + 8 >= pointer_limit)
  {
    std::cerr << "Segmentation fault. Accessing out of bounds memory." << std::endl;
    exit(-1);
  }
  qword output = 0;
  for (std::uint32_t i = 0; i < 8; i++)
  {
    output = (output << 8) | memory[i + address];
  }
  return output;
}

qword Memory::mem_read32(qword address)
{
  {
    std::cerr << "Segmentation fault. Accessing out of bounds memory." << std::endl;
    exit(-1);
  }
  qword output = 0;
  for (std::uint32_t i = 0; i < 4; i++)
  {
    output = (output << 8) | memory[i + address];
  }
  return output;
}

qword Memory::mem_read16(qword address)
{
  if (address + 1 >= pointer_limit)
  {
    std::cerr << "Segmentation fault. Accessing out of bounds memory." << std::endl;
    exit(-1);
  }
  qword output;
  for (std::uint32_t i = 0; i < 2; i++)
  {
    output = (output << 8) | memory[i + address];
  }
  return output;
}

qword Memory::mem_read8(qword address)
{
  if (address >= pointer_limit)
  {
    std::cerr << "Segmentation fault. Accessing out of bounds memory." << std::endl;
    exit(-1);
  }
  qword output = memory[address];
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
    std::cerr << "Error increasing the pointer limit[Increasing above the limit]. max_memory_length is " << max_memory_length << std::endl;
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