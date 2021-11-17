//------------------------------
#include <map>
#include <string>
#include <iostream>
//------------------------------
#include "work_obj.h"
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
    process<TMapCustom>(myMap);
  }

  {
    std::map<int, int> myMap;
    process<std::map<int, int>>(myMap);
  }


  return 0;
};



int TWorkObj::task2()
{
  t_my_list<int> my_list;

  my_list.add(1);


  return 0;
};