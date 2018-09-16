#ifndef HAZARD_POINTER_LOCK_FREE_STACK_H
#define HAZARD_POINTER_LOCK_FREE_STACK_H

#include <atomic>
#include <memory>

#include "hp_owner.h"

template<typename T>
class hazard_pointer_lock_free_stack {
    private:
        struct node {
            std::shared_ptr<T> data;
            node * next;
            node(T const & data_):data(std::make_shared<T>(data_))
            {}
        };
        std::atomic<node*> head;
    public:
        void push(T const & data) {
            node * const new_node = new node(data);
            new_node->next = head;
            while (!head.compare_exchange_weak(new_node->next, new_node,
                                                std::memory_order_release, std::memory_order_relaxed));
        }

        std::shared_ptr<T> pop() {
            std::atomic<void*>& hp = get_hazard_pointer_for_current_thread();
            node * old_head = head.load();
            do {
                node * temp;
                do {
                    // set the hazard pointer and continue only once old_head == temp
                    temp = old_head;
                    hp.store(old_head);
                    old_head = head.load();
                } while (old_head != temp);
            } while (old_head && 
                    !head.compare_exchange_weak(old_head, old_head->next,
                                                std::memory_order_release, std::memory_order_relaxed));
            hp.store(nullptr);
            std::shared_ptr<T> res;
            if (old_head) {
                res.swap(old_head->data); 
                if (outstanding_hazard_pointers_for(old_head))
                    reclaim_later(old_head);
                else 
                    delete old_head;
                reclaim_nodes_no_hazard_list();
            }
            return res;
        }
};

#endif
