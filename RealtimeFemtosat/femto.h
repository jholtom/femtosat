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
