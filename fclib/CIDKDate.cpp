/* **************************************************
**
**    III                DDD  KKK
**    III                DDD  KKK
**                       DDD  KKK
**    III   DDDDDDDDDDD  DDD  KKK            KKK
**    III  DDD           DDD  KKK            KKK
**    III  DDD           DDD  KKK           KKK
**    III  DDD           DDD  KKK        KKKKKK
**    III  DDD           DDD  KKK   KKKKKKKKK
**    III  DDD           DDD  KKK        KKKKKK
**    III  DDD           DDD  KKK           KKK
**    III  DDD           DDD  KKK            KKK
**    III   DDDDDDDDDDDDDDDD  KKK            KKK
**
**
**     SSS         FF
**    S           F   T
**     SSS   OO   FF  TTT W   W  AAA  R RR   EEE
**        S O  O  F   T   W W W  AAAA RR  R EEEEE
**    S   S O  O  F   T   W W W A   A R     E
**     SSS   OO  FFF   TT  W W   AAAA R      EEE
**
**    Copyright: (c) 2015 IDK Software Ltd
**
****************************************************
**
**	Filename	: CRegString.cpp
**	Author		: I.Ferguson
**	Version		: 1.000
**	Date		: 26-05-2015
**
** #$$@@$$# */

#include "cport.h"
#include "CIDKDate.h"
#include <math.h>
#include <time.h>

// Static days in month
unsigned long CIDKDate::m_daysInMonth[12] = {	31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
unsigned long CIDKDate::m_leapYearDaysInMonth[12] = { 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

//	constructors & destructor
time_t CIDKLocalTime::m_UpdateTime = 0;
time_t CIDKLocalTime::m_OffsetTime = 0;
bool CIDKLocalTime::m_Once = true;

CIDKDate::CIDKDate	()
{
	m_nTime = 0;
}


CIDKDate::CIDKDate	(const CIDKDate& dtValue)
{
	m_nTime = dtValue.m_nTime ;
}

CIDKDate::CIDKDate(time_t nTime)
{
	m_nTime = nTime;
}

CIDKDate::~CIDKDate	(void)
{
}

void CIDKDate::Now()
{
	time(&m_nTime);
}


void CIDKDate::Print(FILE *fp)
{
	
	char szDate[16];
	CIDKLocalTime lt;
	struct tm tmp = lt.GetTime((const time_t *)&m_nTime);
	strftime (szDate, 16, "%Y%m%d", &tmp);

	fwrite(szDate, 1, 16, fp);
}



std::string  CIDKDate::Print()
{
	std::string l_TimeStr;

	l_TimeStr = ctime_p((const time_t *)&m_nTime);
	return l_TimeStr.substr(0, l_TimeStr.size() - 1);
}

//	operators

CIDKDate&	CIDKDate::operator=	(const CIDKDate& dtValue)
{
	m_nTime = dtValue.m_nTime;
	return *this ;
}

CIDKDate&	CIDKDate::operator=	(time_t nSeconds)
{
	m_nTime = nSeconds;
	return *this ;
}

CIDKDate	CIDKDate::operator+	(time_t nSeconds)
{
	CIDKDate date = *this;

	date.m_nTime += nSeconds;

	return date ;
}

CIDKDate	CIDKDate::operator-	(time_t nSeconds)
{
	CIDKDate date = *this;

	date.m_nTime -= nSeconds;

	return date ;
}

time_t CIDKDate::operator- (CIDKDate& dtValue)
{
	time_t nResult = m_nTime - dtValue.m_nTime;
	return nResult;
}

bool	CIDKDate::operator==	(CIDKDate& dtValue)
{
    return (m_nTime == dtValue.m_nTime);
}

bool	CIDKDate::operator<	(CIDKDate& dtValue)
{
    return (m_nTime < dtValue.m_nTime);
}

bool	CIDKDate::operator<	(const CIDKDate& dtValue)
{
    return (m_nTime < dtValue.m_nTime);
}

bool	CIDKDate::operator<=	(CIDKDate& dtValue)
{
    return (m_nTime <= dtValue.m_nTime);
}

bool	CIDKDate::operator>	(CIDKDate& dtValue)
{
    return (m_nTime > dtValue.m_nTime);
}

bool	CIDKDate::operator>=	(CIDKDate& dtValue)
{
    return (m_nTime >= dtValue.m_nTime);
}

/*
 * Get the time as Year, Month, day
 *
 *	nYear	=	will hold the year (4-figures)
 *	nMonth	=	will hold the month (01 - 12)
 *	nDay	=	will hold the day of month 
 *
 *	Returns	=	TRUE if successful
 */
bool CIDKDate::GetDate(long& nYear, long& nMonth, long& nDay)
{
	// Make sure this has a valid date

	// First determine the number of years since 1st Jan 1970

	// For this function it is assumed that leap years occur every
	// 4 years.  This gives the effective range of a CIDKDate as 
	// 1 Jan 1970 to 31 Dec 2099.

	unsigned long nCalendarDays = (m_nTime / IDK_DATE_SECONDS_IN_DAY);

	// Determin the number of four year periods
	long fourYearPeriods = nCalendarDays / ((365 * 4) + 1);

	// update the calender count
	nCalendarDays -= (fourYearPeriods * ((365 * 4) + 1));

	// determin which year started this interval (1970, 1974, 1978)
	long nTmpYear = (fourYearPeriods * 4) + IDK_BASE_YEAR;

	// Flag indicating if this year is a leap year
	bool bIsLeapYear = false;

	if (nCalendarDays >= 365)
	{
		// Must be in 1971 - 1973, 1975 - 1977 etc
		nTmpYear++;

		nCalendarDays -= 365;

		if (nCalendarDays >= 365)
		{
			// Must be in 1972 - 1973, 1976 - 1977 etc
			nTmpYear++;

			nCalendarDays -= 365;

			if (nCalendarDays >= 366)	// Check for leap year
			{
				nTmpYear++;

				nCalendarDays -= 366;	// Last year was a leap year

			}
			else
			{
				// This year is a leap year

				bIsLeapYear = true;
			}
		}
	}

	// nTmpYear contains the year
	// Make sure it is in range

	if ((nTmpYear < IDK_BASE_YEAR) || (nTmpYear > IDK_MAX_YEAR))
	{
		return false;
	}

	// nCalendarDays contains the number of days since the start of the year

	// Calculate the month

	// Get array of days in month
	unsigned long* daysInMonth = m_daysInMonth;

	if (bIsLeapYear)
	{
		daysInMonth = m_leapYearDaysInMonth;
	}

	long nTmpMonth = 0;

	while ((nTmpMonth < 11) && (nCalendarDays >= daysInMonth[nTmpMonth]))
	{
		unsigned long dayInMonth = daysInMonth[nTmpMonth];

		nCalendarDays -= daysInMonth[nTmpMonth];
		nTmpMonth++;
	}

	// Add one to nTmpMonth to make range 1 - 12
	nTmpMonth++;

	// Make sure it is in range

	if ((nTmpMonth < 1) || (nTmpMonth > 12))
	{
		return false;
	}

	// Set values
	nYear = nTmpYear;
	nMonth = nTmpMonth;

	// Add one to days since the first day of month = 1.

	nDay = nCalendarDays + 1;

	return true;
}

/*
 * Get the time as Hour, Minute, second
 *
 *	nHour	=	will hold the hour		(00 - 23)
 *	nMinute	=	will hold the minute	(00 - 59)
 *	nSecond	=	will hold the second	(00 - 59) 
 *
 *	Returns	=	TRUE if successful
 */
bool CIDKDate::GetTime(long& nHour, long& nMinute, long& nSecond)
{
	long nTotalSeconds = (m_nTime % IDK_DATE_SECONDS_IN_DAY);

	long nTmpHours = nTotalSeconds / (IDK_DATE_SECONDS_IN_HOUR);

	if ((nTmpHours < 0) || (nTmpHours > 23))
	{
		return false;
	}

	nTotalSeconds -= nTmpHours * (IDK_DATE_SECONDS_IN_HOUR);

	long nTmpMinutes = nTotalSeconds / IDK_DATE_SECONDS_IN_MINUTE;

	if ((nTmpMinutes < 0) || (nTmpMinutes > 59))
	{
		return false;
	}

	nTotalSeconds -=  nTmpMinutes * IDK_DATE_SECONDS_IN_MINUTE;

	if ((nTotalSeconds < 0) || (nTotalSeconds > 59))
	{
		return false;
	}

	nHour = nTmpHours;
	nMinute = nTmpMinutes;
	nSecond = nTotalSeconds;

	return true;
}

/*
 * Set the date & time
 *
 *	nYear	=	The year (4-figures) (1970 - 2099)
 *	nMonth	=	The month (01 - 12)
 *	nDay	=	The day of month 
 *	nHour	=	The hour		(00 - 23)
 *	nMinute	=	The minute	(00 - 59)
 *	nSecond	=	The second	(00 - 59) 
 *
 *	Returns	=	TRUE if successful
 */
bool CIDKDate::SetDate(long nYear, long nMonth, long nDay, long nHour, long nMinute, long nSecond)
{
	unsigned long nTotalSeconds = 0;

	// Check year range

	if ((nYear < IDK_BASE_YEAR) || (nYear > IDK_MAX_YEAR))
	{
		return false;
	}

	// First calculate number of seconds for the year

	nTotalSeconds = (nYear - IDK_BASE_YEAR) * IDK_DATE_SECONDS_IN_YEAR;

	// Make adjustment for leap years

	// For this function it is assumed that leap years occur every
	// 4 years.  This gives the effective range of a CIDKDate as 
	// 1 Jan 1970 to 31 Dec 2099.

	unsigned long extraDays = (((nYear - 1) - IDK_BASE_LEAP_YEAR) / 4);
	nTotalSeconds +=  extraDays * IDK_DATE_SECONDS_IN_DAY;

	// Do months

	unsigned long* pDaysInMonth = m_daysInMonth;

	// Check for leap year

	if (((nYear - IDK_BASE_LEAP_YEAR) % 4) == 0)
	{
		pDaysInMonth = m_leapYearDaysInMonth;
	}

	// Check month range
	if ((nMonth < 1) || (nMonth > 12))
	{
		return false;
	}

	// Adjust month into 0 - 11 range
	nMonth--;

	for (long i = 0; i < nMonth; i++)
	{
		unsigned long dayInMonth = pDaysInMonth[i];

		nTotalSeconds += (pDaysInMonth[i] * IDK_DATE_SECONDS_IN_DAY);
	}

	// Do days

	// Check day range 
	if ((nDay < 1) || (nDay > (long)pDaysInMonth[nMonth]))
	{
		return false;
	}

	// Allow for the fact that 1 is the first day of month
	nDay--;

	nTotalSeconds += (nDay * IDK_DATE_SECONDS_IN_DAY);

	if (!SetTime(nTotalSeconds, nHour, nMinute, nSecond))
	{
		return false;
	}

	m_nTime = nTotalSeconds;

	return true;
}

/*
 * Set the time
 *
 *	nHour	=	The hour		(00 - 23)
 *	nMinute	=	The minute	(00 - 59)
 *	nSecond	=	The second	(00 - 59) 
 *
 *	Returns	=	TRUE if successful
 */
bool CIDKDate::SetTime(long nHour, long nMinute, long nSecond)
{
	unsigned long nTotalSeconds = 0;

	if (!SetTime(nTotalSeconds, nHour, nMinute, nSecond))
	{
		return false;
	}

	m_nTime = nTotalSeconds;

	return true;
}

/*
 * Set the time.  Used by SetTime and SetDate
 *
 *	nTime	=	will hold the number of seconds
 *	nHour	=	The hour		(00 - 23)
 *	nMinute	=	The minute	(00 - 59)
 *	nSecond	=	The second	(00 - 59) 
 *
 *	Returns	=	TRUE if successful
 */
bool CIDKDate::SetTime(unsigned long& nTime, long nHour, long nMinute, long nSecond)
{
	if ((nHour < 0) || (nHour > 23))
	{
		return false;
	}

	nTime += (nHour * IDK_DATE_SECONDS_IN_HOUR);

	if ((nMinute < 0) || (nMinute > 59))
	{
		return false;
	}

	nTime += (nMinute * IDK_DATE_SECONDS_IN_MINUTE);

	if ((nSecond < 0) || (nSecond > 59))
	{
		return false;
	}

	nTime += nSecond;

	return true;
}


std::string CIDKDate::Format(const char *szFormat)
{
	char szDate[161];
	CIDKLocalTime lt;
	struct tm tmp = lt.GetTime((const time_t *)&m_nTime);
	strftime (szDate, 160, szFormat, &tmp);
	std::string l_Str = szDate;
	return l_Str;
}

/*
 *	Do rounding.  The original CIDKDate is uneffected
 *
 *	Returns	=	the number of seconds since 1 Jan 1970,
 *				rounded to appropriate value.
 */
unsigned long CIDKDate::RoundToYear()
{
	// First determine the number of years since 1st Jan 1970

	// For this function it is assumed that leap years occur every
	// 4 years.  This gives the effective range of a CIDKDate as 
	// 1 Jan 1970 to 31 Dec 2099.

	unsigned long nTotalSeconds = m_nTime;

	// Determin the number of four year periods
	long fourYearPeriods = nTotalSeconds / (IDK_DATE_SECONDS_IN_FOUR_YEAR);

	// update the calender count
	nTotalSeconds -= (fourYearPeriods * (IDK_DATE_SECONDS_IN_FOUR_YEAR));

	if (nTotalSeconds >= IDK_DATE_SECONDS_IN_YEAR)
	{
		nTotalSeconds -= IDK_DATE_SECONDS_IN_YEAR;

		if (nTotalSeconds >= IDK_DATE_SECONDS_IN_YEAR)
		{
			// Must be in 1972 - 1973, 1976 - 1977 etc

			nTotalSeconds -= IDK_DATE_SECONDS_IN_YEAR;

			if (nTotalSeconds >= (IDK_DATE_SECONDS_IN_YEAR + IDK_DATE_SECONDS_IN_DAY))	// Check for leap year
			{
				nTotalSeconds -= (IDK_DATE_SECONDS_IN_YEAR + IDK_DATE_SECONDS_IN_DAY);	// Last year was a leap year
			}
		}
	}
	
	// nCalendarDays has the number of days "left over"

	unsigned long nRoundedSeconds = (m_nTime - nTotalSeconds);

	return nRoundedSeconds;
}

unsigned long CIDKDate::RoundToMonth()
{
	unsigned long nYearSeconds = RoundToYear();

	long nTmpMonth = 0;

	unsigned long* pDaysInMonth = m_daysInMonth;

	//Check for leap year

	unsigned long nSecondsSinceLeapYearBase = nYearSeconds + ((IDK_BASE_YEAR - IDK_BASE_LEAP_YEAR) * IDK_DATE_SECONDS_IN_YEAR) + (IDK_DATE_SECONDS_IN_DAY);

	unsigned long nRet = nSecondsSinceLeapYearBase % (IDK_DATE_SECONDS_IN_FOUR_YEAR);

	if (nRet == 0)
	{
		pDaysInMonth = m_leapYearDaysInMonth;
	}

	// The number of seconds elapsed since the start of the year
	unsigned long nLeftOverSeconds = m_nTime - nYearSeconds;

	// Calculate the number of seconds since the start of the month

	unsigned long nLeftOverMonthSeconds = nLeftOverSeconds;

	while ((nTmpMonth < 11) && (nLeftOverMonthSeconds >= (pDaysInMonth[nTmpMonth] * IDK_DATE_SECONDS_IN_DAY)))
	{
		nLeftOverMonthSeconds -= pDaysInMonth[nTmpMonth] * IDK_DATE_SECONDS_IN_DAY;
		nTmpMonth++;
	}

	// Year Seconds + Month Seconds
	unsigned long nRoundedSeconds = nYearSeconds + (nLeftOverSeconds - nLeftOverMonthSeconds);

	return nRoundedSeconds;
}

unsigned long CIDKDate::RoundToDay()
{
	// Get the number of seconds that is the "time" portion

	long nTotalSeconds = (m_nTime % IDK_DATE_SECONDS_IN_DAY);

	// Subtract this from total seconds
	unsigned long nRoundedSeconds = (m_nTime - nTotalSeconds);

	return nRoundedSeconds;
}

unsigned long CIDKDate::RoundToHour()
{
	// Get the number of seconds that is the "time" portion

	long nTotalSeconds = (m_nTime % IDK_DATE_SECONDS_IN_DAY);

	// Subtract this from total time
	unsigned long nWholeDaySeconds = (m_nTime - nTotalSeconds);

	// Round time section to the hour 
	nTotalSeconds = (nTotalSeconds / IDK_DATE_SECONDS_IN_HOUR) * (IDK_DATE_SECONDS_IN_HOUR);

	// Add back to date part
	unsigned long nRoundedSeconds = nWholeDaySeconds + nTotalSeconds;

	return nRoundedSeconds;
}

int CIDKDate::GetElement(time_t inTime, int elementType)
{

	struct tm		brkTime;
	int				ret;

	errno_t err =  gmtime_p(brkTime, &inTime);

	switch (elementType)
	{
		case IDK_MINUTE:	ret = brkTime.tm_min; break;
		case IDK_HOUR:	ret = brkTime.tm_hour; break;
		case IDK_SECOND:	ret = brkTime.tm_sec; break;
		case IDK_WEEKDAY:	ret = brkTime.tm_wday; ret++; break;
		case IDK_YEAR:	ret = brkTime.tm_year; break;
		case IDK_FULLYEAR:	ret = brkTime.tm_year + 1900; break;
		case IDK_MONTH:	ret = brkTime.tm_mon + 1; break;
		case IDK_DAY:	ret = brkTime.tm_mday; break;

		default:	break;
	}
			//brkTime->tm_isdst, brkTime->tm_year + 1,
			//brkTime->tm_mon, brkTime->tm_mday,
			//brkTime->tm_hour, brkTime->tm_min,
			//brkTime->tm_sec );

	return(ret);
	
	
}
/*
 *	GetJulianDate()  calculates the number of days elapsed
 *	since 01/01/-4713 at mid-day.
 *  The Julian date is returned as a floating point number.
 *	The fractional part of which denotes the time, thus
 *  0.75 represents 6pm, 0.5 12pm etc.
 *
 *	Returns	=	true if conversion successful.
 *				false if conversion not successful.
 *
 *  Algorithm as specified in Astronomy with your personal
 *	computer. Peter Duffet-Smith
 */
bool CIDKDate::GetJulianDate(double &theJulianDate)
{
	// First, lets get the date and time
	long theYear;
	long theMonth;
	long theDay;

	if ( false == GetDate(theYear,theMonth,theDay) )
	{
		return false;
	}

	long theHour;
	long theMinutes;
	long theSeconds;
	
	if ( false == GetTime(theHour,theMinutes,theSeconds) )
	{
		return false;
	}

	unsigned int	m = theMonth;
	double			y = theYear;
	double			b = 0.0;
	double			a = 0.0;
	double			c = 0.0;

	// The day needs to be expressed as a floating point number
	// where the fractional part refers to the time.
	// such that 12.25 refers to the 12th at 6am
	double			day = theDay + (theHour + (1.0 * theMinutes/60.0) + (1.0 * theSeconds/3600.0)) / 24.0;
				
	if ( y < 0.0 )
	{
		// No year 0
		y += 1.0;
	}

	if ( theMonth < 3.0 )
	{
		m = theMonth + 12;
		y -= 1.0;
	}

	if ( !(	 (theYear < 1582)
		 || ((theYear == 1582) && (theMonth < 10))
		 || ((theYear == 1582) && (theMonth == 10) && (day < 15))) )
	{
		// Take account of the change to the Gregorian calendar
		modf(y * 0.01,&a);

		double iPart;
		modf(a * 0.25,&iPart);
		b = 2.0 - a + iPart;
	}

	double iPart;
	double Corr = 0.0;

	if ( y < 0.0 )
	{
		Corr = -0.75;
	}

	modf((365.25 * y) + Corr, &iPart);
	c = iPart - 694025;

	double			d;
	modf((30.6001*(m+1.0)),&d);

	theJulianDate = 2415020.0 + b + c + d + day - 0.5;

	return true;
}

/*
 *	SetJulianDate()  converts the number of Julian days
 *					 since mid-day on 01/01/-4713 into
 *					 the calendar date.
 *
 *	Returns	=	true if conversion successful.
 *				false if conversion not successful.
 *
 *  Algorithm as specified in Astronomy with your personal
 *	computer. Peter Duffet-Smith
 */
bool CIDKDate::SetJulianDate(double theDate)
{
	long	year;
	long	month;
	long	day;
	long	hour = 0;
	long	minutes = 0;
	long	seconds = 0;

	theDate -= 2415020.0;

	double d = theDate + 0.5;
    double i = floor(d);
    double f = (d - i);


    if ( f == 1 )
    {
        f = 0;
        i = i + 1;
    }

    if ( i > -115860 )
    {
        double a;
        a = floor((i/36524.25) + 0.99835726) + 14.0;
        i = i + 1.0 + a - floor(a * 0.25);
    }

    double b;
    b = floor((i/365.25) + 0.802601);
    double ce = i - floor((365.25 * b)+0.750001) + 416.0;
    double g = floor(ce/30.6001);
 
	month = (long) (g - 1.0);
    day = (long) (ce - floor(30.6001 * g) + f);
	year = (long) (b + 1899.0);

    if ( g > 13.5 )
    {
        month = (long) (g - 13.0);
    }

    if( month < 2.5 )
    {
        year = (long) (b + 1900);
    }

    if ( year < 1.0 ) 
    {
        year = year - 1;
    }

	double	theIpart;

	// Convert the fractional part of the Julian date
	// which represents the time, to hours, minutes seconds
	f		= modf(f * 24.0 ,&theIpart) * 60.0;
	hour	= (long) theIpart;

	f		= modf(f,&theIpart) * 60.0;
	minutes	= (long) theIpart;

	f		= modf(f + 0.5,&theIpart);
	seconds = (long) theIpart;

	return SetDate(year,month,day,hour,minutes,seconds);
}

