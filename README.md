## Overview
In this project, a robot was developed to traverse an arena that contains walls, obstacle blocks, images attached to the obstacle blocks, the start zone, and the goal zone. The aim of the robot is to explore the arena, capture images, and race from the start to the goal zone through the fastest path. To develop such robot, hardware components such as an Arduino Uno, a Raspberry Pi, a 6-volt battery, power banks, DC motors with wheels, Infrared (IR) sensors, and a Raspberry Pi camera, is required.

In this page, we will only be introducing the foundation hardware components of the robot which are:
1. Arduino Uno
2. 6-Volt Battery
3. DC motors with Encoders
4. Infrared (IR) Sensors (Long and short range)


## Arduino Uno
The Arduino Uno was used as the microcontroller board in our robot, along with the Pololu Dual VHN5019 Motor Driver Shield mounted on top of it to drive the DC motors for its mobility (See Figure 2).

![alt text](README%20Images/0J5212.300.jpg=250x250 "Arduino UNO")

Figure 2: Motor Driver Shield mounted on the Arduino Uno

The aim of the Arduino Uno is to provide the basic movements of the robot by controlling the DC motor. These movements includes moving forward, backward, and angle rotations of the robot. The program code offers predetermined values to move between each grid (each grid is 10cm), as well as 90 and 180 degree turns. Our code also includes functionalities such as taking in infrared (IR) sensor readings, obstacle detection and calibrations to align the robot’s position to provide accuracy and ensure that it stays on its original path at all times. These functionalities will be utilized as and when the main algorithm of the robot, deems it is useful.


## 6-Volt Battery
A SLA 6-Volt TLA633 battery (see Figure 3) is mounted at the back of the robot. It supplies power to the power regulator board that is mounted on top of the Motor Driver Shield. The power regulator board then distributes enough power for the rest of the components such as DC motors, IR sensors and the Arduino Uno to operate.

![alt text](README%20Images/6v-33ah-sealed-lead-acid-battery.jpg "6V Battery")
Figure 3: An SLA 6-volt TLA633 battery


## DC Motors
![alt text](README%20Images/DC_Motor_with_Encoder.jpg "DC Motor with Encoder")
Figure 4: DC Motor

To give the robot the ability to move, two DC motors were used. However, even though both motors are of the same model, their speed performances may slightly be off, which causes the robot to swerve to the side. In order to have the motors run at the same speed and ensure that it travels in a straight line, a closed-loop control system was implemented.


### Encoders
![alt text](README%20Images/Encoder.jpg "Encoder")
Figure 4.1: Encoder

Attached to the motors, are the encoders. They help to measure the pulse width and also in calculating the number of degrees the wheels have rotated. This allows us to keep track of how far the robot have moved, the number of revolutions it needs to turn either to the left or the right and the steps it needs to take for each grid. This would be used as the feedback for the close-loop control system.


### Data Findings of DC Motors
The first step was to compute the number of revolutions per minute (RPM). Using the encoders attached on the motors that measures the pulse width, the following formula was used to compute in Arduino with interrupts:

> RPM = 60000/(1.1245*Pulse Width)

Next, an RPM vs Speed graph was plotted to estimate the speed needed to achieved the desired RPM and observe if any of motors is under performing. This was done by plotting at different speeds ranging from 0 to 400 at an increment of 50.