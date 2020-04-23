/*
    Author: Hawk Ticehurst

    Description
    ---------------
    Interface for controlling a motion controlled camera slider via bluetooth.

    MocoSlider
    ---------------
    Reference the MocoSlider class in this repo for implementation details 
    regarding the slider functionality (i.e. slider motor control).

    Hardware Used
    ---------------
    Board: Arduino Uno
    Stepper Motor: NEMA 17 (~78oz/in torque)
    Stepper Motor Control: CNC Shield & A4988 Stepper Motor Driver
    Bluetooth: HM-10 Module
*/
#include <MocoSlider.h>

MocoSlider slider;
int incomingByte = 0;         // Incoming serial data
int totalStrLength = 20;      // Total possible length of the input string
int currStrLength = 0;        // Current string length
int strIndex = 0;             // Current string index
char inputStr[20];            // Input string character array (i.e. the input string)
bool isInputComplete = false; // String completetion boolean flag
long stringData = 0;          // Data retrieved from input string (i.e. number of steps)

void setup()
{
    // Start serial monitor
    Serial.begin(9600);
    Serial.println("########################################");
    Serial.println("#          MOCO Camera Slider          #");
    Serial.println("########################################");
    Serial.println();
    Serial.println("Camera slider control via bluetooth.");
    Serial.println();
    Serial.println("Start by setting the slider boundaries!");
    Serial.println("    1. Set the left boundary");
    Serial.println("    2. Set the right boundary");
    Serial.println();
}

void loop()
{
    if (Serial.available())
    {
        // ---- Byte Input ----
        // Serial.println(Serial.peek());
        incomingByte = Serial.read();
        handleByteInput();

        // ---- String Input ----
        // buildInputString();
        // if (isInputComplete)
        // {
        //     checkStringForData();
        //     printInputString();
        //     Serial.println(stepsData);
        //     handleStringInput();
        //     isInputComplete = false;
        //     strIndex = 0;
        // }
    }
}

// ================== Handle User Input ==================

/**
 * Execute slider functionality based on user input.
 * 
 * Input bytes are arbitrarily mapped to slider functionality 
 * at this point. Below function "handleStringInput" is being developed 
 * in hopes of making the mapping from bluetooth user input to slider 
 * functionality less abritrary.
 */
void handleByteInput()
{
    switch (incomingByte)
    {
    case 48:
        slider.moveSlider(25000);
        break;
    case 49:
        slider.moveSlider(-25000);
        break;
    case 50:
        slider.setLeftBoundary();
        break;
    case 51:
        slider.setRightBoundary();
        break;
    case 52:
        slider.runToLeftBoundary();
        break;
    case 53:
        slider.runToCenter();
        break;
    case 54:
        slider.runToRightBoundary();
        break;
    case 55:
        slider.printDebugInfo();
        break;
    case 56:
        // Set slow acceleration
        slider.setAcceleration(500);
        break;
    case 57:
        // Set medium acceleration
        slider.setAcceleration(5000);
        break;
    case 58:
        // Set fast acceleration
        slider.setAcceleration(50000);
        break;
    case 59:
        // Set slow speed
        slider.setSpeed(500);
        break;
    case 60:
        // Set medium speed
        slider.setSpeed(1000);
        break;
    case 61:
        // Set fast acceleration
        slider.setSpeed(5000);
        break;
    // case 62:
    //     slider.moveSliderNonBlocking(20000);
    //     break;
    case 63:
        slider.stopSlider();
        break;
    }
}

/**
 * NOTE: Function in development.
 * 
 * Execute slider functionality based on user input.
 */
void handleStringInput()
{
    if ((String)inputStr == "mv")
    {
        slider.moveSlider(stringData);
    }
}

// ==================== Build Input String ====================

/**
 * Build input string from incoming bytes of serial data.
 */
void buildInputString()
{
    switch (incomingByte)
    {
    case 252:
        // Ignore byte marking the beginning of a bluetooth transmission
        break;
    case 254:
        // Ignore byte marking the beginning of a bluetooth transmission
        break;
    case 250:
        // Terminate string when an end of transmission byte is received
        terminateString();
        break;
    default:
        appendToString();
        break;
    }
}

/**
 * Append the incoming byte of serial data to the input string.
 * 
 * Prints a warning if the input string exceeds the total length allocated
 * for the string.
 */
void appendToString()
{
    if (currStrLength >= totalStrLength)
    {
        Serial.println("Warning: Not enough space allocated for string.");
    }
    else
    {
        // Append the incoming byte/character to the input string
        inputStr[strIndex] = (char)incomingByte;
        strIndex++;
        currStrLength++;
    }
}

/**
 * Terminate the current input string.
 * 
 * Termination is achieved by appending \0 to the char array.
 */
void terminateString()
{
    inputStr[strIndex] = '\0';
    isInputComplete = true;
}

// ======================== Extract String Data ========================

/**
 * Check if input string contains data and call functions to extract data
 * if present.
 * 
 * An input string contains data if a colon is present in the string. The 
 * following schema is used to transmit both a slider function and its 
 * associated data over bluetooth:
 * 
 *      <slider function>:<input data>
 * 
 * As an example:
 * 
 *      mv:5000 => Call moveSlider(5000)
 */
void checkStringForData()
{
    // Index representing where the input string should be split in order to
    // retrieve data within the string
    int strSplitIndex = -1;

    for (int i = 0; i < currStrLength; i++)
    {
        if (inputStr[i] == ':')
        {
            strSplitIndex = i;
        }
    }

    // If data is present, extract it
    if (strSplitIndex > -1)
    {
        extractDataFromString(strSplitIndex);
        // Reset the input string to only contain the string without data
        strIndex = strSplitIndex;
        currStrLength = strSplitIndex;
        terminateString();
    }
}

/**
 * Extract data from an input string.
 * 
 * Reference above 'checkStringForData' function definition for explanation
 * of input string schema.
 * 
 * @param strSplitIndex The index value where the input string should be split.
 */
void extractDataFromString(int strSplitIndex)
{
    char inputData[6];
    int inputDataIndex = 0;

    // Extract data from string
    for (int i = strSplitIndex + 1; i < currStrLength; i++)
    {
        inputData[inputDataIndex] = inputStr[i];
        inputDataIndex++;
    }

    // Terminate inputData string
    inputData[inputDataIndex] = '\0';
    stringData = atof(inputData);
}

// ===================== Print User Input =====================

/**
 * Print incoming serial data to the monitor.
 * 
 * Prints both the byte and the character the byte represents.
 */
void printByte()
{
    Serial.print("Incoming Byte: ");
    Serial.print(incomingByte, DEC);
    Serial.print(" | ");
    Serial.println((char)incomingByte);
}

/**
 * Print the current input string.
 */
void printInputString()
{
    Serial.println(inputStr);
}