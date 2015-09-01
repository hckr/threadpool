#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <functional>
#include <deque>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <exception>
#include <memory>

/**
 * Gives the functionality of a thread pool.
 * Fixed number of threads perform tasks added to the queue.
 *
 * @author Jakub Młokosiewicz https://github.com/hckr
 */
class ThreadPool
{
    /**
     * Queue containing tasks to be processed.
     */
    std::deque<std::function<void()>> m_tasks;

    /**
     * Vector containing threads in thread pool.
     */
    std::vector<std::thread> m_threads;

    /**
     * True if shutdown() was called and thread pool is shutting down.
     */
    bool m_shutdown;

    /**
     * True if all previously added tasks will be processed before shutdown.
     */
    bool m_finish_queue;

    /**
     * Mutex ensuring that shared state will be modified safely.
     */
    std::mutex m_mutex;

    /**
     * Object providing the ability to wait on the lock until certain condition occurs.
     */
    std::condition_variable m_barrier;


    /**
     * Function executed by each function in thread pool
     */
    void thread_handler();

public:

    /**
     * Creates new ThreadPool.
     *
     * @param number_of_threads How many threads should be in a thread pool.
     */
    ThreadPool(size_t number_of_threads);

    /**
     * Adds new task to the queue.
     *
     * @param task Task to be added.
     * @param args Arguments for the task.
     *
     * @return std::future object containing
     */
    template<typename F, typename ...Args>
    auto enqueue(F&& task, Args&&... args) -> std::future<typename std::result_of<F(Args...)>::type>
    {
        using return_type = typename std::result_of<F(Args...)>::type;
        std::future<return_type> future_result;

        if(m_shutdown)
        {
            throw std::logic_error("ThreadPool is shutting down - cannot add new task.");
        }
        else
        {
            {
                // scoped lock
                std::unique_lock<std::mutex>(m_mutex);

                auto packaged_task = std::make_shared<std::packaged_task<return_type()>>(
                    std::bind(std::forward<F>(task), std::forward<Args>(args)...)
                );
                future_result = packaged_task->get_future();

                m_tasks.push_back([packaged_task]{
                    (*packaged_task)();
                });
            }

            m_barrier.notify_one(); // notify one thread waiting for the condition to change
        }

        return future_result;
    }

    /**
     * Shuts down the thread pool. Removes all tasks from the queue (see below).
     *
     * @param finish_queue Whether to complete all the tasks added to the queue or not.
     */
    void shutdown(bool finish_queue);

    /**
     * Destructor waits until all tasks added to the queue will finish.
     * If you want to prevent this, call shutdown() previously.
     */
    ~ThreadPool();
};

#endif // THREADPOOL_H
