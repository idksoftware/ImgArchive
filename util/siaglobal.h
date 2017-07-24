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
**	@File		siaglobal.h
**	@Author		I.Ferguson
**	@Version	1.000
**	@Date		26-05-2015
**
** #$$@@$$# */

#pragma once

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>

#ifdef _WIN32
#include <crtdbg.h>
#endif

#ifdef _DEBUG
#ifndef DEBUG_NEW
#define DEBUG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DEBUG_NEW
#endif
#endif  // _DEBUG

#define LOG_OK					0
#define LOG_DUPLICATE			10
#define LOG_INVALID_FILE_TYPE	11
#define LOG_WORKSPACE_NOT_FOUND	12

#define LOG_STARTING			30
#define LOG_ANALISING			31
#define LOG_IMPORTING			32

#define LOG_IMAGE_SUMMARY		101
#define LOG_FOLDER_SUMMARY		102
#define LOG_CURRENT_IMAGE		103
#define LOG_IMAGE_NAME			104

