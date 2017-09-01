#include <RFM69.h>
#include <SPI.h>
#define NETWORKID     70   // Must be the same for all nodes (0 to 255)
#define MYNODEID      1   // My node ID (0 to 255)
#define TONODEID      2   // Destination node ID (0 to 254, 255 = broadcast)
#define FREQUENCY     RF69_915MHZ
#define LED           9 // LED positive pin
#define GND           8 // LED ground pin

RFM69 radio;

void setup()
{
    Serial.begin(9600);
    Serial.print("Node ");
    Serial.print(MYNODEID,DEC);
    Serial.println(" ready");
    pinMode(LED,OUTPUT);
    digitalWrite(LED,LOW);
    pinMode(GND,OUTPUT);
    digitalWrite(GND,LOW);
    radio.initialize(FREQUENCY, MYNODEID, NETWORKID);
    radio.setHighPower(); // Always use this for RFM69HCW
}

void loop()
{
    // Set up a "buffer" for characters that we'll send:

    static char sendbuffer[22] = "Hello Femtosat World!";
    static int sendlength = 22;

    Serial.print("sending to node ");
    Serial.print(TONODEID, DEC);
    Serial.print(": [");
    for (byte i = 0; i < sendlength; i++)
	Serial.print(sendbuffer[i]);
    Serial.println("]");

    radio.send(TONODEID, sendbuffer, sendlength);

    sendlength = 0; // reset the packet
    blink(LED,10);
    delay(5000);
}

void blink(byte PIN, int DELAY_MS)
{
    digitalWrite(PIN,HIGH);
    delay(DELAY_MS);
    digitalWrite(PIN,LOW);
}
