/*
    MocoSlider.h – Library for managing a 1-axis motion controlled (moco) camera slider.
    Created by Hawk Ticehurst

    Dependencies
    ---------------
    MocoSlider uses the AccelStepper library as the foundation for controlling the slider 
    carriage motor (i.e. a stepper motor). Documentation and download links for the 
    AccelStepper library can be found at:

        https://www.airspayce.com/mikem/arduino/AccelStepper/index.html
    
*/

#include "Arduino.h"
#include "MocoSlider.h"
#include "AccelStepper.h"

MocoSlider::MocoSlider()
{
    LEFT_BOUNDARY = -5000000; // Set arbitrarily low to allow user to move slider to edge without being blocked
    RIGHT_BOUNDARY = 5000000; // Set arbitrarily high to allow user to move slider to edge without being blocked
    SLIDER_MOVE_START = 0;
    SlIDER_MOVE_END = 0;
    CURRENT_ACCELERATION = 50000;
    CURRENT_SPEED = 5000;

    setAcceleration(CURRENT_ACCELERATION);
    setSpeed(CURRENT_SPEED);
}

/**
 * Set max speed of the slider motor.
 * 
 * Based on initial testing the available range of input values using a NEMA 17
 * motor (~78oz/in torque) and an Arduino Uno is:
 *      MIN INPUT VALUE: >1 (0% Speed)
 *      MAX INPUT VALUE: 5,000 (100% Speed)
 * 
 * @param speed The max speed of the slider motor (in steps/second).
 */
void MocoSlider::setSpeed(long speed)
{
    CURRENT_SPEED = speed;
    stepperX.setMaxSpeed(CURRENT_SPEED);
}

/**
 * Set max acceleration of the slider motor.
 * 
 * Based on initial testing the available range of input values using a NEMA 17
 * motor (~78oz/in torque) and an Arduino Uno is:
 *      MIN INPUT VALUE: >1 (0% Ramp)
 *      MAX INPUT VALUE: 50,000 (100% Ramp)
 * 
 * @param acceleration The max acceleration of the slider motor (in steps/second/second).
 */
void MocoSlider::setAcceleration(long acceleration)
{
    CURRENT_ACCELERATION = acceleration;
    stepperX.setAcceleration(CURRENT_ACCELERATION);
}

/**
 * Set the current carriage position to be the left-most boundary of the slider.
 * 
 * Note: The left and right boundaries must be set in order for other slider 
 * functionality to be used.
 * 
 * The left boundary is considered to be the starting boundary and should be set
 * first in order for the right boundary to be correctly calculated.
 */
void MocoSlider::setLeftBoundary()
{
    stepperX.setCurrentPosition(0);
    LEFT_BOUNDARY = stepperX.currentPosition();
}

/**
 * Set the current position of the carriage to be the right-most boundary of the slider.
 * 
 * Note: The left and right boundaries must be set in order for other slider 
 * functionality to be used.
 * 
 * The left boundary is considered to be the ending boundary and should be set
 * second in order to be correctly calculated.
 */
void MocoSlider::setRightBoundary()
{
    if (LEFT_BOUNDARY == -5000000)
    {
        Serial.println("Please set left boundary first.");
    }
    else
    {
        RIGHT_BOUNDARY = stepperX.currentPosition();
    }
}

/**
 * Set the current carriage position to be the starting position of a slider move.
 */
void MocoSlider::setMoveStartPosition()
{
    SLIDER_MOVE_START = stepperX.currentPosition();
}

/**
 * Set the current carriage position to be the ending position of a slider move.
 */
void MocoSlider::setMoveEndPosition()
{
    SlIDER_MOVE_END = stepperX.currentPosition();
}

/**
 * Move the carriage from its current position to the left-most (starting) boundary
 * of the slider.
 * 
 * Note: The left boundary must be set before this method can be used.
 */
void MocoSlider::runToLeftBoundary()
{
    if (LEFT_BOUNDARY == -5000000)
    {
        Serial.println("Please set left boundary first.");
    }
    else
    {
        stepperX.runToNewPosition(LEFT_BOUNDARY);
    }
}

/**
 * Move the carriage from its current position to the right-most (ending) boundary 
 * of the slider.
 * 
 * Note: The right boundary must be set before this method can be used.
 */
void MocoSlider::runToRightBoundary()
{
    if (RIGHT_BOUNDARY == 5000000)
    {
        Serial.println("Please set right boundary first.");
    }
    else
    {
        stepperX.runToNewPosition(RIGHT_BOUNDARY);
    }
}

/**
 * Move the carriage from its current position to the set starting move position.
 */
void MocoSlider::runToMoveStart()
{
    stepperX.runToNewPosition(SLIDER_MOVE_START);
}

/**
 * Move the carriage from its current position to the set ending move position.
 */
void MocoSlider::runToMoveEnd()
{
    stepperX.runToNewPosition(SlIDER_MOVE_END);
}

/**
 * Move the carriage from its current position to the center of the slider.
 * 
 * Note: The left and right slider boundaries must be set before this method
 * can be used.
 */
void MocoSlider::runToCenter()
{
    if (LEFT_BOUNDARY == -5000000 || RIGHT_BOUNDARY == 5000000)
    {
        Serial.println("Please set the left and right slider boundaries first.");
    }
    else
    {
        float centerPosition = (RIGHT_BOUNDARY - LEFT_BOUNDARY) / 2;
        stepperX.runToNewPosition(centerPosition);
    }
}

/**
 * Move the carriage the given number of steps from its current position.
 * 
 * A positive input value will result in a rightward movement, while a negative 
 * input value will result in a leftward movement relative to the current position.
 * 
 * This method will also check and ensure that the carriage does not move past the 
 * left and right boundaries of the slider once set.
 * 
 * @param steps The number of steps the carriage should move from its current
 *      position.
 */
void MocoSlider::moveSlider(long steps)
{
    long currPosition = stepperX.currentPosition();
    long newPosition = currPosition + steps;
    if (newPosition <= LEFT_BOUNDARY)
    {
        stepperX.runToNewPosition(LEFT_BOUNDARY);
    }
    else if (newPosition >= RIGHT_BOUNDARY)
    {
        stepperX.runToNewPosition(RIGHT_BOUNDARY);
    }
    else
    {
        stepperX.runToNewPosition(newPosition);
    }
}

/**
 * Stop the carriage as quickly as possible.
 * 
 * The timing of the stop still obeys the currently set acceleration.
 */
void MocoSlider::stopSlider()
{
    stepperX.stop();
    stepperX.runToPosition();
}

/**
 * Print the current slider and carriage information to the serial monitor.
 */
void MocoSlider::printDebugInfo()
{
    Serial.println("----- DEBUG INFO -----");
    Serial.print("Current Position: ");
    Serial.println(stepperX.currentPosition());
    Serial.print("Current Max Speed: ");
    Serial.println(CURRENT_SPEED);
    Serial.print("Current Acceleration: ");
    Serial.println(CURRENT_ACCELERATION);
    Serial.print("Slider Move Start: ");
    Serial.println(SLIDER_MOVE_START);
    Serial.print("Slider Move End: ");
    Serial.println(SlIDER_MOVE_END);
    Serial.print("Slider Left Boundary: ");
    Serial.println(LEFT_BOUNDARY);
    Serial.print("Slider Right Boundary: ");
    Serial.println(RIGHT_BOUNDARY);
    Serial.println();
}

/**
 * NOTE: Method in development.
 * 
 * Move the carriage to given location on the slider in a non-blocking fashion.
 * 
 * @param location The absolute location (in steps) from the starting edge of 
 *      slider the carriage should move to.
 */
void MocoSlider::moveSliderNonBlocking(long location)
{
    stepperX.moveTo(location);
    bool shouldStop = false;
    Serial.println(Serial.peek());
    Serial.read(); // Read in 250 ascii value
    Serial.println(Serial.peek());
    while (stepperX.distanceToGo() != 0)
    {
        // shouldStop = checkForStop();
        if (stepperX.distanceToGo() == 10)
        {
            Serial.println("Inside the should stop if check");
            stopSlider();
            return;
        }
        stepperX.run();
    }
}

/**
 * NOTE: Method in development.
 * 
 * Check whether a request to stop the slider has been given over bluetooth.
 * 
 * Currently the ASCII value 63 has been mapped to represent a call to 
 * the stopSlider function.
 * 
 * @return Boolean value representing whether a request to stop the slider 
 *      has been given.
 */
bool MocoSlider::checkForStop()
{
    if (Serial.available())
    {
        Serial.println(Serial.peek());
        return Serial.peek() == 63;
    }
    return false;
}