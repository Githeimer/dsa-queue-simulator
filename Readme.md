# DSA Queue Simulator 🚦

This project is a **Traffic Light Simulation** using **SDL3**.  
It simulates vehicles **queuing** and **dequeueing** based on traffic light changes.

### Features:

- **Traffic Light System** (changes every few seconds)
- **Vehicle Queue** (randomly generated & dequeued on green light)
- **Intersection with 3-Lane Roads**
- **Basic Road & Zebra Crossing Rendering**

🚗💨 Built with **C** and **SDL3**.

The Generator works properly:
![alt text](./images/Desktop2025.02.22-13.51.38.02-ezgif.com-video-to-gif-converter.gif)

The Data is Successfully parsed in simulator side
![alt text](./images/Desktop2025.02.22-19.39.36.05-ezgif.com-video-to-gif-converter.gif)

Simulation:
It works upto the switching of lanes, dequeue implementation and lights remaining:
![alt text](./images/ezgif.com-video-to-gif-converter.gif)

## Description

### **Vehicle Generator:**

The vehicle generator continuously runs and generates vehicle data. It assigns a vehicle a unique ID, entry lane, exit lane, direction, and movement state. The data for each vehicle is written into a file (e.g., `AL2.txt`).

**Entry lanes**: L1 (incoming lanes), L2 (outgoing lanes with traffic control).

**Exit lanes**: L3 (outgoing lanes).

**Direction**: North (N), East (E), South (S), or West (W).

**State**:

- `0`: Queued
- `1`: Moving
- `2`: Passed

The generator simulates the behavior of vehicles by randomly selecting entry and exit lanes, speed, and direction, and writes the data to a file.

### **Traffic Simulator:**

The traffic simulator reads the vehicle data from the file(s) generated by the generator and simulates the movement of the vehicles through the lanes. It processes the vehicles, moving them from one lane to another according to predefined traffic rules.

#### Lane Movement Rules:

- **L3 lanes** always go to their left lanes:

  - AL1 → BL3
  - BL3 → CL1
  - CL1 → DL3
  - DL3 → AL1

- **L2 lanes** can either go straight or turn right:
  - AL2 → CL1 or DL1
  - BL2 → AL1 or DL1
  - CL2 → AL2 or BL1

The vehicles are processed from the queue in the order they appear and moved through the system. Their states are updated from `queued` to `moving`, and once they pass, their state changes to `passed`.

## Data Exchange Between Generator and Simulator

- The **generator** writes the vehicle data to a file, which is continually updated as new vehicles are generated.
- The **simulator** reads the file and processes the vehicle data line by line.
- The **vehicle data format** written to the file is as follows:
