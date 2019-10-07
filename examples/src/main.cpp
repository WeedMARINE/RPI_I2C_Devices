//Modified from test.cpp example

#include <stdint.h>
#include <math.h>
#include <iostream>
#include <cstring>
#include <fstream>
#include <chrono>
#include <thread>
#include <atomic>
#include <ctime>
#include "headers/MLX90640_API.h"
#include "headers/INA219_API.h"

#define MLX_I2C_ADDR 0x33
#define INA_I2C_ADDR 0x40

//atomic_boolean for stoping loop thread
std::atomic_bool stop = { false };

//declaration of global parameters
static uint16_t eeMLX90640[832];
paramsMLX90640 mlx90640;
float emissivity = 0.85;
uint16_t frame[834];
float eTa = 0;
static float mlx90640To[768];
time_t rawtime ;

Adafruit_INA219 ina219;
float shuntvoltage = 0;
float busvoltage = 0;
float current_mA = 0;
float loadvoltage = 0;
float power_mW = 0;

std::fstream fout;

//Loop function
void loop() {
    while(!stop)
    {
        // loop body here
        
        //get data from MLX90640
        MLX90640_GetFrameData(MLX_I2C_ADDR, frame);
        //MLX90640_InterpolateOutliers(frame, eeMLX90640);
        eTa = MLX90640_GetTa(frame, &mlx90640);
        MLX90640_CalculateTo(frame, &mlx90640, emissivity, eTa, mlx90640To);

        MLX90640_BadPixelsCorrection((&mlx90640)->brokenPixels, mlx90640To, 1, &mlx90640);
        MLX90640_BadPixelsCorrection((&mlx90640)->outlierPixels, mlx90640To, 1, &mlx90640);

        //get data from INA219
        shuntvoltage = ina219.getShuntVoltage_mV();
        busvoltage = ina219.getBusVoltage_V();
        current_mA = ina219.getCurrent_mA();
        power_mW = ina219.getPower_mW();
        loadvoltage = busvoltage + (shuntvoltage / 1000);

        //round out put for logging
        int decimalP = 100; //100 for 2 decimal places
        float nearest_To = roundf(mlx90640To[760] * decimalP) / decimalP;
        float nearest_V = roundf(loadvoltage * decimalP) / decimalP;
        float nearest_I = roundf(current_mA * decimalP) / decimalP;
        float nearest_P = roundf(power_mW * decimalP) / decimalP;

        //append data to csv
        time (&rawtime);
        fout << rawtime << ", " << nearest_To<< ", " << nearest_V<< ", " << nearest_I<< ", " << nearest_P<< "\n";
    }
}

int main(){
    //initializing
    printf("Starting...\n");
    
    MLX90640_SetDeviceMode(MLX_I2C_ADDR, 0);
    MLX90640_SetSubPageRepeat(MLX_I2C_ADDR, 0);
    MLX90640_SetRefreshRate(MLX_I2C_ADDR, 0b010);
    MLX90640_SetChessMode(MLX_I2C_ADDR);
    //MLX90640_SetSubPage(MLX_I2C_ADDR, 0);
    printf("MLX90640 Configured...\n");

    MLX90640_DumpEE(MLX_I2C_ADDR, eeMLX90640);
    MLX90640_ExtractParameters(eeMLX90640, &mlx90640);

    int refresh = MLX90640_GetRefreshRate(MLX_I2C_ADDR);
    printf("MLX90640 EE Dumped...\n");

    //Initialize INA219
    ina219.init();
    printf("INA219 Initialized\n");

    //Add DateTime to file name
    time ( &rawtime );
    const tm* timeinfo = localtime ( &rawtime ) ;
    char ddMONtime[18] ;
    strftime( ddMONtime, sizeof(ddMONtime), "%d%b%H%M", timeinfo ) ;
    const std::string path_suffix = "PowerAndTemperatureLog.csv" ;
    const std::string file_name = ddMONtime +  path_suffix ;
    fout.open(file_name, std::ios::out | std::ios::app);
    
    //Loop in another thread
    std::thread t(loop); // Separate thread for loop.

    // Wait for input character (this will suspend the main thread, but the loop
    // thread will keep running).
    std::cout<<"Press any key and press ENTER to terminate logging\n";
    std::cin.get();

    // Set the atomic boolean to true. The loop thread will exit from 
    // loop and terminate.
    stop = true;

    t.join();
    
    fout.close(); 

    return 0;
}