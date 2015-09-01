ThreadPool
==========

Create new thread pool
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

    tp.enqueue(greet);

... or anything else which can be abstracted with `std::function<void()>`.


Shutting down
-------------

Thread pool can be shut down in two ways.

Finish all tasks added to the queue:

    tp.shutdown(true);

Finish only currently running tasks:

    tp.shutdown(false);

After this function is called:

- queue is empty,
- object should not be used (exceptions will be thrown).