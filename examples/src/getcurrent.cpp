//Modified from test.cpp example

#include <stdint.h>
#include <iostream>
#include <cstring>
#include <fstream>
#include <chrono>
#include <thread>
#include <ctime>
//#include "headers/MLX90640_API.h"
#include "headers/INA219_API.h"

#define INA_I2C_ADDR 0x40
Adafruit_INA219 ina219;

int main(){
    uint32_t currentFrequency;
    
  printf("Hello!");
  
  // Initialize the INA219.
  // By default the initialization will use the largest range (32V, 2A).  However
  // you can call a setCalibration function to change this range (see comments).
  ina219.init();
  // To use a slightly lower 32V, 1A range (higher precision on amps):
  //ina219.setCalibration_32V_1A();
  // Or to use a lower 16V, 400mA range (higher precision on volts and amps):
  //ina219.setCalibration_16V_400mA();

  printf("Measuring voltage and current with INA219 ...");
    
    while (1){
        float shuntvoltage = 0;
        float busvoltage = 0;
        float current_mA = 0;
        float loadvoltage = 0;
        float power_mW = 0;

        shuntvoltage = ina219.getShuntVoltage_mV();
        busvoltage = ina219.getBusVoltage_V();
        current_mA = ina219.getCurrent_mA();
        power_mW = ina219.getPower_mW();
        loadvoltage = busvoltage + (shuntvoltage / 1000);
  
        printf("Bus Voltage:   "); printf("%.6f", busvoltage); printf(" V\n");
        printf("Shunt Voltage: "); printf("%.6f", shuntvoltage); printf(" mV\n");
        printf("Load Voltage:  "); printf("%.6f", loadvoltage); printf(" V\n");
        printf("Current:       "); printf("%.6f", current_mA); printf(" mA\n");
        printf("Power:         "); printf("%.6f", power_mW); printf(" mW\n");
    }

    return 0;
}