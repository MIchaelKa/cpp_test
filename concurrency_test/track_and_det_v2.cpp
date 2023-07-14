//
//  track_and_det_v2.cpp
//  concurrency_test
//
//  Created by Калинин Михаил Андреевич on 05.06.2023.
//

#include "track_and_det_v2.hpp"

#include <iostream>
#include <thread>

using namespace std::this_thread; // sleep_for, sleep_until
using namespace std::chrono; // nanoseconds, system_clock, seconds

void TrackAndDetV2::read(int frame) {
    
    std::cout << "[read] frame: " << frame << std::endl;
    
    std::unique_lock<std::mutex> detection_lock(detection_mutex_);
    
    if (!detection_started_) {
        std::cout << "[read] run_detetion on: " << frame << std::endl;
        detection_started_ = true;
        detection_lock.unlock();
        
        std::thread t(&TrackAndDetV2::detection_thread_func, this, frame);
        t.detach();
    } else {
        detection_lock.unlock();
        std::unique_lock<std::mutex> lock(readed_frames_mutex_);
        std::cout << "[read] put frame to readed_frames_: " << frame << std::endl;
        readed_frames_.push(frame);
    }
}

void TrackAndDetV2::detection_thread_func(int frame) {
    std::cout << "[detect] start frame: " << frame << std::endl;
    sleep_for(milliseconds(140));
    std::cout << "[detect] end frame: " << frame << std::endl;
    
    std::unique_lock<std::mutex> detection_lock(detection_mutex_);
    detection_started_ = false;
    detection_lock.unlock();
    
    start_ocr(frame);
    start_tracking(frame);
    
    std::cout << "[detect] finish thread: " << frame << std::endl;
}

void TrackAndDetV2::start_tracking(int frame) {

    std::unique_lock<std::mutex> lock(readed_frames_mutex_);
    
    std::cout << "[tracking] tracking_started_: " << tracking_started_ << ", queue.size: " << readed_frames_.size() << std::endl;
    
    // TODO:
    // is it better to calculate it under detection_mutex_?
    // then to use std::lock for two mutexes: detection_mutex_ and readed_frames_mutex_
    last_detected_frame_ = max_frame_to_track_;
    max_frame_to_track_ = readed_frames_.back() + 1; // next frame will be passed to detector
    
    std::cout << "[tracking] last_detected_frame_: " << last_detected_frame_;
    std::cout << ", update max_frame_to_track_ to: " << max_frame_to_track_ << std::endl;
    
    if (!readed_frames_.empty()) {
        while (!readed_frames_.empty() && readed_frames_.front() < last_detected_frame_) {
            std::cout << "[tracking] pop: " << readed_frames_.front() << std::endl;
            readed_frames_.pop();
        }
        std::cout << "[tracking] readed_frames_.front: " << readed_frames_.front() << std::endl;
    }
    
    // tracker->start(frame);
    
    if (!tracking_started_) {
        tracking_started_ = true;
        lock.unlock();
        
        std::thread t(&TrackAndDetV2::tracking_thread_func, this);
        t.detach();
    }
}

void TrackAndDetV2::tracking_thread_func() {
    std::cout << "[tracking] initialize thread" << std::endl;
    
    std::unique_lock<std::mutex> lock(readed_frames_mutex_);
    
    while(!readed_frames_.empty() && readed_frames_.front() < max_frame_to_track_) {
        int frame = readed_frames_.front();
        readed_frames_.pop();
        lock.unlock();
        run_tracking(frame);
        lock.lock();
    }
    
    std::cout << "[tracking] finish thread" << std::endl;
    tracking_started_ = false;
}

void TrackAndDetV2::run_tracking(int frame) {
    std::cout << "[tracking] start frame: " << frame << std::endl;
    sleep_for(milliseconds(10));
    std::cout << "[tracking] end frame: " << frame << std::endl;
            
    start_ocr(frame);
    
    // TODO: skip if frame < last_detected_frame_ ?
    // Probably we need to track less frames if we cannot to track them all
    // std::unique_lock<std::mutex> lock(readed_frames_mutex_);
}

void TrackAndDetV2::start_ocr(int frame) {
    std::cout << "[ocr] try to start: " << frame << std::endl;
    
    std::unique_lock<std::mutex> lock(ocr_frames_mutex_);
    
    std::cout << "[ocr] push frame to ocr_frames_: " << frame << std::endl;
    ocr_frames_.push(frame);
    
    if (!ocr_started_) {
        std::cout << "[ocr] start thread: " << frame << std::endl;
        ocr_started_ = true;
        lock.unlock();
        
        std::thread t(&TrackAndDetV2::ocr_thread_func, this);
        t.detach();
    }
    
    std::cout << "[ocr] end of start" << std::endl;
}

void TrackAndDetV2::ocr_thread_func() {
    std::cout << "[ocr] initialize thread" << std::endl;
    
    std::unique_lock<std::mutex> lock(readed_frames_mutex_);
    
    while(!ocr_frames_.empty()) {
        int frame = ocr_frames_.front();
        ocr_frames_.pop();
        lock.unlock();
        run_ocr(frame);
        lock.lock();
    }
    
    std::cout << "[ocr] finish thread" << std::endl;
    ocr_started_ = false;
    
}

void TrackAndDetV2::run_ocr(int frame) {
    std::cout << "[ocr] start frame: " << frame << std::endl;
    sleep_for(milliseconds(15));
    std::cout << "[ocr] end frame: " << frame << std::endl;
}
