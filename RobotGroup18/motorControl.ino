// Global speed multipliers for each motor
float motor1Multiplier = 1.8;
float motor2Multiplier = 1.0;

void motorControl(int speed1, int speed2) {
  // Apply the multipliers to the motor speeds
  int adjustedSpeed1 = speed1 * motor1Multiplier;
  int adjustedSpeed2 = speed2 * motor2Multiplier;
  
  // Ensure the adjusted speeds do not exceed the PWM range [0, 255]
  adjustedSpeed1 = max(-255, min(255, adjustedSpeed1));
  adjustedSpeed2 = max(-255, min(255, adjustedSpeed2));
  
  // Apply the adjusted speeds to the motors
  analogWrite(MOTOR_PIN1, adjustedSpeed1 > 0 ? adjustedSpeed1 : 0);
  analogWrite(MOTOR_PIN2, adjustedSpeed1 < 0 ? -adjustedSpeed1 : 0);
  analogWrite(MOTOR_PIN3, adjustedSpeed2 > 0 ? adjustedSpeed2 : 0);
  analogWrite(MOTOR_PIN4, adjustedSpeed2 < 0 ? -adjustedSpeed2 : 0);
}
