#pragma once

#include <memory> // std::allocator


template <typename T, typename allocator = std::allocator<T> >
class t_my_list
{
public:

  // внутренний контейнер хранения
  class t_node_record
  {
  public:
    T m_date; //payload
    t_node_record* p_next = nullptr;
  };



  class iterator
  {
  public:


    iterator(t_node_record* apNode) : m_pNode(apNode) { };

    iterator& operator++ () {
      if (!m_pNode) {
        throw std::runtime_error("next it nullptr");
        //return *this;
      }
      m_pNode = m_pNode->p_next;

      return *this;
    };

    bool operator!= (const iterator& other)
    {
      return m_pNode != other.m_pNode;
    }

    const T& operator*() const
    {
      return m_pNode->m_date;
    }


  private:

    t_node_record* m_pNode;
  };




  t_my_list()
  {
  };

  ~t_my_list()
  {
    t_node_record* pCurr = m_pHead;
    t_node_record* pTmp = nullptr;
    while (pCurr) {

      pTmp = pCurr;
      pCurr = pCurr->p_next;

      //delete pTmp;
      node_traits::destroy(node_alloc, pTmp);
      node_traits::deallocate(node_alloc, pTmp, 1);
    }
  }

  void push_front(T aValue) {

    //using allocator_t = typename std::allocator_traits<allocator>::template rebind_alloc< t_node_record >;
    //t_node_record* pNew = new t_node_record();
    //t_node_record* pNew = m_allocator.allocate(1);

    t_node_record* pNew = node_traits::allocate(node_alloc, 1);

    //node_traits::construct(node_alloc, pNew, std::move(aValue), head);

    node_traits::construct(node_alloc, pNew);

    pNew->m_date = aValue;
    pNew->p_next = m_pHead;
    m_pHead = pNew;
  };

  iterator begin() const {
    return { m_pHead };
  }

  iterator end() const {
    return { nullptr };
  }


  using node_traits = typename std::allocator_traits<allocator>::template rebind_traits<t_node_record>;
  using node_allocator_type = typename node_traits::allocator_type;

  t_node_record* m_pHead = nullptr;

  //allocator m_allocator;
  node_allocator_type node_alloc;

};