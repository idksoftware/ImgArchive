// UniqueId.cpp : Defines the entry point for the console application.
//
#ifdef _WIN32
#include <windows.h>
#include <direct.h>
#include <stdarg.h>
#else
#include <sys/stat.h>
#endif

#include <string>

using namespace std;


#define ENCODE_LENGTH 64

static char base64[] = {
	'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
	'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
	'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
	'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
	'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/' };


std::string encode(int victim)
{
	std::string list;
	do {
		list += base64[victim % ENCODE_LENGTH];
		victim /= ENCODE_LENGTH;
	} while (victim > 0);
	std::reverse(list.begin(), list.end());

	return list;
}


int decode(std::string encoded)
{
	int ret = 0;
	int len = encoded.length();
	int num = 0;
	for (int i = 0; i < len; i++) {
		char c = encoded[i];
		if (c >= 'A' && c <= 'Z') {
			num = c - 'A';
		}
		else if (c >= 'a' && c <= 'z') {
			num = (c - 'a') + 26;
		}
		else if (c >= '0' && c <= '9') {
			num = (c - '0') + 52;
		}
		else if (c == '+') {
			num = (c - '+') + 62;
		}
		else if (c == '/') {
			num = (c - '/') + 63;
		}
		ret *= ENCODE_LENGTH;
		ret += num;
	}
	
	return ret;
}

/*
int main(int argc, char* argv[])
{
	int i = 0;
	
	//std::string test = "BA";
	//int num = decode(test);
	//printf("%d\n", num);
	
	for (int i = 0; i < 64 * 4; i++) {
			printf(" %d encode %s decode %d\n", i, encode(i).c_str(), decode(encode(i)));
	}
	
	return 0;
}
*/
