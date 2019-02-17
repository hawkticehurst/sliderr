#include <AccelStepper.h>
// Define a stepper and the pins it will use
AccelStepper stepper; // Defaults to AccelStepper::FULL4WIRE (4 pins) on 2, 3, 4, 5

void setup() {
    stepper.setMaxSpeed(1000.0);
    stepper.setAcceleration(4000.0);
    const int
    const int NUM_PICTURES = 100;
    const int START_POSITION = 0;
    const int END_POSITION =
    for (int i = 0; i < numPictures; i++) {
      stepper.runToNewPosition(currPosition);
      delay(1000);
      currPosition += 1000;
    }
}

void loop() {
}
