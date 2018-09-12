#include <iostream>

#include "lock_free_stack.h"

using namespace std;

int main() {
    lock_free_stack<int> test;
    test.push(10);
    cout << *test.pop() << endl;
    return 1;
}
