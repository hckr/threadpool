ThreadPool
==========

Create a new thread pool
----------------------

    ThreadPool tp(10); // new thread pool with 10 threads

Thread pool starts immediately.


Add tasks to the queue
----------------------

Tasks can be lambdas:

    tp.enqueue([]{
        std::cout << "Hello from thread. :)\n";
    });

... or plain old functions:

    void greet()
    {
        std::cout << "Hello from thread. :)\n";
    }

    ...

    tp.enqueue(greet);

... or any other callable (function object).


Pass arguments
----------------

    void greet(std::string name)
    {
        std::cout << "Hello from thread, " + name + ". :)\n";
    }

    ...

    tp.enqueue(greet, "Jessica");
    tp.enqueue(greet, "John");

    ...

    void add(int first, int second)
    {
        std::cout << first + second;
    }

    ...

    tp.enqueue(add, 13, 37);


Retrieve the results
--------------------

`enque` returns `std::future` which can be used to retrieve the result of the task.

    std::future<int> future_result = tp.enqueue([]{
        return 42;
    });

    ...

    if(future_result.valid())
    {
        std::cout << future_result.get();
    }

Combined with example from previous section:

    int add(int first, int second)
    {
        return first + second;
    }

    auto future_result = tp.enqueue(add, 19, 23);

    ...

    if(future_result.valid())
    {
        std::cout << future_result.get();
    }


Shut down (optional)
--------------------

By default thread pool is shut down automatically during destruction.
In that case all tasks previously added to the queue are processed first.

Thread pool can be also shut down manually in two ways:

- finishing all tasks added to the queue:

        tp.shutdown(true);


- finishing only currently running tasks:

        tp.shutdown(false);


After this function is called:

- queue is empty,
- threads from thread pool are joined to the main thread,
- object should not be used (exceptions will be thrown).
