#include <thread>
#include <iostream>
#include <chrono>

class CTimer{
 public:
     // 析构函数
     virtual ~CTimer(){
  
     }
     // 开始
     void start()
     {
         b_exit = false;
         i = 10;
         t = new std::thread(&CTimer::run, this);
     }
  
     void run()
     {
         std::this_thread::sleep_for(std::chrono::milliseconds(1000));
 //         i = 800;
         stop();
         return;
     }
  
     // 结束
     void stop()
     {
         int lll = i;
         b_exit = true;
         if (t->joinable())
         {
             t->join();
         }
     }
 private:
     std::thread *t;
     std::thread *t1;
     int i;
     bool b_exit;
 };

int main(int argc, char *argv[])
{
    CTimer time_test;
    time_test.start();
    time_test.stop();
  return 0;
}
