#include "header.h"
#include <opencv2/video/tracking.hpp>
#include <thread>
#include <chrono>

Localization::Localization(SharedState& s, std::atomic<bool>& r) : state(s), running(r) {}

void Localization::run() {
    cv::VideoCapture cap(0);
    cv::Mat old_gray, frame, frame_gray;
    std::vector<cv::Point2f> p0, p1;

    // Initialize optical flow tracker
    if (cap.read(frame)) {
        cv::cvtColor(frame, old_gray, cv::COLOR_BGR2GRAY);
        cv::goodFeaturesToTrack(old_gray, p0, 100, 0.3, 7, cv::Mat(), 7, false, 0.04);
    }

    while (running) {
        if (!cap.read(frame)) break;
        cv::cvtColor(frame, frame_gray, cv::COLOR_BGR2GRAY);

        std::vector<uchar> status;
        std::vector<float> err;
        
        if (!p0.empty()) {
            cv::calcOpticalFlowPyrLK(old_gray, frame_gray, p0, p1, status, err);
            
            // Demo: Translate optical flow into physical displacement
            Pose p = state.getPose();
            p.x += 0.02f; 
            p.y += 0.01f;
            state.updatePose(p);
        }

        old_gray = frame_gray.clone();
        p0 = p1; 
        std::this_thread::sleep_for(std::chrono::milliseconds(30)); // 30 FPS
    }
}
