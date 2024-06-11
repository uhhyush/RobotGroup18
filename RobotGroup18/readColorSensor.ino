// Initialize the TCS3200 color sensor with specified pins
tcs3200 tcs(COLOR_0, COLOR_1, COLOR_2, COLOR_3, COLOR_OUT);

// Function to read color sensor values
void readColorSensor() {
  const int samples = 5;                                     // Number of samples to average
  long sumRed = 0, sumGreen = 0, sumBlue = 0, sumWhite = 0;  // Variables to store sum of readings

  // Loop for taking multiple samples and summing up the readings
  for (int i = 0; i < samples; i++) {
    sumRed += tcs.colorRead('r');    // Read red component
    sumGreen += tcs.colorRead('g');  // Read green component
    sumBlue += tcs.colorRead('b');   // Read blue component
    sumWhite += tcs.colorRead('c');  // Read clear (white) component
  }

  // Calculate average readings
  int avgRed = sumRed / samples;
  int avgGreen = sumGreen / samples;
  int avgBlue = sumBlue / samples;
  int avgWhite = sumWhite / samples;

  // Print average color sensor readings
  Serial.print("Average Color Sensor Readings: ");
  Serial.print(" R= ");
  Serial.print(avgRed);
  Serial.print(" G= ");
  Serial.print(avgGreen);
  Serial.print(" B= ");
  Serial.print(avgBlue);
  Serial.print(" W(clear)= ");
  Serial.println(avgWhite);

  // Define variables to store previous LED state
  static int prevRed = 0;
  static int prevGreen = 0;
  static int prevBlue = 0;

  // Update RGB LED based on the color sensor readings
  if (avgRed > 20 && avgGreen > 15) {  // Condition for a specific color
    // Update only if the color is different from the previous one
    if (prevRed != 0 || prevGreen != 255 || prevBlue != 255) {
      prevRed = 0;
      prevGreen = 255;
      prevBlue = 255;
      WiFiDrv::analogWrite(27, prevRed);
      WiFiDrv::analogWrite(25, prevGreen);
      WiFiDrv::analogWrite(26, prevBlue);
    }
  } else if (avgRed > 15) {  // Condition for red color
    if (prevRed != 255 || prevGreen != 0 || prevBlue != 0) {
      prevRed = 255;
      prevGreen = 0;
      prevBlue = 0;
      WiFiDrv::analogWrite(26, prevRed);
      WiFiDrv::analogWrite(25, prevGreen);
      WiFiDrv::analogWrite(27, prevBlue);
    }
  } else if (avgGreen > 15) {  // Condition for green color
    if (prevRed != 0 || prevGreen != 255 || prevBlue != 0) {
      prevRed = 0;
      prevGreen = 255;
      prevBlue = 0;
      WiFiDrv::analogWrite(25, prevGreen);
      WiFiDrv::analogWrite(26, prevBlue);
      WiFiDrv::analogWrite(27, prevRed);
    }
  } else if (avgBlue > 15) {  // Condition for blue color
    if (prevRed != 0 || prevGreen != 0 || prevBlue != 255) {
      prevRed = 0;
      prevGreen = 0;
      prevBlue = 255;
      WiFiDrv::analogWrite(27, prevBlue);
      WiFiDrv::analogWrite(26, prevGreen);
      WiFiDrv::analogWrite(25, prevRed);
    }
  }
}
