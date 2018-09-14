#include <iostream>
#include <future>
#include <thread>
#include <vector>

#include "basic_lock_free_stack.h"
#include "lock_free_stack.h"

lock_free_stack<int> test;

void add_to_queue() {
    for (int i = 0; i < 10000; i++)
        test.push(i);
}
void add_to_queue_different() {
    for (int i = 200; i < 30000; i++)
        test.push(i);
}

int main() { 
    /*
    test.push(10);
    test.push(2);
    auto i = *test.pop();
    auto j = *test.pop();
    cout << i << endl;
    cout << j << endl;
    */
    std::vector<std::thread> joiner = {};
    for (int i = 0; i < 4; i++) {
        std::thread t1(add_to_queue);
        std::thread t2(add_to_queue_different);
        joiner.push_back(std::move(t1));
        joiner.push_back(std::move(t2));
    }

    for (int i = 0; i < joiner.size(); i++) {
        if (joiner[i].joinable())
            joiner[i].join();
    }
    

    for (int i = 0; i < 100000; i++) {
        if (test.pop() == nullptr)
            return 1;
        std::cout << *test.pop() << std::endl;
    }
    return 0;
}
