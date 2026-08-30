# ISRO IRoCU 2025 Demo: GPS and Compass-Denied Navigation System

This repository contains a structural demonstration of an autonomous drone navigation and landing system designed for Martian terrain (GPS and compass-denied environments). 

The architecture is split into a high-performance, multithreaded C++ core for localization and mapping, and a standalone Python node for reactive boundary avoidance.

## System Workflow

The system is designed around a multi-threaded Object-Oriented architecture using shared memory state. 

### C++ Core Engine (4 Threads)
1. **Localization Thread:** Utilizes OpenCV and the Lucas-Kanade optical flow method on a downward-facing camera to estimate the drone's velocity and update its pose in real-time (Visual Odometry).
2. **LiDAR Thread:** Simulates the ingestion of 3D point cloud data from a sweeping LiDAR and servo mechanism.
3. **Mapping Thread:** Consumes the LiDAR point cloud, translates it into a 2.5D elevation grid, and analyzes terrain variance to detect flat, safe landing zones.
4. **Main Thread (Orchestrator):** Manages the subsystem threads, monitors the shared state, and handles dispatching MAVLink landing commands to the flight controller once a safe zone is identified.

### Python Side-Node
* **Boundary Detector (`boundary_detection.py`):** Runs asynchronously to the main C++ engine. It processes a secondary camera feed using HSV masking to detect yellow arena boundaries. If a boundary is approached, it directly overrides the flight controller via MAVLink to execute an evasive maneuver.

## Requirements

* **C++ Compiler:** C++14 or higher with `pthread` support.
* **OpenCV:** C++ and Python bindings (`cv2`).
* **Python 3.x**
* **NumPy**
