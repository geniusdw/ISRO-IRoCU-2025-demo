#include "header.h"
#include <iostream>
#include <thread>
#include <atomic>
#include <chrono>

// Dummy MAVLink controller for the demo
class MAVLinkController {
public:
    void connect(const std::string& port) {
        std::cout << "[MAVLink] Connected to Flight Controller on " << port << std::endl;
    }
    
    void sendLandingCommand(const Point3D& target) {
        std::cout << "[MAVLink] MAV_CMD_NAV_LAND dispatched to Local NED X:" 
                  << target.x << " Y:" << target.y << std::endl;
    }
};

int main() {
    std::cout << "--- ISRO IRoCU Demo: GPS/Compass Denied Navigation ---" << std::endl;

    SharedState shared_state;
    std::atomic<bool> system_running{true};
    
    MAVLinkController fc;
    fc.connect("/dev/ttyACM0");

    // Initialize OOP Subsystems
    Localization loc(shared_state, system_running);
    LidarSensor lidar(shared_state, system_running);
    TerrainMapper mapper(shared_state, system_running);

    // Spawn 3 worker threads
    std::thread t_loc(&Localization::run, &loc);
    std::thread t_lidar(&LidarSensor::run, &lidar);
    std::thread t_map(&TerrainMapper::run, &mapper);

    // Main thread (4th thread) orchestrates landing
    while (system_running) {
        Point3D safe_target;
        if (shared_state.getSafeLandingZone(safe_target)) {
            std::cout << "[Main Thread] Safe Landing Zone acquired from Mapper." << std::endl;
            fc.sendLandingCommand(safe_target);
            
            // Demo completed, trigger shutdown
            system_running = false; 
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    // Clean up
    t_loc.join();
    t_lidar.join();
    t_map.join();

    std::cout << "System Shutdown Complete." << std::endl;
    return 0;
}
