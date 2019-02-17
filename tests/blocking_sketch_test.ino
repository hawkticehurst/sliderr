#include <AccelStepper.h>
// Define a stepper and the pins it will use
AccelStepper stepper; // Defaults to AccelStepper::FULL4WIRE (4 pins) on 2, 3, 4, 5

void setup() {  
    stepper.setMaxSpeed(1000.0);
    stepper.setAcceleration(5000.0);
}
void loop() {
    int numPictures = 100;
    int currPosition = 0;
    for (int i = 0; i < numPictures; i++) {
      stepper.runToNewPosition(currPosition);
      delay(1000);
      currPosition += 1000;
    }
}
