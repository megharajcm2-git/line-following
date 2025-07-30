🤖 Line Following Robot using Arduino
This project is a simple yet effective Line Following Robot that uses IR sensors to detect and follow a black line on a white surface. It uses DC motors controlled via the Adafruit Motor Shield, making it a great beginner-level robotics project.

📌 Features
🔍 IR sensor-based black line detection

🔁 Autonomous movement (forward, left, right)

⚙️ 4-wheel drive with motor control

💬 Real-time serial monitoring

🛠️ Hardware Requirements
Component	Quantity
Arduino Uno / Mega	1
Adafruit Motor Shield	1
IR Sensors (Analog)	2
DC Motors	4
Power Supply (Battery)	1
Jumper Wires	As needed
Chassis (robot frame)	1

🔌 Circuit Connections
IR Sensors:
Left Sensor → A1

Right Sensor → A0

Motors (via Motor Shield):
Motor1 → M1

Motor2 → M2

Motor3 → M3

Motor4 → M4

💡 Working Principle
The robot uses two IR sensors to follow a black line:

If both sensors detect black, it moves forward.

If left detects black, it turns left.

If right detects black, it turns right.

If both detect white, it stops or waits.

🚀 Getting Started
Connect all hardware components as described above.

Upload the Arduino code from line_follower.ino to your Arduino board.

Place your robot on a track with a black line on white background.

Power it up and watch it follow the path!

🔧 Libraries Used
AFMotor

[Wire] (built-in)

Install these libraries from the Library Manager in Arduino IDE.
