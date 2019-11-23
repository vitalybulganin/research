/*!==========================================================================
* \file
* - Program:       databus-grpc
* - File:          message-reader.h
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
#ifndef __MESSAGE_READER_H_DA80014E_4C39_4F24_AE62_6D79E16EF6BE__
#define __MESSAGE_READER_H_DA80014E_4C39_4F24_AE62_6D79E16EF6BE__
//-------------------------------------------------------------------------//
#include <thread>
//-------------------------------------------------------------------------//
#include "../../thread-context.h"
//-------------------------------------------------------------------------//
namespace databus
{
    namespace grpc
    {
        namespace thread
        {
//-------------------------------------------------------------------------//
            class message_reader final
            {
                //!< Keeps a context of thread.
                thread::thread_context & context;
                //!< Keeps max pool size.
                const size_t maxsize = 10;
                //!< Keeps a child thread.
                std::thread thread;

            public:
                /**
                 * Constructor.
                 * @param context [in, out] - A context of thread.
                 * @param pool_size [in] - Max size of pool.
                 * @param logger [in] - A logger.
                 */
                message_reader(thread_context & context, const size_t & pool_size);

                /**
                 * Destructor.
                 * @throw None.
                 */
                ~message_reader() noexcept = default;

                /**
                 * Waits while the thread finish.
                 */
                auto join() -> void;

            public:
                //!< Executes a thread.
                auto run() -> void;
            };
//-------------------------------------------------------------------------//
        }; // namespace thread
    }; // namespace grpc
}; // namespace databus
//-------------------------------------------------------------------------//
#endif // __MESSAGE_READER_H_DA80014E_4C39_4F24_AE62_6D79E16EF6BE__
