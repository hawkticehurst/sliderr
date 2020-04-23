/*
    MocoSlider.h – Library for managing a 1-axis motion controlled (moco) camera slider.
    Created by Hawk Ticehurst
*/

#ifndef MocoSlider_h
#define MocoSlider_h

#include "Arduino.h"
#include "AccelStepper.h"

class MocoSlider
{
public:
    MocoSlider();
    void setSpeed(long speed);
    void setAcceleration(long acceleration);
    void setLeftBoundary();
    void setRightBoundary();
    void setMoveStartPosition();
    void setMoveEndPosition();
    void runToLeftBoundary();
    void runToRightBoundary();
    void runToMoveStart();
    void runToMoveEnd();
    void runToCenter();
    void moveSlider(long steps);
    void stopSlider();
    void printDebugInfo();
    void moveSliderNonBlocking(long location);
    bool checkForStop();

private:
    AccelStepper stepperX;     // Defaults to AccelStepper::FULL4WIRE (4 pins) on 2, 3, 4, 5
    long LEFT_BOUNDARY;        // The left-most edge of the slider (in steps)
    long RIGHT_BOUNDARY;       // The right-most edge of the slider (in steps)
    long SLIDER_MOVE_START;    // Starting position (in steps) of a slider move
    long SlIDER_MOVE_END;      // Ending position (in steps) of a slider move
    long CURRENT_ACCELERATION; // Currently set max acceleration of the slider (in steps/sec/sec)
    long CURRENT_SPEED;        // Currently set max speed of the slider (in steps/sec)
};

#endif