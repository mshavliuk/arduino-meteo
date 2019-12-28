cmake_minimum_required(VERSION 3.8)
#set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/cmake/ArduinoToolchain.cmake)
#
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/libraries/Arduino-CMake-NG/cmake/Arduino-Toolchain.cmake)
set(CMAKE_CXX_STANDARD 11)
set(PROJECT_NAME meteo)

set(BOARD nano)
set(${PROJECT_NAME}_BOARD nano)
set(ARDUINO_CPU atmega328old)

option(DEBUG_OUTPUT "Debug output" OFF)
IF (DEBUG_OUTPUT)
    add_compile_definitions(DEBUG true)
ENDIF (DEBUG_OUTPUT)


project(${PROJECT_NAME} LANGUAGES C CXX)

# Define additional source and header files or default arduino sketch files
# set(${PROJECT_NAME}_SRCS)
#set(${PROJECT_NAME}_HDRS User_Setup.h)

### Additional static libraries to include in the target.
# set(${PROJECT_NAME}_LIBS)

### Add project directories into the build
# add_subdirectory()

### Additional settings to add non-standard or your own Arduino libraries.
# For this example (libs will contain additional arduino libraries)
# An Arduino library my_lib will contain files in libs/my_lib/: my_lib.h, my_lib.cpp + any other cpp files
#link_directories(${CMAKE_CURRENT_SOURCE_DIR}/)


# For nested library sources replace ${LIB_NAME} with library name for each library
# set(_RECURSE true)

#### Additional settings for programmer. From programmers.txtsadf
# set(${PROJECT_NAME}_PROGRAMMER avrispmkii)
# set(${PROJECT_NAME}_PORT /dev/cu.usbserial-00000000)
set(nano.upload.speed 9600)


# Call a framework utility function, passing it information about the hardware board that will
# be used - This function returns a structure known only


# Create an executable suitable for the Arduino firmware using CMake-style target-creation
#add_arduino_executable(${PROJECT_NAME} ${board_id} cmake-build-debug/helloworld_helloworld.ino.cpp)
# Upload the created target through a connected Serial Port (Where your board is connected to)
#upload_arduino_target(${PROJECT_NAME} "${board_id}" /dev/cu.usbserial-1430)

## Verbose build process
set(${PROJECT_NAME}_AFLAGS -v)
#generate_arduino_firmware(${PROJECT_NAME})

get_board_id(board_id ${BOARD} ${ARDUINO_CPU})

file(GLOB SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp ${PROJECT_SOURCE_DIR}/src/**/*.cpp)
add_arduino_executable(${PROJECT_NAME} ${board_id} ${SOURCES})

add_arduino_library(BME280 ${board_id}
        libraries/Adafruit_BME280/Adafruit_BME280.h
        libraries/Adafruit_BME280/Adafruit_BME280.cpp)
add_arduino_header_only_library(adafruit_sensor ${board_id} libraries/Adafruit_Sensor/Adafruit_Sensor.h)
link_arduino_library(BME280 adafruit_sensor ${board_id} HEADER_ONLY)
link_arduino_library(${PROJECT_NAME} BME280 ${board_id})


add_arduino_library(MHZ19 ${board_id}
        libraries/mhz19_uart/src/MHZ19_uart.h
        libraries/mhz19_uart/src/MHZ19_uart.cpp)
link_platform_library(MHZ19 SoftwareSerial ${board_id})
link_arduino_library(${PROJECT_NAME} MHZ19 ${board_id})

add_arduino_library(RTC ${board_id}
        libraries/RTClib/RTClib.h
        libraries/RTClib/RTClib.cpp)
link_arduino_library(${PROJECT_NAME} RTC ${board_id})

add_arduino_library(STL ${board_id}
        libraries/ArduinoSTL/src/ArduinoSTL.h
        libraries/ArduinoSTL/src/vector.cpp
        libraries/ArduinoSTL/src/array
        libraries/ArduinoSTL/src/func_exception.cpp)
link_arduino_library(${PROJECT_NAME} STL ${board_id})

add_arduino_library(FREE_MEM ${board_id}
        libraries/MemoryFree/MemoryFree.h
        libraries/MemoryFree/MemoryFree.cpp
        )
link_arduino_library(${PROJECT_NAME} FREE_MEM ${board_id})

add_arduino_library(GYVER_BUTTON ${board_id}
        libraries/GyverButton/GyverButton.h
        libraries/GyverButton/GyverButton.cpp
        )
link_arduino_library(${PROJECT_NAME} GYVER_BUTTON ${board_id})

add_arduino_library(TIMER ${board_id}
        libraries/GyverTimer/GyverTimer.h
        libraries/GyverTimer/GyverTimer.cpp)
link_arduino_library(${PROJECT_NAME} TIMER ${board_id})

add_arduino_library(LCD ${board_id}
        libraries/LiquidCrystal_I2C/LiquidCrystal_I2C.h
        libraries/LiquidCrystal_I2C/LiquidCrystal_I2C.cpp)
link_arduino_library(${PROJECT_NAME} LCD ${board_id})


link_platform_library(${PROJECT_NAME} EEPROM ${board_id})
link_platform_library(${PROJECT_NAME} Wire ${board_id})

upload_arduino_target(${PROJECT_NAME} "${board_id}" /dev/cu.usbserial-143230)