#ifndef WORK_STEALING_QUEUE_H
#define WORK_STEALING_QUEUE_H

#include <deque>
#include <mutex>

#include "function_wrapper.h"

class work_stealing_queue {
    private:
        using function_wrapper data_type;
        std::deque<data_type> local_queue;
        mutable std::mutex mutex;
    public:
        work_stealing_queue()
        {}
        work_stealing_queue(const work_stealing_queue& other) = delete;
        work_stealing_queue& operator=(const work_stealing_queue& other) = delete;
        void push(data_type data) {
            std::lock_guard<std::mutex> lock(mutex);
            local_queue.push_front(std::move(data));
        }
        bool empty() const {
            std::lock_guard<std::mutex> lock(mutex);
            return local_queue.empty();
        }
        bool try_pop(data_type& res) {
            std::lock_guard<std::mutex> lock(mutex);
            if (local_queue.empty())
                return false;
            res = std::move(local_queue.front());
            local_queue.pop_front();
            return true;

        }
        bool try_steal(data_type& res) {
            std::lock_guard<std::mutex> lock(mutex);
            if (local_queue.empty())
                return false;
            res = std::move(local_queue.back());
            local_queue.pop_back();
            return true;
        }
};

#endif
