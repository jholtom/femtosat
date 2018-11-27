/**
 * Femtosat full program
 * Reads relevant data from light, pressure, and inertial sensors and transmits it via the RFM69
 */



#include <Wire.h>
#include <stdint.h>
#include <RFM69.h>
#include <SPI.h>
#define NETWORKID     0
#define MYNODEID      1
#define TONODEID      2
#define FREQUENCY     RF69_915MHZ

#include "SparkFunBME280.h"
#include "SparkFunISL29125.h"
#include "SparkFunMPU9250-DMP.h"
MPU9250_DMP imuSensor;
SFE_ISL29125 lightSensor;
BME280 pressureSensor;
RFM69 radio;

#define SERIAL_BAUD   115200
#define LED_PIN       3
#define SERIAL_VALUES


 /**
  * Frame definition:
  *   The entire frame is transmitted in ASCII hex, space delimited
  *   This allows it to be easily viewed with a text editor.
  *   Frame type 1:
  *   | 'JACOB' | count (2) | 'T' | time in ms (8) | 'F' | 'A' | 'B' | altitude (4) | Temperature (4) | 'L' | Red (4) | Green (4) | Blue (4) | 'WILLIS' |
  *
  *   Frame type 2:
  *   | 'JACOB' | count (2) | 'F' | 'I' | 'A' | Accel X (4) | Accel Y (4) | Accel Z (4) | 'G' | Gyro X (4) | Gyro Y (4) | Gyro Z (4) | 'M' | Mag X (4) | Mag Y (4) | Mag Z (4) | 'WILLIS' | // 42 Bytes
  */

  // Frame 2, IMU
#define IMU_FRAME_BYTES 54
typedef union {
  // 54 bytes total
  struct {
    char firstName[5];
    uint16_t count;
    char frameTypeID;
    uint8_t frameType;
    char accelTypeID;
    uint32_t accelX;
    uint32_t accelY;
    uint32_t accelZ;
    char gyroTypeID;
    uint32_t gyroX;
    uint32_t gyroY;
    uint32_t gyroZ;
    char magTypeID;
    uint32_t magX;
    uint32_t magY;
    uint32_t magZ;
    char lastName[6];
  };
  uint8_t tx_array[IMU_FRAME_BYTES];
} imuFrame_u;

void imuFrame_init(imuFrame_u *frame) {
  frame->firstName[0] = 'J';  
  frame->firstName[1] = 'A';
  frame->firstName[2] = 'C';
  frame->firstName[3] = 'O';
  frame->firstName[4] = 'B';
  frame->count = 0;
  frame->frameTypeID = 'F';
  frame->frameType = 2;
  frame->accelTypeID = 'A';
  frame->accelX = 0;
  frame->accelY = 0;
  frame->accelZ = 0;
  frame->gyroTypeID = 'G';
  frame->gyroX = 0;
  frame->gyroY = 0;
  frame->gyroZ = 0;
  frame->magTypeID = 'M';
  frame->magX = 0;
  frame->magY = 0;
  frame->magZ = 0;
  frame->lastName[0] = 'W';
  frame->lastName[1] = 'I';
  frame->lastName[2] = 'L';
  frame->lastName[3] = 'L';
  frame->lastName[4] = 'I';
  frame->lastName[5] = 'S';
  }

// Frame 1, acceleration and light sensor
#define ACCEL_LIGHT_FRAME_BYTES 46
typedef union {
  // 46 bytes total
  struct {
    char firstName[5];
    uint16_t count;
    char timeFieldID;
    uint64_t time_ms;
    char frameTypeID;
    char frameType;
    char baroTypeID;
    float altitude;
    float temp;
    char lightTypeID;
    uint32_t red;
    uint32_t green;
    uint32_t blue;
    char lastName[6];
  };
  uint8_t tx_array[ACCEL_LIGHT_FRAME_BYTES];
} accelLightFrame_u;

void accelLightFrame_init(accelLightFrame_u *frame) {
  frame->firstName[0] = 'J';  
  frame->firstName[1] = 'A';
  frame->firstName[2] = 'C';
  frame->firstName[3] = 'O';
  frame->firstName[4] = 'B';
  frame->count = 0;
  frame->timeFieldID = 'T';
  frame->time_ms = 0;
  frame->frameTypeID = 'F';
  frame->frameType = 1;
  frame->baroTypeID = 'B';
  frame->altitude = 0;
  frame->temp = 0;
  frame->lightTypeID = 'L';
  frame->red = 0;
  frame->green = 0;
  frame->blue = 0;
  frame->lastName[0] = 'W';
  frame->lastName[1] = 'I';
  frame->lastName[2] = 'L';
  frame->lastName[3] = 'L';
  frame->lastName[4] = 'I';
  frame->lastName[5] = 'S';
}


accelLightFrame_u accelLightFrame_g;
imuFrame_u imuFrame;


unsigned short tx_count = 0;

void setup() {
  /* create tx frames */
  altitudeLightFrame_init(&accelLightFrame_g);
  imuFrame_init(&imuFrame_g);

  /* initialize LED */
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  /* initialize serial */
  Serial.begin(SERIAL_BAUD);
  Serial.println("Serial Intialized");

  /* initialize pressure sensor */
  pressureSensor.setI2CAddress(0x76);
  if (pressureSensor.beginI2C() == false)
  {
    Serial.println("Pressure sensor failed to communicate");
  }
  else
  {
    pressureSensor.setFilter(3); // HW filter to reduce affect of wind, etc
    pressureSensor.setTempOverSample(5); //Set oversample to 16
    pressureSensor.setPressureOverSample(5); //Set oversample to 16
    pressureSensor.setHumidityOverSample(0); //Turn off humidity sensing

    Serial.println("Pressure Sensor Initialization Successful");
  }

  /* Initialize light sensor */
  if (lightSensor.init())
  {
    Serial.println("Light Sensor Initialization Successful");
  }
  else
  {
    Serial.println("Light Sensor Initialization Failed");
  }

  /* Initialize inertial measurement unit */
  if (imuSensor.begin() != INV_SUCCESS) {
    Serial.println("IMU Initialization Failed");
  }
  else
  {
    imuSensor.setSensors(INV_XYZ_GYRO | INV_XYZ_ACCEL | INV_XYZ_COMPASS); // Enable all sensors:
    imuSensor.setGyroFSR(2000); // Set gyro to 2000 dps
    imuSensor.setAccelFSR(16); // Set accel to +/-16g (maximum)
    imuSensor.setLPF(188); // Set LPF corner frequency to 188Hz
    imuSensor.setSampleRate(1000); // Set sample rate to max
    imuSensor.setCompassSampleRate(100); // Set mag rate to 10Hz
  }

  /* Initialize the Radio */
    radio.initialize(FREQUENCY, MYNODEID, NETWORKID);
    radio.setHighPower();

  #ifdef SERIAL_VALUES
    Serial.println("Count, Time, Altitude, Temperature, Red, Blue, Green, ");
  #endif
}

void loop() {
  /* LED On */
  digitalWrite(LED_PIN, HIGH);
  /* update time */
  unsigned long time_ms = millis();
  /* read values from pressure sensor */
  float ps_altitude_f = pressureSensor.readFloatAltitudeMeters();
  float ps_temp_f = pressureSensor.readTempC();

  //int ps_altitude = (int)*((int*)&pressureSensor.readFloatAltitudeMeters());
  //int ps_temp = (int)*((int*)&pressureSensor.readTempC());

  /* read values from light sensor */
  int ls_red = lightSensor.readRed();
  int ls_blu = lightSensor.readBlue();
  int ls_gre = lightSensor.readGreen();

  /* read values from imu */
  imuSensor.update(UPDATE_ACCEL | UPDATE_GYRO | UPDATE_COMPASS);

  /* LED Off while transmitting */
  digitalWrite(LED_PIN, LOW);

  /* print values over serial if requested */
#ifdef SERIAL_VALUES
  Serial.print(tx_count, HEX);
  Serial.print(", ");
  Serial.print(time_ms, HEX);
  Serial.print(", ");
  Serial.print(ps_altitude_f, HEX);
  Serial.print(", ");
  Serial.print(ps_temp_f, HEX);
  Serial.print(", ");
  Serial.print(ls_red, HEX);
  Serial.print(", ");
  Serial.print(ls_blu, HEX);
  Serial.print(", ");
  Serial.print(ls_gre, HEX);
  Serial.print(", ");
  Serial.print(imuSensor.ax, HEX);
  Serial.print(", ");
  Serial.print(imuSensor.ay, HEX);
  Serial.print(", ");
  Serial.print(imuSensor.az, HEX);
  Serial.print(", ");
  Serial.print(imuSensor.gx, HEX);
  Serial.print(", ");
  Serial.print(imuSensor.gy, HEX);
  Serial.print(", ");
  Serial.print(imuSensor.gz, HEX);
  Serial.print(", ");
  Serial.print(imuSensor.mx, HEX);
  Serial.print(", ");
  Serial.print(imuSensor.my, HEX);
  Serial.print(", ");
  Serial.print(imuSensor.mz, HEX);
  Serial.println();
#endif
  /* pack values for TX */
  accelLightFrame_g.count = tx_count;
  accelLightFrame_g.time_ms = time_ms;
  accelLightFrame_g.altitude = ps_altitude_f;
  accelLightFrame_g.temp = ps_temp_f;
  accelLightFrame_g.red = ls_red;
  accelLightFrame_g.blue = ls_blu;
  accelLightFrame_g.green = ls_gre;

  /* transmit frame 1 */
  radio.send(TONODEID, accelLightFrame_g.tx_array, ACCEL_LIGHT_FRAME_BYTES);

  /* pack values for frame 2 */
  imuFrame_g.count = tx_count;
  imuFrame_g.accelX = imuSensor.ax;
  imuFrame_g.accelY = imuSensor.ay;
  imuFrame_g.accelZ = imuSensor.az;
  imuFrame_g.gyroX = imuSensor.gx;
  imuFrame_g.gyroY = imuSensor.gy;
  imuFrame_g.gyroZ = imuSensor.gz;
  imuFrame_g.magX = imuSensor.mx;
  imuFrame_g.magY = imuSensor.my;
  imuFrame_g.magZ = imuSensor.mz;

  /* transmit frame 2 */
  radio.send(TONODEID, imuFrame_g.tx_array, IMU_FRAME_BYTES)

  tx_count++;
}