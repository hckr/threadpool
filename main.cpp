#include <iostream>
#include "threadpool.h"

int main()
{
    ThreadPool tp(10);

    for(int i = 1; i < 21; ++i)
    {
        tp.enqueue([i]{
            std::cout << std::to_string(i) + "\n";
        });
    }
}
