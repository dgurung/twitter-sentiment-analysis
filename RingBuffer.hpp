#ifndef RINGBUFFER_HPP_
#define RINGBUFFER_HPP_

#include <boost/lockfree/spsc_queue.hpp> // ring buffer

#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/allocators/allocator.hpp>
#include <boost/interprocess/containers/string.hpp>

namespace shm {
    namespace bip = boost::interprocess;
    using char_alloc        = bip::allocator<char, bip::managed_shared_memory::segment_manager>;
    using shared_string     = bip::basic_string<char, std::char_traits<char>, char_alloc>;
    using spsc_ring_buffer  = boost::lockfree::spsc_queue<shared_string, boost::lockfree::capacity<256>>;
    
    struct shm_remove {
       shm_remove() {  bip::shared_memory_object::remove("MySharedMemory"); }
       ~shm_remove(){  bip::shared_memory_object::remove("MySharedMemory"); }
    };
} 

template<class T>
class Queue {
public:

    static T& instance(void)
    {
        static T queue;
        return queue;
    }

    Queue(Queue const&) = delete;             // Copy construct
    Queue(Queue&&) = delete;                  // Move construct
    Queue& operator=(Queue const&) = delete;  // Copy assign
    Queue& operator=(Queue &&) = delete;      // Move assign

private:
    Queue(void);
};


struct RingBuffer{
    shm::shm_remove                             remover;
    boost::interprocess::managed_shared_memory  segment;
    shm::char_alloc                             char_alloc;
    shm::spsc_ring_buffer                       *spsc_ring_buffer;                                
    RingBuffer()
        : remover(shm::shm_remove())
        // create segment and corresponding allocator
        , segment(boost::interprocess::managed_shared_memory(boost::interprocess::open_or_create, "MySharedMemory", 65536))
        , char_alloc(shm::char_alloc(segment.get_segment_manager()))
        // create ring buffer
        , spsc_ring_buffer(segment.find_or_construct<shm::spsc_ring_buffer>("queue")()) {
    }
};

#endif //