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

#pragma once

#include <memory>
#include <string>
#include <cstdint>

enum class LightSourceValues : std::uint8_t {
	Unknown = 0,
	Daylight = 1,
	Fluorescent = 2,
	Tungsten_Incandescent = 3,
	Flash = 4,
	Fine_Weather = 9,
	Cloudy = 10,
	Shade = 11,
	Daylight_Fluorescent = 12,
	DayWhite_Fluorescent = 13,
	CoolWhite_Fluorescent = 14,
	White_Fluorescent = 15,
	WarmWhite_Fluorescent = 16,
	Standard_Light_A = 17,
	Standard_Light_B = 18,
	Standard_Light_C = 19,
	D55 = 20,
	D65 = 21,
	D75_Tungsten_Incandescent = 22,
	D50 = 23,
	ISO_Studio_Tungsten = 24,
	Other = 255
};

std::ostream& operator<< (std::ostream& os, LightSourceValues type);



enum class MeteringModeValue {
	Unknown = 0,
	Average,		// 1: average
	Center,			// 2: center weighted average
	Spot,			// 3: spot
	MultiSpot,		// 4: multi-spot
	MultiSegment	// 5: multi-segment
};

std::ostream& operator<< (std::ostream& os, MeteringModeValue type);




enum class WhiteBalanceValue {
	Auto = 0,
	Manual = 1
};

std::ostream& operator<< (std::ostream& os, WhiteBalanceValue type);

enum class FlashValues {
	No_Flash = 0x0,
	Fired = 0x1,
	Fired_Return_not_detected = 0x5,
	Fired_Return_detected = 0x7,
	On_Did_not_fire = 0x8,
	On_Fired = 0x9,
	On_Return_not_detected = 0xd,
	On_Return_detected = 0xf,
	Off_Did_not_fire = 0x10,
	Off_Did_not_fire_Return_not_detected = 0x14,
	Auto_Did_not_fire = 0x18,
	Auto_Fired = 0x19,
	Auto_Fired_Return_not_detected = 0x1d,
	Auto_Fired_Return_detected = 0x1f,
	No_flash_function = 0x20,
	Off_No_flash_function = 0x30,
	Fired_Redeye_reduction = 0x41,
	Fired_Redeye_reduction_Return_not_detected = 0x45,
	Fired_Redeye_reduction_Return_detected = 0x47,
	On_Redeye_reduction = 0x49,
	On_Redeye_reduction_Return_not_detected = 0x4d,
	On_Redeye_reduction_Return_detected = 0x4f,
	Off_Redeye_reduction = 0x50,
	Auto_Did_not_fire_Redeye_reduction = 0x58,
	Auto_Fired_Redeye_reduction = 0x59,
	Auto_Fired_Redeye_reduction_Return_not_detected = 0x5d,
	Auto_Fired_Redeye_reduction_Return_detected = 0x5f
};

std::ostream& operator<< (std::ostream& os, FlashValues type);


enum class CompressionValue {
	Uncompressed = 1,
	CCITT_1D = 2,
	T4Group_3_Fax = 3,
	T6Group_4_Fax = 4,
	LZW = 5,
	JPEG_old_style = 6,
	JPEG = 7,
	Adobe_Deflate = 8,
	JBIG_BandW = 9,
	JBIG_Color = 10,
	JPEG_DUP = 99,
	Kodak_262 = 262,
	Next = 32766,
	Sony_ARW_Compressed = 32767,
	Packed_RAW = 32769,
	Samsung_SRW_Compressed = 32770,
	CCIRLEW = 32771,
	Samsung_SRW_Compressed_2 = 32772,
	PackBits = 32773,
	Thunderscan = 32809,
	Kodak_KDC_Compressed = 32867,
	IT8CTPAD = 32895,
	IT8LW = 32896,
	IT8MP = 32897,
	IT8BL = 32898,
	PixarFilm = 32908,
	PixarLog = 32909,
	Deflate = 32946,
	DCS = 32947,
	JBIG = 34661,
	SGILog = 34676,
	SGILog24 = 34677,
	JPEG_2000 = 34712,
	Nikon_NEF_Compressed = 34713,
	JBIG2_TIFF_FX = 34715,
	Microsoft_Document_Imaging_MDI_Binary_Level_Codec = 34718,
	Microsoft_Document_Imaging_MDI_Progressive_Transform_Codec = 34719,
	Microsoft_Document_Imaging_MDI_Vector = 34720,
	Lossy_JPEG = 34892,
	Kodak_DCR_Compressed = 65000,
	Pentax_PEF_Compressed = 65535
};

std::ostream& operator<< (std::ostream& os, CompressionValue type);


enum class SceneCaptureTypeValue {
	stardard = 0,
	Landscape = 1,
	Portrait = 2,
	Night = 3
};

std::ostream& operator<< (std::ostream& os, SceneCaptureTypeValue type);


enum class GainControlValue {
	None = 0,
	Low_gain_up = 1,
	High_gain_up = 2,
	Low_gain_down = 3,
	High_gain_down = 4
};

std::ostream& operator<< (std::ostream& os, GainControlValue type);


enum class ContrastValue {
	Normal = 0,
	Low = 1,
	High = 2
};

std::ostream& operator<< (std::ostream& os, ContrastValue type);


enum class SaturationValue {
	Normal = 0,
	Low = 1,
	High = 2
};

std::ostream& operator<< (std::ostream& os, SaturationValue type);

enum class SharpnessValue {
	Normal = 0,
	Soft = 1,
	Hard = 2
};

std::ostream& operator<< (std::ostream& os, SharpnessValue type);


enum class ResolutionUnitValue {
	None = 1,
	inches = 2,
	cm = 3
};



enum class ExposureProgramValue {
	Not_defined = 0,
	Manual = 1,
	Normal_program = 2,
	Aperture_priority = 3,
	Shutter_priority = 4,
	Creative_program = 5,
	Action_program = 6,
	Portrait_mode = 7,
	Landscape_mode = 8,
	//Other = reserved
};

std::ostream& operator<< (std::ostream& os, ExposureProgramValue type);


enum class ColorSpaceValue {
	sRGB = 1,
	Adobe_RGB = 2,
	Wide_Gamut_RGB = 0xfffd,
	ICC_Profile = 0xfffe,
	Uncalibrated = 0xffff
};

std::ostream& operator<< (std::ostream& os, ColorSpaceValue type);


enum class PhotometricInterpretationValue {
	WhiteIsZero = 0,
	BlackIsZero = 1,
	RGB = 2,
	RGB_Palette = 3,
	Transparency_Mask = 4,
	CMYK = 5,
	YCbCr = 6,
	CIELab = 8,
	ICCLab = 9,
	ITULab = 10,
	Color_Filter_Array = 32803,
	Pixar_LogL = 32844,
	Pixar_LogLuv = 32845,
	Linear_Raw = 34892,
	Not_Set = 11
};

std::ostream& operator<< (std::ostream& os, PhotometricInterpretationValue type);

enum class SubjectDistanceRangeValue {
	Unknown,
	Macro,
	CloseView,
	DistantView
};

struct Rational {
	int numerator;
	int denominator;
	double rational;
};

class JpegEXIF
{
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
	void print();

	// Data fields filled out by parseFrom()

	char ByteAlign;                   // 0 = Motorola byte alignment, 1 = Intel

	// Basic EXIF list ordered
// Description
	std::string title;							// XP Title
	std::string subject;						// XP Subject
	unsigned short rating;						// Short Rating tag used by Windows
	unsigned short ratingPercent;				// Rating tag used by Windows, value in percent
	std::string ImageDescription;				// Image description
	std::string keywords;						// XP Keywords Byte Keywords tag used by Windows, encoded in UCS2
// Origin
	std::string comment;						// XP Comment Byte Comment tag used by Windows, encoded in UCS2
	std::string author;							// XP Author Byte Author tag used by Windows, encoded in UCS2				
	std::string DateTime;						// File change date and time
	std::string DateTimeOriginal;				// Original file date and time (may not exist)
	std::string DateTimeDigitized;				// Digitization date and time (may not exist)
	std::string SubSecTimeOriginal;				// Sub-second time that original picture was taken
	std::string copyright;						// File copyright information
// Image
	unsigned ImageWidth;						// Image width reported in EXIF data
	unsigned ImageHeight;						// Image height reported in EXIF data
	unsigned short Orientation;					// Image orientation, start of data corresponds to
	unsigned short BitsPerSample;				// Number of bits per component
	double xResolution;							// XResolution
	double yResolution;							// YResolution
	unsigned short resolutionUnit;				// The unit for measuring <XResolution> and <YResolution>. The same unit is used for both <XResolution> and <YResolution>.
												// If the image resolution is unknown, 2 (inches) is designated.
	unsigned compression;						// Compression
// Camera	
	std::string Make;							// Camera manufacturer's name
	std::string Model;							// Camera model
	Rational ExposureTime;						// Exposure time in seconds
	Rational FNumber;							// F/stop
	unsigned short ISOSpeedRatings;				// ISO speed
	Rational ExposureBiasValue;					// Exposure bias value in EV
	unsigned short exposureProgram;				// The class of the program used by the camera to set exposure when the picture is taken.
	double SubjectDistance;						// Distance to focus point in meters
	double FocalLength;							// Focal length of lens in millimeters
	unsigned short FocalLengthIn35mm;			// Focal length in 35mm film
	char Flash;									// 0 = no flash, 1 = flash used
	unsigned short meteringMode;				// Metering mode
	double DigitalZoomRatio;					// This tag indicates the digital zoom ratio when the image was shot.If the numerator of the recorded value is 0, this indicates that digital zoom was not used.
// Advanced photo 	
	unsigned short lightSource;					// The kind of light source.
	unsigned short colorSpace;					// The color space information tag (ColorSpace) is always recorded as the color space specifier
	unsigned short  whiteBalance;				// Short This tag indicates the white balance mode set when the image was shot.
	unsigned short SceneCaptureType;			// This tag indicates the type of scene that was shot.It can also be used to record the mode in which the image was shot.Note that this differs from the <SceneType> tag.
	unsigned short Contrast;					// This tag indicates the direction of contrast processing applied by the camera when the image was shot.
	unsigned short Saturation;					// This tag indicates the direction of saturation processing applied by the camera when the image was shot.
	unsigned short Sharpness;					// This tag indicates the direction of sharpness processing applied by the camera when the image was shot.
	Rational brightnessValue;					// The value of brightness. The unit is the APEX(Additive System of Photographic Exposure) setting. Ordinarily it is given in the range of - 99.99 to 99.99.Note
												// that if the numerator of the recorded value is hex FFFFFFFF, Unknown shall be indicated.
	unsigned short SubjectDistanceRange;		// This tag indicates the distance to the subject.
	Rational maxApertureValue;					// The smallest F number of the lens. The unit is the APEX value. Ordinarily it is given in the range of 00.00 to 99.99,
												// but it is not limited to this range.
	Rational compressedBitsPerPixel;			// Specific to compressed data; states the compressed bits per pixel. The compression mode used for a compressed image is indicated in unit bits per pixel.
	double flashEnergy;							// Amount of flash energy(BCPS).
	std::string exifVersion;					// The version of this standard supported. Nonexistence of this field is taken to mean nonconformance to the standard.
	std::string Software;						// Software used. This tag records the name and version of the software or firmware of the camera or image input device used to generate the image.
												// The detailed format is not specified, but it is recommended that the example shown below be followed. When the field is left blank, it is
												// treated as unknown.
	unsigned short photometricInterpretation;	// The pixel composition. In JPEG compressed data a JPEG marker is used instead of this tag.
// GPS
	struct Geolocation_t {						// GPS information embedded in file
		double Latitude;						// Image latitude expressed as decimal
		double Longitude;						// Image longitude expressed as decimal
		double Altitude;						// Altitude in meters, relative to sea level
		char AltitudeRef;						// 0 = above sea level, -1 = below sea level
		struct Coord_t {
			double degrees;
			double minutes;
			double seconds;
			char direction;
		} LatComponents, LonComponents;			// Latitude, Longitude expressed in deg/min/sec 
	} GeoLocation;

	JpegEXIF() {
		clear();
	}

	int error;

	static const char *toString(LightSourceValues type);
	static const char *toString(MeteringModeValue type);
	static const char *toString(FlashValues type);
	static const char *toString(WhiteBalanceValue type);
	static const char *toString(CompressionValue type);
	static const char *toString(SceneCaptureTypeValue type);
	static const char *toString(GainControlValue type);
	static const char *toString(ResolutionUnitValue type);
	static const char *toString(ContrastValue type);
	static const char *toString(SaturationValue type);
	static const char *toString(SharpnessValue type);
	static const char *toString(ExposureProgramValue type);
	static const char *toString(ColorSpaceValue type);
	static const char *toString(PhotometricInterpretationValue type);
	static const char *toString(SubjectDistanceRangeValue type);
};


std::ostream& operator<< (std::ostream& os, ResolutionUnitValue type);

/*
Description
	Title
	Subject
	Rating
	Tags
	Comments
Origin
	Author
	Date taken
	Program name
	Date accquired
	Copyright
Image
	Image ID
	Dimensions
	Width
	Height
	Horizontal resolution
	Vertical resolution
	Bit depth
	Compression
	Resolution unit
	Color representation
	Compressed bits/pixel
Camera
	Camera maker
	Camera model
	F-stop
	Exposure time
	ISO speed
	Exposure Bias
	Focal Length
	Max aperture
	Metering mode
	Subject distance
	Flash mode
	Flash Energy
	35mm focal length
Advanced photo
	Lens maker
	Lens modal
	Camera serial number
	Contrast
	Brightness
	Light source
	Exposure program
	Saturation
	Sharpness
	White balance
	Photomatic interpratation
	Digital zoom
	Exif Version
*/
/*
<Width unit="Pixels">3968</Width>
<Height unit="Pixels">2976</Height>
<Resolution unit="DPI">314</Resolution>
<Depth unit="Bits">24</Depth>
<ViewRotation>1</ViewRotation>
<SampleColor>R:80 G:A0 B:D0</SampleColor>
<Pages>1</Pages>
<ColorSpace>RGB </ColorSpace>
<Compression>65546</Compression>
<PrimaryEncoding>Photo - JPEG</PrimaryEncoding>
<ColorProfile/>
*/
/*
<Maker>OLYMPUS IMAGING CORP. </Maker>
<Model>TG-2 </Model>
<Software>Version 1.0 </Software>
<SourceURL/>
<ExifVersion>2.3</ExifVersion>
<CaptureDate>2017:01:16 09:18:17</CaptureDate>
<ExposureProgram strID="5">Depth of Field Bias</ExposureProgram>
<ISOSpeedRating>100</ISOSpeedRating>
<ExposureBias>+0.0</ExposureBias>
<ExposureTime>1/400</ExposureTime>
<Aperture>f2.8</Aperture>
<MeteringMode strID="5">Pattern</MeteringMode>
<LightSource strID="0">Auto</LightSource>
<Flash flash="16">OFF - Compulsory flash mode</Flash>
<FocalLength>4.5</FocalLength>
<Contrast strID="2">Hard</Contrast>
<Sharpness strID="2">Hard</Sharpness>
<DigitalZoom>1.0</DigitalZoom>
*/

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
