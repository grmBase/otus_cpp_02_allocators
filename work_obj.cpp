//------------------------------
#include <map>
#include <string>
#include <iostream>
//------------------------------
#include "work_obj.h"

// разные заходы на поиски рабочего варианта:
//#include "allocator1.h"
#include "allocator3.h"

#include "my_list.h"
//------------------------------


// todo: переделать на шаблоны? или на массив, чтобы было чуть пооптимальнее?
size_t factorial(size_t anArg)
{
  if (anArg <= 1)
    return 1;
  
  return anArg * factorial(anArg - 1);
}


int TWorkObj::task1()
{

  {
    using TMapCustom = std::map<int, int, std::less<int>,
      t_allocator<std::pair<const int, int>, 500>
    >;

    TMapCustom myMap;
    process_map<TMapCustom>(myMap);
  }

  {
    std::map<int, int> myMap;
    process_map<std::map<int, int>>(myMap);
  }

  return 0;
};



int TWorkObj::task2()
{
  
  {
    t_my_list<int> my_list;
    process_my_cntr(my_list);
  }

  {
    t_my_list<int, t_allocator <int, 500 > > my_list;
    process_my_cntr(my_list);
  }

  return 0;
};