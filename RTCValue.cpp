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

void RTCValue::date(unsigned short year, unsigned char month, unsigned char day)
{
    a_date = (a_date & 0xFF) | (dec2bcd(year % 100)<<24) | (dec2bcd(month)<<16) | (dec2bcd(day)<<8);
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

static int  dmsize[] =
    { -1, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

#define isleap(y) (((y) % 4) == 0 && ((y) % 100) != 0 || ((y) % 400) == 0)

/*
 Still UNTESTED: create Unix like long representing the date/time
 */							
unsigned long RTCValue::getTimestamp()
{
    uint32_t ayear = year() + 2000;
    uint32_t amonth = month();
    uint32_t tv_sec = 0;

    if (isleap(ayear) && amonth > 2)
        ++tv_sec;

    for (--ayear; ayear >= 1970; --ayear)
        tv_sec += isleap(ayear) ? 366 : 365;

    while (--amonth)
        tv_sec += dmsize[amonth];

    tv_sec += day() - 1;
    tv_sec = 24 * tv_sec + hours();
    tv_sec = 60 * tv_sec + minutes();
    tv_sec = 60 * tv_sec + seconds();
    return tv_sec;
}

#define SECS_PER_DAY 86400
#define SECS_PER_HOUR 3600
#define SECS_PER_MIN 60
#define DAYS_PER_WEEK7
#define EPOCH_YEAR 1970
#define TM_YEAR_BASE 2000
#define MONS_PER_YEAR 12

static int  year_lengths[2] = {
    365, 366
};

static int  mon_lengths[2][MONS_PER_YEAR] = {
    { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 },
    { 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 },
};

void RTCValue::setTimestamp(unsigned long clock) {
    register long       days;
    register long       rem;
    register int        y;
    register int        yleap;
    register int *      ip;

    uint32_t tm_hour;
    uint32_t tm_min;
    uint32_t tm_sec;
    uint32_t tm_year;
    uint32_t tm_yday;
    uint32_t tm_mon;
    uint32_t tm_mday;

    days = clock / SECS_PER_DAY;
    rem = clock % SECS_PER_DAY;
    while (rem < 0) {
        rem += SECS_PER_DAY;
        --days;
    }
    while (rem >= SECS_PER_DAY) {
        rem -= SECS_PER_DAY;
        ++days;
    }
    tm_hour = (int) (rem / SECS_PER_HOUR);
    rem = rem % SECS_PER_HOUR;
    tm_min = (int) (rem / SECS_PER_MIN);
    tm_sec = (int) (rem % SECS_PER_MIN);
    y = EPOCH_YEAR;
    if (days >= 0)
        for ( ; ; ) {
            yleap = isleap(y);
            if (days < (long) year_lengths[yleap])
                break;
            ++y;
            days = days - (long) year_lengths[yleap];
        }
    else do {
        --y;
        yleap = isleap(y);
        days = days + (long) year_lengths[yleap];
    } while (days < 0);
    tm_year = y - TM_YEAR_BASE;
    tm_yday = (int) days;
    ip = mon_lengths[yleap];
    for (tm_mon = 0; days >= (long) ip[tm_mon]; ++(tm_mon))
        days = days - (long) ip[tm_mon];
    tm_mday = (int) (days + 1);

    time(tm_hour, tm_min, tm_sec);
    date(tm_year, tm_mon, tm_mday);
}
