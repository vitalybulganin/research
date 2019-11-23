#include "queue-factory.h"
//-------------------------------------------------------------------------//
#include <mutex>
//-------------------------------------------------------------------------//
namespace databus
{
    namespace grpc
    {
        namespace common
        {
//-------------------------------------------------------------------------//
            auto queue_factory::getInstance() -> queue_factory &
            {
                static queue_factory * s_instance = nullptr;
                static std::mutex s_lock;

                if (s_instance == nullptr)
                {
                    std::unique_lock<std::mutex> sync(s_lock);

                    if (s_instance == nullptr) { s_instance = new queue_factory(); }
                }
                return *s_instance;
	        }
//-------------------------------------------------------------------------//
        }; // namespace common
    }; // namespace grpc
}; // namespace databus
