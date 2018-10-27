/* BYUSpacecraft Club Femtosat Radio Module Test
 * 
 * Author: Jacob Willis
 * Date Started: 26-Oct-2017
 * 
 * Resources:
 * 	Sparkfun Tutorial
 *		https://learn.sparkfun.com/tutorials/rfm69hcw-hookup-guide?_ga=2.242584834.1668914149.1503938181-1161071162.1503938181
 * 	Explanation by Jacob Holtom:
 * 		https://gist.github.com/jholtom/3eee12cba81181da740f2ffba391fcac	
 *	Club Website
 *		spacecraft.byu.edu/club
 */

#include <RFM69.h>
#include <SPI.h>

#define NETWORKID     	0
#define MYNODEID      	1
#define TONODEID      	2
#define FREQUENCY     	RF69_915MHZ

#define LED_PIN			3

RFM69 radio;

void setup () {

	pinMode(LED_PIN, OUTPUT);
	Serial.begin(9600);

	radio.initialize(FREQUENCY, MYNODEID, NETWORKID);
	radio.setHighPower();
  radio.setPowerLevel(20);
  radio.encrypt(0);
}

void loop () {

	char buf[] = "Hello from the BYU Spacecraft Club Femtosat 1!\n";
	
	//send data
	digitalWrite(LED_PIN, HIGH); // indicate that we are transmitting
	radio.send(TONODEID, buf, sizeof(buf));
	Serial.print(buf);
	digitalWrite(LED_PIN, LOW);
  delay(250);
}
