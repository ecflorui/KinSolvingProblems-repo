//To use this program and the TCS34725 in general, connect the color sensor pins VIN, GND, SDA, and SCL to the ESP32 pins 
//3V3, GND, GPIO21, and GPIO22 respectively. Doing so sets up the I2C connection. 

#include "sdkconfig.h"
#ifndef CONFIG_BLUEPAD32_PLATFORM_ARDUINO
#error "Must only be compiled when using Bluepad32 Arduino platform"
#endif  // !CONFIG_BLUEPAD32_PLATFORM_ARDUINO

#include <Arduino.h>
#include <Bluepad32.h>

#include <ESP32Servo.h>
#include <ESP32SharpIR.h>
#include <QTRSensors.h>

#include <Arduino_APDS9960.h>
#include <bits/stdc++.h>

#define APDS9960_INT 2
#define I2C_SDA 21
#define I2C_SCL 22
#define I2C_FREQ 100000

#define MODE 1 //Mode 1 is used to test get color using sensor, Mode 2 is used to test color detection

TwoWire I2C_0 = TwoWire(0);
APDS9960 apds = APDS9960(I2C_0, APDS9960_INT);


int detect() {
    int r, g, b, a;
    // Wait until color is read from the sensor 
    while (!apds.colorAvailable()) { delay(5); }
    apds.readColor(r, g, b, a);
    // Read color from sensor apds.readColor(r, g, b, a);
    // Print color in decimal 
    Serial.print("RED: ");
    Serial.print(r);
    Serial.print(" GREEN: ");
    Serial.print(g);
    Serial.print(" BLUE: ");
    Serial.print(b);
    Serial.print(" AMBIENT: ");
    Serial.println(a);
    delay(100);
    return (0);
}

int compare() {
    int r, g, b, a;
    apds.readColor(r, g, b, a);
    //set variable to whatever we need to compared the detected values with
    int rc, gc, bc, ac;
    rc = 100;
    gc = 100;
    bc = 100;
    ac = 100;
    //set a margin of acceptability
    int mar = 25;

    if ((abs(rc-r) <= mar) && (abs(bc-b) <= mar) && (abs(gc-g) <= mar)) {
        Serial.print("Color Detected");
    }

    delay(100);
    return 0;
}

void setup() {
    //sets up I2C protocol; no need to worry about how it works, just used to pull info from the color sensor
    I2C_0.begin(I2C_SDA, I2C_SCL, I2C_FREQ);

    //sets up color sensor
    apds.setInterruptPin(APDS9960_INT);
    apds.begin();
    Serial.begin(115200);
}

void loop() {
   if  (MODE == 1) {
    detect();
   }

   if (MODE == 2) {
    compare();
   }
}
