//
//  main.cpp
//  concurrency_test
//
//  Created by Калинин Михаил Андреевич on 02.06.2023.
//

#include <iostream>
#include <thread>

using namespace std::this_thread; // sleep_for, sleep_until
using namespace std::chrono; // nanoseconds, system_clock, seconds

//
// Launch
//

void test_1() {
    for (int i = 0; i < 10; i++) {
        std::cout << "test_1: " << i << std::endl;
        sleep_for(milliseconds(100));
    }
}

void test_thread_launch() {
    std::thread t(test_1);
    t.detach();
    
    for (int i = 0; i < 2; i++) {
        std::cout << "main: " << i << std::endl;
        sleep_for(milliseconds(100));
    }
}

//
// Pass params
//

struct TestStruct {
    int counter;
};

struct TestStructWithMemory {
    int* array;
};

void test_2(int& i) {
    sleep_for(milliseconds(100));
    i = 10;
}

void test_3(const TestStruct& s) {
    sleep_for(milliseconds(100));
//    s.counter = 10;
}

void test_4(const TestStructWithMemory& s) {
    sleep_for(milliseconds(100));
    std::cout << "test_4 array[3]: " << s.array[3] << std::endl;
}

void test_pass_args_to_thread() {
    int i = 5;
    TestStruct s = {.counter = 5};
    
    std::thread t(test_2, std::ref(i));
    t.join();
    
    std::cout << "i: " << i << std::endl;
    std::cout << "s.counter: " << s.counter << std::endl;
}

void test_pass_args_to_thread_2() {
    {
        int i = 5;
        int array[5] = {1,2,3,4,5}; // как понять он скопировался или нет?
        TestStructWithMemory s2 = { .array = array };
        
//        std::thread t(test_4, s2); //std::ref(s2)
        std::thread t(test_2, std::ref(i));
        t.detach();
    }
    std::cout << "1" << std::endl;
    sleep_for(milliseconds(120));
    std::cout << "2" << std::endl;
    
}

//
// Track and det
//

#include "track_and_det.hpp"
#include "track_and_det_v2.hpp"

void test_track_and_det() {
    
//    TrackAndDet detector;
    TrackAndDetV2 detector;
    
    auto start_0 = std::chrono::steady_clock::now();
    
    for (int i = 0; i < 30; i++) {
        detector.read(i);
        sleep_for(milliseconds(30));
    }
    
    auto end_0 = std::chrono::steady_clock::now();
    std::chrono::duration<double, std::milli> elapsed_time_0 = end_0 - start_0;
    std::cout << "time 0: " << elapsed_time_0.count() << std::endl;
    
    sleep_for(milliseconds(1000));
}

//
// Atomic
//

std::atomic<bool> ready_;
int data_ = 0;

void foo() {
    data_ = 42;
    ready_.store(true, std::memory_order_release);
}

void bar() {
    if (ready_.load(std::memory_order_acquire)) {
        assert(data_ == 42);
    }
}

void setup_atomic_test_1() {
    //create two threads and run foo() and bar()
}


int main(int argc, const char * argv[]) {
    
    test_track_and_det();
    
    return 0;
}
