// Function to read distance from an ultrasonic sensor
void readUltrasonic() {
  long duration, distance;  // Variables to store duration and calculated distance

  // Send a short LOW pulse to ensure clean trigger
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  // Send a HIGH pulse for 10 microseconds to trigger the ultrasonic sensor
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Measure the duration of the pulse received by the echo pin
  duration = pulseIn(ECHO_PIN, HIGH);

  // Calculate the distance based on the duration of the pulse
  // Speed of sound is approximately 0.034 cm per microsecond, divide by 2 for round-trip
  distance = duration * 0.034 / 2;

  // Print the measured distance
  Serial.print("Distance: ");
  Serial.println(distance);

  // Update the global variable for current distance
  currentDistance = distance; 
}
