//---------------------------------------------------------------------------
#ifndef __allocator1_h__
#define __allocator1_h__
//---------------------------------------------------------------------------


#ifndef __PRETTY_FUNCTION__
#include "pretty.h"
#endif

#include <iostream>
#include <iomanip>
#include <cstring>
#include <sstream>


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
      //m_pPoll = (uint8_t*)std::malloc(aunSize);
      
      std::memset(m_pPoll, 0x00, aunSize);
      std::cout << "<< in empty args cnstr() of allocator, this: " << this << ", allocated ptr: " << (void*)m_pPoll << std::endl;
    };
    

    
    ~t_allocator() 
    {
      std::cout << "<< destr() of allocator, this: " << this <<  std::endl;

      // TODO: couldn't find for_each for "char* + size"
      if (m_unMax && m_pPoll)
      {

        std::cout << std::hex << std::setfill('0');

        std::cout << "total size of buf: " << m_unMax << ", used: " << m_unUsed << std::endl;
        for (size_t i = 0; i<m_unMax; ++i) {
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


    T* allocate(std::size_t n) {
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
#ifndef USE_PRETTY
        std::cout << "deallocate: [n  = " << n << "] " << std::endl;
#else
        //std::cout << "<< in deallocate(), this: " << this << ", func: " << __PRETTY_FUNCTION__ << "[n = " << n << "]" << std::endl;
      std::cout << "<< in deallocate(), this: " << this << ", ptr: " << (void*)p << ", block_size: " << n << std::endl;
#endif
        // TODO: можно в реальной реализации наваять аналог FAT в памяти и реально чистить "области"
        // тут пока просто запполним датой BADF00D
        //std::free(p);


        /*
        // Проверяем освобождаемое влазит в наши левые и правые границы:
        if ((uint8_t*)p< m_pPoll || ((uint8_t*)p + n) > (m_pPoll + m_unUsed)) {
          throw std::runtime_error("Bad operation with memory. Invesigate around there");
        }

        ::memset((void*)p, 0xBF, n);
        */

        // todo: пометить блок в "FAT" как свободный
    }

    template<typename U, typename ...Args>
    void construct(U *p, Args &&...args) {
#ifndef USE_PRETTY
        std::cout << "construct" << std::endl;
#else
        //std::cout << "<< in construct(), this: " << this << ", func: " << __PRETTY_FUNCTION__ << <<std::endl;
      std::cout << "<< in construct(), this: " << this << ", ptr to place: " << (void*)p << ", sizeof of type: " << sizeof(U) << std::endl;
#endif
        new(p) U(std::forward<Args>(args)...);
    };

    void destroy(T *p) {
#ifndef USE_PRETTY
        std::cout << "destroy" << std::endl;
#else
        //std::cout << "<< in destroy(), this: " << this << ", func: " << __PRETTY_FUNCTION__ << std::endl;
      std::cout << "<< in destroy(), this: " << this << ", obj ptr to destructor: " << (void*)p << std::endl;
#endif
        p->~T();
    }

  public:

    uint8_t* m_pPoll;
    size_t m_unUsed;
    size_t m_unMax;
};


/*
template <class T, class U>
constexpr bool operator==(const t_allocator<T>&, const t_allocator<U>&) {
  return false; 
}

template <class T, class U>
constexpr bool operator!=(const t_allocator<T>&, const t_allocator<U>&) {
  return true; 
}
*/


#endif
