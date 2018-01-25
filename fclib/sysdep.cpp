#include "stdafx.h"
  #include <stdio.h>
  #include "sysdep.h"
#include "cport.h"

  /* system dependent call to get IEEE node ID.
     This sample implementation generates a random node ID
     */
  void get_ieee_node_identifier(uuid_node_t *node) {
    char seed[16];
    FILE * fd;
    static bool inited = false;
    static uuid_node_t saved_node;

    if (!inited) {
		fd = nullptr;
		fopen_p(fd, "nodeid", "rb");
        if (fd) {
             fread(&saved_node, sizeof(uuid_node_t), 1, fd);
             fclose(fd);
        }
        else {
             get_random_info(seed);
             seed[0] |= 0x80;
             memcpy(&saved_node, seed, sizeof(uuid_node_t));
			 fd = nullptr;
			 fopen_p(fd, "nodeid", "wb");
             if (fd) {
                    fwrite(&saved_node, sizeof(uuid_node_t), 1, fd);
                    fclose(fd);
             };
        };
        inited = true;
    };

    *node = saved_node;
  };

  /* system dependent call to get the current system time.
     Returned as 100ns ticks since Oct 15, 1582, but resolution may be
     less than 100ns.
  */
  #ifdef _WINDOWS_

  void get_system_time(uuid_time_t *uuid_time) {
    ULARGE_INTEGER time;

    GetSystemTimeAsFileTime((FILETIME *)&time);

      /* NT keeps time in FILETIME format which is 100ns ticks since
       Jan 1, 1601.  UUIDs use time in 100ns ticks since Oct 15, 1582.
       The difference is 17 Days in Oct + 30 (Nov) + 31 (Dec)
       + 18 years and 5 leap days.
    */

      time.QuadPart +=
            (unsigned __int64) (1000*1000*10)       // seconds
          * (unsigned __int64) (60 * 60 * 24)       // days
          * (unsigned __int64) (17+30+31+365*18+5); // # of days

    *uuid_time = time.QuadPart;

  };

  void get_random_info(char seed[16]) {
    
    typedef struct {
        MEMORYSTATUS m;
        SYSTEM_INFO s;
        FILETIME t;
        LARGE_INTEGER pc;
        DWORD tc;
        DWORD l;
        char hostname[MAX_COMPUTERNAME_LENGTH + 1];
    } randomness;
    randomness r;

   
    /* memory usage stats */
    GlobalMemoryStatus(&r.m);
    /* random system stats */
    GetSystemInfo(&r.s);
    /* 100ns resolution (nominally) time of day */
    GetSystemTimeAsFileTime(&r.t);
    /* high resolution performance counter */
    QueryPerformanceCounter(&r.pc);
    /* milliseconds since last boot */
    r.tc = GetTickCount64();
    r.l = MAX_COMPUTERNAME_LENGTH + 1;
    //GetComputerName(r.hostname, &r.l );
	/*
	MD5_CTX c;
	MD5Init(&c);
    MD5Update(&c, (unsigned char *)&r, sizeof(randomness));
    MD5Final((unsigned char *)seed, &c);
	*/
	MD5 md5;
	
	md5.update((unsigned char *)&r, sizeof(randomness));
	md5.finalize();
	memcpy(seed, md5.getDigest(), 16);
  };
  #else

  void get_system_time(uuid_time_t *uuid_time)
  {
      struct timeval tp;

      gettimeofday(&tp, (struct timezone *)0);

      /* Offset between UUID formatted times and Unix formatted times.
         UUID UTC base time is October 15, 1582.
         Unix base time is January 1, 1970.
      */
      *uuid_time = (tp.tv_sec * 10000000) + (tp.tv_usec * 10) +
        I64(0x01B21DD213814000);
  };

  void get_random_info(char seed[16]) {

    typedef struct {
        struct sysinfo s;
        struct timeval t;
        char hostname[257];
    } randomness;
    randomness r;


    sysinfo(&r.s);
    gettimeofday(&r.t, (struct timezone *)0);
    /*
    MD5_CTX c;
    MD5Init(&c);
    gethostname(r.hostname, 256);
    MD5Update(&c, &r, sizeof(randomness));
    MD5Final(seed, &c);
    */
  };

  #endif
