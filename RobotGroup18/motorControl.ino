// Function to control the motor driver with specified speeds for two motors
void motorControl(int speed1, int speed2) {
  // Multipliers for adjusting motor speeds
  float motor1Multiplier = 1.4;  // Multiplier for left wheel
  float motor2Multiplier = 1.1;  // Multiplier for right wheel

  // Adjust speeds based on multipliers
  int adjustedSpeed1 = speed1 * motor1Multiplier;  // Adjusted speed for motor 1 (left wheel)
  int adjustedSpeed2 = speed2 * motor2Multiplier;  // Adjusted speed for motor 2 (right wheel)

  // Ensure adjusted speeds are within the valid range (-255 to 255)
  adjustedSpeed1 = max(-255, min(255, adjustedSpeed1));
  adjustedSpeed2 = max(-255, min(255, adjustedSpeed2));

  // Control motors based on adjusted speeds
  analogWrite(MOTOR_PIN1, adjustedSpeed1 > 0 ? adjustedSpeed1 : 0);   // Motor 1 forward
  analogWrite(MOTOR_PIN2, adjustedSpeed1 < 0 ? -adjustedSpeed1 : 0);  // Motor 1 backward
  analogWrite(MOTOR_PIN3, adjustedSpeed2 > 0 ? adjustedSpeed2 : 0);   // Motor 2 forward
  analogWrite(MOTOR_PIN4, adjustedSpeed2 < 0 ? -adjustedSpeed2 : 0);  // Motor 2 backward
}
