#include <thread>
#include <iostream>
#include <exception>
#include <chrono>

void fun()
{
  std::cout<<std::this_thread::get_id()<<"  in fun"<<std::endl;
  throw 3;
}

int main(int argc, char* argv[])
{
  std::thread a = std::thread(fun);
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  a.join();
  std::cout<<std::this_thread::get_id()<<"  in main"<<std::endl;
  return 0;
}

