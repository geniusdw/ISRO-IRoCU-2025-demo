#include "header.h"
#include <thread>
#include <chrono>
#include <cstdlib>

LidarSensor::LidarSensor(SharedState& s, std::atomic<bool>& r) : state(s), running(r) {}

void LidarSensor::run() {
    while (running) {
        std::vector<Point3D> scan;
        
        // Mock Lidar Sweep Data Generation
        for(int i = 0; i < 50; i++) {
            scan.push_back({
                (float)(rand() % 100) / 10.0f, 
                (float)(rand() % 100) / 10.0f, 
                (float)(rand() % 20) / 10.0f
            });
        }
        
        state.addLidarPoints(scan);
        std::this_thread::sleep_for(std::chrono::milliseconds(100)); // 10Hz Sweep rate
    }
}
