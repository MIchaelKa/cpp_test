//
//  track_and_det.cpp
//  concurrency_test
//
//  Created by Калинин Михаил Андреевич on 05.06.2023.
//

#include "track_and_det.hpp"

#include <iostream>
#include <thread>

using namespace std::this_thread; // sleep_for, sleep_until
using namespace std::chrono; // nanoseconds, system_clock, seconds


void TrackAndDet::read(int frame) {
    
    std::cout << "[read] frame: " << frame << std::endl;
    
    if (!start_tracking_started_) {
        std::thread t(&TrackAndDet::start_tracking_thread_func, this);
        t.detach();
    }
    
    if (!detection_started_) {
        std::cout << "[read] run_detetion on: " << frame << std::endl;
        std::thread t(&TrackAndDet::run_detetion, this, frame);
        t.detach();
    } else {
        std::unique_lock<std::mutex> lock(readed_frames_mutex_);
        std::cout << "[read] put frame in readed_frames_: " << frame << std::endl;
        readed_frames_.push(frame);
        readed_frames_cond_.notify_all();
    }
}


void TrackAndDet::run_detetion(int frame) {
    detection_started_ = true;
    
    max_frame_to_track_ = frame;
    readed_frames_cond_.notify_all();
    
    std::cout << "[detect] start frame: " << frame << std::endl;
    sleep_for(milliseconds(140));
    std::cout << "[detect] end frame: " << frame << std::endl;
    
    detection_started_ = false;

    std::unique_lock<std::mutex> detected_frames_lock(detected_frames_mutex_);
    detected_frames_.push(frame);
    std::cout << "[detect] put frame in detected_frames_: " << frame << std::endl;
    detected_frames_cond_.notify_all();
    detected_frames_lock.unlock();
    
    std::unique_lock<std::mutex> readed_frames_lock(readed_frames_mutex_);
    int last_readed_frame = readed_frames_.back();
    max_frame_to_track_ = last_readed_frame;
    std::cout << "[detect] update max_frame_to_track_ to: " << last_readed_frame << std::endl;
    readed_frames_cond_.notify_all();
}

void TrackAndDet::start_tracking_thread_func() {
    std::cout << "[start tracking] initialize thread_func" << std::endl;
    start_tracking_started_ = true;
    
    while(true) {
        std::cout << "[start tracking] check" << std::endl;
        
        std::unique_lock<std::mutex> lock(detected_frames_mutex_);
        detected_frames_cond_.wait(lock, [this] {
            std::cout << "[start tracking] check detected_frames_" << std::endl;
            return !detected_frames_.empty();
        });
        
        int frame = detected_frames_.front();
        detected_frames_.pop(); // TODO: how to save it for OCR?
        
        lock.unlock();
        
        //start tracking
        std::cout << "[start tracking] started for frame: " << frame << std::endl;
        if (!tracking_started_) {
            std::thread t(&TrackAndDet::tracking_thread_func, this);
            t.detach();
        }
    }
}

void TrackAndDet::tracking_thread_func() {
    std::cout << "[tracking] initialize thread" << std::endl;
    tracking_started_ = true;
    
    while(true) {
        std::cout << "[tracking] check" << std::endl;
        
        std::unique_lock<std::mutex> lock(readed_frames_mutex_);
        readed_frames_cond_.wait(lock, [this] {
            std::cout << "[tracking] check readed_frames_" << std::endl;
            return !readed_frames_.empty() && readed_frames_.front() < max_frame_to_track_;
        });
        
        int frame = readed_frames_.front();
        readed_frames_.pop();
        lock.unlock();
        run_tracking(frame);
    }
}

void TrackAndDet::run_tracking(int frame) {
    std::cout << "[track] start frame: " << frame << std::endl;
    sleep_for(milliseconds(5));
    std::cout << "[track] end frame: " << frame << std::endl;
}

void TrackAndDet::run_ocr(int frame) {
    std::cout << "start ocr frame: " << frame << std::endl;
    sleep_for(milliseconds(10));
    std::cout << "end ocr frame: " << frame << std::endl;
}
