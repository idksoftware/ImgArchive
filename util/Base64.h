#pragma once

#include <string>
/*

	RFC 4648 

Table 1: The Base 64 Alphabet
 Value Encoding Value Encoding Value Encoding Value Encoding
 0 A 17 R 34 i 51 z
 1 B 18 S 35 j 52 0
 2 C 19 T 36 k 53 1
 3 D 20 U 37 l 54 2
 4 E 21 V 38 m 55 3
 5 F 22 W 39 n 56 4
 6 G 23 X 40 o 57 5
 7 H 24 Y 41 p 58 6
 8 I 25 Z 42 q 59 7
 9 J 26 a 43 r 60 8
 10 K 27 b 44 s 61 9
 11 L 28 c 45 t 62 +
 12 M 29 d 46 u 63 /
 13 N 30 e 47 v
 14 O 31 f 48 w (pad) =
 15 P 32 g 49 x
 16 Q 33 h 50 y
*/
/*
Base 64 Encoding with URL and Filename Safe Alphabet
The Base 64 encoding with an URL and filename safe alphabet has been
used in[12].
An alternative alphabet has been suggested that would use "~" as the
63rd character.Since the "~" character has special meaning in some
file system environments, the encoding described in this section is
recommended instead.The remaining unreserved URI character is ".",
but some file system environments do not permit multiple "." in a
filename, thus making the "." character unattractive as well.
The pad character "=" is typically percent - encoded when used in an
URI[9], but if the data length is known implicitly, this can be
avoided by skipping the padding; see section 3.2.
This encoding may be referred to as "base64url".This encoding
should not be regarded as the same as the "base64" encoding and
should not be referred to as only "base64".Unless clarified
otherwise, "base64" refers to the base 64 in the previous section.
This encoding is technically identical to the previous one, except
for the 62:nd and 63 : rd alphabet character, as indicated in Table 2.
Josefsson Standards Track[Page 7]
RFC 4648 Base - N Encodings October 2006
Table 2 : The "URL and Filename safe" Base 64 Alphabet
Value Encoding Value Encoding Value Encoding Value Encoding

0 A 17 R 34 i 51 z
1 B 18 S 35 j 52 0
2 C 19 T 36 k 53 1
3 D 20 U 37 l 54 2
4 E 21 V 38 m 55 3
5 F 22 W 39 n 56 4
6 G 23 X 40 o 57 5
7 H 24 Y 41 p 58 6
8 I 25 Z 42 q 59 7
9 J 26 a 43 r 60 8
10 K 27 b 44 s 61 9
11 L 28 c 45 t 62 - (minus)
12 M 29 d 46 u 63 _ (underline)
13 N 30 e 47 v
14 O 31 f 48 w
15 P 32 g 49 x
16 Q 33 h 50 y(pad) =
*/

class Base64 {
	std::string m_str;
	unsigned long m_int;
public:
	Base64() {};
	virtual ~Base64() {};
	const char *tobase64(long dec);
	int toDecimal(std::string encoded);
	const char *getBase64() {
		return m_str.c_str();
	}
};

class Base64url {
	std::string m_str;
	unsigned long m_int;
public:
	Base64url() {};
	virtual ~Base64url() {};
	const char* tobase64(long dec);
	int toDecimal(std::string encoded);
	const char* getBase64() {
		return m_str.c_str();
	}
};
