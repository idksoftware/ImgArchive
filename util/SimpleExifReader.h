/**************************************************************************
  exif.h  -- A simple ISO C++ library to parse basic EXIF 
             information from a JPEG file.
  Based on the description of the EXIF file format at:
  -- http://park2.wakwak.com/~tsuruzoh/Computer/Digicams/exif-e.html
  -- http://www.media.mit.edu/pia/Research/deepview/exif.html
  -- http://www.exif.org/Exif2-2.PDF
  Copyright (c) 2010-2015 Mayank Lahiri
  mlahiri@gmail.com
  All rights reserved.
  VERSION HISTORY:
  ================
  2.2: Release December 2014
       -- 
  2.1: Released July 2013
       -- fixed a bug where JPEGs without an EXIF SubIFD would not be parsed
       -- fixed a bug in parsing GPS coordinate seconds
       -- fixed makefile bug
       -- added two pathological test images from Matt Galloway
          http://www.galloway.me.uk/2012/01/uiimageorientation-exif-orientation-sample-images/
       -- split main parsing routine for easier integration into Firefox
  2.0: Released February 2013
       -- complete rewrite
       -- no new/delete
       -- added GPS support
  1.0: Released 2010
  
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
#ifndef __EXIF_H
#define __EXIF_H

#include <string>

// 
// Class responsible for storing and parsing EXIF information from a JPEG blob
//
class EXIFInfo {
 public:
  // Parsing function for an entire JPEG image buffer.
  //
  // PARAM 'data': A pointer to a JPEG image.
  // PARAM 'length': The length of the JPEG image.
  // RETURN:  PARSE_EXIF_SUCCESS (0) on succes with 'result' filled out
  //          error code otherwise, as defined by the PARSE_EXIF_ERROR_* macros
  int parseFrom(const unsigned char *data, unsigned length);
  int parseFrom(const std::string &data);

  // Parsing function for an EXIF segment. This is used internally by parseFrom()
  // but can be called for special cases where only the EXIF section is 
  // available (i.e., a blob starting with the bytes "Exif\0\0").
  int parseFromEXIFSegment(const unsigned char *buf, unsigned len);

  // Set all data members to default values.
  void clear();

  // Data fields filled out by parseFrom()
  char ByteAlign;                   // 0 = Motorola byte alignment, 1 = Intel 
  std::string ImageDescription;     // Image description
  std::string Make;                 // Camera manufacturer's name
  std::string Model;                // Camera model
  unsigned short Orientation;       // Image orientation, start of data corresponds to
                                    // 0: unspecified in EXIF data
                                    // 1: upper left of image
                                    // 3: lower right of image
                                    // 6: upper right of image
                                    // 8: lower left of image
                                    // 9: undefined
  unsigned short BitsPerSample;     // Number of bits per component
  std::string Software;             // Software used
  std::string DateTime;             // File change date and time
  std::string DateTimeOriginal;     // Original file date and time (may not exist)
  std::string DateTimeDigitized;    // Digitization date and time (may not exist)
  std::string SubSecTimeOriginal;   // Sub-second time that original picture was taken
  std::string copyright;            // File copyright information
  std::string title;				// XP Title
  std::string comment;				// XP Comment Byte Comment tag used by Windows, encoded in UCS2
  std::string author;				// XP Author Byte Author tag used by Windows, encoded in UCS2
  std::string keywords;				// XP Keywords Byte Keywords tag used by Windows, encoded in UCS2
  std::string subject;				// XP Subject
  unsigned short rating;			// Short Rating tag used by Windows
  unsigned short ratingPercent;		// Rating tag used by Windows, value in percent
  double ExposureTime;              // Exposure time in seconds
  double FNumber;                   // F/stop
  unsigned short ISOSpeedRatings;   // ISO speed
  double ShutterSpeedValue;         // Shutter speed (reciprocal of exposure time)
  double ExposureBiasValue;         // Exposure bias value in EV
  unsigned short exposureProgram;	// The class of the program used by the camera to set exposure when the picture is taken.
  double SubjectDistance;           // Distance to focus point in meters
  double FocalLength;               // Focal length of lens in millimeters
  unsigned short FocalLengthIn35mm; // Focal length in 35mm film
  char Flash;                       // 0 = no flash, 1 = flash used
  unsigned short MeteringMode;      // Metering mode
                                    // 1: average
                                    // 2: center weighted average
                                    // 3: spot
                                    // 4: multi-spot
                                    // 5: multi-segment
  unsigned ImageWidth;              // Image width reported in EXIF data
  unsigned ImageHeight;             // Image height reported in EXIF data
  unsigned short lightSource;       // The kind of light source.
									// 0 = Unknown
									// 	  1 = Daylight
								    // 	  2 = Fluorescent
								    // 	  3 = Tungsten(incandescent light)
								    // 	  4 = Flash
								    // 	  9 = Fine weather
								    // 	  10 = Cloudy weather
								    // 	  11 = Shade
								    // 	  12 = Daylight fluorescent(D 5700 - 7100K)
								    // 	  13 = Day white fluorescent(N 4600 - 5400K)
								    // 	  14 = Cool white fluorescent(W 3900 - 4500K)
								    // 	  15 = White fluorescent(WW 3200 - 3700K)
								    // 	  17 = Standard light A
								    // 	  18 = Standard light B
								    // 	  19 = Standard light C
								    // 	  20 = D55
								    // 	  21 = D65
								    // 	  22 = D75
								    // 	  23 = D50
								    // 	  24 = ISO studio tungsten
								    // 	  255 = Other light source
 
  unsigned short  whiteBalance;		// Short This tag indicates the white balance mode set when the image was shot.
  
  double DigitalZoomRatio;			// This tag indicates the digital zoom ratio when the image was shot.If the numerator of the recorded value is 0, this indicates that digital zoom was not used.

  unsigned short SceneCaptureType;	// This tag indicates the type of scene that was shot.It can also be used to record the mode in which the image was shot.Note that this differs from the <SceneType> tag.
  
  unsigned short Contrast;			// This tag indicates the direction of contrast processing applied by the camera when the image was shot.
  unsigned short Saturation;		// This tag indicates the direction of saturation processing applied by the camera when the image was shot.
  unsigned short Sharpness;		    // This tag indicates the direction of sharpness processing applied by the camera when the image was shot.
  
  unsigned short SubjectDistanceRange; // This tag indicates the distance to the subject.

  
 // Resolution?
 // Depth?
  // ColorSpace?
  // Compression? 
  // PrimaryEncoding?
  
  
  // Aperture? using Fnumber
  // SensingMethod?
  // DigitalZoom


  struct Geolocation_t {            // GPS information embedded in file
    double Latitude;                  // Image latitude expressed as decimal
    double Longitude;                 // Image longitude expressed as decimal
    double Altitude;                  // Altitude in meters, relative to sea level
    char AltitudeRef;                 // 0 = above sea level, -1 = below sea level
    struct Coord_t {
      double degrees;               
      double minutes;
      double seconds;
      char direction;
    } LatComponents, LonComponents;   // Latitude, Longitude expressed in deg/min/sec 
  } GeoLocation;
  EXIFInfo() {
    clear();
  }
};

// Parse was successful
#define PARSE_EXIF_SUCCESS                    0
// No JPEG markers found in buffer, possibly invalid JPEG file
#define PARSE_EXIF_ERROR_NO_JPEG              1982
// No EXIF header found in JPEG file.
#define PARSE_EXIF_ERROR_NO_EXIF              1983
// Byte alignment specified in EXIF file was unknown (not Motorola or Intel).
#define PARSE_EXIF_ERROR_UNKNOWN_BYTEALIGN    1984
// EXIF header was found, but data was corrupted.
#define PARSE_EXIF_ERROR_CORRUPT              1985

#endif

/*
Flikr
Nikon D800



28-200mm f/3.5-5.6


Make -  NIKON CORPORATION
Orientation -  Horizontal (normal)
X-Resolution -  300 dpi
Y-Resolution -  300 dpi
Software -  Ver.1.10
Date and Time (Modified) -  2016:08:21 13:50:49
Artist -  Iain Ferguson
White Point -  0.313 0.329
Primary Chromaticities -  0.64 0.33 0.21 0.71 0.15 0.06
YCb Cr Coefficients -  0.299 0.587 0.114
YCbCr Positioning -  Co-sited
Copyright -  l.Ferguson@IDK.co.uk
ISO Speed -  100
Sensitivity Type -  Recommended Exposure Index
Exif Version -  0230
Date and Time (Original) -  2016:08:21 13:50:49
Date and Time (Digitized) -  2016:08:21 13:50:49
Components Configuration -  Y, Cb, Cr, -
Compressed Bits Per Pixel -  4
Exposure Bias -  -1/3 EV
Max Aperture Value -  4.8
Metering Mode -  Center-weighted average
Light Source -  Flash
Sub Sec Time -  10
Sub Sec Time Original -  10
Sub Sec Time Digitized -  10
Flashpix Version -  0100
Color Space -  Uncalibrated
Sensing Method -  One-chip color area
File Source -  Digital Camera
Scene Type -  Directly photographed
CFAPattern -  [Red,Green][Green,Blue]
Custom Rendered -  Normal
Exposure Mode -  Auto
White Balance -  Manual
Digital Zoom Ratio -  1
Focal Length (35mm format) -  75 mm
Scene Capture Type -  Standard
Gain Control -  None
Contrast -  Normal
Saturation -  Normal
Sharpness -  Normal
Subject Distance Range -  Unknown
Gamma -  2.2
Maker Note Version -  2.10
Quality -  Fine
White Balance -  Flash
Focus Mode -  AF-S
White Balance Fine Tune -  0 0
WB_ RBLevels -  2.1875 1.20703125 1 1
Program Shift -  0
Exposure Difference -  0
Flash Exposure Comp -  0
Image Boundary -  0 0 7360 4912
External Flash Exposure Comp -  0
Flash Exposure Bracket Value -  0.0
Exposure Bracket Value -  0
Crop Hi Speed -  Off (7424x4924 cropped to 7424x4924 at pixel 0,0)
Exposure Tuning -  0
Serial Number -  6016406
Color Space -  Adobe RGB
VRInfo Version -  0100
Vibration Reduction -  Off
VRMode -  Normal
Active D- Lighting -  Off
Picture Control Version -  0100
Picture Control Name -  Neutral
Picture Control Base -  Neutral
Picture Control Adjust -  Default Settings
Picture Control Quick Adjust -  n/a
Sharpness -  2
Contrast -  Normal
Brightness -  Normal
Saturation -  Normal
Hue Adjustment -  None
Filter Effect -  n/a
Toning Effect -  n/a
Toning Saturation -  n/a
Timezone -  +00:00
Daylight Savings -  Yes
Date Display Format -  D/M/Y
ISOExpansion -  Off
ISO2 -  100
ISOExpansion2 -  Off
Vignette Control -  Normal
Auto Distortion Control -  Off
HDRInfo Version -  0100
HDR -  Off
HDRLevel -  Auto
HDRSmoothing -  Off
HDRLevel2 -  n/a
Lens Type -  D
Flash Mode -  Did Not Fire
Shooting Mode -  Continuous
Lens FStops -  5.33
Shot Info Version -  0222
Firmware Version -  1.10b
Repeating Flash Output External -  1
Flash Exposure Comp2 -  -1/3
Sequence Number -  0
Shutter Count -  35124
Noise Reduction -  Off
WB_ GRBGLevels -  256 560 309 256
Lens Data Version -  0204
Exit Pupil Position -  85.3 mm
AFAperture -  4.9
Focus Position -  0x01
Focus Distance -  26.61 m
Lens IDNumber -  77
Min Focal Length -  28.3 mm
Max Focal Length -  201.6 mm
Max Aperture At Min Focal -  3.6
Max Aperture At Max Focal -  5.7
MCUVersion -  98
Effective Max Aperture -  4.9
Retouch History -  None
Image Data Size -  10829249
Flash Info Version -  0105
Flash Source -  None
External Flash Firmware -  n/a
External Flash Flags -  (none)
Flash Commander Mode -  Off
Flash Control Mode -  Off
Flash Compensation -  0
Flash GNDistance -  0
Flash Color Filter -  None
Flash Group AControl Mode -  Off
Flash Group BControl Mode -  Off
Flash Group CControl Mode -  Off
Flash Group ACompensation -  0
Flash Group BCompensation -  0
Flash Group CCompensation -  0
External Flash Compensation -  0
Flash Exposure Comp3 -  0
Flash Exposure Comp4 -  0
Multi Exposure Version -  0100
Multi Exposure Mode -  Off
Multi Exposure Shots -  0
Multi Exposure Auto Gain -  Off
High ISONoise Reduction -  Normal
Power Up Time -  2016:01:22 17:13:25
AFInfo2 Version -  0100
Contrast Detect AF -  Off
AFArea Mode -  Single Area
Phase Detect AF -  On (51-point)
Primary AFPoint -  C7
AFPoints Used -  C7
Contrast Detect AFIn Focus -  No
File Info Version -  0100
Directory Number -  112
File Number -  4682
AFFine Tune -  On (1)
AFFine Tune Index -  n/a
AFFine Tune Adj -  0
Compression -  JPEG (old-style)
XResolution -  300
YResolution -  300
Preview Image Start -  14416
Preview Image Length -  25201
YCb Cr Positioning -  Co-sited
Auto Bracketing Set -  AE Only
Auto Bracket Order -  0,-,+
Auto Bracketing Mode -  Flash/Speed/Aperture
Flash Sync Speed -  1/250 s
Flash Shutter Speed -  30 s
Flash Control Built-in -  TTL
Commander Channel -  1
Commander Internal Flash -  TTL
Commander Internal Manual Output -  Full
Commander Group AMode -  TTL
Commander Group AManual Output -  Full
Commander Group BMode -  TTL
Commander Group BManual Output -  Full
Modeling Flash -  On
Commander Internal TTLComp -  0
Commander Group A_ TTL- AAComp -  0
Commander Group B_ TTL- AAComp -  0
Interop Index -  R03 - DCF option file (Adobe RGB)
Interop Version -  0100
Compression -  JPEG (old-style)
Thumbnail Offset -  39776
Thumbnail Length -  12053
MPFVersion -  0100
Number Of Images -  2
MPImage Flags -  Representative image, Dependent parent image
MPImage Format -  JPEG
MPImage Type -  Baseline MP Primary Image
MPImage Length -  10901518
MPImage Start -  0
Dependent Image1 Entry Number -  2
Dependent Image2 Entry Number -  0
MPImage Flags -  Dependent child image
MPImage Format -  JPEG
MPImage Type -  Large Thumbnail (full HD equivalent)
MPImage Length -  618856
MPImage Start -  10902016
Dependent Image1 Entry Number -  0
Dependent Image2 Entry Number -  0
Camera ID -  54532
Camera Type -  Digital SLR

*/