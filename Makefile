I2C_MODE = RPI
I2C_LIBS = -lbcm2835
SRC_DIR = examples/src/
BUILD_DIR = examples/
LIB_DIR = $(SRC_DIR)lib/

examples = test main rawrgb step fbuf interp video hotspot sdlscale getcurrent temp_csv_log
examples_objects = $(addsuffix .o,$(addprefix $(SRC_DIR), $(examples)))
examples_output = $(addprefix $(BUILD_DIR), $(examples))

ifeq ($(I2C_MODE), LINUX)
	I2C_LIBS =
endif

all: examples

examples: $(examples_output)

libMLX90640_API.so: functions/MLX90640_API.o functions/$(I2C_MODE)_I2C_Driver.o
	$(CXX) -fPIC -shared $^ -o $@ $(I2C_LIBS)

libMLX90640_API.a: functions/MLX90640_API.o functions/$(I2C_MODE)_I2C_Driver.o
	ar rcs $@ $^
	ranlib $@

libINA219_API.so: functions/INA219_API.o functions/$(I2C_MODE)_I2C_Driver.o
	$(CXX) -fPIC -shared $^ -o $@ $(I2C_LIBS)

libINA219_API.a: functions/INA219_API.o functions/$(I2C_MODE)_I2C_Driver.o
	ar rcs $@ $^
	ranlib $@

functions/INA219_API.o functions/MLX90640_API.o functions/RPI_I2C_Driver.o functions/LINUX_I2C_Driver.o : CXXFLAGS+=-fPIC -I headers -shared $(I2C_LIBS)

$(examples_objects) : CXXFLAGS+=-std=c++11

$(examples_output) : CXXFLAGS+=-I. -std=c++11

examples/src/lib/interpolate.o : CC=$(CXX) -std=c++11

examples/src/sdlscale.o : CXXFLAGS+=`sdl2-config --cflags --libs`

$(BUILD_DIR)sdlscale: $(SRC_DIR)sdlscale.o libMLX90640_API.a
	$(CXX) -L/home/pi/mlx90640-library $^ -o $@ $(I2C_LIBS) `sdl2-config --libs`

$(BUILD_DIR)hotspot: $(SRC_DIR)hotspot.o $(LIB_DIR)fb.o libMLX90640_API.a
	$(CXX) -L/home/pi/mlx90640-library $^ -o $@ $(I2C_LIBS)

$(BUILD_DIR)test: $(SRC_DIR)test.o libMLX90640_API.a libINA219_API.a
	$(CXX) -L/home/pi/mlx90640-library $^ -o $@ $(I2C_LIBS)

$(BUILD_DIR)temp_csv_log: $(SRC_DIR)temp_csv_log.o libMLX90640_API.a libINA219_API.a
	$(CXX) -L/home/pi/mlx90640-library $^ -o $@ $(I2C_LIBS)

$(BUILD_DIR)main: $(SRC_DIR)main.o libMLX90640_API.a libINA219_API.a
	$(CXX) -pthread -L/home/pi/mlx90640-library $^ -o $@ $(I2C_LIBS)

$(BUILD_DIR)getcurrent: $(SRC_DIR)getcurrent.o libMLX90640_API.a libINA219_API.a
	$(CXX) -L/home/pi/mlx90640-library $^ -o $@ $(I2C_LIBS)

$(BUILD_DIR)rawrgb: $(SRC_DIR)rawrgb.o libMLX90640_API.a
	$(CXX) -L/home/pi/mlx90640-library $^ -o $@ $(I2C_LIBS)

$(BUILD_DIR)step: $(SRC_DIR)step.o libMLX90640_API.a
	$(CXX) -L/home/pi/mlx90640-library $^ -o $@ $(I2C_LIBS)

$(BUILD_DIR)fbuf: $(SRC_DIR)fbuf.o $(LIB_DIR)fb.o libMLX90640_API.a
	$(CXX) -L/home/pi/mlx90640-library $^ -o $@ $(I2C_LIBS)

$(BUILD_DIR)interp: $(SRC_DIR)interp.o $(LIB_DIR)interpolate.o $(LIB_DIR)fb.o libMLX90640_API.a
	$(CXX) -L/home/pi/mlx90640-library $^ -o $@ $(I2C_LIBS)

$(BUILD_DIR)video: $(SRC_DIR)video.o $(LIB_DIR)fb.o libMLX90640_API.a
	$(CXX) -L/home/pi/mlx90640-library $^ -o $@ $(I2C_LIBS) -lavcodec -lavutil -lavformat -lbcm2835

bcm2835-1.55.tar.gz:	
	wget http://www.airspayce.com/mikem/bcm2835/bcm2835-1.55.tar.gz

bcm2835-1.55: bcm2835-1.55.tar.gz
	tar xzvf bcm2835-1.55.tar.gz

bcm2835: bcm2835-1.55
	cd bcm2835-1.55; ./configure; make; sudo make install

clean:
	rm -f $(examples_output)
	rm -f $(SRC_DIR)*.o
	rm -f $(LIB_DIR)*.o
	rm -f functions/*.o
	rm -f *.o
	rm -f *.so
	rm -f *.a
