/*
 * Femtolib release 001
 *
 * Written for the BYU Spacecraft Competition to support easy radio design
 * Relies on the RFM69 library and a SPI library
 *
 *    This module:
 *              This module is free software; you can redistribute it and/or
 *              modify it under the terms of the GNU General Public License
 *              as published by the Free Software Foundation; either version
 *              2 of the License, or (at your option) any later version.
 *
 *    History:
 *      001 - Jacob Holtom
 *
 *    Authors: Jacob Holtom <jacob@holtom.me>
 *
 */
#include <RFM69.h>
#include <SPI.h>

/* These will be defined for everyone since they determine syncword.  Possibly change these to be the CCSDS syncwords for cool factor */
#define NETWORKID 0
#define MYNODEID 1
#define TONODEID 2

/* Frequency will be set by an input given to the module.
 * There are four channels defined
 * Channel 1 -
 * Channel 2 -
 * Channel 3 -
 * Channel 4 -
 * Select one by passing a number to the initializer
 */

/* ****THESE ARE TEMPORARY VALUES***** FIXME */
#define CHNL1 0xE1
#define CHNL2 0xE2
#define CHNL3 0xE3
#define CHNL4 0xE4

class FemtoRadio {
    protected:
        RFM69 radio;
        int _chan;
    public:
        //Set up the methods they should be able to call
        FemtoRadio(int channel) {
            // Set up the internal values and stuff
            // Set up the RFM69 instance
            switch(channel){
                1:
                    _chan = CHNL1;
                    break;
                2:
                    _chan = CHNL2;
                    break;
                3:
                    _chan = CHNL3;
                    break;
                4:
                    _chan = CHNL4;
                    break;
                default:
                    _chan = RF69_915MHZ;
                    //Hopefully we don't get here...but its a good fallback
            }
            radio.initialize(
        }
}
