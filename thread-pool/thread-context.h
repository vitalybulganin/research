/*!==========================================================================
 * \file
 *   Program:       databus-common
 *   File:          thread-context.h
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
#ifndef __THREAD_CONTEXT_H_F99E92B7_DBAE_4C06_8222_2AFB54673429__
#define __THREAD_CONTEXT_H_F99E92B7_DBAE_4C06_8222_2AFB54673429__
//-------------------------------------------------------------------------//
#include <stdexcept>
#include <thread>
#include <atomic>
//-------------------------------------------------------------------------//
namespace databus
{
    namespace grpc
    {
        namespace thread
        {
//-------------------------------------------------------------------------//
            class thread_context final
            {
                //!< Supported state of thread.
                enum class state_t {started = 1, stopped = 2};
                //!< Keeps a state of thread.
                volatile std::atomic<state_t> state;

            public:
                /**
                 * Constructor.
                 */
                thread_context() : state(state_t::started) {}

                /**
                 * Destructor.
                 * @throw None.
                 */
                ~thread_context() noexcept = default;

                //!< Sets STOP state.
                auto stop() -> void { this->state = state_t::stopped; }

                /**
                 * Gets a state of context.
                 * @return true, if thread should be stopped.
                 * @throw None.
                 */
                auto stopped() const noexcept -> bool { return this->state == state_t::stopped; }
            };
//-------------------------------------------------------------------------//
        }; // namespace thread
    }; // namespace grpc
}; // namespace databus
//-------------------------------------------------------------------------//
#endif // __THREAD_CONTEXT_H_F99E92B7_DBAE_4C06_8222_2AFB54673429__
