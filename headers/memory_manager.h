#pragma once

#include <tuple>
#include <set>

struct memory_manager
{
  struct BlockInfo
  {
    BlockInfo (size_t _available, size_t _size,
               size_t _allocationsCount, char* _address) :
      available{_available}, size{_size},
      allocationsCount{_allocationsCount}, address{_address}{}

    size_t available;         // bytes available
    size_t size;              // block size
    size_t allocationsCount;  // alloc calls count
    char* address;            // block address
  };

  using BlocksVector = std::vector<BlockInfo>;

  memory_manager(){}

  ~memory_manager()
  {
    for (auto& block : blocks)
    {
      delete[] block.address;
    }
  }

  void* capture (size_t bytesToCapture, size_t bytesToReserve = 0) throw (std::bad_alloc)
  {
    if (blocks.empty() == true)
    {
      return createNewBlock(bytesToCapture, bytesToReserve);
    }
    else
    {
      auto& wantedBlock {blocks.back()};
      if (wantedBlock.available >= bytesToCapture) // we have enough space in a block
      {
        size_t offset {wantedBlock.size - wantedBlock.available};
        wantedBlock.available -= bytesToCapture;
        wantedBlock.allocationsCount++;
        return reinterpret_cast<void*>(wantedBlock.address + offset);
      }
      else // we don't have enough space in present blocks, create a new one
      {
        return createNewBlock(bytesToCapture, bytesToReserve);
      }
    }
  }

  void* createNewBlock(size_t bytesToCapture, size_t bytesToReserve = 0) throw (std::bad_alloc)
  {
    char* address {new char[bytesToCapture + bytesToReserve]};
    if (nullptr == address)
      throw std::bad_alloc();
    blocks.push_back(BlockInfo{bytesToReserve, bytesToCapture + bytesToReserve, 1, address});
    return reinterpret_cast<void*>(address);
  }

  void release(void* pointerToVoid)
  {
    char* pointerToChar {reinterpret_cast<char*>(pointerToVoid)};
    /* Looking for a block containg this address */
    for (auto& wantedBlock : blocks)
    {
      size_t offset = pointerToChar - wantedBlock.address;
      if (offset < 0 || offset > wantedBlock.size)
      {
        wantedBlock.allocationsCount--;
        if (0 == wantedBlock.allocationsCount) // That was the last allocated segment in a block
        {
          delete[] wantedBlock.address;
          wantedBlock.available = 0;
          wantedBlock.size = 0;
          wantedBlock.address = nullptr;
        }
        break;
      }
    }
  }

  BlocksVector blocks;
};
