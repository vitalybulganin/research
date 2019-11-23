#include "message-reader.h"
//-------------------------------------------------------------------------//
#include <functional>
//-------------------------------------------------------------------------//
#include "../common/queue-factory.h"
//-------------------------------------------------------------------------//
#include "../../thread-pool.h"
//-------------------------------------------------------------------------//
namespace databus
{
    namespace grpc
    {
        namespace thread
        {
//-------------------------------------------------------------------------//
            namespace
            {
                auto onthread(message_reader & reader) -> void
                {
                    reader.run();
                }
            }; // namespace
//-------------------------------------------------------------------------//
            message_reader::message_reader(thread::thread_context & contxt, const size_t & pool_size)
                : context(contxt), maxsize(pool_size), thread(onthread, std::ref(*this))
            {
            }

            auto message_reader::join() -> void
            {
                if (this->thread.joinable() != false) { this->thread.join(); }
            }
//-------------------------------------------------------------------------//
            auto message_reader::run() -> void
            {
                // Creating a proceed thread pool.
                thread::thread_pool pool(this->maxsize);
                // Getting a reference on message queue.
                auto & queue = common::queue_factory::getInstance().get();

                while (this->context.stopped() != true)
                {
                    if (queue.empty() != true)
                    {
                        // Reading a message from queue.
                        pool.enqueue([&queue]() -> void {
                            // Getting a message from the queue.
                            auto request = queue.dequeue();
                            // Executing the request.
                            //<???> reinterpret_cast<api::dispatchable *>(request)->dispatch();
                        });
                    }
                    std::this_thread::sleep_for(std::chrono::milliseconds(50));
                }
            }
//-------------------------------------------------------------------------//
        }; // namespace thread
    }; // namespace mmt
}; // namespace search
