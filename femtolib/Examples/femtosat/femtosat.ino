#include <RFM69.h>
#include <SPI.h>

#define NETWORKID     0   // This should really stay 0.
#define MYNODEID      1   // Easy to just keep this one and have a dest of two ALWAYS so we can easily determine generated syncword.
#define TONODEID      2

#define FREQUENCY     RF69_915MHZ //This translates directly to 915.000 MHz

#define USEACK        false // Request ACKs or not

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
    radio.setHighPower();
}
void loop()
{
    static char sendbuffer[62];
    static int sendlength = 0;

    if (Serial.available() > 0)
    {
        char input = Serial.read();

        if (input != '\r') // not a carriage return
        {
            sendbuffer[sendlength] = input;
            sendlength++;
        }
        if ((input == '\r') || (sendlength == 61)) // CR or buffer full
        {
            Serial.print("sending to node ");
            Serial.print(TONODEID, DEC);
            Serial.print(", message [");
            for (byte i = 0; i < sendlength; i++)
                Serial.print(sendbuffer[i]);
            Serial.println("]");

            radio.send(TONODEID, sendbuffer, sendlength);

            sendlength = 0; // reset the packet
            Blink(LED,10);
        }
    }
}

void Blink(byte pin, int delay_ms)
    // Blink an LED for a given number of ms
{
    digitalWrite(pin,HIGH);
    delay(delay_ms);
    digitalWrite(pin,LOW);
}

