# RPI_I2C_Devices
A modification of Pimoroni's MLX90640 library for connecting I2C devices on Raspberry Pi


## Raspberry Pi Users

** EXPERIMENTAL **

This port uses either generic Linux I2C or the  bcm2835 library.

### Generic Linux I2C Mode

Make sure the Linux I2C dev library is installed:

```text
sudo apt-get install libi2c-dev
```

To get the best out of your sensor you should modify `/boot/config.txt` and change your I2C baudrate.

The fastest rate recommended for compatibility with other sensors is 400kHz. This is compatible with SMBus devices:

```text
dtparam=i2c1_baudrate=400000
```

This will give you a framerate of - at most - 8FPS.

If you're just using the MLX90640 and, for example, the 1.12" OLED, you can safely use 1MHz:

```text
dtparam=i2c1_baudrate=1000000
```

This will give you a framerate of - at most - 32FPS.

Now build the MLX90640 library and examples in LINUX I2C mode:

```text
make clean
make I2C_MODE=LINUX
```

### BCM2835 Library Mode

** NOT WOKING ON RPI4 (tested on 10/7/2019) **

To use the bcm2835 library, install like so:


```text
make bcm2835
```

Or, step by step:

```text
wget http://www.airspayce.com/mikem/bcm2835/bcm2835-1.55.tar.gz
tar xvfz bcm2835-1.55.tar.gz
cd bcm2835-1.55
./configure
make
sudo make install
```
# mlx90640-library
MLX90640 library functions

### Dependencies

libav for `video` example:

```text
sudo apt-get install libavutil-dev libavcodec-dev libavformat-dev
```

SDL2 for `sdlscale` example:

```text
sudo apt install libsdl2-dev
```

Then just `make examples/<examplename>` and `sudo examples/<exampleame>` for one of the examples listed below:

# mlx90640 Examples
# 1.fbuf

```
make examples/fbuf
sudo examples/fbuf
```

This example uses direct-to-framebuffer rendering and black-blue-green-yellow-red-purple-white false colouring.

If you gave issues with the output image, set "`IMAGE_SCALE`" to a smaller number.

# 2.interp

```
make examples/interp
sudo examples/interp
```

This example uses direct-to-framebuffer rendering and black-blue-green-yellow-red-purple-white false colouring.

It also has 2x bicubic resize filter.

If you have issues with the output image, set "`IMAGE_SCALE`" to a smaller number.

# 3.test

```
make examples/test
sudo examples/test
```

This example draws out to the console using ANSI colours and the full block char.

To see the actual temperature values, change "`FMT_STRING`" from the block char to the float format.

# 4.step

```
make examples/step
sudo examples/step
```

Attempt to run in step by step mode (experimental)

# 5.sdlscale

Displays the MLX90640 sensor full-screen using hardware acceleration in SDL2.

Hit Spacebar to change from aspect-ratio correct to full-screen-stretched modes.

Hit Escape to exit.

```
make examples/sdlscale
sudo examples/sdlscale
```

Requires SDL2 libraries:

```
sudo apt install libsdl2-dev
```

On Raspbian Lite you may wish to build SDL2 from source with X support disabled to avoid pulling in ~200MB of dependencies. Before configuring/compiling ensure you have `libudev-dev` installed for input support.

# 6.temp_csv_log

```
make examples/temp_csv_log
sudo examples/temp_csv_log
```

This example log temperature of a pixel with timestamp in csv format

# INA219-library
A port of Adafruit INA219 Arduino library

# INA219 Examples
# 1.getcurrent

```
make examples/getcurrent
sudo examples/getcurrent
```

This example read voltage, current and power from INA219 registry and display them in the Terminal.

# 1.main

```
make examples/main
sudo examples/main
```

This example log voltage, current, power and temperature pixels with time stamp in a uniquely named csv file.

