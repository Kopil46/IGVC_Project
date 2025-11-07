// light


// DK21 - SPANITI TSK Safety Light Control
// RED button stops motor, Mode button changes mode

#include <stdint.h>
#include <stdbool.h>
#include "tm4c123gh6pm.h"
#include "gpio.h"
#include "wait.h"

// Pin Definitions - Port B
#define RED_PIN      4    // PB4 - Stop motor when pressed
#define ORANGE_PIN   5    // PB5
#define GREEN_PIN    6    // PB6
#define MODE_BUTTON  0    // PB0 - Change mode
#define STOP_BUTTON  1    // PB1 - Stop motor (RED function)

// System States
typedef enum {
    STATE_MANUAL = 0,     // Solid Green
    STATE_AUTONOMOUS,     // Flashing Orange
    STATE_STOPPED         // Solid Red (motor stopped)
} system_state_t;

volatile system_state_t current_state = STATE_MANUAL;
volatile bool flash_state = false;
volatile uint32_t flash_timer = 0;

void testAllLEDs(void)
{
// Ensure all LEDs are OFF first
    setPinValue(PORTB, RED_PIN, 0);
    setPinValue(PORTB, ORANGE_PIN, 0);
    setPinValue(PORTB, GREEN_PIN, 0);
    waitMicrosecond(1000);

    // Run the sequence once using a do-while loop
    uint8_t cycles = 0;
    do
    {
        // RED: on then off
        setPinValue(PORTB, RED_PIN, 1);
        waitMicrosecond(100000);
        setPinValue(PORTB, RED_PIN, 0);
        waitMicrosecond(100000);

        // ORANGE: on then off
        setPinValue(PORTB, ORANGE_PIN, 1);
        waitMicrosecond(100000);
        setPinValue(PORTB, ORANGE_PIN, 0);
        waitMicrosecond(100000);

        // GREEN: on then off
        setPinValue(PORTB, GREEN_PIN, 1);
        waitMicrosecond(100000);
        setPinValue(PORTB, GREEN_PIN, 0);
        waitMicrosecond(100000);

        cycles++;
    }
    while (cycles < 1);
}
void initSystem(void)
{
    // Enable Port B
    enablePort(PORTB);

    // Configure LED pins as outputs
    selectPinPushPullOutput(PORTB, RED_PIN);
    selectPinPushPullOutput(PORTB, ORANGE_PIN);
    selectPinPushPullOutput(PORTB, GREEN_PIN);

    // Configure buttons as inputs with pull-up
    selectPinDigitalInput(PORTB, MODE_BUTTON);
    enablePinPullup(PORTB, MODE_BUTTON);
    selectPinDigitalInput(PORTB, STOP_BUTTON);
    enablePinPullup(PORTB, STOP_BUTTON);

    // Test all LEDs on startup
    testAllLEDs();

    // Start in MANUAL mode (solid green)
    current_state = STATE_MANUAL;
    setPinValue(PORTB, GREEN_PIN, 1);
}

bool isModeButtonPressed(void)
{
    return getPinValue(PORTB, MODE_BUTTON) == 0;  // Active LOW
}

bool isStopButtonPressed(void)
{
    return getPinValue(PORTB, STOP_BUTTON) == 0;  // Active LOW
}

void handleStopButton(void)
{
    static bool last_stop_state = true;
    bool current_stop_state = isStopButtonPressed();

    if (current_stop_state != last_stop_state) {
        waitMicrosecond(50);  // Debounce delay

        if (isStopButtonPressed() == false) { // Stop button pressed
            // Turn on RED LED and stop motor
            current_state = STATE_STOPPED;

            // Here you would add code to actually stop your motors:
            // stopLeftMotor();
            // stopRightMotor();
            // disableMotorDrivers();
        }
        last_stop_state = current_stop_state;
    }
}

void handleModeButton(void)
{
    static bool last_mode_state = true;
    bool current_mode_state = isModeButtonPressed();

    // Only process mode button if not in STOPPED state
    if (current_state != STATE_STOPPED && current_mode_state != last_mode_state) {
        waitMicrosecond(50);  // Debounce delay

        if (isModeButtonPressed() == false) { // Mode button pressed
            // Toggle between manual and autonomous
            if (current_state == STATE_MANUAL) {
                current_state = STATE_AUTONOMOUS;
            } else {
                current_state = STATE_MANUAL;
            }
        }
        last_mode_state = current_mode_state;
    }
}

void updateSafetyLight(void)
{
    switch(current_state) {
        case STATE_MANUAL:
            // Solid GREEN for manual mode
            setPinValue(PORTB, RED_PIN, 0);
            setPinValue(PORTB, ORANGE_PIN, 0);
            setPinValue(PORTB, GREEN_PIN, 1);
            break;

        case STATE_AUTONOMOUS:
            // Flashing ORANGE for autonomous mode
            flash_timer++;
            if(flash_timer >= 300) {
                flash_state = !flash_state;
                flash_timer = 0;

                setPinValue(PORTB, ORANGE_PIN, flash_state);
                setPinValue(PORTB, RED_PIN, 0);
                setPinValue(PORTB, GREEN_PIN, 0);
            }
            break;

        case STATE_STOPPED:
            // Solid RED when stop button pressed
            setPinValue(PORTB, RED_PIN, 1);
            setPinValue(PORTB, ORANGE_PIN, 0);
            setPinValue(PORTB, GREEN_PIN, 0);
            break;
    }
}

int main(void)
{
    initSystem();

    while (1) {
        testAllLEDs();

//        handleStopButton();     // Check for stop button first
//        handleModeButton();     // Then check for mode changes
//        updateSafetyLight();    // Update the lights
        waitMicrosecond(100000);            // Main loop delay
    }
}
