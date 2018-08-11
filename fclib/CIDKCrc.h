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

#if !defined(CIDKCrc_h)
#define CIDKCrc_h

#include <stdio.h>
/*
15. Appendix: Sample CRC Code
The following sample code represents a practical implementation of the CRC
(Cyclic Redundancy Check) employed in PNG chunks.
(See also ISO 3309 [ISO-3309] or ITU-T V.42 [ITU-V42] for a formal specification.) 
The sample code is in the ANSI C programming language.
Non C users may find it easier to read with these hints: 

& 
Bitwise AND operator. 
^ 
Bitwise exclusive-OR operator. (Caution: elsewhere in this document, ^ represents exponentiation.) 
>> 
Bitwise right shift operator. When applied to an unsigned quantity, as here, right shift inserts zeroes at the left. 
! 
Logical NOT operator. 
++ 
"n++" increments the variable n. 
0xNNN 
0x introduces a hexadecimal (base 16) constant. Suffix L indicates a long value (at least 32 bits). 

*/

class CIDKCrc
{

	/* Table of CRCs of all 8-bit messages. */
	static unsigned long crc_table[256];
   
	/* Flag: has the table been computed? Initially false. */
	static bool crc_table_computed;
   
	/* Make the table for a fast CRC. */
	void make_crc_table();
	
   
	/*
	Update a running CRC with the bytes buf[0..len-1]--the CRC
	should be initialized to all 1's, and the transmitted value
	is the 1's complement of the final running CRC (see the
	crc() routine below)).
	*/
   
	unsigned long update_crc(unsigned long crc,
					unsigned char *buf, int len);
	unsigned long update_crc(unsigned long crc,
					FILE *fp);
public:

	CIDKCrc() noexcept
	{
		make_crc_table();
	}

	/* Return the CRC of the bytes buf[0..len-1]. */
	unsigned long crc(unsigned char *buf, int len)
	{
		return update_crc(0xffffffffL, buf, len) ^ 0xffffffffL;
	};

	unsigned long crc(FILE *fp)
	{
		return update_crc(0xffffffffL, fp) ^ 0xffffffffL;
	};

	static unsigned long MakeCRC(const char *szPath,bool &status);
};

#endif
