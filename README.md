# Fire Extinguishing Simulator
2025 Embedded System Team Project


### Target Device
CNDI eCUBE  
<img src="https://github.com/user-attachments/assets/0abd7804-e528-4b7a-9f1d-8f9858d05f10" width="600" height="400"/>
-  Ubuntu 14.0LTS
-  NXP i.MX6 ARM Quad-Core 1G HZ
-  LCD, FND, Buzzer, LED, Button, Gyro, Aceel, Magnet, Temp sensor, ColrLED
-  12V 3A DC power Requried

## breif 
- In modern society, technologies related to safety are becoming increasingly important, especially in terms of rapid response and simulation training during fire emergencies.  
- This project was inspired by recent wildfires. In the simulator, users must extinguish a fire that has broken out in the 3rd Engineering Building using a hose within a limited amount of time.

## Used sensor for each kit
|Sensor|kit1(simulator)|ki2(hose)|
|---|---|---|
|button|-|Init/Exit game,  Fire the water|
|gyro|-|For aim(cursor) to move|
|buzzer|Play BGM|Emits sound when the cursor moves or a shot is fired|
|led|Indicate Life |-|
|fnd|Indicate Score|-|
|textLCD|Displays the progress of the simulatrion|-|
|LCD|Displays background and fire using frame buffer|-|
## System Overview
- Device Communication (TCP socket)
  - Kit1 and Kit2 are connected via a TCP-based socket communication protocol.
- Gyroscope-Based Aiming Mechnism
  - In Kit2, Gyro sensor is used to detect physical movement, which is translated into cursor(aim) movement on Kit1's dispaly in real time
- Data Transmission
  - When the cursor position changes, the new coordinates ares immediately sent from Kit2 to Kit1,
  - Pressing the fire button on Kit2 triggers the transmission of a fire command to kit1
- Dynamic Fire Generation
  - Kit1 generates fire events at random positions every 7 seconds, simulating unpredictable emergency conditions
- Game Mechanics - Life and Score
  - If a fire is not extinguished within 7 seconds, the player loses one life, indicated by an LED turning off.
  - Successfully extinguishing the fire within the time limit awards the player a score based on the speed of the response
## How to play
1. Install required sensor modules using the shell script insmod.sh located in the utilScript directory.
2. Run the executable (ELF) file on each kit:
   - Kit1 should run as the simulator (server)
   - Kit2 should run as the hose controller (client)
3. On Kit2, put the IP address of Kit1 to establish socket communication between the two devices.
4. Hold Kit2 like a gun to simulate aiming and shooting.
5. Press the HOME button on Kit2 to start the simulation.
6. The aim (blue dot) on Kit1's screen updates in real-time based on Kit2’s movement.
7. Press the VOL_UP button on Kit2 to exit the simulation.

## Flow Chart
needs to be updated
## Demo 
