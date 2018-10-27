### Femtosat Radio Tutorial
Hi, Welcome to straya's (jholtom) tutorial on the basics of the femtosat radio!

### Includes
```
#include <RFM69.h>
#include <SPI.h>
```
These two lines should be at the very front of your main Arduino file.
They include the RFM69 (Our radio) library and the required supporting SPI library.
You will need to install them into the arduino IDE as the prior radio tutorial indicated.

### Definitions
```
#define NETWORKID     0
#define MYNODEID      1
#define TONODEID      2
#define FREQUENCY     RF69_915MHZ
```
These lines set up some important variables for use with the radio's functions.
The `NETWORKID`, `MYNODEID`, and `TONODEID` do not need to be changed in the current design.  I may however change them later to improve synchronization.
The `FREQUENCY` will be given to you closer to the deadline and flight time.  For now you can use the given `RF69_915MHZ` for testing on your own and with the ground station.

### Initialization
`RFM69 radio;`
This line needs to be included to initialize a radio object to be interacted with.  It should be placed after your definitions, but before your `setup()` or `loop()` functions.
```
void setup(){
    ......
    radio.initialize(FREQUENCY, MYNODEID, NETWORKID);
    radio.setHighPower();
    radio.setPowerLevel(20);
    radio.encrypt(0);
    ......
}
```
Next up, you need to add these four lines to your `setup()` function.  They configure the radio to use the given frequency and its syncword's.
The second and third lines sets the radio to use the correct power levels for our application.
The fourth disables encryption.

### Using the radio!
```
radio.send(TONODEID, sendbuffer, sendlength);
```
This is very simple to use, `sendbuffer` is a `void *`, but you may find it easy to use a character array or whatever else you like.  `sendlength` is the length of the buffer you are passing in.
