#include <thread>
#include <iostream>
#include <exception>
#include <chrono>

void fun()
{
  std::cout<<std::this_thread::get_id()<<"  in fun"<<std::endl;
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  throw 3;
}

int main(int argc, char* argv[])
{
  //std::thread a = std::thread(fun);
  //std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  try{
    std::thread a = std::thread(fun);
    a.join();
  }
  catch(...)
  {
    std::cout<<std::this_thread::get_id()<<"  in main got exception"<<std::endl;
  }
  std::cout<<std::this_thread::get_id()<<"  in main"<<std::endl;
  return 0;
}
