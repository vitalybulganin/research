/*!==========================================================================
* \file
* - Program:       databus-grpc
* - File:          queue-factory.h
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
#ifndef __QUEUE_FACTORY_H_A70202BB_536D_480E_A6E3_ACD9A53D6FC1__
#define __QUEUE_FACTORY_H_A70202BB_536D_480E_A6E3_ACD9A53D6FC1__
//-------------------------------------------------------------------------//
#include "message-queue.h"
//-------------------------------------------------------------------------//
namespace databus
{
    namespace grpc
    {
        namespace common
        {
//-------------------------------------------------------------------------//
            class queue_factory final
            {
                //!< Keeps a message queue.
                message_queue_t queue;

            public:
                queue_factory(const queue_factory &) = delete;
                queue_factory(const queue_factory &&) = delete;
                auto operator=(const queue_factory &) -> queue_factory & = delete;

            public:
                /**
                 * Gets the instance of factory.
                 * @return The instance.
                 */
                static auto getInstance() -> queue_factory &;

            public:
                /**
                 * Gets the queue instance.
                 * @return The instance of queue.
                 * @throw None.
                 */
                auto get() noexcept -> message_queue_t & { return this->queue; }

            private:
                /**
                 * Constructor.
                 */
                queue_factory() = default;
            };
//-------------------------------------------------------------------------//
        }; // namespace common
    }; // namespace grpc
}; // namespace databus
//-------------------------------------------------------------------------//
#endif // __QUEUE_FACTORY_H_A70202BB_536D_480E_A6E3_ACD9A53D6FC1__
