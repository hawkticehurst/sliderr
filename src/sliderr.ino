#include <AccelStepper.h>

AccelStepper stepper;
int incomingByte = 0;
char incomingCommand[8];
int incomingCmdIdx = 0;

// Boundaries are initially set arbitrarily low/high to allow 
// user to move the slider to each edge without being blocked
long LEFT_BOUNDARY = -100000;
long RIGHT_BOUNDARY = 100000;

void setup() {
  Serial.begin(9600);
  while (!Serial);
  Serial.println("Welcome to Sliderr");
  // Set default stepper acceleration and speed
  stepper.setAcceleration(75000);
  stepper.setMaxSpeed(5000);
}

void loop() {
  if (Serial.available()) {
    incomingByte = Serial.read();
    handleByteInput();
  }
  stepper.run();
}

void handleByteInput() {
  switch (incomingByte) {
    case '!':     
      incomingCommand[incomingCmdIdx] = '\0';
      setupCommandExecution();
      break;
    case 10:
      // Ignore Line Feed character
      break;
    default:
      incomingCommand[incomingCmdIdx] = (char) incomingByte;
      incomingCmdIdx++;
      break;
  }
}

void setupCommandExecution() {
  // Cast incoming command char array to a string  
  String fullCommand = (String) incomingCommand;

  // Reset incoming command index
  incomingCmdIdx = 0;

  // Get command from full command string
  String command = fullCommand.substring(0, 4);
  
  // Get command data if it exists
  String commandData;
  if (fullCommand.indexOf(":") != -1) {
    int fromIdx = fullCommand.indexOf(":") + 1;
    int toIdx = fullCommand.length();
    commandData = fullCommand.substring(fromIdx, toIdx);
  }

  executeCommand(command, commandData.toInt());
}

void executeCommand(String command, int data) {
  if (command == "stop") {
    stepper.stop();
  } else if (command == "movL") {
    stepper.moveTo(LEFT_BOUNDARY);
  } else if (command == "movR") {
    stepper.moveTo(RIGHT_BOUNDARY);
  } else if (command == "setL") {
    stepper.setCurrentPosition(0);
    LEFT_BOUNDARY = stepper.currentPosition();
  } else if (command == "setR") {
    RIGHT_BOUNDARY = stepper.currentPosition();
  } else if (command == "home") {
    stepper.moveTo(LEFT_BOUNDARY);
  } else if (command == "setA") {
    // Accel range: 1000 - 100000
    stepper.setAcceleration(data * 1000);
  } else if (command == "setS") {
    // Speed range: 100 - 10000
    stepper.setMaxSpeed(data * 100);
  }
}
