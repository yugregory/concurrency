#include <iostream>
#include <future>
#include <thread>
#include <vector>

#include "basic_lock_free_stack.h"
#include "lock_free_stack.h"
#include "hazard_pointer_lock_free_stack.h"

hazard_pointer_lock_free_stack<int> test;

void add_to_queue(int, int);
void basic_tests();

int main() { 
    basic_tests();
    return 0;
}

void add_to_queue(int begin, int end) {
    for (int i = begin; i < end; i++)
        test.push(i);
}

void basic_tests() {
    std::vector<std::thread> joiner = {};
    for (int i = 0; i < 1; i++) {
        std::thread t1(add_to_queue, 0, 100);
        std::thread t2(add_to_queue, 100, 200);
        std::thread t3(add_to_queue, 200, 300);
        std::thread t4(add_to_queue, 300, 400);
        joiner.push_back(std::move(t1));
        joiner.push_back(std::move(t2));
        joiner.push_back(std::move(t3));
        joiner.push_back(std::move(t4));
    }
    
    for (int i = 0; i < joiner.size(); i++) {
        if (joiner[i].joinable())
            joiner[i].join();
    }
    std::vector<int> completion(400, 1);
    
    for (int i = 0; i < 1000000; i++) {
        auto it = test.pop(); 
        if (it == nullptr)
            break;
        std::cout << *it << std::endl;
        completion[*it]--;
    }

    for (int i = 0; i < completion.size(); i++) {
        if (completion[i] != 0) {
            std::cout << "this is failing basic cases" << std::endl;
            return;
        }
    }
    std::cout << "this is passing basic cases" << std::endl;
    return;
}
