//-----------------------------------------------------------------------------
#include <iostream> //std::cout
//-----------------------------------------------------------------------------
#include "work_obj.h"
//-----------------------------------------------------------------------------


int main(int, const char* [])
{

  TWorkObj workObj;


  try {


    int nResult = workObj.task1();
    if(nResult) {
      std::cout << "Error in task1(), code: " << nResult << std::endl;
      return nResult;
    }

  }
  catch(const std::exception& exc)
  {
    std::cout << "Exception caught. Info: " << exc.what() << std::endl;
  }

  return 0;
}
//-----------------------------------------------------------------------------
