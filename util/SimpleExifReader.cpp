/**************************************************************************
  exif.cpp  -- A simple ISO C++ library to parse basic EXIF 
               information from a JPEG file.
  Copyright (c) 2010-2015 Mayank Lahiri
  mlahiri@gmail.com
  All rights reserved (BSD License).
  See exif.h for version history.
  Redistribution and use in source and binary forms, with or without 
  modification, are permitted provided that the following conditions are met:
  -- Redistributions of source code must retain the above copyright notice, 
     this list of conditions and the following disclaimer.
  -- Redistributions in binary form must reproduce the above copyright notice, 
     this list of conditions and the following disclaimer in the documentation 
     and/or other materials provided with the distribution.
     THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ``AS IS'' AND ANY EXPRESS 
     OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES 
     OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN 
     NO EVENT SHALL THE FREEBSD PROJECT OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
     INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, 
     BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, 
     DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY 
     OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
     NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, 
     EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <stdio.h>
#include <algorithm>
#include "JpegEXIF.h"
#include "SimpleExifReader.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
//#define new DEBUG_NEW
#endif

using std::string;

namespace {

	/*
	4.6.2
	IFD Structure
	The  IFD  used  in  this  standard  consists  of  a  2-
	byte  count  (number  of  fields),  12-byte  field
	Interoperability arrays, and 4-byte offset to the nex
	t IFD, in conformance with TIFF Rev. 6.0.Each of
	the 12-byte field Interoperability consists of
	the following four elements respectively.
	Bytes  0-1      Tag
	Bytes 2-3		Type
	Bytes 4-7		Count
	Bytes 8-11		Value Offset
	Each element is explained briefly below. For details see TIFF Rev. 6.0.
	Tag
	Each tag is assigned a unique 2-byte number to identify the field. The tag
	numbers in the Exif 0th IFD and 1st IFD are all the same as the TIFF tag numbers.
	Type
	The following types are used in Exif:
	*/
	enum class ExifType {
		BYTE = 1,		// BYTE		An 8 - bit unsigned integer.
		ASCII = 2,		// ASCII                       An  8 - bit  byte  containing  one  7 - bit  ASCII  code.
		// The final byte is terminated with NULL.
		SHORT = 3,		// SHORT                    A  16 - bit(2 - byte)  unsigned  integer,
		LONG = 4,		// LONG                      A  32 - bit(4 - byte)  unsigned  integer, JEITA  CP - 3451C CIPADC - 008 - 201226
		RATIONAL = 5,	// RATIONAL            Two  LONGs.The  first  LONG  is  the  numerator  and  the  second  LONG
		// expresses the denominator.
		SBYTE = 6,		// signed byte
		UNDEFINED = 7,	// UNDEFINED           An 8 - bit byte that may take any value depending on the field definition.
		SSHORT = 8,		// signed short
		SLONG = 9,		//  Signed LONG  A 32 - bit(4 - byte) signed integer(2's complement notation). 
		SRATIONAL = 10	// SRATIONAL Two Signed LONGs.The first SLONG is the numerator and the second SLONG is the denominator
	};
/*
	Count
		The number of values. It should be noted carefully that the count is not the sum of the bytes. In the
		case of one value of SHORT (16 bits), for exampl
		e, the count is '1' even though it is 2 Bytes.
		Value Offset
		This tag records the offset from the start of the
		TIFF header to the position where the value itself is
		recorded. In cases where the value fits in 4 Bytes,
		the value itself is recorded. If the value is smaller
		than 4 Bytes, the value is stored in
		the 4-Byte area starting from the left, i.e., from the lower end of
		the byte offset area. For example, in big endian form
		at, if the type is SHORT and the value is 1, it is
		recorded as 00010000.H.
		Note  that  field  Interoperability  shall  be  recorded  in
		sequence  starting  from  the  smallest  tag  number.
		There is no stipulation regarding the order or
		position of tag value (Value) recording.
*/
	
	// IF Entry 
	struct IFEntry {
		// Raw fields
		unsigned short tag;
		ExifType format;
		unsigned data;
		unsigned length;

		// Parsed fields
		string val_string;
		unsigned short val_16;
		unsigned val_32;
		//double val_rational;
		unsigned char val_byte;
		unsigned char val_bytes[80];
		Rational val_rational;
	};


	char byte2numberChar(unsigned char b) {
		return (b + '0');
	}
	// Helper functions
	unsigned int parse32(const unsigned char *buf, bool intel) {
		if (intel)
			return ((unsigned)buf[3] << 24) |
			((unsigned)buf[2] << 16) |
			((unsigned)buf[1] << 8) |
			buf[0];

		return ((unsigned)buf[0] << 24) |
			((unsigned)buf[1] << 16) |
			((unsigned)buf[2] << 8) |
			buf[3];
	}

	unsigned short parse16(const unsigned char *buf, bool intel) {
		if (intel)
			return ((unsigned)buf[1] << 8) | buf[0];
		return ((unsigned)buf[0] << 8) | buf[1];
	}

	string parseEXIFString(const unsigned char *buf,
		const unsigned num_components,
		const unsigned data,
		const unsigned base,
		const unsigned len) {
		string value;
		if (num_components <= 4)
			value.assign((const char*)&data, num_components);
		else {
			if (base + data + num_components <= len)
				value.assign((const char*)(buf + base + data), num_components);
		}
		return value;
	}

	string parseUCS2String(const unsigned char *buf,
		const unsigned num_components,
		const unsigned data,
		const unsigned base,
		const unsigned len) {
		string value;

		if (num_components <= 4) {
			value.assign((const char*)&data, num_components);

		}
		else {
			int j = 0;
			char *outBuf = new char[num_components];
			for (int i = 0; i < num_components; i++) {
				unsigned char c = (unsigned char)*(buf + base + data + i);
				if (c == 0) {
					continue;
				}
				outBuf[j++] = c;
			}
			outBuf[j] = '\0';
			value = outBuf;
		}

		return value;
	}



	static int Get32s(void * Long, bool intel)
	{
		if (!intel){
			return  (((char *)Long)[0] << 24) | (((unsigned char *)Long)[1] << 16)
				| (((unsigned char *)Long)[2] << 8) | (((unsigned char *)Long)[3] << 0);
		}
		else{
			return  (((char *)Long)[3] << 24) | (((unsigned char *)Long)[2] << 16)
				| (((unsigned char *)Long)[1] << 8) | (((unsigned char *)Long)[0] << 0);
		}
	}

	int parseEXIFNumerator(const unsigned char *buf, bool intel) {
		return parse32(buf, intel);
	}

	int parseEXIFDenominator(const unsigned char *buf, bool intel) {
		return parse32(buf + 4, intel);
	}

	double parseEXIFRational(const unsigned char *buf, bool intel) {


		//printf("%d/%d\n", Get32s((void *)buf, intel), Get32s(4 + (char *)buf, intel));
		double numerator = 0;
		double denominator = 1;

		numerator = (double)parse32(buf, intel);
		denominator = (double)parse32(buf + 4, intel);
		if (denominator < 1e-20)
			return 0;
		return numerator / denominator;
	}

	IFEntry parseIFEntry(const unsigned char *buf,
		const unsigned offs,
		const bool alignIntel,
		const unsigned base,
		const unsigned len) {
		IFEntry result;

		// Each directory entry is composed of:
		// 2 bytes: tag number (data field)
		// 2 bytes: data format
		// 4 bytes: number of components
		// 4 bytes: data value or offset to data value
		result.tag = parse16(buf + offs, alignIntel);


		result.format = (ExifType)parse16(buf + offs + 2, alignIntel);
		result.length = parse32(buf + offs + 4, alignIntel);
		result.data = parse32(buf + offs + 8, alignIntel);

		if (result.tag == 0x9c9b || //Image Exif.Image.XPTitle Byte Title tag used by Windows, encoded in UCS2
			result.tag == 0x9c9c || // Image Exif.Image.XPComment Byte Comment tag used by Windows, encoded in UCS2
			result.tag == 0x9c9d || // Image Exif.Image.XPAuthor Byte Author tag used by Windows, encoded in UCS2
			result.tag == 0x9c9e || // Image Exif.Image.XPKeywords Byte Keywords tag used by Windows, encoded in UCS2
			result.tag == 0x9c9f) { // Image Exif.Image.XPSubject
			
			result.val_string = parseUCS2String(buf, result.length, result.data, base, len);
			result.format = ExifType::SBYTE;
		}
		else {
			// Parse value in specified format
			switch (result.format) {
			case ExifType::BYTE:
				result.val_byte = (unsigned char)*(buf + offs + 8);
				break;
			case ExifType::ASCII:
				result.val_string = parseEXIFString(buf, result.length, result.data, base, len);
				break;
			case ExifType::SHORT:
				result.val_16 = parse16((const unsigned char *)buf + offs + 8, alignIntel);
				break;
			case ExifType::LONG:
				result.val_32 = result.data;
				break;
			case ExifType::RATIONAL:
				if (base + result.data + 8 <= len) {
					result.val_rational.rational = parseEXIFRational(buf + base + result.data, alignIntel);
					result.val_rational.numerator = parseEXIFNumerator(buf + base + result.data, alignIntel);
					result.val_rational.denominator = parseEXIFDenominator(buf + base + result.data, alignIntel);
				}
				break;
			case ExifType::UNDEFINED:
				result.val_32 = result.data;
				break;
			case ExifType::SSHORT:
				break;
			case ExifType::SLONG:
				break;
			case ExifType::SRATIONAL:
				if (base + result.data + 8 <= len) {
					result.val_rational.rational = parseEXIFRational(buf + base + result.data, alignIntel);
					result.val_rational.numerator = parseEXIFNumerator(buf + base + result.data, alignIntel);
					result.val_rational.denominator = parseEXIFDenominator(buf + base + result.data, alignIntel);
				}
				break;
			default:
				result.tag = 0xFF;
			}
		}
		return result;
	}
}

//
// Locates the EXIF segment and parses it using parseFromEXIFSegment 
//
JpegExif_Ptr EXIFInfo::parseFrom(const unsigned char *buf, unsigned len) {
	// Sanity check: all JPEG files start with 0xFFD8 and end with 0xFFD9
	// This check also ensures that the user has supplied a correct value for len.
	JpegExif_Ptr jpegExif(new JpegEXIF);
	jpegExif->clear();
	unsigned char b1 = buf[0];
	unsigned char b2 = buf[1];
	unsigned char b3 = buf[len - 2];
	unsigned char b4 = buf[len - 1];
	if (!buf || len < 4) {
		jpegExif->error = PARSE_EXIF_ERROR_NO_EXIF;
		return jpegExif;
	}
	if (buf[0] != 0xFF || buf[1] != 0xD8) {
		jpegExif->error = PARSE_EXIF_ERROR_NO_JPEG;
		return jpegExif;
	}
		
	//if (buf[len - 2] != 0xFF || buf[len - 1] != 0xD9)
	//	return PARSE_EXIF_ERROR_NO_JPEG;
	

	// Scan for EXIF header (bytes 0xFF 0xE1) and do a sanity check by 
	// looking for bytes "Exif\0\0". The marker length data is in Motorola
	// byte order, which results in the 'false' parameter to parse16().
	// The marker has to contain at least the TIFF header, otherwise the
	// EXIF data is corrupt. So the minimum length specified here has to be:
	//   2 bytes: section size
	//   6 bytes: "Exif\0\0" string
	//   2 bytes: TIFF header (either "II" or "MM" string)
	//   2 bytes: TIFF magic (short 0x2a00 in Motorola byte order)
	//   4 bytes: Offset to first IFD
	// =========
	//  16 bytes
	unsigned offs = 0;        // current offset into buffer
	for (offs = 0; offs < len - 1; offs++)
		if (buf[offs] == 0xFF && buf[offs + 1] == 0xE1)
			break;
	if (offs + 4 > len) {
		jpegExif->error = PARSE_EXIF_ERROR_NO_EXIF;
		return jpegExif;
	}
	offs += 2;
	unsigned short section_length = parse16(buf + offs, false);
	if (offs + section_length > len || section_length < 16) {
		jpegExif->error = PARSE_EXIF_ERROR_CORRUPT;
		return jpegExif;
	}
	offs += 2;

  return parseFromEXIFSegment(buf + offs, len - offs);
}

JpegExif_Ptr EXIFInfo::parseFrom(const string &data) {
  return parseFrom((const unsigned char *)data.data(), data.length());
}

//
// Main parsing function for an EXIF segment.
//
// PARAM: 'buf' start of the EXIF TIFF, which must be the bytes "Exif\0\0".
// PARAM: 'len' length of buffer
//
JpegExif_Ptr EXIFInfo::parseFromEXIFSegment(const unsigned char *buf, unsigned len) {

	JpegExif_Ptr jpegExif(new JpegEXIF);

	bool alignIntel = true;     // byte alignment (defined in EXIF header)
	unsigned offs = 0;        // current offset into buffer
	if (!buf || len < 6) {
		jpegExif->error = PARSE_EXIF_ERROR_NO_EXIF;
		return jpegExif;
	}
	if (!std::equal(buf, buf + 6, "Exif\0\0")) {
		jpegExif->error = PARSE_EXIF_ERROR_NO_EXIF;
		return jpegExif;
	}
  offs += 6;
  
  // Now parsing the TIFF header. The first two bytes are either "II" or
  // "MM" for Intel or Motorola byte alignment. Sanity check by parsing
  // the unsigned short that follows, making sure it equals 0x2a. The
  // last 4 bytes are an offset into the first IFD, which are added to 
  // the global offset counter. For this block, we expect the following
  // minimum size:
  //  2 bytes: 'II' or 'MM'
  //  2 bytes: 0x002a
  //  4 bytes: offset to first IDF
  // -----------------------------
  //  8 bytes
  if (offs + 8 > len) {
	  jpegExif->error = PARSE_EXIF_ERROR_CORRUPT;
	  return jpegExif;
  }
  unsigned tiff_header_start = offs;
  if (buf[offs] == 'I' && buf[offs+1] == 'I')
    alignIntel = true;
  else {
    if(buf[offs] == 'M' && buf[offs+1] == 'M')
      alignIntel = false;
	else {
		jpegExif->error = PARSE_EXIF_ERROR_UNKNOWN_BYTEALIGN;
		return jpegExif;
	}
  }
  jpegExif->ByteAlign = alignIntel;
  offs += 2;
  if (0x2a != parse16(buf + offs, alignIntel)) {
	  jpegExif->error = PARSE_EXIF_ERROR_CORRUPT;
	  return jpegExif;
  }
  offs += 2;
  unsigned first_ifd_offset = parse32(buf + offs, alignIntel);
  offs += first_ifd_offset - 4;
  if (offs >= len) {
	  jpegExif->error = PARSE_EXIF_ERROR_CORRUPT;
	  return jpegExif;
  }

  // Now parsing the first Image File Directory (IFD0, for the main image).
  // An IFD consists of a variable number of 12-byte directory entries. The
  // first two bytes of the IFD section contain the number of directory
  // entries in the section. The last 4 bytes of the IFD contain an offset
  // to the next IFD, which means this IFD must contain exactly 6 + 12 * num
  // bytes of data.
  if (offs + 2 > len) {
	  jpegExif->error = PARSE_EXIF_ERROR_CORRUPT;
	  return jpegExif;
  }
  int num_entries = parse16(buf + offs, alignIntel);
  if (offs + 6 + 12 * num_entries > len) {
	  jpegExif->error = PARSE_EXIF_ERROR_CORRUPT;
	  return jpegExif;
  }
  offs += 2;
  unsigned exif_sub_ifd_offset = len;
  unsigned gps_sub_ifd_offset  = len;
  while (--num_entries >= 0) {
    IFEntry result = parseIFEntry(buf, offs, alignIntel, tiff_header_start, len);
    offs += 12;
    switch(result.tag) {
      case 0x102:
        // Bits per sample
        if (result.format == ExifType::SHORT)
			jpegExif->BitsPerSample = result.val_16;
        break;

	  case 0x0103:
		  // compression
		  if (result.format == ExifType::SHORT)
			  jpegExif->compression = result.val_16;
		  break;

	  case 0x0106:
		  // compression
		  if (result.format == ExifType::SHORT)
			  jpegExif->photometricInterpretation = result.val_16;
		  break;


      case 0x10E:
        // Image description
        if (result.format == ExifType::ASCII)
			jpegExif->ImageDescription = result.val_string;
        break;

      case 0x10F:
        // Digicam make
		  if (result.format == ExifType::ASCII)
			jpegExif->Make = result.val_string;
        break;

      case 0x110:
        // Digicam model
		  if (result.format == ExifType::ASCII)
			jpegExif->Model = result.val_string;
        break;

      case 0x112:
        // Orientation of image
		  if (result.format == ExifType::SHORT)
			jpegExif->Orientation = result.val_16;
        break;
	  case 0x011a:
		  if (result.format == ExifType::RATIONAL)
			  jpegExif->xResolution = result.val_rational.rational;
		  break;
	  case 0x011b:
		  if (result.format == ExifType::RATIONAL)
			  jpegExif->yResolution = result.val_rational.rational;
		  break;
	  case 0x0128:
		  if (result.format == ExifType::SHORT)
			  jpegExif->resolutionUnit = result.val_16;
		  break;

      case 0x131:
        // Software used for image
		  if (result.format == ExifType::ASCII)
			jpegExif->Software = result.val_string;
        break;

      case 0x132:
        // EXIF/TIFF date/time of image modification
		  if (result.format == ExifType::ASCII)
			jpegExif->DateTime = result.val_string;
        break;
	  case 0x9c9b:
		  // Image Exif.Image.XPTitle Byte Title tag used by Windows, encoded in UCS2
		  if (result.format == ExifType::SBYTE)
			jpegExif->title = result.val_string;
			//printf("%s", result.val_string.c_str());
		break;
	  case 0x9c9c:
		  // Image Exif.Image.XPComment Byte Comment tag used by Windows, encoded in UCS2
		  if (result.format == ExifType::SBYTE)
			  //printf("%s", result.val_string.c_str());
			jpegExif->comment = result.val_string;
		  break;
	  case 0x9c9d:
		  // Image Exif.Image.XPAuthor Byte Author tag used by Windows, encoded in UCS2
		  if (result.format == ExifType::SBYTE)
			  //printf("%s", result.val_string.c_str());
			jpegExif->author = result.val_string;
		  break;
	  case 0x9c9e:
		  // Image Exif.Image.XPKeywords Byte Keywords tag used by Windows, encoded in UCS2
		  if (result.format == ExifType::SBYTE)
			jpegExif->keywords = result.val_string;
			// printf("%s", result.val_string.c_str());
		  break;
	  case 0x9c9f:
		  // Image Exif.Image.XPSubject
		  if (result.format == ExifType::SBYTE)
			  //printf("%s", result.val_string.c_str());
		  jpegExif->subject = result.val_string;
		  break;
	  case 0x4746:
		  // Image Exif.Image.Rating Short Rating tag used by Windows
		  if (result.format == ExifType::SHORT)
			  jpegExif->rating = result.val_16;
		  break;
	  case 0x4749:
		  // Image Exif.Image.RatingPercent Short Rating tag used by Windows, value in percent
		  if (result.format == ExifType::SHORT)
			  jpegExif->ratingPercent = result.val_16;
			
		  break;
      case 0x8298:
        // Copyright information
		  if (result.format == ExifType::ASCII)
			jpegExif->copyright = result.val_string;
        break;

      case 0x8825:
        // GPS IFS offset
        gps_sub_ifd_offset = tiff_header_start + result.data;
        break;

      case 0x8769:
        // EXIF SubIFD offset
        exif_sub_ifd_offset = tiff_header_start + result.data;
        break;

	 
	  
    }
  }

  // Jump to the EXIF SubIFD if it exists and parse all the information
  // there. Note that it's possible that the EXIF SubIFD doesn't exist.
  // The EXIF SubIFD contains most of the interesting information that a
  // typical user might want.
  if (exif_sub_ifd_offset + 4 <= len) {
    offs = exif_sub_ifd_offset;
    int num_entries = parse16(buf + offs, alignIntel);
	if (offs + 6 + 12 * num_entries > len) {
		jpegExif->error = PARSE_EXIF_ERROR_CORRUPT;
		return jpegExif;
	}
    offs += 2;
    while (--num_entries >= 0) {
      IFEntry result = parseIFEntry(buf, offs, alignIntel, tiff_header_start, len);
      switch(result.tag) {
        case 0x829a:
          // Exposure time in seconds
			if (result.format == ExifType::RATIONAL)
			  jpegExif->ExposureTime = result.val_rational;
          break;

        case 0x829d:
          // FNumber
			if (result.format == ExifType::RATIONAL) {
				jpegExif->FNumber = result.val_rational;
				//printf("%d/%d\n", result.val_rational.numerator, result.val_rational.denominator);
			}
          break;
		case 0x8822: 
		  // Exposure Program
			if (result.format == ExifType::SHORT)
				jpegExif->exposureProgram = result.val_16;
			break;
        case 0x8827:
          // ISO Speed Rating
			if (result.format == ExifType::SHORT)
			  jpegExif->ISOSpeedRatings = result.val_16;
          break;

		case 0x9000:
			// Original date and time
		{
			if (result.format == ExifType::UNDEFINED) {
				char buff[5];
				unsigned long val = result.val_32;
				buff[0] = (char)(val & 0x000000FF);
				buff[1] = (char)((val >> 8) & 0x000000FF);
				buff[2] = (char)((val >> 16) & 0x000000FF);
				buff[3] = (char)((val >> 24) & 0x000000FF);
				buff[4] = '\0';
				//printf("%s", buff);
				jpegExif->exifVersion = buff;
			}
		}
			break;
        case 0x9003:
          // Original date and time
			if (result.format == ExifType::ASCII)
			  jpegExif->DateTimeOriginal = result.val_string;
          break;

        case 0x9004:
          // Digitization date and time
			if (result.format == ExifType::ASCII)
			  jpegExif->DateTimeDigitized = result.val_string;
          break;

		case 0x9102:
			// compressed data; states the compressed bits per pixel. 
			if (result.format == ExifType::RATIONAL) {
				jpegExif->compressedBitsPerPixel.rational = result.val_rational.rational;
			}
			break;
		case 0x9205:
			// Max aperture value
			if (result.format == ExifType::RATIONAL)
				jpegExif->maxApertureValue.rational = result.val_rational.rational;
			break;
			
     
		case 0x9203:
			// Exposure bias value 
			if (result.format == ExifType::RATIONAL)
				jpegExif->brightnessValue = result.val_rational;
			if (result.format == ExifType::SRATIONAL) {
				jpegExif->brightnessValue = result.val_rational;
				//printf("%d/%d\n", result.val_rational.numerator, result.val_rational.denominator);
			}
			jpegExif->brightnessValue.numerator = result.val_rational.rational;
			break;
        case 0x9204:
          // Exposure bias value 
			if (result.format == ExifType::RATIONAL)
			  jpegExif->ExposureBiasValue = result.val_rational;
			if (result.format == ExifType::SRATIONAL) {
				jpegExif->ExposureBiasValue = result.val_rational;
				//printf("%d/%d\n", result.val_rational.numerator, result.val_rational.denominator);
			}
          break;
		  
        case 0x9206:
          // Subject distance
			if (result.format == ExifType::RATIONAL)
			  jpegExif->SubjectDistance = result.val_rational.rational;
          break;

        case 0x9209:
          // Flash used
			if (result.format == ExifType::SHORT)
			  jpegExif->Flash = result.data;
          break;
		case 0x920b:
			if (result.format == ExifType::RATIONAL) {
				jpegExif->flashEnergy = result.val_rational.rational;
			}
			break;
        case 0x920a:
          // Focal length
			if (result.format == ExifType::RATIONAL)
			  jpegExif->FocalLength = result.val_rational.rational;
          break;

        case 0x9207:
          // Metering mode
			if (result.format == ExifType::SHORT)
			  jpegExif->meteringMode = result.val_16;
          break;

		case 0x9208:
		  //LightSource Short The kind of light source.
		  if (result.format == ExifType::SHORT)
			  jpegExif->lightSource = result.val_16;
		  break;
		 
        case 0x9291:
          // Subsecond original time
			if (result.format == ExifType::ASCII)
			  jpegExif->SubSecTimeOriginal = result.val_string;
          break;
		case 0xa001:
			// Color space
			if (result.format == ExifType::SHORT)
				jpegExif->colorSpace = result.val_16;
			break;
        case 0xa002:
          // EXIF Image width
          if (result.format == ExifType::LONG)
			  jpegExif->ImageWidth = result.val_32;
		  if (result.format == ExifType::SHORT)
			  jpegExif->ImageWidth = result.val_16;
          break;

        case 0xa003:
          // EXIF Image height
			if (result.format == ExifType::LONG)
			  jpegExif->ImageHeight = result.val_32;
		  if (result.format == ExifType::SHORT)
			  jpegExif->ImageHeight = result.val_16;
          break;

		case 0xa403:
			// WhiteBalance
			if (result.format == ExifType::SHORT)
				jpegExif->whiteBalance = result.val_16;
			break;

		case 0xa404:
			// DigitalZoomRatio
			if (result.format == ExifType::RATIONAL)
				jpegExif->DigitalZoomRatio = result.val_rational.rational;
			break;

        case 0xa405:
          // Focal length in 35mm film
			if (result.format == ExifType::SHORT)
			  jpegExif->FocalLengthIn35mm = result.val_16;
          break;
		case 0xa406:
			// SceneCaptureType
			if (result.format == ExifType::SHORT)
				jpegExif->SceneCaptureType = result.val_16;
			break;
		case 0xa408:
			// Contrast
			if (result.format == ExifType::SHORT)
				jpegExif->Contrast = result.val_16;
			break;
		case 0xa409:
			// Saturation
			if (result.format == ExifType::SHORT)
				jpegExif->Saturation = result.val_16;
			break;
		case 0xa40a:
			// Sharpness
			if (result.format == ExifType::SHORT)
				jpegExif->Sharpness = result.val_16;
			break;

		case 0xa40c:
			// SubjectDistanceRange
			if (result.format == ExifType::SHORT)
				jpegExif->SubjectDistanceRange = result.val_16;
			break;
			/*
			0xa430 42032 Photo Exif.Photo.CameraOwnerName Ascii This tag records the owner of a camera used in photography as an ASCII string.
			0xa431 42033 Photo Exif.Photo.BodySerialNumber Ascii This tag records the serial number of the body of the camera that was used in photography as an ASCII string.
			
			0xa433 42035 Photo Exif.Photo.LensMake Ascii This tag records the lens manufactor as an ASCII string.
			0xa434 42036 Photo Exif.Photo.LensModel Ascii This tag records the lens's model name and model number as an ASCII string.
			0xa435 42037 Photo Exif.Photo.LensSerialNumber Ascii This tag records the serial number of the interchangeable lens that was used in photography as an ASCII string.
			Exif version
			*/
      }
      offs += 12;
    }
  }

  // Jump to the GPS SubIFD if it exists and parse all the information
  // there. Note that it's possible that the GPS SubIFD doesn't exist.
  if (gps_sub_ifd_offset + 4 <= len) {
    offs = gps_sub_ifd_offset;
    int num_entries = parse16(buf + offs, alignIntel);
	if (offs + 6 + 12 * num_entries > len) {
		jpegExif->error = PARSE_EXIF_ERROR_CORRUPT;
		return jpegExif;
	}
    offs += 2;
    while (--num_entries >= 0) {
      unsigned short tag    = parse16(buf + offs, alignIntel);
      unsigned short format = parse16(buf + offs + 2, alignIntel);
      unsigned length       = parse32(buf + offs + 4, alignIntel);
      unsigned data         = parse32(buf + offs + 8, alignIntel);
      switch(tag) {
        case 1:
          // GPS north or south
			jpegExif->GeoLocation.LatComponents.direction = *(buf + offs + 8);
			if ('S' == jpegExif->GeoLocation.LatComponents.direction)
				jpegExif->GeoLocation.Latitude = -jpegExif->GeoLocation.Latitude;
          break;

        case 2:
          // GPS latitude
          if (format == 5 && length == 3) {
			  jpegExif->GeoLocation.LatComponents.degrees =
              parseEXIFRational(buf + data + tiff_header_start, alignIntel);
			  jpegExif->GeoLocation.LatComponents.minutes =
              parseEXIFRational(buf + data + tiff_header_start + 8, alignIntel);
			  jpegExif->GeoLocation.LatComponents.seconds =
              parseEXIFRational(buf + data + tiff_header_start + 16, alignIntel);
			  jpegExif->GeoLocation.Latitude =
				  jpegExif->GeoLocation.LatComponents.degrees +
				  jpegExif->GeoLocation.LatComponents.minutes / 60 +
				  jpegExif->GeoLocation.LatComponents.seconds / 3600;
			  if ('S' == jpegExif->GeoLocation.LatComponents.direction)
				jpegExif->GeoLocation.Latitude = -jpegExif->GeoLocation.Latitude;
          }
          break;

        case 3:
          // GPS east or west
			jpegExif->GeoLocation.LonComponents.direction = *(buf + offs + 8);
			if ('W' == jpegExif->GeoLocation.LonComponents.direction)
				jpegExif->GeoLocation.Longitude = -jpegExif->GeoLocation.Longitude;
          break;

        case 4:
          // GPS longitude
          if (format == 5 && length == 3) {
			  jpegExif->GeoLocation.LonComponents.degrees =
              parseEXIFRational(buf + data + tiff_header_start, alignIntel);
			  jpegExif->GeoLocation.LonComponents.minutes =
              parseEXIFRational(buf + data + tiff_header_start + 8, alignIntel);
			  jpegExif->GeoLocation.LonComponents.seconds =
              parseEXIFRational(buf + data + tiff_header_start + 16, alignIntel);
			  jpegExif->GeoLocation.Longitude =
				  jpegExif->GeoLocation.LonComponents.degrees +
				  jpegExif->GeoLocation.LonComponents.minutes / 60 +
				  jpegExif->GeoLocation.LonComponents.seconds / 3600;
			  if ('W' == jpegExif->GeoLocation.LonComponents.direction)
				  jpegExif->GeoLocation.Longitude = -jpegExif->GeoLocation.Longitude;
          }
          break;

        case 5:
          // GPS altitude reference (below or above sea level)
			jpegExif->GeoLocation.AltitudeRef = *(buf + offs + 8);
			if (1 == jpegExif->GeoLocation.AltitudeRef)
				jpegExif->GeoLocation.Altitude = -jpegExif->GeoLocation.Altitude;
          break;

        case 6:
          // GPS altitude reference
          if (format == 5) {
			  jpegExif->GeoLocation.Altitude =
              parseEXIFRational(buf + data + tiff_header_start, alignIntel);
			  if (1 == jpegExif->GeoLocation.AltitudeRef)
				  jpegExif->GeoLocation.Altitude = -jpegExif->GeoLocation.Altitude;
          }
          break;
      }
      offs += 12;
    }
  }

  jpegExif->error = PARSE_EXIF_SUCCESS;
  jpegExif->print();

  return jpegExif;
}

/*
void EXIFInfo::clear() {
  // Strings
  ImageDescription  = "";
  Make              = "";
  Model             = "";
  Software          = "";
  DateTime          = "";
  DateTimeOriginal  = "";
  DateTimeDigitized = ""; 
  SubSecTimeOriginal= "";
  copyright         = "";

  // Shorts / unsigned / double
  ByteAlign         = 0;
  Orientation       = 0; 

  BitsPerSample     = 0;
  ExposureTime      = 0;
  FNumber           = 0;
  ISOSpeedRatings   = 0;
  ShutterSpeedValue = 0;
  ExposureBiasValue = 0;
  SubjectDistance   = 0;
  FocalLength       = 0;
  FocalLengthIn35mm = 0;
  Flash             = 0;
  MeteringMode      = 0;
  ImageWidth        = 0;
  ImageHeight       = 0;

  // Geolocation
  GeoLocation.Latitude    = 0;
  GeoLocation.Longitude   = 0;
  GeoLocation.Altitude    = 0;
  GeoLocation.AltitudeRef = 0;
  GeoLocation.LatComponents.degrees   = 0;
  GeoLocation.LatComponents.minutes   = 0;
  GeoLocation.LatComponents.seconds   = 0;
  GeoLocation.LatComponents.direction = 0;
  GeoLocation.LonComponents.degrees   = 0;
  GeoLocation.LonComponents.minutes   = 0;
  GeoLocation.LonComponents.seconds   = 0;
  GeoLocation.LonComponents.direction = 0;
}
*/
