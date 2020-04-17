<p align="center">
  <img src="README%20Images/WhatsApp Image 2020-04-08 at 13.35.02.jpeg" width="300" title="Our robot">
</p>

## 1. Overview
In this project, a robot was developed to traverse an arena that contains walls, obstacle blocks, images attached to the obstacle blocks, the start zone, and the goal zone. The aim of the robot is to explore the arena, capture images, and race from the start to the goal zone through the fastest path. To develop such robot, hardware components such as an Arduino Uno, a Raspberry Pi, a 6-volt battery, power banks, DC motors with wheels, Infrared (IR) sensors, and a Raspberry Pi camera, is required.

In this page, we will only be introducing the foundation hardware components of the robot which are:
1. Arduino Uno
2. 6-Volt Battery
3. DC motors with Encoders
4. Infrared (IR) Sensors (Long and short range)


## 2. Arduino Uno
The Arduino Uno was used as the microcontroller board in our robot, along with the Pololu Dual VHN5019 Motor Driver Shield mounted on top of it to drive the DC motors for its mobility (See Figure 2).

<p align="center">
  <img src="README%20Images/0J5212.300.jpg" width="300" title="Arduino UNO">
</p>

<div align="center"><i>Figure 2: Motor Driver Shield mounted on the Arduino Uno</i></div>

The aim of the Arduino Uno is to provide the basic movements of the robot by controlling the DC motor. These movements includes moving forward, backward, and angle rotations of the robot. The program code offers predetermined values to move between each grid (each grid is 10cm), as well as 90 and 180 degree turns. Our code also includes functionalities such as taking in infrared (IR) sensor readings, obstacle detection and calibrations to align the robot’s position to provide accuracy and ensure that it stays on its original path at all times. These functionalities will be utilized as and when the main algorithm of the robot, deems it is useful.


## 3. 6-Volt Battery
A SLA 6-Volt TLA633 battery (see Figure 3) is mounted at the back of the robot. It supplies power to the power regulator board that is mounted on top of the Motor Driver Shield. The power regulator board then distributes enough power for the rest of the components such as DC motors, IR sensors and the Arduino Uno to operate.

<p align="center">
  <img src="README%20Images/6v-33ah-sealed-lead-acid-battery.jpg" width="300" title="6V Battery">
</p>

<div align="center"><i>Figure 3: An SLA 6-volt TLA633 battery</i></div>


## 4. DC Motors
<p align="center">
  <img src="README%20Images/DC_Motor_with_Encoder.jpg" width="300" title="DC Motor with Encoder">
</p>

<div align="center"><i>Figure 4: DC Motor</i></div>

To give the robot the ability to move, two DC motors were used. However, even though both motors are of the same model, their speed performances may slightly be off, which causes the robot to swerve to the side. In order to have the motors run at the same speed and ensure that it travels in a straight line, a closed-loop control system was implemented.


### 4.1. Encoders
<p align="center">
  <img src="README%20Images/Encoder.jpg" width="300" title="Encoder">
</p>

<div align="center"><i>Figure 4.1: Encoder</i></div>

Attached to the motors, are the encoders. They help to measure the pulse width and also in calculating the number of degrees the wheels have rotated. This allows us to keep track of how far the robot have moved, the number of revolutions it needs to turn either to the left or the right and the steps it needs to take for each grid. This would be used as the feedback for the close-loop control system.


### 4.2. Data Findings of DC Motors
The first step was to compute the number of revolutions per minute (RPM). Using the encoders attached on the motors that measures the pulse width, the following formula was used to compute in Arduino with interrupts:

> RPM = 60000/(1.1245*Pulse Width)

Next, an RPM vs Speed graph was plotted to estimate the speed needed to achieved the desired RPM and observe if any of motors is under performing. This was done by plotting at different speeds ranging from 0 to 400 at an increment of 50.

<p align="center">
  <img src="README%20Images/LeftMotorRPM.xlsx-Excel.png" width="425" title="Left Motor RPM">
  <img src="README%20Images/RightMotorRPM.xlsx-Excel.png" width="425" title="Right Motor RPM">
</p>

<div align="center"><i>Figure 4.2a: RPM vs Speed Graph</i></div>

To analyze the behavior of how each motors performed, a motor step test was plotted. This time, the graph was recorded by setting the speed at 300 for the first 5ms, followed by a sudden drop in speed at 250 for another 5ms.

<p align="center">
  <img src="README%20Images/LeftMotorStepTest.xlsx-Excel.png" width="425" title="Left Motor Step Test">
  <img src="README%20Images/RightMotorStepTest.xlsx-Excel.png" width="425" title="Right Motor Step Test">
</p>

<div align="center"><i>Figure 4.2b: Motor Step Test Graph</i></div>

It can be observed that the RPM values for the right motor fluctuates a lot more compared to the left motor. As expected, this causes the robot to swerve to the right as its RPM is slightly slower compared to the left motor at any given speed.


### 4.3. Closed-Loop Control
After gathering the data needed, a Proportional-Integral-Derivative(PID) Controller was used for our closed-loop control system to reduce the error. In this case, the PID controller was used on the right motor, with the left motor chosen as its set point since it has a more stable RPM at any given speed. This way, the PID controller would aim to increase the the right motor’s RPM to catch up with the left motor’s to ensure it rotates at the same desired speed.

<p align="center">
  <img src="README%20Images/PID-Controller-Block-Diagram.pptx.png" title="PID Controller Block Diagram">
</p>

<div align="center"><i>Figure 4.3: PID Controller Block Diagram</i></div>

After some tests with trials and errors, it was found that the value of the proportional response, Kp, is to be at 20. However, although the error is now zero and the robot moves straight, it tends to deviate to the right at the start due to the right motor needing some time to catch up to the left motor thus, building accumulated constant error overtime.

To counter this problem, we added the integral response, Ki which helps to remove constant errors in the control system. This would continually sum the error up, no matter how small it is which will eventually be significantly enough to adjust the controller output. Eventually the value of Ki, was found to be at 80, further reducing the steady-state error and finally allowing our robot to move in a straight line motion.


## 5. Infrared (IR) Sensors
As the robot traverse around the arena, sensors are required for the robot to detect obstacle blocks and walls. Therefore, our robot used 5 Sharp GP2Y0A21YK short-range IR sensors and 1 Sharp GP2Y0A02YK long-range sensor (see Figure 5) to traverse around the arena.

<p align="center">
  <img src="README%20Images/IR_Sensors.jpg" width="300" title="IR Sensors">
</p>

<div align="center"><i>Figure 5: A short-range IR sensor (left) and a long-range IR sensor (right)</i></div>


### 5.1. Infrared (IR) Sensors Calibration
When reading raw values from the IR sensors, the analog output voltage will be returned. The closer the obstacle is from the IR sensor, the higher the analog output voltage (see Figure 5.1).

<p align="center">
  <img src="README%20Images/babQg.png" width="300" title="IR graph distance vs voltage">
</p>

<div align="center"><i>Figure 5.1: Graph of the distance of object vs the analog output voltage</i></div>

However, having an analog output voltage is not very useful. We have to convert it into a distance in centimeters from the obstacle to the robot. Below are the formulas used to the short-range sensor and the long-range sensor:

**Short-range sensor**
> Distance = ((67870.0 / (AnalogOutputVoltage- 3.0)) - 40.0)/10

**Long-range sensor**
> Distance = 65*pow(AnalogOutputVoltage*0.0048828125, -1.10)

Below are the steps to calibrate the IR sensors:

1. Firstly, the IR sensors are to be mounted on the robot to get the analog output voltage. This will take consideration of the offset of the IR sensor from the edge of the robot.
2. Next, place the edge of the robot at the start of the measuring tape.
3. Place the obstacle away from the robot.
4. Record the distance of the obstacle away from the robot and the distance value obtained.
5. Offset the distance value to the right value.

Despite obtaining the distance during sensor reading, noises may occur, causing inaccurate readings. To solve this issue, sampling was implemented. 11 sample readings were taken, placed in an array, sorted in ascending order and the median value was taken. To increased the accurate more, we did another 16 samplings of the median values that were obtained. Therefore, the sampling is 11 by 16 which is the optimal sampling size to allow us to have a high accuracy of readings and not sacrificing the speed of computation.


### 5.2. Robot obstacle detection in the arena
As our robot is to be placed in the center of a 3 by 3 spacing, IR sensors have to be placed in different positions of the robot and facing different directions. This allows the robot to detect obstacles and not having any blind spots. Due to the algorithm of the exploration of the map, our robot will be doing the left wall hug. Therefore, it affected the decision of the placement of the IR sensors. More IR sensors are placed facing the front and the left of the robot.

After each movement or when requested by the main algorithm that is hosted in a Personal Computer, values of 1 to 8 will be sent over the communication. Value 1 to 7 represents the number of grids the obstacle is away from the robot. Value 8 means no obstacle is detected within the effective range. When there is an obstacle out of the effective range, the value readings are very unstable. Thus, only an effective range will be used. Therefore, short-range sensors will give a value of 1, 2, or 8 while the long-range sensor will give a value between 1 to 8.

Below shows the IR sensors placement and the sensor readings to be returned to the main algorithm:

<p align="center">
  <img src="README%20Images/Untitled Diagram.png" title="Sensor reading to Algorithm">
</p>

<div align="center"><i>Figure 5.2: Placement of robot and possible sensor readings</i></div>


### 5.3. Position Calibration of the Robot
As the robot traverse around the arena, it may begin to misalign. The robot may not face the correct angle or at the wrong position. Therefore, calibration is required to align the robot. This calibration functionality will be utilized by the main algorithm in the Personal Computer after the robot traversed every 3 grids during arena exploration.

When the calibration command is issued from the main algorithm from the Personal Computer, the robot will first calibrate the angle of the robot. To calibrate the angle of the robot, the robot will use the front right and the left short-range sensors to compare the distance difference. If the difference in distance is larger than the allowable threshold (we used 0.15cm), angle calibration will occur. As we used the distance received from the left sensor minus the distance received from the right sensor, if the difference in distance is positive, it means the robot is facing the left more (see Figure 5.3a). The robot has to turn right to face perpendicular to the front. Else, the robot has to turn left till it faces perpendicular to the front.

<p align="center">
  <img src="README%20Images/calibrationAngle.png" width="200" title="Calibrate angle of the robot">
</p>

<div align="center"><i>Figure 5.3a: Angle Calibration of the Robot</i></div>

In a situation when the robot faces a corner when it is misaligned, utilizing only the left and right short-range sensors will not work. The robot will not know it is facing a corner and calibrating using the left and right short-range sensors result in it to alight to the corner in a straight line. Therefore, the middle short-range sensor will be utilized to solve this issue. If the distance received from the middle sensor is longer than the right and left sensors, it means the robot is facing a corner (see Figure 5.3b). The robot will keep turning 10 degrees left until it is no longer facing a corner if there is no left wall. If there is a left wall, it may be facing a corner of the front and left wall (see right of Figure 5.3b). Turning 10 degrees left will mess up the position. Therefore, it will turn left 90 degrees to calibrate before turning back to the front to calibrate.

<p align="center">
  <img src="README%20Images/CalibrateCorner.png" width="425" title="Calibrate corner of the robot">
</p>

<div align="center"><i>Figure 5.3b: Angle Calibration when facing a corner</i></div>

Despite calibrating the angle of the robot it is facing, calibration of the position of the robot is required. Using the front right and left short-range sensors, the robot will move forward or backward if the distance from the wall to the robot is too big or small respectively till it is within a threshold (see Figure 5.3c).

<p align="center">
  <img src="README%20Images/CalibrateDist.png" width="200" title="Calibrate distance of the robot">
</p>

<div align="center"><i>Figure 5.3c: Distance Calibration of the robot from the wall</i></div>

The algorithm of the robot:

1. Check if the robot if is facing a corner. If true, calibrate away from the corner.
2. Calibrate the angle of the robot against the wall or obstacle blocks.
3. Calibrate the distance of the robot against the wall or obstacle blocks.
4. Calibrate the angle of the robot again.
5. If there is a left wall, turn left and repeat from step 2 before turning right to face the front again.


## Extra Notes to Juniors
1. Every hardware is different. The offsets for sensors and PID values cannot be reused from this code. Copy and pasting the code won't work. You need to start from scratch and follow the instructions from the lab manual for PID. Altneratively, most people will do trial and error for PID values which is faster.
2. Robot weight matters. Adding of Raspberry Pi and Powerbank will affect the robot's turning and moving straight. Add powerbank from the start so you won't have to waste time recalibrating again during integration with all components.
3. Watch out your battery's voltage. Get a multimeter. Always measure the voltage. Find the ideal voltage. Calibrate to that ideal voltage and do a run only at that ideal voltage. Ideally, only 2 runs are possible before the voltage drops even more and the robot's movement and readings get affected.
4. If your robot skids, it means the point of contact of the wheels to the ground is not strong enough. Either apply weight or other ways to force the wheels to have more contact to the ground. If it has strong contact to the ground, even dusty/slippery area of the arena won't cause your robot to skid.
5. Make sure your Algorithm team members are very smart. You will see a difference during week 8 which team doesn't have smart people in their Algorithm team. Observe their robot's decision especially when phantom block occurs. You will know. Thankfully we had very smart Algorithm teammates to allow us to have good placing in the leadership board.
6. Good luck and have fun! 

## About
AY19/20 Semester 2

Arduino parter: https://github.com/kevinsashimi

