#ifndef _RTC_H_INCLUDED
#define _RTC_H_INCLUDED


#include <WProgram.h>

#include "RTCValue.h"

// Base RTC class that all RTC devices must inherit.
// This class does nothing other than define the standard
// interface to the various RTC devices.

class RTC {
	public:
        // Get and set the main TIME and DATE value as an RTCValue object
		virtual RTCValue value() = 0;
		virtual void set(RTCValue setting) = 0;

        // Get and set the validity of the time value.  See RTCValue.h for validity macros.
		virtual char getValidity() { return _validity; };
		virtual void setValidity(char val) { _validity = val; };

        // On devices with an output these will enable and disable that output.  Do nothing by default.
		virtual void outputEnable() { };
		virtual void outputEnable(unsigned char) { };
		virtual void outputDisable() { };

        // Alarm functions.  We assume a single alarm, with the possibility of chime functionality.
        // Default to doing nothing, as not all chips have the alarm functionality, so we don't really
        // want to be forcing the developer to provide functions when they don't need to.

        // Get and set the alarm value
		virtual RTCValue alarmValue() { return NULL; };
		virtual void alarmSet(RTCValue val) { };

        // Turn the alarm or chime on and off
		virtual void alarmEnable() { };
		virtual void alarmDisable() { };
		virtual void chimeEnable() { };
		virtual void chimeDisable() { };

        // Set the alarm's mask value and repeat flag
		virtual void alarmMask(unsigned char) { };
		virtual void alarmRepeat(unsigned char) { };

        // Some systems have the ability to attach an interrupt, but not all.  This isn't really
        // a common feature, but included for completeness.  A default of "do nothing" is provided.
		virtual void attachInterrupt(void (*)()) { };

	protected:
        // Common store for the time validity value
		char _validity;
};

// Internal PIC32 RTCC device
#include "RTCC.h" 

// Dallas (aka Maxim) SPI two wire (not I2C, SPI over 2 wires - half duplex) clock module
#include "DS1302.h" 

#endif
