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
  <img src="README%20Images/RightMotorRPM.xlsx-Excel.png" width="425" title="Right Motor Step Test">
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