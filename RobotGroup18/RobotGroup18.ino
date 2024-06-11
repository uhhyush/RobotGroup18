#include <tcs3200.h> // Library for TCS3200 color sensor
#include <WiFiNINA.h> // Library for WiFi functionality

// Define IR sensor pins
#define IR_1 4
#define IR_2 7
#define IR_3 8
#define IR_4 12

// Define color sensor pins
#define COLOR_0 A0
#define COLOR_1 A1
#define COLOR_2 A2
#define COLOR_3 A3
#define COLOR_OUT A4

// Define ultrasonic sensor pins
#define TRIG_PIN 10
#define ECHO_PIN 11

// Define motor control pins
#define MOTOR_PIN1 3
#define MOTOR_PIN2 5
#define MOTOR_PIN3 6
#define MOTOR_PIN4 9

// Define RGB LED pins
#define LED_R 25
#define LED_G 26
#define LED_B 27

// Define state variables
String currentState = "Null";
String oldCurrentState = "Null";

// Define timing variables for sensor readings
unsigned long irSensorMillis = 0;
unsigned long colorSensorMillis = 0;
unsigned long ultrasonicSensorMillis = 0;
unsigned long motorlogicMillis = 0;

// Define variables for turning direction and state
String turnDirection = "";
String lastTurnDirection = "";
bool isTurning = false;
bool wallDetected = false;

// Define variable for distance measurement
int currentDistance = 1000;

// Variable to store current time in milliseconds
unsigned long currentMillis;

void setup() {
  // Start serial communication at 250000 bps
  Serial.begin(250000);

  // Set IR sensor pins as input
  pinMode(IR_1, INPUT);
  pinMode(IR_2, INPUT);
  pinMode(IR_3, INPUT);
  pinMode(IR_4, INPUT);

  // Set ultrasonic sensor pins
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  // Set motor control pins as output
  pinMode(MOTOR_PIN1, OUTPUT);
  pinMode(MOTOR_PIN2, OUTPUT);
  pinMode(MOTOR_PIN3, OUTPUT);
  pinMode(MOTOR_PIN4, OUTPUT);

  // Set RGB LED pins as output using WiFi library functions
  WiFiDrv::pinMode(LED_R, OUTPUT);
  WiFiDrv::pinMode(LED_G, OUTPUT);
  WiFiDrv::pinMode(LED_B, OUTPUT);
}

// The loop function runs over and over again forever
void loop() {
  // Get the current time in milliseconds
  currentMillis = millis();
  Serial.println(currentMillis);
  
  // Read color sensor every 20 milliseconds
  if (currentMillis - colorSensorMillis >= 20) {
    colorSensorMillis = currentMillis;
    readColorSensor();
  }

  // Read ultrasonic sensor every 20 milliseconds
  if (currentMillis - ultrasonicSensorMillis >= 20) {
    ultrasonicSensorMillis = currentMillis;
    readUltrasonic();
  }
  
  // Run robot logic
  robotLogic();
}
