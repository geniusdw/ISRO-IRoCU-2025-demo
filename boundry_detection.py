import cv2
import numpy as np

# Note: pymavlink logic is mocked here to keep the script self-contained for the demo.
class MockMavlink:
    def send_evasive_velocity(self, vx, vy):
        print(f"[MAVLink Overide] Evasive maneuver: Vx={vx}, Vy={vy}")

def main():
    print("Starting Yellow Boundary Detector...")
    fc = MockMavlink()
    
    # Yellow HSV range
    lower_yellow = np.array([20, 100, 100])
    upper_yellow = np.array([30, 255, 255])
    
    cap = cv2.VideoCapture(1) 
    
    while True:
        ret, frame = cap.read()
        if not ret: break
            
        hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
        mask = cv2.inRange(hsv, lower_yellow, upper_yellow)
        contours, _ = cv2.findContours(mask, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
        
        if contours:
            c = max(contours, key=cv2.contourArea)
            if cv2.contourArea(c) > 1000: # Ignore small noise
                M = cv2.moments(c)
                if M["m00"] != 0:
                    cx = int(M["m10"] / M["m00"])
                    center = frame.shape[1] // 2
                    
                    if cx > center + 50:
                        fc.send_evasive_velocity(0, -1.0) # Push Left
                    elif cx < center - 50:
                        fc.send_evasive_velocity(0, 1.0)  # Push Right
                        
        cv2.imshow("Boundary Mask", mask)
        if cv2.waitKey(1) & 0xFF == ord('q'): 
            break
            
    cap.release()
    cv2.destroyAllWindows()

if __name__ == "__main__":
    main()
