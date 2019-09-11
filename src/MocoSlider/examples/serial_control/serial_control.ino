/*
    Author: Hawk Ticehurst

    Description
    ---------------
    Interface for controlling a motion controlled camera slider via 
    the Arduino Serial Monitor.

    MocoSlider
    ---------------
    Reference the MocoSlider class in this repo for implementation details 
    regarding the slider functionality (i.e. slider motor control).

    Hardware Used 
    ---------------
    Board: Arduino Uno
    Stepper Motor: NEMA 17 (~78oz/in torque)
    Stepper Motor Control: CNC Shield & A4988 Stepper Motor Driver
*/

#include <MocoSlider.h>

MocoSlider slider;
String serialInput = "";      // A string to hold incoming serial data
bool isInputComplete = false; // Boolean flag for keeping track of input completion

void setup()
{
    // Start serial monitor
    Serial.begin(9600);
    Serial.println("########################################");
    Serial.println("#          MOCO Camera Slider          #");
    Serial.println("########################################");
    Serial.println();
    Serial.println("Camera slider control via the Arduino");
    Serial.println("Serial Monitor.");
    Serial.println();
    Serial.println("Start by setting the slider boundaries!");
    Serial.println("    1. Set the left boundary");
    Serial.println("    2. Set the right boundary");
    Serial.println();

    // Reserve 200 bytes for the serial input string
    serialInput.reserve(200);
}

void loop()
{
    if (isInputComplete)
    {
        if (serialInput != "")
        {
            handleInput();
        }
        resetInput();
    }
}

// =================== Handle User Input ===================

/**
 * Handle input from the serial monitor.
 * 
 * Variations of the "r" and "l" input represent moving right (r) or 
 * left (l) at different amounts. For example,
 *      "r" = move right 5,000 steps
 *      "rr" = move right 10,000 steps
 */
void handleInput()
{
    if (serialInput == "r")
    {
        slider.moveSlider(5000);
    }
    else if (serialInput == "rr")
    {
        slider.moveSlider(10000);
    }
    else if (serialInput == "rrr")
    {
        slider.moveSlider(15000);
    }
    else if (serialInput == "rrrr")
    {
        slider.moveSlider(25000);
    }
    else if (serialInput == "l")
    {
        slider.moveSlider(-5000);
    }
    else if (serialInput == "ll")
    {
        slider.moveSlider(-10000);
    }
    else if (serialInput == "lll")
    {
        slider.moveSlider(-15000);
    }
    else if (serialInput == "llll")
    {
        slider.moveSlider(-25000);
    }
    else if (serialInput == "set speed slow")
    {
        slider.setSpeed(500);
    }
    else if (serialInput == "set speed medium")
    {
        slider.setSpeed(2500);
    }
    else if (serialInput == "set speed fast")
    {
        slider.setSpeed(5000);
    }
    else if (serialInput == "set accel slow")
    {
        slider.setAcceleration(500);
    }
    else if (serialInput == "set accel medium")
    {
        slider.setAcceleration(5000);
    }
    else if (serialInput == "set accel fast")
    {
        slider.setAcceleration(50000);
    }
    else if (serialInput == "set boundary left")
    {
        slider.setLeftBoundary();
    }
    else if (serialInput == "set boundary right")
    {
        slider.setRightBoundary();
    }
    else if (serialInput == "set move start")
    {
        slider.setMoveStartPosition();
    }
    else if (serialInput == "set move end")
    {
        slider.setMoveEndPosition();
    }
    else if (serialInput == "run move start")
    {
        slider.runToMoveStart();
    }
    else if (serialInput == "run move end")
    {
        slider.runToMoveEnd();
    }
    else if (serialInput == "run right boundary")
    {
        slider.runToRightBoundary();
    }
    else if (serialInput == "run left boundary")
    {
        slider.runToLeftBoundary();
    }
    else if (serialInput == "run center")
    {
        slider.runToCenter();
    }
    else if (serialInput == "debug")
    {
        slider.printDebugInfo();
    }
    else
    {
        Serial.println("Not a valid input. Please try again.");
    }
}

/**
 * Read serial input data and build up the input string.
 * 
 * If a newline character is received mark the input string as complete.
 * 
 * SerialEvent Code Referenced From: https://www.arduino.cc/en/Tutorial/SerialEvent
*/
void serialEvent()
{
    while (Serial.available())
    {
        char inputChar = (char)Serial.read();
        if (inputChar == '\n')
        {
            isInputComplete = true;
        }
        else
        {
            serialInput += inputChar;
        }
    }
}

/**
 * Clear the serial input string and change the input completion flag.
 */
void resetInput()
{
    serialInput = "";
    isInputComplete = false;
}