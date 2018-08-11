



/*
  ** Copyright (c) 1990- 1993, 1996 Open Software Foundation, Inc.
  ** Copyright (c) 1989 by Hewlett-Packard Company, Palo Alto, Ca. &
  ** Digital Equipment Corporation, Maynard, Mass.
  ** Copyright (c) 1998 Microsoft.
  ** To anyone who acknowledges that this file is provided "AS IS"
  ** without any express or implied warranty: permission to use, copy,
  ** modify, and distribute this file for any purpose is hereby
  ** granted without fee, provided that the above copyright notices and
  ** this notice appears in all source code copies, and that none of
  ** the names of Open Software Foundation, Inc., Hewlett-Packard
  ** Company, or Digital Equipment Corporation be used in advertising
  ** or publicity pertaining to distribution of the software without
  ** specific, written prior permission.  Neither Open Software
  ** Foundation, Inc., Hewlett-Packard Company, Microsoft, nor Digital
  Equipment
  ** Corporation makes any representations about the suitability of
  ** this software for any purpose.
  */
#include <stdio.h>
#include "sysdep.h"

#undef uuid_t
  typedef struct _uuid_t {
      unsigned32          time_low;
      unsigned16          time_mid;
      unsigned16          time_hi_and_version;
      unsigned8           clock_seq_hi_and_reserved;
      unsigned8           clock_seq_low;
      byte                node[6];
  } uuid_t;

class CIDKUuid
{
	char m_szUuid[80];
	struct _uuid_t *m_Uuid;
public:
//private:
  /* uuid_create -- generate a UUID */
  int Create(uuid_t * uuid);

  /* uuid_create_from_name -- create a UUID using a "name"
     from a "name space" */
  void CreateFromName(
					uuid_t * uuid,        /* resulting UUID */
					uuid_t nsid,          /* UUID to serve as context, so identical
											names from different name spaces generate
											different UUIDs */
					void * name,          /* the name from which to generate a UUID */
					int namelen           /* the length of the name */
					);

public:

  /* uuid_compare --  Compare two UUID's "lexically" and return
          -1   u1 is lexically before u2
           0   u1 is equal to u2
           1   u1 is lexically after u2
     Note:   lexical ordering is not temporal ordering!
  */

  int Compare(uuid_t *u1, uuid_t *u2);

	CIDKUuid(const char *uuid);
	
	CIDKUuid() noexcept
	{
		m_Uuid = new struct _uuid_t;
		int m_Res = Create(m_Uuid);
		//printf("%s\n",GetUuid());
	};

	~CIDKUuid() {
		delete m_Uuid;
	}


	const char *GetUuid(uuid_t u)
	{
		int i;
#ifdef _WIN32
		sprintf_s(m_szUuid, 80, "%8.8x-%4.4x-%4.4x-%2.2x%2.2x-", u.time_low, u.time_mid,
								u.time_hi_and_version, u.clock_seq_hi_and_reserved,
								u.clock_seq_low);
		for (i = 0; i < 6; i++)
		{
			char tbuffer[30];
			sprintf_s(tbuffer, 30, "%2.2x", u.node[i]);
			strcat_s(m_szUuid,tbuffer);
		}
#else
		sprintf(m_szUuid, "%8.8x-%4.4x-%4.4x-%2.2x%2.2x-", u.time_low, u.time_mid,
										u.time_hi_and_version, u.clock_seq_hi_and_reserved,
										u.clock_seq_low);
		for (i = 0; i < 6; i++)
		{
			char tbuffer[30];
			sprintf(tbuffer, "%2.2x", u.node[i]);
			strcat(m_szUuid,tbuffer);
		}
#endif
		return m_szUuid;
	};

	const char *GetUuid()
	{
		return GetUuid(*m_Uuid);
	}
};
