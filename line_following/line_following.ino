#include <AFMotor.h>
#include <Wire.h>
#include <Adafruit_TCS34725.h>

// Defining pins and variables for the line-following sensors
#define lefts A1
#define rights A0

// Defining motors
AF_DCMotor motor1(1, MOTOR12_1KHZ); 
AF_DCMotor motor2(2, MOTOR12_1KHZ);
AF_DCMotor motor3(3, MOTOR34_1KHZ);
AF_DCMotor motor4(4, MOTOR34_1KHZ);

// Initialize the TCS34725 color sensor
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_600MS, TCS34725_GAIN_1X);

void setup() {
  // Setting the speed of motors
  motor1.setSpeed(90);
  motor2.setSpeed(90);
  motor3.setSpeed(90);
  motor4.setSpeed(90);
  
  // Declaring pin types for sensors
  pinMode(lefts, INPUT);
  pinMode(rights, INPUT);
  
  // Begin serial communication
  Serial.begin(9600);

  // Check if the color sensor is connected
  if (tcs.begin()) {
    Serial.println("TCS34725 found.");
  } else {
    Serial.println("No TCS34725 found. Check your connections.");
    while (1); // Halt the program if sensor is not found
  }
}

void loop() {
  // Read values from the IR sensors
  int leftValue = analogRead(lefts);
  int rightValue = analogRead(rights);

  // Line-following logic
  if (leftValue <= 350 && rightValue <= 350) {
    // Both sensors detect the line, move forward
    motor1.run(FORWARD);
    motor2.run(FORWARD);
    motor3.run(FORWARD);
    motor4.run(FORWARD);
  }
  else if (leftValue <= 350 && !(rightValue <= 350)) {
    // Left sensor detects the line, turn left
    motor1.run(FORWARD);
    motor2.run(FORWARD);
    motor3.run(BACKWARD);
    motor4.run(BACKWARD);
  }
  else if (!(leftValue <= 350) && rightValue <= 350) {
    // Right sensor detects the line, turn right
    motor1.run(BACKWARD);
    motor2.run(BACKWARD);
    motor3.run(FORWARD);
    motor4.run(FORWARD);
  }
  else if (!(leftValue <= 350) && !(rightValue <= 350)) {
    // No line detected, stop
    motor1.run(RELEASE);
    motor2.run(RELEASE);
    motor3.run(RELEASE);
    motor4.run(RELEASE);

    // If both sensors exceed 350, trigger color-detecting function
    if (leftValue > 350 && rightValue > 350) {
      Serial.println("Both sensors exceed 350, triggering color detection.");
      processColorAction();
      delay(1000);
    }
  }
}

// Function to process the color and take action
void processColorAction() {
  // Move forward for 3 seconds at the start of each loop
  Serial.println("Moving forward for 3 seconds...");
  goForward();
  delay(1000); // Move forward for 3 seconds
  
  // Get the color values
  uint16_t r, g, b, c;
  tcs.getRawData(&r, &g, &b, &c);

  // Print the raw color values
  Serial.print("RGB Values - R: ");
  Serial.print(r);
  Serial.print(" G: ");
  Serial.print(g);
  Serial.print(" B: ");
  Serial.println(b);

  // Check for color detection and take action
  if (r < 2500 && g < 2500 && b < 2500) {
    Serial.println("No color detected. Stopping and waiting for color...");
    stopMotors();
    delay(100); // Short delay to prevent continuous checking too quickly
  } else {
    if (isRedDominant(r, g, b)) {
      handleRed();
    } else if (isGreenDominant(r, g, b)) {
      handleGreen();
    } else if (isBlueDominant(r, g, b)) {
      handleBlue();
    } else {
      Serial.println("No dominant color detected.");
      stopMotors();
    }
  }
}

// Function to check if red is dominant
bool isRedDominant(uint16_t r, uint16_t g, uint16_t b) {
  return (r > g && r > b);
}

// Function to check if green is dominant
bool isGreenDominant(uint16_t r, uint16_t g, uint16_t b) {
  return (g > r && g > b);
}

// Function to check if blue is dominant
bool isBlueDominant(uint16_t r, uint16_t g, uint16_t b) {
  return (b > r && b > g);
}

// Action for red color: Move forward
void handleRed() {
  Serial.println("Dominant Color: Red - Moving forward.");
  goForward();
}

// Action for green color: Turn right
void handleGreen() {
  Serial.println("Dominant Color: Green - Turning right.");
  turnRight();
}

// Action for blue color: Turn left
void handleBlue() {
  Serial.println("Dominant Color: Blue - Turning left.");
  turnLeft();
}

// Function to move forward
void goForward() {
  motor1.run(FORWARD);
  motor2.run(FORWARD);
  motor3.run(FORWARD);
  motor4.run(FORWARD);
  Serial.println("Moving forward...");
  delay(5000);
  stopMotors();
}

// Function to turn left
void turnLeft() {
  motor1.run(FORWARD);  // Right front motor moves forward
  motor2.run(FORWARD);  // Right back motor moves forward
  motor3.run(BACKWARD); // Left front motor moves backward
  motor4.run(BACKWARD); // Left back motor moves backward
  Serial.println("Turning left...");
  delay(5000); // Time for turning left
  stopMotors();
}

// Function to turn right
void turnRight() {
  motor1.run(BACKWARD);  // Right front motor moves backward
  motor2.run(BACKWARD);  // Right back motor moves backward
  motor3.run(FORWARD);   // Left front motor moves forward
  motor4.run(FORWARD);   // Left back motor moves forward
  Serial.println("Turning right...");
  delay(5000); // Time for turning right
  stopMotors();
}

// Function to stop all motors
void stopMotors() {
  motor1.run(RELEASE);
  motor2.run(RELEASE);
  motor3.run(RELEASE);
  motor4.run(RELEASE);
  Serial.println("Motors stopped.");
}
