#ifndef _RTCVALUE_H
#define _RTCVALUE_H

// Increasing quality of time set in RTC
// Unitialized
#define RTC_VAL_NOT 0
// Initialized with compilation date/time
#define RTC_VAL_GCC 1
// Initialized with date/time received from another computer
#define RTC_VAL_BEE 2
// Initialized by manual configuration
#define RTC_VAL_MAN 3
// Initialized from GSM network
#define RTC_VAL_GSM 4
// Initialized from a NTP server
#define RTC_VAL_NTP 5

/*
 This class represents a date/time and its validity
 */
class RTCValue {
	private:
		char _validity;
		uint32_t a_date;
		uint32_t a_time;
		unsigned char bcd2dec(unsigned char);
		unsigned char dec2bcd(unsigned char);
		void bcd2str(unsigned char * res, unsigned char bcd);
	public:
		RTCValue();
		RTCValue(uint32_t date, uint32_t time, uint8_t val);
		RTCValue(const RTCValue * rv);
		void setValidity(char val);
		char getValidity();

		uint32_t date();
		uint32_t time();
		
		// Check if data in BCD minimally make sense
		// (detect date/time from uninitialised clock)
		bool valid();

		// Set time
		void time(unsigned char hours, unsigned char minutes, unsigned char seconds);
		// Show time hh:mm:ss
		void time(unsigned char * res);
		// Set date jj/mm/yy
		void date(unsigned char year, unsigned char month, unsigned char day);
		// Show date
		void date(unsigned char * res);
		
		// returns 0 to 23
		unsigned char hours();
		// Set hour to 0 to 23
		void hours(unsigned char);
		// returns 0 to 59
		unsigned char minutes();
		// Set minutes
		void minutes(unsigned char);
		// returns 0 to 59
		unsigned char seconds();
		// Set seconds
		void seconds(unsigned char);
		
		// BEWARE 2100 !!!!
		// returns 0 to 99 for 2000 to 2099
		unsigned char year();
		// Set year (0 to 99 for 2000 to 2099)
		void year(unsigned char);
		
		// returns 1 to 12
		unsigned char month();
		// Set month
		void month(unsigned char);
        // returns 1 to 31
		unsigned char day();
		void day(unsigned char);

		unsigned char dayOfWeek();
		void dayOfWeek(unsigned char);

        // Very handy for teletransmission or storage: get an UNSIGNED 32 bits representation of date/time
		uint32_t getInt();
		// Set date/time from UNSIGNED 32 bits
		void setInt(uint32_t timeAsInt);

	    // Still UNTESTED: create Unix like long representing the date/time
		unsigned long getTimestamp();
		
		// NOT IMPLEMENTED BUT WOULD BE NICE !
		void setTimestamp(unsigned long unixTime);
};

#endif
