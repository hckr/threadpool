#include <iostream>
#include "threadpool.h"

void greet(std::string name)
{
    std::cout << "Hello from thread, " + name + ". :)\n";
}

int add(int first, int second)
{
    return first + second;
}

int main()
{
    ThreadPool tp(10);

    auto future_result_1 = tp.enqueue([]{
        return 2+2;
    });

    auto future_result_2 = tp.enqueue([]{
        return 42;
    });

    auto future_result_3 = tp.enqueue(add, 19, 23);

    tp.enqueue(greet, "Jessica");
    tp.enqueue(greet, "John");

    for(int i = 1; i < 21; ++i)
    {
        tp.enqueue([i]{
            std::cout << ("From for loop: " + std::to_string(i) + "\n");
        });
    }

    if(future_result_1.valid())
    {
        std::cout << ("2+2 is " + std::to_string(future_result_1.get()) + "\n");
    }

    if(future_result_2.valid())
    {
        std::cout << ("The answer is: " + std::to_string(future_result_2.get()) + "\n");
    }

    if(future_result_3.valid())
    {
        std::cout << ("19 + 23 is " + std::to_string(future_result_3.get()) + "\n");
    }
}
