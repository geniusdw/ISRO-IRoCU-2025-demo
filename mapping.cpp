#include "header.h"
#include <thread>
#include <chrono>
#include <iostream>

TerrainMapper::TerrainMapper(SharedState& s, std::atomic<bool>& r) : state(s), running(r) {}

void TerrainMapper::run() {
    while (running) {
        std::vector<Point3D> points = state.consumeLidarPoints();
        Pose current_pose = state.getPose();

        if (!points.empty()) {
            analyzeLandingZones(points, current_pose);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(200)); // 5Hz Mapping loop
    }
}

void TerrainMapper::analyzeLandingZones(const std::vector<Point3D>& points, const Pose& pose) {
    // Demo: In real code, we insert points into a 2.5D Grid and check variance.
    // Here we simulate finding a flat area after processing points.
    
    Point3D safe_spot = {pose.x + 3.0f, pose.y + 1.0f, 0.0f}; 
    state.setSafeLandingZone(safe_spot);
    
    std::cout << "[Mapping Thread] 2.5D map updated. Flat terrain detected at X:" 
              << safe_spot.x << " Y:" << safe_spot.y << std::endl;
}
