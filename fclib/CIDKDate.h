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

#if !defined(CIDKDate_h)
#define CIDKDate_h

#include <stdio.h>
//#include "CIDKStr.h"
//#include "IDKMachine.h"
#include "cport.h"
#include <time.h>
#include <string>

/*
**	Class containing a date and time.
*/

// Some useful defines

#define IDK_BASE_YEAR		1970
#define	IDK_BASE_LEAP_YEAR	1968
#define IDK_MAX_YEAR			2099

#define IDK_DATE_SECONDS_IN_MINUTE			60
#define IDK_DATE_SECONDS_IN_HOUR				3600
#define IDK_DATE_SECONDS_IN_DAY				86400
#define IDK_DATE_SECONDS_IN_YEAR				31536000	// not a leap year
#define IDK_DATE_SECONDS_IN_FOUR_YEAR		126230400  // = (31536000 * 4) + IDK_DATE_SECONDS_IN_DAY

#define IDK_MINUTE		1
#define IDK_HOUR		2
#define IDK_SECOND		3
#define IDK_WEEKDAY		4
#define IDK_YEAR		5
#define IDK_FULLYEAR	6
#define IDK_MONTH		7
#define IDK_DAY			8

#define SECS_PER_DAY		IDK_DATE_SECONDS_IN_DAY
#define SECS_PER_HOUR	IDK_DATE_SECONDS_IN_HOUR


class CIDKDate
{
	//friend class CIDKVariant;

public:
	bool SetJulianDate( double theJulianDate );
	bool GetJulianDate( double &theJulianDate );

	/*******************************************************
	**
	**	Methods
	**
	********************************************************/


	/*
	 *	Constructors
	 */

	//	default constructor
    CIDKDate	() ;						
    
	//	copy constructor
	CIDKDate	(const CIDKDate& dtValue) ;			
    
	//	assignment constructor
	CIDKDate(time_t nTime);
    
	//	destructor
	~CIDKDate() ;							

	// sets the time Now
	void Now();

	// Assignment
    CIDKDate&	operator=	(const CIDKDate& dtValue) ;
	
	CIDKDate&	operator=	(time_t nSeconds);

	//	add a number of seconds
	CIDKDate	operator+	(time_t nSeconds);
	
	//	subtract a number of seconds
	CIDKDate	operator-	(time_t nSeconds);
	
	time_t operator- (CIDKDate& dtValue);

    bool		operator==	(CIDKDate& dtValue) ;
    bool		operator<	(CIDKDate& dtValue) ;

	// Required for STL operations
    bool		operator<	(const CIDKDate& dtValue);
    friend bool	operator<(const CIDKDate &theLHS, const CIDKDate &theRHS)
	{
		return theLHS.m_nTime < theRHS.m_nTime;
	}

    bool		operator<=	(CIDKDate& dtValue) ;
    bool		operator>	(CIDKDate& dtValue) ;
    bool		operator>=	(CIDKDate& dtValue) ;

	/*
	 *	Get the total number of seconds
	 */
	unsigned long GetTotalSeconds()
	{
		return (unsigned long)m_nTime;
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
	bool GetDate(long& nYear, long& nMonth, long& nDay);

	/*
	 * Get the time as Hour, Minute, second
	 *
	 *	nHour	=	will hold the hour		(00 - 23)
	 *	nMinute	=	will hold the minute	(00 - 59)
	 *	nSecond	=	will hold the second	(00 - 59) 
	 *
	 *	Returns	=	TRUE if successful
	 */
	bool GetTime(long& nHour, long& nMinute, long& nSecond);
	
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
	bool SetDate(long nYear, long nMonth, long nDay, long nHour, long nMinute, long nSecond);
	
	/*
	 * Set the time
	 *
	 *	nHour	=	The hour		(00 - 23)
	 *	nMinute	=	The minute	(00 - 59)
	 *	nSecond	=	The second	(00 - 59) 
	 *
	 *	Returns	=	TRUE if successful
	 */
	bool SetTime(long nHour, long nMinute, long nSecond);


	/*
	 *	Do rounding.  The original CIDKDate is uneffected
	 *
	 *	Returns	=	the number of seconds since 1 Jan 1970,
	 *				rounded to appropriate value.
	 */
	unsigned long RoundToYear();

	unsigned long RoundToMonth();

	unsigned long RoundToDay();

	unsigned long RoundToHour();

	/*
	 *	Static functions to return useful durations
	 */
	static unsigned long SecondsInHour()
	{
		return IDK_DATE_SECONDS_IN_HOUR;
	}

	static unsigned long SecondsInDay()
	{
		return IDK_DATE_SECONDS_IN_DAY;
	}


	// For backwards compatibility

	CIDKDate(unsigned long nNumberOfDays,unsigned long nNumberOfSeconds)
	{
		m_nTime = nNumberOfDays * IDK_DATE_SECONDS_IN_DAY + nNumberOfSeconds;
	}

	bool SetSeconds(long nSeconds = 0)
	{
		m_nTime = (m_nTime / IDK_DATE_SECONDS_IN_DAY) * IDK_DATE_SECONDS_IN_DAY;
		m_nTime += nSeconds;
		return true;
	}

	bool SetDays(long nDays = -1)
	{
		if (nDays < 0)
		{
			m_nTime = m_nTime % IDK_DATE_SECONDS_IN_DAY;
		}
		else
		{
			long nTotalSeconds = nDays * IDK_DATE_SECONDS_IN_DAY;

			m_nTime = m_nTime % IDK_DATE_SECONDS_IN_DAY;

			m_nTime += nTotalSeconds;
		}
		return true;
	}

	unsigned long GetSeconds()
	{
		return (unsigned long)(m_nTime - (GetDays() * IDK_DATE_SECONDS_IN_DAY));
	}

	unsigned long GetDays()
	{
		return (unsigned long)(m_nTime / IDK_DATE_SECONDS_IN_DAY);
	}

	void Print(FILE *fp);

	std::string Print();
	
	std::string Format(const char *szFormat);

	static int GetElement(time_t Time, int ElementType);
protected:

	/*******************************************************
	**
	**	Properties
	**
	********************************************************/


	time_t m_nTime;

	static unsigned long m_daysInMonth[12];
	static unsigned long m_leapYearDaysInMonth[12];

	/*******************************************************
	**
	**	Methods
	**
	********************************************************/

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
	bool SetTime(unsigned long& nTime, long nHour, long nMinute, long nSecond);

private:


};

class CIDKLocalTime
{

	static time_t m_UpdateTime; // This is the time
						 // when the local time
						// looked up for time saving
	static time_t m_OffsetTime;
	static bool m_Once;

	time_t GetOffSet()
	{
		time_t l_SysTime = time(0);
		tm l_TMLocalTime;
		localtime_p(l_TMLocalTime, &l_SysTime);
		time_t l_LocalTime = mktime(&l_TMLocalTime);

		m_OffsetTime = 	l_LocalTime - l_SysTime;

		return m_OffsetTime;
	}

public:
	CIDKLocalTime()
	{
		if (m_Once)
		{
			m_OffsetTime = GetOffSet();	// set the update time for each 60 sec
			m_UpdateTime = time(0) + 60;
			m_Once = false;
		}
	}
	
	struct tm *GetTime(const time_t *timer)
	{	
		time_t l_Now = time(0);
		if (l_Now > m_UpdateTime)
		{
			m_OffsetTime = GetOffSet();	
			m_UpdateTime = l_Now + 60;
		}
		const time_t l_LocalTime = m_OffsetTime + *timer;
		struct tm timeptr;
		gmtime_p(timeptr, &l_LocalTime);
		
		/*
		time_t rawtime;
		struct tm timeinfo;
		time(&rawtime);
		_gmtime64_s(&timeinfo,&rawtime);
		char buffer[30];
		strftime(buffer,30,"%a, %d %b %Y %X GMT",&timeinfo);
		return static_cast<string>(buffer);
		*/


		return &timeptr;
	}

	struct tm *GetTimeNow()
	{
		time_t l_Now = time(0);

		return GetTime(&l_Now);
	}
};

#endif /* CIDKDate_h */



