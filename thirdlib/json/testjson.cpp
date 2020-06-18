#include <iostream>
#include <string>
#include "json.hpp"

using json_t = nlohmann::json;

int main()
{
json_t j;                                   // JSON对象

j["age"] = 23;                              // "age":23
j["name"] = "spiderman";                    // "name":"spiderman"
j["gear"]["suits"] = "2099";                // "gear":{"suits":"2099"}
j["jobs"] = {"superhero"};                  // "jobs":["superhero"]  

std::vector<int> v = {1,2,3};                   // vector容器
j["numbers"] = v;                          // "numbers":[1,2,3]

std::map<std::string, int> m =                       // map容器
    {{"one",1}, {"two", 2}};               // 初始化列表 
j["kv"] = m;                               // "kv":{"one":1,"two":2}

std::cout << j.dump() << std::endl;



std::string str = R"({
    "name": "peter",
    "age" : 23,
    "married" : true
})";

std::cout<<str<<std::endl;
try
{
    auto j2 = json_t::parse(str);    // 从字符串反序列化
}
catch(std::exception& e) // 捕获异常
{   std::cout << e.what() << std::endl;}

//assert(j2["age"] == 23);        // 验证序列化是否正确
//assert(j2["name"] == "peter");

    return 0;
}
