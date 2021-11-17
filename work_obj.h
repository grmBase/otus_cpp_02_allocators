//---------------------------------------------------------------------------
#ifndef __work_obj_h__
#define __work_obj_h__
//---------------------------------------------------------------------------
#include <string>
//---------------------------------------------------------------------------

class TWorkObj
{
  public:

    int task1();


    template <typename _Tx>
    void process(_Tx& aMap)
    {
      for(size_t i = 0; i < 10; ++i) {
        aMap[i] = factorial(i);
      }

      std::cout << std::endl << "---------------------------" << std::endl;
      std::cout << "Before print of map. Size of items: " << aMap.size() << std::endl;
      for (const auto& kv : aMap) {
        std::cout << kv.first << " " << kv.second << std::endl;
      }
      std::cout << std::endl << "printing completed" << std::endl;

    };



    int task2();

};
//---------------------------------------------------------------------------

#endif