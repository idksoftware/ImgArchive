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
#include "CIDKCrc.h"

//
// CPP
//
unsigned long CIDKCrc::crc_table[256];
bool CIDKCrc::crc_table_computed = false;

/* Make the table for a fast CRC. */
void CIDKCrc::make_crc_table()
{
	if (crc_table_computed == true) {
		return;
	}
	unsigned long c;
	int n, k;
   
	for (n = 0; n < 256; n++) {
		c = (unsigned long) n;
		for (k = 0; k < 8; k++) {
			if (c & 1)
				c = 0xedb88320L ^ (c >> 1);
			else
				c = c >> 1;
		}
		crc_table[n] = c;
	}
	crc_table_computed = true;
}

/* Update a running CRC with the bytes buf[0..len-1]--the CRC
   should be initialized to all 1's, and the transmitted value
   is the 1's complement of the final running CRC (see the
   crc() routine below)).
*/
   
unsigned long CIDKCrc::update_crc(unsigned long crc, unsigned char *buf,
                            int len)
{
	unsigned long c = crc;
	int n;
   
	for (n = 0; n < len; n++) {
		c = crc_table[(c ^ buf[n]) & 0xff] ^ (c >> 8);
	}
	return c;
}

/* Update a running CRC with the bytes buf[0..len-1]--the CRC
   should be initialized to all 1's, and the transmitted value
   is the 1's complement of the final running CRC (see the
   crc() routine below)).
*/
   
unsigned long CIDKCrc::update_crc(unsigned long crc, FILE *fp)
{
	unsigned char cbuf;
	unsigned long c = crc;

   	
	while (!feof(fp)) {
		fread(&cbuf,1,1,fp);
		c = crc_table[(c ^ cbuf) & 0xff] ^ (c >> 8);
		if( ferror(fp) ) {
			return false;
        }
	}
	return c;
}

unsigned long CIDKCrc::MakeCRC(const char *szPath,bool &status)
{
	status = true;
	unsigned long count = 0;
	CIDKCrc l_crc;

	FILE *fp = nullptr;
	fopen_p(fp, szPath, "r");
	if(!fp)
	{
		status = false;
		//
		// File may be in use
		//
		return 0;
	}
	
	count = l_crc.crc(fp);
	if (fclose(fp))
	{
		status = false;
		return 0;
	}
	return count;
}
