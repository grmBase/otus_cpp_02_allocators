//------------------------------
#include <map>
#include <string>
#include <iostream>
//------------------------------
#include "work_obj.h"
#include "allocator1.h"
//------------------------------



int TWorkObj::task1()
{


  typedef  allocator1<std::pair<const int, float >> TCurrentAllocator;

  //TCurrentAllocator allocator_instance (TCurrentAllocator(330));

  TCurrentAllocator allocator_instance(330);

  //allocator1<std::pair<const int, float >> allocator_instance();
  

  //std::map<int, int> map2;

  std::map<
    int, float, std::less<int>, 
    //allocator1<std::pair<const int, float>
    TCurrentAllocator
  > myMap{allocator_instance};

  
  for(size_t i=0; i<3; ++i) {
    // добавка, чтобы было видно что float
    myMap[i] = static_cast<float>(i) + 0.1234f;
  }

  // Starting from c++ 17
  /*for(const auto& [key, value] : myMap) {
    std::cout << key << " has value " << value << std::endl;
  }*/

  std::cout << std::endl << "---------------------------" << std::endl;
  std::cout << "Before print of map. Size of items: " << myMap.size() << std::endl;
  for(const auto& kv : myMap) {
    std::cout << kv.first << " has value " << kv.second << std::endl;
  }
  std::cout << std::endl << "printing completed" << std::endl;


  return 0;
};