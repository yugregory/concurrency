#ifndef LOCK_FREE_STACK_H
#define LOCK_FREE_STACK_H

#include <iostream>
#include <atomic>
#include <memory>

template<typename T>
class lock_free_stack {
    private:
        struct node {
            std::shared_ptr<T> data;
            std::shared_ptr<node> next;

            node(T const & data_):data(std::make_shared<T>(data_))
            {}
        };

        std::shared_ptr<node> head;
    public:
        void push(T const & data) {
            std::shared_ptr<node> const new_node = std::make_shared<node>(data);
            new_node->next = std::atomic_load_explicit(&head, std::memory_order_relaxed);
            // std::cout << "is shared pointer lock_free? " << std::atomic_is_lock_free(&head) << std::endl;
            while (!std::atomic_compare_exchange_weak_explicit(&head, &new_node->next, new_node,
                                                            std::memory_order_relaxed, std::memory_order_relaxed));
        }
        
        std::shared_ptr<T> pop() {
            std::shared_ptr<node> old_head = std::atomic_load_explicit(&head, std::memory_order_relaxed);
            while (old_head && 
                    !std::atomic_compare_exchange_weak_explicit(&head, &old_head, old_head->next,
                                                                std::memory_order_relaxed, std::memory_order_relaxed));
            return old_head ? old_head->data : std::shared_ptr<T>();
        }
};

#endif
