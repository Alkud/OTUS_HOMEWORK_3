#pragma once

#include <tuple>
#include <set>

struct memory_manager
{
  using BlockInfo = std::tuple<
                                size_t, // <0> bytes available
                                size_t, // <1> block size
                                size_t, // <2> allocations count
                                char*   // <3> block address
                              >;

  using BlockSet = std::set<BlockInfo, std::greater<BlockInfo>>;

  memory_manager(){}

  ~memory_manager()
  {
    for (auto& block : blocks)
    {
      delete[] std::get<3>(block);
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
      auto wantedBlock {blocks.begin()};
      while(wantedBlock != blocks.end() &&
            std::get<0>(*wantedBlock) < (bytesToCapture))
      {
        wantedBlock++;
      };
      if (wantedBlock != blocks.end() &&
          std::get<0>(*wantedBlock) >= (bytesToCapture)) // we have enough space in a block
      {
        size_t available {};
        size_t size {};
        size_t allocationsCount {};
        char* address {nullptr};
        std::tie(available, size, allocationsCount, address) = *wantedBlock;
        size_t offset {size - available};
        available -= bytesToCapture;
        allocationsCount++;
        blocks.erase(wantedBlock);
        blocks.insert(std::make_tuple(available, size, allocationsCount, address));
        return reinterpret_cast<void*>(address + offset);
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
    blocks.insert(std::make_tuple(bytesToReserve, bytesToCapture + bytesToReserve, 1, address));
    return reinterpret_cast<void*>(address);
  }

  void release(void* pointerToVoid)
  {
    char* pointerToChar {reinterpret_cast<char*>(pointerToVoid)};
    auto wantedBlock {blocks.begin()};
    size_t allocationsCount {};
    size_t size;
    char* address {nullptr};
    std::tie(std::ignore, size, allocationsCount, address) = *wantedBlock;
    int64_t offset{pointerToChar - address};
    /* Looking for a block containg this address */
    while( wantedBlock != blocks.end() &&
           (offset < 0 || offset > size) )
    {
      wantedBlock++;
      std::tie(std::ignore, size, allocationsCount, address) = *wantedBlock;
      offset = pointerToChar - address;
    }
    if (wantedBlock != blocks.end())
    {
      allocationsCount--;
      if (allocationsCount == 0) // That was the last allocated segment in a block
      {
        delete[] address;
        blocks.erase(wantedBlock);
      }
    }
  }

  BlockSet blocks;
};
