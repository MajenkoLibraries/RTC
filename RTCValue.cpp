#include <RTC.h>


/* Default constructor */
RTCValue::RTCValue() {
	a_date = 0;
	a_time = 0;
	_validity = 0;
}

/* Handling a date/time : current RTC, Alarm, logged event, etc. */
RTCValue::RTCValue(uint32_t date, uint32_t time, uint8_t val) {
	a_date = date;
	a_time = time;
	_validity = val;
}

RTCValue::RTCValue(const RTCValue * rv) {
	a_date = rv->a_date;
	a_time = rv->a_time;
	_validity = rv->_validity;
}

/*
 Validity is useful to track the source of current date/time reference...
 */
char RTCValue::getValidity() {
  return _validity;
}

void RTCValue::setValidity(char val) {
  _validity = val;
}
/*
 Internally RTC is working in good old BCD!
 */
unsigned char RTCValue::dec2bcd(unsigned char decimal)
{
    unsigned char bcd;
    unsigned char tens,units;
    tens = decimal / 10;
    units = decimal - (tens * 10);
    bcd = (tens << 4) | units;
    return bcd;
}

unsigned char RTCValue::bcd2dec(unsigned char bcd)
{
    unsigned char decimal;
    unsigned char tens,units;
    tens = bcd >> 4;
    units = bcd & 0x0F;
    decimal = (tens * 10) + units;
    return decimal;
}

void RTCValue::bcd2str(unsigned char * res, unsigned char bcd)
{
    unsigned char tens,units;
    tens = bcd >> 4;
    units = bcd & 0x0F;
    res[0] = tens +'0';
	res[1] = units+ '0';
}

uint32_t RTCValue::date()
{
	return a_date;
}

uint32_t RTCValue::time()
{
	return a_time;
}

// sizeof res must be 9 or bigger
void RTCValue::date(unsigned char * res)
{
	unsigned char ss[2];
	bcd2str(ss, (a_date >> 24) & 0xFF);
	res[0] = ss[0];
	res[1] = ss[1];
	res[2] = '-';
	bcd2str(ss, (a_date >> 16) & 0xFF);
	res[3] = ss[0];
	res[4] = ss[1];
	res[5] = '-';
	bcd2str(ss, (a_date >> 8) & 0xFF);
	res[6] = ss[0];
	res[7] = ss[1];
	res[8] = 0;
}

// sizeof res must be 9 or bigger
void RTCValue::time(unsigned char * res)
{
	unsigned char ss[2];
	bcd2str(ss, (a_time >> 24) & 0xFF);
	res[0] = ss[0];
	res[1] = ss[1];
	res[2] = ':';
	bcd2str(ss, (a_time >> 16) & 0xFF);
	res[3] = ss[0];
	res[4] = ss[1];
	res[5] = ':';
	bcd2str(ss, (a_time >> 8) & 0xFF);
	res[6] = ss[0];
	res[7] = ss[1];
	res[8] = 0;
}

unsigned char RTCValue::year()
{
    return bcd2dec((a_date >> 24) & 0xFF);
}

unsigned char RTCValue::month()
{
    return bcd2dec((a_date >> 16) & 0xFF);
}

unsigned char RTCValue::day()
{
    return bcd2dec((a_date >> 8) & 0xFF);
}

unsigned char RTCValue::dayOfWeek()
{
    return bcd2dec(a_date & 0xFF);
}

unsigned char RTCValue::hours()
{
    return bcd2dec((a_time >> 24) & 0xFF);
}


unsigned char RTCValue::minutes()
{
    return bcd2dec((a_time >> 16) & 0xFF);
}

unsigned char RTCValue::seconds()
{
    return bcd2dec((a_time >> 8) & 0xFF);
}

void RTCValue::time(unsigned char hours, unsigned char minutes, unsigned char seconds)
{
    a_time = (a_time & 0xFF) | (dec2bcd(hours)<<24) | (dec2bcd(minutes)<<16) | (dec2bcd(seconds)<<8);
}

void RTCValue::hours(unsigned char hours)
{
    a_time = (a_time & 0x00FFFFFF) | (dec2bcd(hours)<<24);
}
void RTCValue::minutes(unsigned char minutes)
{
    a_time = (a_time & 0xFF00FFFF) | (dec2bcd(minutes)<<16);
}

void RTCValue::seconds(unsigned char seconds)
{
    a_time = (a_time & 0xFFFF00FF) | (dec2bcd(seconds)<<8);
}

void RTCValue::date(unsigned char year, unsigned char month, unsigned char day)
{
    a_date = (a_date & 0xFF) | (dec2bcd(year)<<24) | (dec2bcd(month)<<16) | (dec2bcd(day)<<8);
}

void RTCValue::year(unsigned char year)
{
    a_date = (a_date & 0x00FFFFFF) | (dec2bcd(year)<<24);
}

void RTCValue::month(unsigned char month)
{
    a_date = (a_date & 0xFF00FFFF) | (dec2bcd(month)<<16);
}

void RTCValue::day(unsigned char day)
{
    a_date = (a_date & 0xFFFF00FF) | (dec2bcd(day)<<8);
}

void RTCValue::dayOfWeek(unsigned char dow)
{
    a_date = (a_date & 0xFFFFFF00) | dec2bcd(dow);
}

bool RTCValue::valid()
{
   char ayear  = year();
   char amonth = month();
   char aday   = day();
   char ahour  = hours();
   char amin   = minutes();
   char asec   = seconds();
   return (ayear >= 0 && ayear <= 99 && amonth >= 1 && amonth <= 12 && aday >= 1 && aday <= 31
           && ahour >= 0 && ahour <= 23 && amin >= 0 && amin <= 59 && asec >= 0 && asec <= 59 );
}

/*
  get an UNSIGNED integer representing date/time in 32 bits (NOT Unix date/time representation)
 */
void RTCValue::setInt(uint32_t timeAsInt) {
   char asec   = timeAsInt % 60;
   timeAsInt = timeAsInt / 60;
   char amin   = timeAsInt % 60;
   timeAsInt = timeAsInt / 60;
   char ahour   = timeAsInt % 24;
   timeAsInt = timeAsInt / 24;
   char aday   = (timeAsInt % 31) + 1;
   timeAsInt = timeAsInt / 31;
   char amonth   = (timeAsInt % 12) + 1;
   char ayear = timeAsInt / 12;
   date (ayear,amonth,aday);
   time (ahour, amin,asec);
}

/*
  Set a date/time using an UNSIGNED integer in 32 bits (NOT Unix date/time representation)
 */
uint32_t RTCValue::getInt() {
   char ayear  = year();
   char amonth = month();
   char aday   = day();
   char ahour  = hours();
   char amin   = minutes();
   char asec   = seconds();
   if (ayear >= 0 && ayear <= 99 && amonth >= 1 && amonth <= 12 && aday >= 1 && aday <= 31
       && ahour >= 0 && ahour <= 23 && amin >= 0 && amin <= 59 && asec >= 0 && asec <= 59 ) {
	return (uint32_t)asec+((uint32_t)amin*60)+((uint32_t)ahour*3600)+(((uint32_t)aday-1)*3600*24)+(((uint32_t)amonth-1)*3600*24*31)+((uint32_t)ayear*3600*24*31*12);
   } else return 0;
}

// Days per month...
unsigned char calendar [] = {31, 28, 31, 30,
							31, 30, 31, 31,
							30, 31, 30, 31};

/*
 Still UNTESTED: create Unix like long representing the date/time
 */							
unsigned long RTCValue::getTimestamp()
{

#define YEAR0 1970
// Belgium but timestamps will be inconsistent: better to store in pseudo UTC!
//#define TIMEZONE 1
// Belgium applies DST BUT timestamps will be inconsistent: better to store in pseudo UTC!
//#define DAYLIGHTSAVING
   int ayear  = 2000+year();
   char amonth = month();
   char aday   = day();
   char ahour  = hours();
   char amin   = minutes();
   char asec   = seconds();
#ifdef DAYLIGHTSAVING
   bool dst = amonth >= 3 & amonth <= 10;
   if (dst) {
		if (amonth == 3) {
// (31 - ((((5 × y) div 4) + 4) mod 7)) March at 01:00 UTC
			char dday = (char)(31 - (((int)((5 * (int)ayear) / 4) + 4) % 7));
			if (aday < dday) dst = false;
			else if (aday == dday & ahour < 1) dst = false;
		} else if (amonth == 10) {
// (31 - ((((5 × y) div 4) + 1) mod 7)) October at 01:00 UTC		
			char dday = (31 - (((int)((5 * (int)ayear) / 4) + 1) % 7));
			if (aday > dday) dst = false;
			else if (aday == dday & ahour >= 1) dst = false;
		}
   }
#endif
   if (ayear >= 0 && ayear <= 99 && amonth >= 1 && amonth <= 12 && aday >= 1 && aday <= 31
       && ahour >= 0 && ahour <= 23 && amin >= 0 && amin <= 59 && asec >= 0 && asec <= 59 ) {
		unsigned long s=0; // stores how many seconds passed from 1.1.1970, 00:00:00
		unsigned char localposition=0,foundlocal=0; // checks if the local area is defined in the map
		static unsigned char k=0;
		if ((!(ayear%4)) && (amonth>2)) s+=86400; // if the current year is a leap one -> add one day (86400 sec)
		amonth-- ; // dec the current month (find how many months have passed from the current year)
		while (amonth) // sum the days from January to the current month
		{
			amonth-- ; // dec the month
			s+=(calendar[amonth])*86400 ; // add the number of days from a month * 86400 sec
		}
		// Next, add to s variable: (the number of days from each year (even leap years)) * 86400 sec,
		// the number of days from the current month
		// the each hour & minute & second from the current day
		s +=((((ayear-YEAR0)*365)+((ayear-YEAR0)/4))*(unsigned long)86400)+(aday-1)*(unsigned long)86400 +
			(ahour*(unsigned long)3600)+(amin*(unsigned long)60)+(unsigned long)asec;
#ifdef DAYLIGHTSAVING
		if (dst) s-=3600;// if Summer Time, substract 1 hour
#endif
#ifdef TIMEZONE
		s-=(TIMEZONE*3600) ; // substract the UTC time difference (in seconds:3600 sec/hour)
#endif
		return s ; // return a Unix timestamp
   } else {
		return 0;
   }
}

// NOT IMPLEMENTED BUT WOULD BE NICE !
void RTCValue::setTimestamp(unsigned long unixTime) {
}
