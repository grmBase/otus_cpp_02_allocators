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

template<typename T>
struct allocator1 {
    using value_type = T;

    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;

    template<typename U>
    struct rebind {
        using other = allocator1<U>;
    };


    //allocator1() = default;
    //~allocator1() = default;

    //const size_t auMaxPollSize = 33;


    template<typename U>
    allocator1(const allocator1<U>& aOther)
      //:m_pPoll(new uint8_t[aOther.m_unMax]),
      :m_pPoll((uint8_t*)std::malloc(aOther.m_unMax)),
       m_unUsed(aOther.m_unUsed),
       m_unMax(aOther.m_unMax)

    {
      std::memset(m_pPoll, 0x00, m_unMax);
      std::cout << "<< in copy cntry from other, this: " << this << ", other ptr: " << &aOther << std::endl;
      std::cout << "<< buffer copied, ptr: " << (void*)m_pPoll << std::endl;
    };

    template<typename U>
    const allocator1<T>& operator = (const allocator1<U>& aOther)
    {
      std::cout << "<< in operator = from other, this: " << this << ", other ptr: " << &aOther << std::endl;
      //m_pPoll = new uint8_t[aOther.m_unMax];
      m_pPoll = std::malloc(aOther.m_unMax);

      m_unMax = aOther.m_unMax;
      m_unUsed = aOther.m_unUsed;

      std::memset(m_pPoll, 0x00, m_unMax);


      return *this;
    };

    template<typename U>
    allocator1<T>& operator = (allocator1<U>&& aOther) noexcept
    {
      std::cout << "<< in move operator = from other, this: " << this << ", other ptr: " << &aOther << std::endl;

      m_pPoll = aOther.m_pPoll;
      m_unMax = aOther.m_unMax;
      m_unUsed = aOther.m_unUsed;

      aOther.m_pPoll = nullptr;
      aOther.m_unMax = 0;
      aOther.m_unUsed = 0;

      return *this;
    };



    template<typename U>
    allocator1<T>(allocator1<U>&& aOther)  noexcept
      :m_pPoll(aOther.m_pPoll),
      m_unUsed(aOther.m_unUsed),
      m_unMax(aOther.m_unMax)

    {
      aOther.m_pPoll = nullptr;
      aOther.m_unMax = 0;
      aOther.m_unUsed = 0;

      std::cout << "<< in move cntry from other, this: " << this << ", other ptr: " << &aOther << std::endl;
    };


    allocator1(size_t auMaxPollSize)
      : //m_pPoll(new uint8_t[auMaxPollSize]),
      m_unUsed(0),
      m_unMax(auMaxPollSize)
    {
      //m_pPoll = ; // todo catch? or useless?
      //m_pPoll = new uint8_t[auMaxPollSize],
      m_pPoll = (uint8_t*)std::malloc(auMaxPollSize),
      
      std::memset(m_pPoll, 0x00, auMaxPollSize);
      std::cout << "<< in empty args cnstr() of allocator, this: " << this << ", allocated ptr: " << (void*)m_pPoll << std::endl;
    };



    /*allocator1()
    {
      std::cout << "<< In constructor of allocator1(). this: " << this << std::endl;
    };*/

    
    ~allocator1() 
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
        //delete[] m_pPoll;
      }
      else
      {
        std::cout << "<< ptr is nullptr. free is skipped" << std::endl;
      }

      
      
      
    };

    /*template<typename U> 
    allocator1(const allocator1<U>&) {

    }*/

    T* allocate(std::size_t n) {
#ifndef USE_PRETTY
      std::cout << "allocate: [n = " << n << "]" << std::endl;
#else
      std::cout << "<< in allocate(), this: " << this << ", asked num of elem: " << n << ", sizeof of each(): " << sizeof(T) << std::endl;
#endif


      
      /*auto p = std::malloc(n * sizeof(T));
      if (!p)
          throw std::bad_alloc();
      return reinterpret_cast<T *>(p);
      */
      
      

      
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


template <class T, class U>
bool operator==(const allocator1<T>&, const allocator1<U>&) {
  return false; 
}

template <class T, class U>
bool operator!=(const allocator1<T>&, const allocator1<U>&) { 
  return true; 
}


#endif
