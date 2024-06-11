bool initialForwardMotion = false;  // Flag to indicate initial forward motion
const unsigned long initialMotionDuration = 275;  // Duration of initial forward motion

unsigned long turnStartTime = 0;  // Timestamp for when turning starts
const unsigned long initialTurnDuration = 600;  // Duration for the initial part of the turn
int turnSpeed = 150;  // Speed for turning

int leftTurnModifier = 40;  // Speed modifier for left turns
int rightTurnModifier = 25;  // Speed modifier for right turns

const unsigned long sensorCheckThreshold = 50;  // Time threshold for sensor checks

unsigned long firstDetectionTime1 = 0;  // Timestamp for first detection by IR sensor 1
unsigned long firstDetectionTime4 = 0;  // Timestamp for first detection by IR sensor 4

// Main robot logic function
void robotLogic() {
  turnLogic();  // Handle turning logic
  motorLogic();  // Handle motor control based on IR sensor detection
}

// Function to handle turning logic
void turnLogic() {
  int* IRvalues = readInfrared();  // Get IR sensor values

  // Check for wall detection
  if (currentDistance < 8 && wallDetected == false) {
    isTurning = true;
    wallDetected = true;
    turnStartTime = currentMillis;
    initialForwardMotion = true;
  }

  // If not turning and no wall detected, handle IR sensor detections
  if (!isTurning && wallDetected == false) {
    if (IRvalues[3] == 1 && firstDetectionTime1 == 0) {
      firstDetectionTime1 = currentMillis;
      Serial.println("Detected Left Sensor");
    }
    if (IRvalues[0] == 1 && firstDetectionTime4 == 0) {
      firstDetectionTime4 = currentMillis;
      Serial.println("Detected Right Sensor");
    }

    // Check for simultaneous detection by IR sensors 1 and 4
    if (firstDetectionTime1 > 0 && currentMillis - firstDetectionTime1 < sensorCheckThreshold && 
        firstDetectionTime4 > 0 && currentMillis - firstDetectionTime4 < sensorCheckThreshold) {
      isTurning = true;
      initialForwardMotion = true;
      turnStartTime = currentMillis;
      turnDirection = lastTurnDirection;
      firstDetectionTime1 = 0;
      firstDetectionTime4 = 0;
      lastTurnDirection = turnDirection;
      Serial.print("Solid Line Detected (Sensors 1 and 4) - turning ");
      Serial.println(turnDirection);
    } 
    // Handle individual sensor detections
    else if (firstDetectionTime1 > 0 && currentMillis - firstDetectionTime1 >= sensorCheckThreshold) {
      isTurning = true;
      initialForwardMotion = true;
      turnStartTime = currentMillis;
      turnDirection = "left";
      firstDetectionTime1 = 0;
      lastTurnDirection = turnDirection;
      Serial.println("Left Turn Detected");
    } 
    else if (firstDetectionTime4 > 0 && currentMillis - firstDetectionTime4 >= sensorCheckThreshold) {
      isTurning = true;
      initialForwardMotion = true;
      turnStartTime = currentMillis;
      turnDirection = "right";
      firstDetectionTime4 = 0;
      lastTurnDirection = turnDirection;
      Serial.println("Right Turn Detected");
    }
  }
}

// Function to handle motor control logic based on sensor values and turning state
void motorLogic() {
  int* IRvalues = readInfrared();  // Get IR sensor values

  // If not turning and no wall detected, control motors based on IR sensor values
  if (!isTurning && !wallDetected) {
    if (IRvalues[1] == 1 && IRvalues[2] == 1) {  // Both center sensors detect the line
      motorControl(220, 220);
      currentState = "Forward";
    } else if (IRvalues[1] == 0 && IRvalues[2] == 1) {  // Left sensor off, right sensor on
      motorControl(160, 220);
      currentState = "Left";
    } else if (IRvalues[1] == 1 && IRvalues[2] == 0) {  // Left sensor on, right sensor off
      motorControl(220, 160);
      currentState = "Right";
    } else if (IRvalues[0] == 1 && IRvalues[1] == 1 && IRvalues[2] == 1 && IRvalues[3] == 1) {  // All sensors detect the line
      if (currentState == "Forward") {
        motorControl(220, 220);
      } else if (currentState == "Left") {
        motorControl(160, 220);
      } else if (currentState == "Right") {
        motorControl(220, 160);
      }
    }
  } 
  // If turning and no wall detected, handle turning logic
  else if (isTurning && !wallDetected) {
    if (initialForwardMotion) {
      motorControl(200, 200);  // Move forward for the initial duration
      currentState = "Bumping Out...";
      delay(initialMotionDuration);
      initialForwardMotion = false;
    } else {
      if (turnDirection == "left") {
        if (currentMillis - turnStartTime >= initialTurnDuration && IRvalues[2] == 1) {
          Serial.println("Detected Center IR Sensor after initial turn duration");
          isTurning = false;  // Stop turning if IR2 detects the line again
          motorControl(0, 0);
          currentState = "Turning Left - Next Line Detected";
          motorControl(turnSpeed, -turnSpeed);  // Counterbalance
          delay(100);
          motorControl(0, 0);
          delay(50);
          currentState = "Forward";
        } else {
          motorControl(-turnSpeed - leftTurnModifier, turnSpeed - rightTurnModifier);  // Continue turning left
          currentState = "Turning Left - No Line Yet Detected";
        }
      } else if (turnDirection == "right") {
        if (currentMillis - turnStartTime >= initialTurnDuration && IRvalues[1] == 1) {
          Serial.println("Detected Center IR Sensor");
          isTurning = false;  // Stop turning if IR1 detects the line again
          motorControl(0, 0);
          currentState = "Turning Right - Next Line Detected";
          motorControl(-turnSpeed, turnSpeed);  // Counterbalance
          delay(100);
          motorControl(0, 0);
          delay(50);
          currentState = "Forward";
        } else {
          motorControl(turnSpeed + leftTurnModifier, -turnSpeed + rightTurnModifier);  // Continue turning right
          currentState = "Turning Right - No Line Yet Detected";
        }
      }
    }
  } 
  // If turning and wall detected, handle wall avoidance logic
  else if (isTurning && wallDetected) {
    if (initialForwardMotion) {
      motorControl(-200, -200);  // Move backward for a short duration
      currentState = "Stopping...";
      delay(100);
      initialForwardMotion = false;
    } else {
      if (currentMillis - turnStartTime >= initialTurnDuration && IRvalues[2] == 1) {
        Serial.println("Detected Center IR Sensor after initial turn duration");
        isTurning = false;  // Stop turning if IR2 detects the line again
        currentState = "Turning Left - Next Line Detected";
        motorControl(turnSpeed, -turnSpeed);  // Counterbalance
        delay(50);
        motorControl(0, 0);
        delay(50);
        wallDetected = false;
        currentState = "Forward";
      } else {
        motorControl(-turnSpeed - leftTurnModifier, turnSpeed - rightTurnModifier);  // Continue turning left
        currentState = "Turning Left - No Line Yet Detected";
      }
    }
  }
}
