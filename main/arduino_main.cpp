/****************************************************************************
Copyright 2021 Ricardo Quesada

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
****************************************************************************/

#include "sdkconfig.h"
#ifndef CONFIG_BLUEPAD32_PLATFORM_ARDUINO
#error "Must only be compiled when using Bluepad32 Arduino platform"
#endif  // !CONFIG_BLUEPAD32_PLATFORM_ARDUINO

#include <Arduino.h>
#include <Bluepad32.h>

#include <ESP32Servo.h>
#include <ESP32SharpIR.h>
#include <QTRSensors.h>

GamepadPtr myGamepads[BP32_MAX_GAMEPADS];
const int plsensor;
const int prsensor;
const int plservo;
const int prservo;

const double bestdistance = 2;
const int maxspd = 180;
const int minspd = 0;

Servo left;
Servo right;
#define model GP2Y0A21YK0F;
ESP32SharpIR leftSensor(GP2Y0A21YK0F,  22);
ESP32SharpIR rightSensor(GP2Y0A21YK0F, 21);

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

// Arduino setup function. Runs in CPU 1
void setup() {
    // Setup the Bluepad32 callbacks
    BP32.setup(&onConnectedGamepad, &onDisconnectedGamepad);
    BP32.forgetBluetoothKeys();

    ESP32PWM::allocateTimer(0);
	ESP32PWM::allocateTimer(1);
	ESP32PWM::allocateTimer(2);
	ESP32PWM::allocateTimer(3);

    // TODO: Write your setup code here
}

// Arduino loop function. Runs in CPU 1
void loop() {
    BP32.update();

    for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
        GamepadPtr myGamepad = myGamepads[i];
        if (myGamepad && myGamepad->isConnected()) {
            // TODO: Write your controller code here

        }
    }

    // TODO: Write your periodic code here
        int zero = maze();
    vTaskDelay(1);
}

int maze(){
    bool avoidBurst = false;
    float leftdist = leftSensor.getDistance(avoidBurst);
    float rightdist = rightSensor.getDistance(avoidBurst);

    //LEFT TURN PROTOCOL
    if(leftdist-rightdist >= 0.5){
        turnleft();
        delay(20);
        maze();
    }

    if(rightdist-leftdist>=.5){
        turnright();
        delay(20);
        maze();
    }

    //FORWARD PROTOCOL
    if(abs(leftdist-rightdist) < 0.5){
        forward();
        delay(250);
        maze();
    }
    return 0;
}

void forward(){
    left.write(maxspd);
    right.write(maxspd);
}

void turnleft(){
    left.write(maxspd);
    right.write(maxspd*.5);
}

void turnright(){
    left.write(maxspd*.5);
    right.write(maxspd);
}