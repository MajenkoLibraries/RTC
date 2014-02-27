/*

All code is copyright 2011 Majenko Technologies. 
The code is provided freely with no warranty either implicitly or explicity implied. 
You may freely use the code for whatever you wish, but Majenko Technologies will not 
be liable for any damage of any form howsoever caused by the use or misuse of this code.

Separation of date/time Class (RTCValue) from RTC management object (RTCCClass) by Christophe Dupriez, DESTIN-Informatique.com 2014

PIC32 version...

*/

#include <RTC.h>

/*
  Interrupt routine called if Alarm/Chime/RTC interrupt is enabled.
  Calls an user configured procedure.
*/
extern "C"
{
    void (*_RTCCInterrupt)();

    void __ISR(_RTCC_VECTOR, ipl3/*_RTCC_IPL_ISR does not work*/) __RTCCInterrupt(void) 
    {
        if(_RTCCInterrupt)
        {
            _RTCCInterrupt();
        }
        IFS1CLR=0x00008000;
    }
}

/*
 Configuration registries unlock/lock sequence...
 */
#define UNLOCK SYSKEY = 0x0; SYSKEY = 0xAA996655; SYSKEY = 0x556699AA;
#define LOCK SYSKEY = 0x0;

void RTCC::begin() 
{
    IEC1CLR=0x00008000;
    IFS1CLR=0x00008000; 
    IPC8CLR=0x1f000000; 
    IPC8SET=0x0d000000; 
    IEC1SET=0x00008000; 
/*Ensure the secondary oscillator is enabled and ready, i.e. OSCCON<1>=1, OSCCON<22>=1,
and RTCC write is enabled i.e. RTCWREN (RTCCON<3>) =1;*/
    UNLOCK
    RTCCONbits.RTCWREN = 1;
    RTCCONbits.ON = 1;
    OSCCONSET = 0x00400002;
    LOCK
	_validity = RTC_VAL_NOT; // 0: very unsure!
}


/*
 * Wait at least 32msec... if a crystal is available!
 * FOREVER IF NO WORKING CRYSTAL?
 */
void RTCC::timeSync()
{
/* THIS IS NOT A GOOD IDEA:
    while(RTCCONbits.RTCSYNC==0); // Wait for a transition and 32ms later
*/
    while(RTCCONbits.RTCSYNC==1) { /*yield*/ };
}

/*
 Enable/Disable an output pin synchronised with RTC
 */
void RTCC::outputEnable()
{
    timeSync();
    UNLOCK
    RTCCONbits.RTCOE = 1;
    RTCCONbits.RTSECSEL = 1;
    LOCK
} 

void RTCC::outputEnable(unsigned char mode)
{
    timeSync();
    UNLOCK
    RTCCONbits.RTCOE = 1;
    RTCCONbits.RTSECSEL = mode;
    LOCK
} 

void RTCC::outputDisable()
{
    timeSync();
    UNLOCK
    RTCCONbits.RTCOE = 0;
    LOCK
} 

/*
 get Current date/time.
 RTCValue class represents a date/time independently from the RTC itself.
 */
RTCValue RTCC::value()
{
    timeSync();
	uint32_t our_date = RTCDATE;
	uint32_t our_time = RTCTIME;
	while (1) { // Ensures there is no date rollover while reading the time
		uint32_t new_date = RTCDATE;
		uint32_t new_time = RTCTIME;
		if (new_date == our_date && new_time == our_time) break;
		our_date = new_date;
		our_time = new_time;
	}
	return RTCValue(our_date,our_time, _validity);
}

/*
 Set date/time and specify its validity (0: very bad, 5: perfect=NTP)
 */
void RTCC::set(RTCValue setting)
{
    timeSync();
	RTCDATE = setting.date();
	RTCTIME = setting.time();
	_validity = setting.getValidity();
}

/*
 Alarms management...
 */
void RTCC::alarmSync()
{
    while(RTCALRMbits.ALRMSYNC==1) { /*yield*/ };
}

void RTCC::alarmEnable()
{
    alarmSync();
    RTCALRMbits.ALRMEN = 1;
}

void RTCC::alarmDisable()
{
    alarmSync();
    RTCALRMbits.ALRMEN = 0;
}

/*
 Chime is an alarm repeating at configured interval (see AL_xxx definitions)
 
 Usage example:
    // Set current time with an RTCValue "rv"
 	RTCC.set(rv);
	
	// Make an Alarm basis synched to Hours
	rv.seconds(0);
	rv.minutes(0);
	RTCC.alarmSet(rv);

	// Setup a Chime every minute
    RTCC.alarmMask(CHIME_PERIOD);
	RTCC.chimeEnable();
    RTCC.attachInterrupt(&JustWakeUp); // JustWakeUp is a void function() to handle RTC interrupts
	RTCC.alarmEnable();

	//Example of JustWakeUp:
	void JustWakeUp(void)
	{
		// Remember when this occurs!
		lastChime = RTCC.value();
	}
 */
void RTCC::chimeEnable()
{
    alarmSync();
    RTCALRMbits.CHIME = 1;
}

void RTCC::chimeDisable()
{
    alarmSync();
    RTCALRMbits.CHIME = 0;
}

void RTCC::alarmMask(unsigned char mask)
{
    alarmSync();
    RTCALRMbits.AMASK = mask;
}

void RTCC::alarmRepeat(unsigned char rpt)
{
    alarmSync();
    RTCALRMbits.ARPT = rpt;
}

void RTCC::alarmSet(RTCValue val) {
	alarmSync();
	ALRMDATE = val.date();
	ALRMTIME = val.time();
}

/*
 Alarm value is necessary for chimes also as it defined the basis of the repeating chime
 */
RTCValue RTCC::alarmValue() {
	alarmSync();
	return RTCValue(ALRMDATE,ALRMTIME,0);
}

void RTCC::attachInterrupt(void (*i)())
{
    _RTCCInterrupt = i;
}
/*
 Validity is useful to track the source of current date/time reference...
 */
char RTCC::getValidity() {
  return _validity;
}

void RTCC::setValidity(char val) {
  _validity = val;
}

