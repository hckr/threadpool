#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <functional>
#include <deque>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <exception>

/**
 * Gives the functionality of a thread pool.
 * Fixed number of threads perform tasks added to the queue.
 *
 * @author Jakub Młokosiewicz https://github.com/hckr
 */
class ThreadPool
{
    using task_type_t = std::function<void()>;

    /**
     * Queue containing tasks to be processed.
     */
    std::deque<task_type_t> m_tasks;

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
     */
    void enqueue(task_type_t task);

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
