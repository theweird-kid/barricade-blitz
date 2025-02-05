#ifndef NET_THREAD_SAFE_QUEUE_HPP
#define NET_THREAD_SAFE_QUEUE_HPP

#include "net_common.hpp"

namespace wkd
{
    namespace net
    {
        template <typename T>
        class thread_safe_queue
        {
        public:
            thread_safe_queue() = default;
            thread_safe_queue(const thread_safe_queue<T>&) = delete;
            virtual ~thread_safe_queue() { clear(); }
        public:
            // Returns and maintains item at the front of queue
            const T& front()
            {
                std::scoped_lock lock(muxQueue);
                return m_Deque.front();
            }

            // Returns and maintains item at the back of queue
            const T& back()
            {
                std::scoped_lock lock(muxQueue);
                return m_Deque.back();
            }

            // removes and returns the item at the front
            T pop_front()
            {
                std::scoped_lock lock(muxQueue);
                auto t = std::move(m_Deque.front());
                m_Deque.pop_front();
                return t;
            }

            // removes and returns the item at the back
            T pop_back()
            {
                std::scoped_lock lock(muxQueue);
                auto t = std::move(m_Deque.back());
                m_Deque.pop_back();
                return t;
            }

            // Adds an item to the back of the queue
            void push_back(const T& item)
            {
                // Push Element to the back of the queue
                std::scoped_lock lock(muxQueue);
                m_Deque.emplace_back(std::move(item));

                // Signal to wake up
                std::unique_lock<std::mutex> ul(muxBlocking);
                cvBlocking.notify_one();
            }

            // Adds an item to front of the queue
            void push_front(const T& item)
            {
                // Push Element to the front of the queue
                std::scoped_lock lock(muxQueue);
                m_Deque.emplace_front(std::move(item));

                // Signal to wake up
                std::unique_lock<std::mutex> ul(muxBlocking);
                cvBlocking.notify_one();
            }

            // Returns true if the queue has no items
            bool empty()
            {
                std::scoped_lock lock(muxQueue);
                return m_Deque.empty();
            }

            // Returns number of items in queue
            size_t count()
            {
                std::scoped_lock lock(muxQueue);
                return m_Deque.size();
            }

            // Clears queue
            void clear()
            {
                std::scoped_lock lock(muxQueue);
                m_Deque.clear();
            }

            void wait()
            {
                while(empty())
                {
                    std::unique_lock<std::mutex> ul(muxBlocking);
                    cvBlocking.wait(ul);
                }
            }

        protected:
            std::mutex muxQueue;
            std::deque<T> m_Deque;

            std::condition_variable cvBlocking;
            std::mutex muxBlocking;
        };
    }
}

#endif // NET_THREAD_SAFE_QUEUE_HPP
