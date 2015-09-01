#include "threadpool.h"

void ThreadPool::thread_handler()
{
    while(1)
    {
        std::function<void()> task;
        {
            // scoped lock
            std::unique_lock<std::mutex> lock(m_mutex);

            m_barrier.wait(lock, [this]{
                // mutex is guaranteed to be locked
                return (m_tasks.size() > 0) || m_shutdown;
            });
            // condition is true and mutex is guaranteed to be locked

            if(m_shutdown)
            {
                if(m_finish_queue)
                {
                    if(m_tasks.size() == 0)
                    {
                        return;
                    }
                    else
                    {
                        // otherwise continue
                    }
                }
                else
                {
                    return;
                }
            }

            task = m_tasks.front();
            m_tasks.pop_front();
        }

        task();
    }
}

ThreadPool::ThreadPool(size_t number_of_threads)
    : m_shutdown(false), m_finish_queue(false)
{
    for(size_t i = 0; i < number_of_threads; ++i)
    {
        m_threads.emplace_back(std::thread(thread_handler, this));
    }
}

void ThreadPool::shutdown(bool finish_queue)
{
    if(m_shutdown)
    {
        throw std::logic_error("shutdown() was already called once.");
    }
    else
    {
        {
            // scoped lock
            std::unique_lock<std::mutex>(m_mutex);
            if(finish_queue)
            {
                m_finish_queue = true;
            }
            m_shutdown = true;
        }

        m_barrier.notify_all(); // notify one thread waiting for the condition to change

        for(auto &thread : m_threads)
        {
            thread.join();
        }
        m_tasks.clear();
    }
}

ThreadPool::~ThreadPool()
{
    if(m_shutdown == false)
    {
        shutdown(true);
    }
}
