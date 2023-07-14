//
//  track_and_det.hpp
//  concurrency_test
//
//  Created by Калинин Михаил Андреевич on 05.06.2023.
//

#ifndef track_and_det_hpp
#define track_and_det_hpp

#include <queue>
#include <mutex>

class TrackAndDet {
public:
    
//    TrackAndDet();
    
    void read(int);
    
private:
    
    void run_detetion(int);
    
    void start_tracking_thread_func();
    void tracking_thread_func();
    void run_tracking(int);
    
    void run_ocr(int);
    
    std::queue<int> readed_frames_;
    std::mutex readed_frames_mutex_;
    std::condition_variable readed_frames_cond_;
    
    std::queue<int> detected_frames_;
    std::mutex detected_frames_mutex_;
    std::condition_variable detected_frames_cond_;
    
    std::atomic<bool> detection_started_ = false;
    std::atomic<bool> tracking_started_ = false;
    std::atomic<bool> start_tracking_started_ = false;
    
    std::atomic<uint64_t> max_frame_to_track_ = 0;
};


#endif /* track_and_det_hpp */
