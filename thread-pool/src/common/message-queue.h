/*!==========================================================================
* \file
* - Program:       databus-grpc
* - File:          message-queue.h
* - Created:       11/02/2019
* - Author:        Vitaly Bulganin
* - Description:
* - Comments:
*
-----------------------------------------------------------------------------
*
* - History:
*
===========================================================================*/
#pragma once
//-------------------------------------------------------------------------//
#ifndef __MESSAGE_QUEUE_H_BD7183AC_D71C_43AD_B167_EC21BEE2B350__
#define __MESSAGE_QUEUE_H_BD7183AC_D71C_43AD_B167_EC21BEE2B350__
//-------------------------------------------------------------------------//
#include <deque>
#include <mutex>
#include <stdexcept>
#include <iostream>
//-------------------------------------------------------------------------//
namespace databus
{
    namespace grpc
    {
        namespace common
        {
//-------------------------------------------------------------------------//
            class queue_factory;
//-------------------------------------------------------------------------//
            template<typename T>
            class message_queue final
            {
                friend class queue_factory;

                using mutex_guard_t = std::unique_lock<std::mutex>;
                //!< Keeps a queue.
                mutable std::deque<T> queue;
                //!< Keeps a mutex.
                mutable std::mutex lock;

            public:
                message_queue(const message_queue &) = delete;
                message_queue(const message_queue &&) = delete;
                auto operator=(const message_queue &) -> message_queue & = delete;

            public:
                /**
                 * Destructor.
                 * @throw None.
                 */
                ~message_queue() noexcept = default;

                /**
                 * Puts an object into queue.
                 * @param object [in] - An object.
                 */
                auto enqueue(const T & object) -> void
                {
                    mutex_guard_t sync(this->lock);
                    // Adding an object into queue.
                    this->queue.push_back(object);
                }

                /**
                 * Gets the first object from queue with removing from it.
                 * @return The object.
                 * @throw std::out_of_range - No data found.
                 */
                auto dequeue() const -> T
                {
                    T object;

                    if (this->queue.empty() != true)
                    {
                        // Getting the first element.
                        object = std::move(this->queue.front());
                        // Removing the first element from the queue.
                        this->queue.pop_front();
                    }
                    return std::move(object);
                }

                /**
                 * Gets a count of objects in the queue.
                 * @return A count of objects.
                 * @throw None.
                 */
                auto size() const noexcept -> size_t
                {
                    mutex_guard_t sync(this->lock);

                    return this->queue.size();
                }

                /**
                 * Checks the queue on empty.
                 * @return true, if the queue is empty, otherwise false.
                 * @throw None.
                 */
                auto empty() const noexcept -> bool
                {
                    mutex_guard_t sync(this->lock);

                    return this->queue.empty();
                }

            private:
                /**
                 * Constructor.
                 */
                message_queue() = default;
            };
//-------------------------------------------------------------------------//
            using message_queue_t = message_queue<void *>;
//-------------------------------------------------------------------------//
        }; // namespace common
    }; // namespace grpc
}; // namespace databus
//-------------------------------------------------------------------------//
#endif // __MESSAGE_QUEUE_H_BD7183AC_D71C_43AD_B167_EC21BEE2B350__
