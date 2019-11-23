cmake_minimum_required(VERSION 3.8)
#set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/cmake/ArduinoToolchain.cmake)
#
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/libraries/Arduino-CMake-NG/cmake/Arduino-Toolchain.cmake)
set(CMAKE_CXX_STANDARD 17)
set(PROJECT_NAME helloworld)

set(BOARD nano)
set(${PROJECT_NAME}_BOARD nano)
set(ARDUINO_CPU atmega328old)
project(${PROJECT_NAME} LANGUAGES C CXX ASM)

# Define additional source and header files or default arduino sketch files
# set(${PROJECT_NAME}_SRCS)
#set(${PROJECT_NAME}_HDRS User_Setup.h)

### Additional static libraries to include in the target.
# set(${PROJECT_NAME}_LIBS)

### Main sketch file
set(${PROJECT_NAME}_SKETCH helloworld.ino)

### Add project directories into the build
# add_subdirectory()

### Additional settings to add non-standard or your own Arduino libraries.
# For this example (libs will contain additional arduino libraries)
# An Arduino library my_lib will contain files in libs/my_lib/: my_lib.h, my_lib.cpp + any other cpp files
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/)


# For nested library sources replace ${LIB_NAME} with library name for each library
# set(_RECURSE true)

#### Additional settings for programmer. From programmers.txt
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


add_arduino_executable(${PROJECT_NAME} ${board_id} src/helloworld.cpp)
upload_arduino_target(${PROJECT_NAME} "${board_id}" /dev/cu.usbserial-1430)


set(Adafruit_BME280_SOURCES
        libraries/Adafruit_BME280/Adafruit_BME280.h
        libraries/Adafruit_BME280/Adafruit_BME280.cpp
        )

set(MHZ19_UART_SOURCES
        libraries/mhz19_uart/src/MHZ19_uart.h
        libraries/mhz19_uart/src/MHZ19_uart.cpp
        )

set(RTC_LIB_SOURCES
        libraries/RTClib/RTClib.h
        libraries/RTClib/RTClib.cpp
        )

add_arduino_header_only_library(adafruit_sensor ${board_id} libraries/Adafruit_Sensor/Adafruit_Sensor.h)
add_arduino_library(Adafruit_BME280 ${board_id} ${Adafruit_BME280_SOURCES})
add_arduino_library(MHZ19_UART ${board_id} ${MHZ19_UART_SOURCES})
add_arduino_library(RTC ${board_id} ${RTC_LIB_SOURCES})

link_platform_library(Adafruit_BME280 Wire ${board_id})
link_platform_library(MHZ19_UART SoftwareSerial ${board_id})

link_arduino_library(Adafruit_BME280 adafruit_sensor ${board_id} HEADER_ONLY)

link_arduino_library(${PROJECT_NAME} Adafruit_BME280 ${board_id})
link_arduino_library(${PROJECT_NAME} MHZ19_UART ${board_id})
link_arduino_library(${PROJECT_NAME} RTC ${board_id})