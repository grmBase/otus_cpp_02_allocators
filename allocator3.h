#pragma once

#ifndef __PRETTY_FUNCTION__
#include "pretty.h"
#endif

#include <iostream>
#include <vector>
#include <map>
#include <iomanip> //setfill
#include <sstream>
#include <cstring> //memset


#define USE_PRETTY 1

template<typename T, size_t aunSize>
struct t_allocator {
    using value_type = T;

    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;

    template<typename U>
    struct rebind {
        using other = t_allocator<U, aunSize>;
    };

    t_allocator()
      : m_pPoll((uint8_t*)std::malloc(aunSize)),
      m_unUsed(0),
      m_unMax(aunSize)
    {
      std::memset(m_pPoll, 0x00, aunSize);
      std::cout << "<< in empty args cnstr() of allocator, this: " << this << ", allocated ptr: " << (void*)m_pPoll << std::endl;
    };

    ~t_allocator()
    {
      std::cout << "<< destr() of allocator, this: " << this << std::endl;

      // TODO: couldn't find for_each for "char* + size"
      if (m_unMax && m_pPoll)
      {

        std::cout << std::hex << std::setfill('0');

        std::cout << "total size of buf: " << m_unMax << ", used: " << m_unUsed << std::endl;
        for (size_t i = 0; i < m_unMax; ++i) {
          std::cout << std::setw(2) << (unsigned)(m_pPoll[i]) << " ";
        }
        std::cout << std::endl;

        std::cout << "<< before delete poll. ptr: " << (void*)m_pPoll << std::endl;

        std::free(m_pPoll);
        m_pPoll = nullptr;

        std::cout << "<< delete passed" << std::endl;
      }
      else
      {
        std::cout << "<< ptr is nullptr. free is skipped" << std::endl;
      }
    };


    /*
    template<typename U> 
    t_allocator(const t_allocator<U, aunSize>&)
      //: m_pPoll(nullptr),
      //m_unUsed(0),
      //m_unMax(0)
      : m_pPoll((uint8_t*)std::malloc(aunSize)),
      m_unUsed(0),
      m_unMax(aunSize)
    {
      //std::cout << "<< in copy constr" << std::endl;
    }*/




    T *allocate(std::size_t n) {

      /*
#ifndef USE_PRETTY
        std::cout << "allocate: [n = " << n << "]" << std::endl;
#else
        std::cout << __PRETTY_FUNCTION__ << "[n = " << n << "]" << std::endl;
#endif
        auto p = std::malloc(n * sizeof(T));
        if (!p)
            throw std::bad_alloc();
        return reinterpret_cast<T *>(p);
        */

#ifndef USE_PRETTY
      std::cout << "allocate: [n = " << n << "]" << std::endl;
#else
      std::cout << "<< in allocate(), this: " << this << ", asked num of elem: " << n << ", sizeof of each(): " << sizeof(T) << std::endl;
#endif


      // check that we have enough room:
      size_t unAskedSize = n * sizeof(T);
      if (m_unUsed + unAskedSize > m_unMax) {
        std::stringstream streamTmp;
        streamTmp << "Asked too much memory. Buff size: " << m_unMax
          << ", used: " << m_unUsed << ", asked to alloc: " << unAskedSize;

        //std::string strInfo = 
        throw std::runtime_error(streamTmp.str());
      }

      T* pTmp = reinterpret_cast<T*>(m_pPoll + m_unUsed);
      m_unUsed += unAskedSize;

      std::cout << "returning ptr: " << (void*)pTmp << std::endl;
      return pTmp;
    }

    void deallocate(T *p, std::size_t n) {

      /*
#ifndef USE_PRETTY
        std::cout << "deallocate: [n  = " << n << "] " << std::endl;
#else
        std::cout << __PRETTY_FUNCTION__ << "[n = " << n << "]" << std::endl;
#endif
        std::free(p);
        */

#ifndef USE_PRETTY
      std::cout << "deallocate: [n  = " << n << "] " << std::endl;
#else
        //std::cout << "<< in deallocate(), this: " << this << ", func: " << __PRETTY_FUNCTION__ << "[n = " << n << "]" << std::endl;
      std::cout << "<< in deallocate(), this: " << this << ", ptr: " << (void*)p << ", block_size: " << n << std::endl;
#endif
      // TODO: можно в реальной реализации наваять аналог FAT в памяти и реально чистить "области"
      // тут пока просто запполним датой BADF00D
      //std::free(p);

      
      // Проверяем освобождаемое влазит в наши левые и правые границы:
      if ((uint8_t*)p< m_pPoll || ((uint8_t*)p + n) > (m_pPoll + m_unUsed)) {
        throw std::runtime_error("Bad operation with memory. Invesigate around there");
      }

      ::memset((void*)p, 0xBF, n);
      

      // todo: пометить блок в "FAT" как свободный

    }




    template<typename U, typename ...Args>
    void construct(U *p, Args &&...args) {
#ifndef USE_PRETTY
        std::cout << "construct" << std::endl;
#else
        std::cout << __PRETTY_FUNCTION__ << std::endl;
#endif
        new(p) U(std::forward<Args>(args)...);
    };

    void destroy(T *p) {
#ifndef USE_PRETTY
        std::cout << "destroy" << std::endl;
#else
        std::cout << __PRETTY_FUNCTION__ << std::endl;
#endif
        p->~T();
    }



    uint8_t* m_pPoll;
    size_t m_unUsed;
    size_t m_unMax;

};

