// Bare minimum code for spinning motors triggered by controller input

// Assumes servo is connected to pin 15

// Assumes motor controller IN1 and IN2 are connected to pins 14 and 12

#include "sdkconfig.h"
#ifndef CONFIG_BLUEPAD32_PLATFORM_ARDUINO
#error "Must only be compiled when using Bluepad32 Arduino platform"
#endif  !CONFIG_BLUEPAD32_PLATFORM_ARDUINO
#include <Arduino.h>
#include <Bluepad32.h>
#include <ESP32Servo.h>
#include <bits/stdc++.h>


#define IN1 12
#define IN2 14

Servo servo;

GamepadPtr myGamepads[BP32_MAX_GAMEPADS];

// This callback gets called any time a new gamepad is connected.
void onConnectedGamepad(GamepadPtr gp) {
    bool foundEmptySlot = false;
    for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
        if (myGamepads[i] == nullptr) {
            myGamepads[i] = gp;
            foundEmptySlot = true;
            break;
        }
    }
}

void onDisconnectedGamepad(GamepadPtr gp) {
    bool foundGamepad = false;
    for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
        if (myGamepads[i] == gp) {
            myGamepads[i] = nullptr;
            foundGamepad = true;
            break;
        }
    }
}

void setup() {
    BP32.setup(&onConnectedGamepad, &onDisconnectedGamepad);
    BP32.forgetBluetoothKeys();
 
    servo.attach(15);

    // motor controller outputs
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
   
    Serial.begin(115200);
}

void loop() {
    BP32.update();
    for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
        GamepadPtr controller = myGamepads[i];
        if (controller && controller->isConnected()) {
           
            if (controller->l1() == 1) {
                Serial.print("Servo move");
                servo.write(1000);
            }
            if (controller->l1() == 0) {
                Serial.print("Servo stop");
                servo.write(1500);
            }

            if(controller->axisRY() > 0) { // negative y is upward on stick
                Serial.println(" DC motor move");
                digitalWrite(IN1, LOW);
                digitalWrite(IN2, HIGH);
            }
            if(controller->axisRY() == 0) { // stop motor 1
                Serial.println(" DC motor stop");
                digitalWrite(IN1, LOW);
                digitalWrite(IN2, LOW);
            }

            // PHYSICAL BUTTON A
            if (controller->b()) {
                Serial.println("button a pressed");
            }

        }
        vTaskDelay(1);
    }
}            



