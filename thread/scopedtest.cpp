#define YAML_DECLARE_STATIC

#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include <mutex>



class A
{
public:
    A(int i) :m_i(i) { std::cout << "A() " << std::this_thread::get_id() << std::endl; }
    A(const A& a) :m_i(a.m_i) { std::cout << "A(const A&) " << std::this_thread::get_id() << std::endl; }
    ~A() { std::cout << "~A() " << std::this_thread::get_id() << std::endl; }
    void operator()() {
        std::cout << m_i << std::endl;
    }
private:
    int m_i;
};

class B
{
public:
    B(A a) :m_a(a) {
        std::cout << "b(A) " << std::this_thread::get_id() << std::endl;
    }
    B(const B& b) :m_a(b.m_a) {
        std::cout << "b(const B) " << std::this_thread::get_id() << std::endl;
    }
    ~B() {
        std::cout << "~b() " << std::this_thread::get_id() << std::endl;
    }
private:
    A m_a;
};

void test1(const A& a)
{
    std::cout << &a << std::endl;
}
void test2(A a)
{
    std::cout << &a << std::endl;
}

void test3(const B& a)
{
    std::cout << &a << std::endl;
}

void test4(B a)
{
    std::cout << &a << std::endl;
}


struct Employee {
    Employee(std::string id) : id(id) {}
    std::string id;
    std::vector<std::string> lunch_partners;
    std::mutex m;
    std::string output() const
    {
        std::string ret = "Employee " + id + " has lunch partners: ";
        for (const auto& partner : lunch_partners)
            ret += partner + " ";
        return ret;
    }
};

void testdeadLock(Employee& e1, Employee& e2)
{
    std::lock_guard<std::mutex> l1(e1.m);
    std::lock_guard<std::mutex> l2(e2.m);
    std::cout << "testdeadLock"<<std::this_thread::get_id() << std::endl;
}

void testdeadLock2(Employee& e1, Employee& e2)
{
    std::socped_mutex l(e1.m, e2.m);
    std::cout << "testdeadLock" << std::this_thread::get_id() << std::endl;
}

int main(int argc, char **argv) {
    int var = 1;
    /*std::thread t(test1, var);
    t.join();
    std::thread t2(test1, var);
    t2.join();*/

    std::cout << "----------"<< std::this_thread::get_id() << std::endl;
    //A a1(var);
    /*std::thread t3(test1, a1);
    t3.join();
    std::cout << "----------" << std::endl;
    std::thread t4(test2, a1);
    t4.join();*/



  /*  A a5(1);
    std::thread t5(a5);
    t5.join();*/

    /*std::thread t6(test3, a1);
    t6.join();*/

    Employee e1("a1");
    Employee e2("a2");
    std::thread t7(testdeadLock, std::ref(e1), std::ref(e2));
    std::thread t8(testdeadLock, std::ref(e2), std::ref(e1));
    t7.join();
    t8.join();

    return 0;

}
