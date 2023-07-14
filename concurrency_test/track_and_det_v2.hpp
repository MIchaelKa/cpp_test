//
//  track_and_det_v2.hpp
//  concurrency_test
//
//  Created by Калинин Михаил Андреевич on 05.06.2023.
//

#ifndef track_and_det_v2_hpp
#define track_and_det_v2_hpp

#include <queue>
#include <mutex>

class TrackAndDetV2 {
public:
    
    void read(int);
    
private:
    
    // detection
    
    void detection_thread_func(int);
    
    std::mutex detection_mutex_;
    bool detection_started_ = false;
    
    // tracking
    
    void start_tracking(int);
    void tracking_thread_func();
    void run_tracking(int);
    
    std::queue<int> readed_frames_;
    std::mutex readed_frames_mutex_;
    bool tracking_started_ = false;
    
    uint64_t last_detected_frame_ = 0;
    uint64_t max_frame_to_track_ = 0;
    
    // ocr
    
    void start_ocr(int);
    void ocr_thread_func();
    void run_ocr(int);
    
    std::queue<int> ocr_frames_;
    std::mutex ocr_frames_mutex_;
    bool ocr_started_ = false;
};

#endif /* track_and_det_v2_hpp */
