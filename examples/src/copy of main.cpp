//Modified from test.cpp example

#include <stdint.h>
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

std::atomic_bool stop = false;
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

        //
    
        //append data to csv
        time (&rawtime);
        fout << rawtime << ", " << mlx90640To[760]<< "\n";
    }
}

int main(){
    //initializing
    printf("Starting...\n");
    static uint16_t eeMLX90640[832];
    float emissivity = 0.85;
    uint16_t frame[834];
    float eTa;
    static float mlx90640To[768];
    
    MLX90640_SetDeviceMode(MLX_I2C_ADDR, 0);
    MLX90640_SetSubPageRepeat(MLX_I2C_ADDR, 0);
    MLX90640_SetRefreshRate(MLX_I2C_ADDR, 0b010);
    MLX90640_SetChessMode(MLX_I2C_ADDR);
    //MLX90640_SetSubPage(MLX_I2C_ADDR, 0);
    printf("Configured...\n");

    paramsMLX90640 mlx90640;
    MLX90640_DumpEE(MLX_I2C_ADDR, eeMLX90640);
    MLX90640_ExtractParameters(eeMLX90640, &mlx90640);

    int refresh = MLX90640_GetRefreshRate(MLX_I2C_ADDR);
    printf("EE Dumped...\n");

    //Initialize INA219
    Adafruit_INA219 ina219;
    ina219.init();

    //Add date&time in filename
    time_t rawtime ;
    time ( &rawtime ) ;
    
    const tm* timeinfo = localtime ( &rawtime ) ;
    char ddMONtime[18] ;
    strftime( ddMONtime, sizeof(ddMONtime), "%d%b%H%M", timeinfo ) ;
    const std::string path_suffix = "PowerAndTemperatureLog.csv" ;
    const std::string file_name = ddMONtime +  path_suffix ;
    std::fstream fout;
    fout.open(file_name, std::ios::out | std::ios::app);
    

    
    //while (1){
    for (int i = 0; i < 100; i++){
        
        //get data from MLX90640
        MLX90640_GetFrameData(MLX_I2C_ADDR, frame);
        //MLX90640_InterpolateOutliers(frame, eeMLX90640);
        eTa = MLX90640_GetTa(frame, &mlx90640);
        MLX90640_CalculateTo(frame, &mlx90640, emissivity, eTa, mlx90640To);

        MLX90640_BadPixelsCorrection((&mlx90640)->brokenPixels, mlx90640To, 1, &mlx90640);
        MLX90640_BadPixelsCorrection((&mlx90640)->outlierPixels, mlx90640To, 1, &mlx90640);


    
        //append data to csv
        time (&rawtime);
        fout << rawtime << ", " << mlx90640To[760]<< "\n";
    }
    
    fout.close(); 

    return 0;
}