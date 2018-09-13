#include <iostream>

#include "basic_lock_free_stack.h"
#include "lock_free_stack.h"

using namespace std;

int main() {
    lock_free_stack<int> test;
    test.push(10);
    test.push(2);
    auto i = *test.pop();
    auto j = *test.pop();
    cout << i << endl;
    cout << j << endl;
    return 1;
}
