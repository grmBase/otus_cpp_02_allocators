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


  t_my_list()
  {
  };

  void add(T aValue) {
    t_node_record* pNew = new t_node_record();
    pNew->m_date = aValue;
    pNew->p_next = m_pHead;
    m_pHead = pNew;
  };


  t_node_record* m_pHead = nullptr;

};