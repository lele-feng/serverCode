#include <iostream>

bool judeg_by_mod(int num)
{
 return num % 2 != 0;
}

bool judeg_by_bit_op(int num)
{
 return num & 1 == 1;
}

int main(int argc, char* argv[])
{
  std::cout<<judeg_by_mod(3)<<std::endl;
  std::cout<<judeg_by_bit_op(3)<<std::endl;
  return 0;
}
