/*!==========================================================================
 * \file
 *   Program:       databus-common
 *   File:          thread-pool
 *   Created:       11/05/2019
 *   Author:        Vitaly Bulganin
 *   Description:
 *   Comments:
 *
-----------------------------------------------------------------------------
 *
 * History:
 *
===========================================================================*/
#pragma once
//-------------------------------------------------------------------------//
#ifndef __THREAD_POOL_H_81153F4F_7B03_4056_8FCE_B05B1F0B4629__
#define __THREAD_POOL_H_81153F4F_7B03_4056_8FCE_B05B1F0B4629__
//-------------------------------------------------------------------------//
#include <stdexcept>
#include <thread>
#include <vector>
#include <atomic>
#include <mutex>
#include <queue>
#include <future>
#include <functional>
#include <condition_variable>
//-------------------------------------------------------------------------//
namespace databus
{
    namespace grpc
    {
        namespace thread
        {
//-------------------------------------------------------------------------//
            class thread_pool final
            {
                using job_t = std::function<void()>;
                using jobs_t = std::queue<job_t>;
                using mutex_guard_t = std::unique_lock<std::mutex>;
                //!<
                struct sync_queue {
                    //!< Keeps a mutex.
                    std::mutex lock;
                    //!< Keeps a condition.
                    std::condition_variable cv;
                };
                //!< Keeps a size of pool.
                const size_t maxsize;
                //!< Keeps a context of threads.
                std::atomic_bool stopped;
                //!< Keeps a list of jobs.
                jobs_t jobs;
                //!< Keeps a list of threads.
                std::vector<std::thread> threads;
                //!< Keeps a queue sync.
                sync_queue sync_jobs;

            public:
                /**
                 * Constructor.
                 * @Note A size of pool will be equaled to a size of core.
                 */
                thread_pool() : thread_pool(0)
                {
                }

                /**
                 * Constructor.
                 * @param size [in] - A size of pool.
                 */
                explicit thread_pool(size_t size) : maxsize((size > 0) ? size : std::thread::hardware_concurrency()), stopped(false)
                {
                    for (size_t i = 0; i < this->maxsize; ++i)
                    {
                        this->threads.emplace_back([&]() {

                            while (this->stopped != true)
                            {
                                mutex_guard_t sync(this->sync_jobs.lock);

                                this->sync_jobs.cv.wait(sync, [&]() -> bool {
                                    return (this->stopped != true && this->jobs.empty() != true);
                                });
                                if (this->stopped != false) { return; }

                                try
                                {
                                    // Getting the first task and execute.
                                    auto task = this->dequeue();
                                }
                                catch (const std::exception & exc)
                                {
                                }
                            }
                        });
                    }
                }

                /**
                 * Destructor.
                 * @throw None.
                 */
                ~thread_pool() noexcept
                {
                    // Stopping the pool.
                    this->stopped = true;
                    // Notifying the pool about stopping.
                    this->sync_jobs.cv.notify_all();
                    // Waiting child threads.
                    for (auto & thread : this->threads) {
                        if (thread.joinable() != false) { thread.join(); }
                    }
                }

                /**
                 * Adds a request to processing.
                 * @param dispatch [in] - A request.
                 */
                template<typename F, typename... Args>
                auto enqueue(F && f, Args &&... args) -> std::future<typename std::result_of<F(Args...)>::type>
                {
                    using return_type = typename std::result_of<F(Args...)>::type;

                    auto task = std::make_shared< std::packaged_task<return_type()>>(std::bind(std::forward<F>(f), std::forward<Args>(args)...));
                    std::future<return_type> res = task->get_future();
                    {
                        // don't allow enqueueing after stopping the pool
                        if (this->stopped != false) { return {}; }

                        std::unique_lock<std::mutex> sync(this->sync_jobs.lock);
                        this->jobs.emplace([task](){ (*task)(); });
                    }
                    this->sync_jobs.cv.notify_one();

                    return res;
                }

                //!< Stops handling.
                auto stop() -> void { this->stopped = true; }
                //!< Gets a count of tasks in the queue.
                auto size() const noexcept -> jobs_t::size_type { return this->jobs.size(); }

            protected:
                //!< Builds a list of threads.
                auto dequeue() -> job_t
                {
                    // Getting the first element.
                    auto task = std::move(this->jobs.front());
                    // Removing the first one.
                    this->jobs.pop();
                    return std::move(task);
                }
            };
//-------------------------------------------------------------------------//
        }; // namespace thread
    }; // namespace grpc
}; // namespace databus
//-------------------------------------------------------------------------//
#endif // __THREAD_POOL_H_81153F4F_7B03_4056_8FCE_B05B1F0B4629__
