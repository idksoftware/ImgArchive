#include "JpegEXIF.h"
#include <iostream>


std::ostream& operator<< (std::ostream& os, LightSourceValues type)
{
	switch (type)
	{
	case LightSourceValues::Unknown: return os << "Unknown";
	case LightSourceValues::Daylight: return os << "Daylight";
	case LightSourceValues::Fluorescent: return os << "Fluorescent";
	case LightSourceValues::Tungsten_Incandescent: return os << "Tungsten Incandescent";
	case LightSourceValues::Flash: return os << "Flash";
	case LightSourceValues::Fine_Weather: return os << "Fine Weather";
	case LightSourceValues::Cloudy: return os << "Cloudy";
	case LightSourceValues::Shade: return os << "Shade";
	case LightSourceValues::Daylight_Fluorescent: return os << "Daylight Fluorescent";
	case LightSourceValues::DayWhite_Fluorescent: return os << "DayWhite Fluorescent";
	case LightSourceValues::CoolWhite_Fluorescent: return os << "CoolWhite Fluorescent";
	case LightSourceValues::White_Fluorescent: return os << "White Fluorescent";
	case LightSourceValues::WarmWhite_Fluorescent: return os << "WarmWhite Fluorescent";
	case LightSourceValues::Standard_Light_A: return os << "Standard Light A";
	case LightSourceValues::Standard_Light_B: return os << "Standard Light B";
	case LightSourceValues::Standard_Light_C: return os << "Standard Light C";
	case LightSourceValues::D55: return os << "D55";
	case LightSourceValues::D65: return os << "D65";
	case LightSourceValues::D75_Tungsten_Incandescent: return os << "D75 Tungsten Incandescent";
	case LightSourceValues::D50: return os << "D50";
	case LightSourceValues::ISO_Studio_Tungsten: return os << "D50";
	case LightSourceValues::Other: return os << "Other";
		// omit default case to trigger compiler warning for missing cases
	};
	return os << "Unknown";
}

const char *JpegEXIF::toString(LightSourceValues type)
{
	switch (type)
	{
	case LightSourceValues::Unknown: return "Unknown";
	case LightSourceValues::Daylight: return "Daylight";
	case LightSourceValues::Fluorescent: return "Fluorescent";
	case LightSourceValues::Tungsten_Incandescent: return "Tungsten Incandescent";
	case LightSourceValues::Flash: return "Flash";
	case LightSourceValues::Fine_Weather: return "Fine Weather";
	case LightSourceValues::Cloudy: return "Cloudy";
	case LightSourceValues::Shade: return "Shade";
	case LightSourceValues::Daylight_Fluorescent: return "Daylight Fluorescent";
	case LightSourceValues::DayWhite_Fluorescent: return "DayWhite Fluorescent";
	case LightSourceValues::CoolWhite_Fluorescent: return "CoolWhite Fluorescent";
	case LightSourceValues::White_Fluorescent: return "White Fluorescent";
	case LightSourceValues::WarmWhite_Fluorescent: return "WarmWhite Fluorescent";
	case LightSourceValues::Standard_Light_A: return "Standard Light A";
	case LightSourceValues::Standard_Light_B: return "Standard Light B";
	case LightSourceValues::Standard_Light_C: return "Standard Light C";
	case LightSourceValues::D55: return "D55";
	case LightSourceValues::D65: return "D65";
	case LightSourceValues::D75_Tungsten_Incandescent: return "D75 Tungsten Incandescent";
	case LightSourceValues::D50: return "D50";
	case LightSourceValues::ISO_Studio_Tungsten: return "D50";
	case LightSourceValues::Other: return "Other";
		// omit default case to trigger compiler warning for missing cases
	}
	return "";
}

std::ostream& operator<< (std::ostream& os, FlashValues type)
{
	switch (type)
	{
	case FlashValues::No_Flash: return os << "No Flash";
	case FlashValues::Fired: return os << "Fired";
	case FlashValues::Fired_Return_not_detected: return os << "Fired, Return not detected";
	case FlashValues::Fired_Return_detected: return os << "Fired, Return detected";
	case FlashValues::On_Did_not_fire: return os << "On, Did not fire";
	case FlashValues::On_Fired: return os << "On, Fired";
	case FlashValues::On_Return_not_detected: return os << "On, Return not detected";
	case FlashValues::On_Return_detected: return os << "On, Return detected";
	case FlashValues::Off_Did_not_fire: return os << "Off, Did not fire";
	case FlashValues::Off_Did_not_fire_Return_not_detected: return os << "Off, Did not fire, Return not detected";
	case FlashValues::Auto_Did_not_fire: return os << "Auto, Did not fire";
	case FlashValues::Auto_Fired: return os << "Auto, Fired";
	case FlashValues::Auto_Fired_Return_not_detected: return os << "Auto, Fired, Return not detected";
	case FlashValues::Auto_Fired_Return_detected: return os << "Auto, Fired, Return detected";
	case FlashValues::No_flash_function: return os << "No flash function";
	case FlashValues::Off_No_flash_function: return os << "Off, No flash function";
	case FlashValues::Fired_Redeye_reduction: return os << "Fired, Red-eye reduction";
	case FlashValues::Fired_Redeye_reduction_Return_not_detected : return os << "Fired, Red-eye reduction, Return not detected";
	case FlashValues::Fired_Redeye_reduction_Return_detected: return os << "Fired, Red-eye reduction, Return detected";
	case FlashValues::On_Redeye_reduction: return os << "On, Red-eye reduction";
	case FlashValues::On_Redeye_reduction_Return_not_detected: return os << "On, Red-eye reduction, Return not detected";
	case FlashValues::On_Redeye_reduction_Return_detected: return os << "On, Red-eye reduction, Return detected";
	case FlashValues::Off_Redeye_reduction: return os << "Off, Red-eye reduction";
	case FlashValues::Auto_Did_not_fire_Redeye_reduction: return os << "Auto, Did not fire, Red-eye reduction";
	case FlashValues::Auto_Fired_Redeye_reduction: return os << "Auto, Fired, Red-eye reduction";
	case FlashValues::Auto_Fired_Redeye_reduction_Return_not_detected: return os << "Auto, Fired, Red-eye reduction, Return not detected";
	case FlashValues::Auto_Fired_Redeye_reduction_Return_detected: return os << "Auto, Fired, Red-eye reduction, Return detected";
	}
	return os << "Unknown";
}

const char *JpegEXIF::toString(FlashValues type)
{
	switch (type)
	{
	case FlashValues::No_Flash: return "No Flash";
	case FlashValues::Fired: return "Fired";
	case FlashValues::Fired_Return_not_detected: return "Fired, Return not detected";
	case FlashValues::Fired_Return_detected: return "Fired, Return detected";
	case FlashValues::On_Did_not_fire: return "On, Did not fire";
	case FlashValues::On_Fired: return "On, Fired";
	case FlashValues::On_Return_not_detected: return "On, Return not detected";
	case FlashValues::On_Return_detected: return "On, Return detected";
	case FlashValues::Off_Did_not_fire: return "Off, Did not fire";
	case FlashValues::Off_Did_not_fire_Return_not_detected: return "Off, Did not fire, Return not detected";
	case FlashValues::Auto_Did_not_fire: return "Auto, Did not fire";
	case FlashValues::Auto_Fired: return "Auto, Fired";
	case FlashValues::Auto_Fired_Return_not_detected: return "Auto, Fired, Return not detected";
	case FlashValues::Auto_Fired_Return_detected: return "Auto, Fired, Return detected";
	case FlashValues::No_flash_function: return "No flash function";
	case FlashValues::Off_No_flash_function: return "Off, No flash function";
	case FlashValues::Fired_Redeye_reduction: return "Fired, Red-eye reduction";
	case FlashValues::Fired_Redeye_reduction_Return_not_detected: return "Fired, Red-eye reduction, Return not detected";
	case FlashValues::Fired_Redeye_reduction_Return_detected: return "Fired, Red-eye reduction, Return detected";
	case FlashValues::On_Redeye_reduction: return "On, Red-eye reduction";
	case FlashValues::On_Redeye_reduction_Return_not_detected: return "On, Red-eye reduction, Return not detected";
	case FlashValues::On_Redeye_reduction_Return_detected: return "On, Red-eye reduction, Return detected";
	case FlashValues::Off_Redeye_reduction: return "Off, Red-eye reduction";
	case FlashValues::Auto_Did_not_fire_Redeye_reduction: return "Auto, Did not fire, Red-eye reduction";
	case FlashValues::Auto_Fired_Redeye_reduction: return "Auto, Fired, Red-eye reduction";
	case FlashValues::Auto_Fired_Redeye_reduction_Return_not_detected: return "Auto, Fired, Red-eye reduction, Return not detected";
	case FlashValues::Auto_Fired_Redeye_reduction_Return_detected: return "Auto, Fired, Red-eye reduction, Return detected";
	}
	return "";
}

std::ostream& operator<< (std::ostream& os, CompressionValue type)
{
	switch (type)
	{
	case CompressionValue::Uncompressed: return os << "Uncompressed";
	case CompressionValue::CCITT_1D: return os << "CCITT 1D";
	case CompressionValue::T4Group_3_Fax: return os << "T4/Group 3 Fax";
	case CompressionValue::T6Group_4_Fax: return os << "T6/Group 4 Fax";
	case CompressionValue::LZW: return os << "LZW";
	case CompressionValue::JPEG_old_style: return os << "JPEG (old-style)";
	case CompressionValue::JPEG: return os << "JPEG";
	case CompressionValue::Adobe_Deflate: return os << "Adobe Deflate";
	case CompressionValue::JBIG_BandW: return os << "JBIG B&W";
	case CompressionValue::JBIG_Color: return os << "JBIG Color";
	case CompressionValue::JPEG_DUP: return os << "JPEG";
	case CompressionValue::Kodak_262: return os << "Kodak 262";
	case CompressionValue::Next: return os << "Next";
	case CompressionValue::Sony_ARW_Compressed: return os << "Sony ARW Compressed";
	case CompressionValue::Packed_RAW: return os << "Packed RAW";
	case CompressionValue::Samsung_SRW_Compressed: return os << "Samsung SRW Compressed";
	case CompressionValue::CCIRLEW: return os << "CCIRLEW";
	case CompressionValue::Samsung_SRW_Compressed_2: return os << "Samsung SRW Compressed 2";
	case CompressionValue::PackBits: return os << "PackBits";
	case CompressionValue::Thunderscan: return os << "Thunderscan";
	case CompressionValue::Kodak_KDC_Compressed: return os << "Kodak KDC Compressed";
	case CompressionValue::IT8CTPAD: return os << "IT8CTPAD";
	case CompressionValue::IT8LW: return os << "IT8LW";
	case CompressionValue::IT8MP: return os << "IT8MP";
	case CompressionValue::IT8BL: return os << "IT8BL";
	case CompressionValue::PixarFilm: return os << "PixarFilm";
	case CompressionValue::PixarLog: return os << "PixarLog";
	case CompressionValue::Deflate: return os << "Deflate";
	case CompressionValue::DCS: return os << "DCS";
	case CompressionValue::JBIG: return os << "JBIG";
	case CompressionValue::SGILog: return os << "SGILog";
	case CompressionValue::SGILog24: return os << "SGILog24";
	case CompressionValue::JPEG_2000: return os << "JPEG 2000";
	case CompressionValue::Nikon_NEF_Compressed: return os << "Nikon NEF Compressed";
	case CompressionValue::JBIG2_TIFF_FX: return os << "CompressionValue::JBIG2 TIFF FX";
	case CompressionValue::Microsoft_Document_Imaging_MDI_Binary_Level_Codec:
		return os << "Microsoft Document Imaging (MDI) Binary Level Codec";
	case CompressionValue::Microsoft_Document_Imaging_MDI_Progressive_Transform_Codec:
		return os << "Microsoft Document Imaging (MDI) Progressive Transform Codec";
	case CompressionValue::Microsoft_Document_Imaging_MDI_Vector:
		return os << "Microsoft Document Imaging (MDI) Vector";
	case CompressionValue::Lossy_JPEG: return os << "Lossy JPEG";
	case CompressionValue::Kodak_DCR_Compressed: return os << "Kodak DCR Compressed";
	case CompressionValue::Pentax_PEF_Compressed: return os << "Pentax PEF Compressed";
	}
	return os << "Unknown";
}


const char *JpegEXIF::toString(CompressionValue type)
{
	switch (type)
	{
	case CompressionValue::Uncompressed: return "Uncompressed";
	case CompressionValue::CCITT_1D: return "CCITT 1D";
	case CompressionValue::T4Group_3_Fax: return "T4/Group 3 Fax";
	case CompressionValue::T6Group_4_Fax: return "T6/Group 4 Fax";
	case CompressionValue::LZW: return "LZW";
	case CompressionValue::JPEG_old_style: return "JPEG (old-style)";
	case CompressionValue::JPEG: return "JPEG";
	case CompressionValue::Adobe_Deflate: return "Adobe Deflate";
	case CompressionValue::JBIG_BandW: return "JBIG B&W";
	case CompressionValue::JBIG_Color: return "JBIG Color";
	case CompressionValue::JPEG_DUP: return "JPEG";
	case CompressionValue::Kodak_262: return "Kodak 262";
	case CompressionValue::Next: return "Next";
	case CompressionValue::Sony_ARW_Compressed: return "Sony ARW Compressed";
	case CompressionValue::Packed_RAW: return "Packed RAW";
	case CompressionValue::Samsung_SRW_Compressed: return "Samsung SRW Compressed";
	case CompressionValue::CCIRLEW: return "CCIRLEW";
	case CompressionValue::Samsung_SRW_Compressed_2: return "Samsung SRW Compressed 2";
	case CompressionValue::PackBits: return "PackBits";
	case CompressionValue::Thunderscan: return "Thunderscan";
	case CompressionValue::Kodak_KDC_Compressed: return "Kodak KDC Compressed";
	case CompressionValue::IT8CTPAD: return "IT8CTPAD";
	case CompressionValue::IT8LW: return "IT8LW";
	case CompressionValue::IT8MP: return "IT8MP";
	case CompressionValue::IT8BL: return "IT8BL";
	case CompressionValue::PixarFilm: return "PixarFilm";
	case CompressionValue::PixarLog: return "PixarLog";
	case CompressionValue::Deflate: return "Deflate";
	case CompressionValue::DCS: return "DCS";
	case CompressionValue::JBIG: return "JBIG";
	case CompressionValue::SGILog: return "SGILog";
	case CompressionValue::SGILog24: return "SGILog24";
	case CompressionValue::JPEG_2000: return "JPEG 2000";
	case CompressionValue::Nikon_NEF_Compressed: return "Nikon NEF Compressed";
	case CompressionValue::JBIG2_TIFF_FX: return "CompressionValue::JBIG2 TIFF FX";
	case CompressionValue::Microsoft_Document_Imaging_MDI_Binary_Level_Codec:
		return "Microsoft Document Imaging (MDI) Binary Level Codec";
	case CompressionValue::Microsoft_Document_Imaging_MDI_Progressive_Transform_Codec:
		return "Microsoft Document Imaging (MDI) Progressive Transform Codec";
	case CompressionValue::Microsoft_Document_Imaging_MDI_Vector:
		return "Microsoft Document Imaging (MDI) Vector";
	case CompressionValue::Lossy_JPEG: return "Lossy JPEG";
	case CompressionValue::Kodak_DCR_Compressed: return "Kodak DCR Compressed";
	case CompressionValue::Pentax_PEF_Compressed: return "Pentax PEF Compressed";
	}
	return "";
}

std::ostream& operator<< (std::ostream& os, ExposureProgramValue type)
{
	switch (type)
	{
	case ExposureProgramValue::Not_defined: return os << "Not defined";
	case ExposureProgramValue::Manual: return os << "Manual";
	case ExposureProgramValue::Normal_program: return os << "Normal program";
	case ExposureProgramValue::Aperture_priority: return os << "Aperture priority";
	case ExposureProgramValue::Shutter_priority: return os << "Shutter priority";
	case ExposureProgramValue::Creative_program: return os << "Creative program (biased toward depth of field)";
	case ExposureProgramValue::Action_program : return os << "Action program (biased toward fast shutter speed)";
	case ExposureProgramValue::Portrait_mode : return os << "Portrait mode (for closeup photos with the background out of focus)";
	case ExposureProgramValue::Landscape_mode : return os << "Landscape mode (for landscape photos with the background in focus)";
	}
	return os << "Unkown";
}

const char *JpegEXIF::toString(ExposureProgramValue type)
{
	switch (type)
	{
	case ExposureProgramValue::Not_defined: return "Not defined";
	case ExposureProgramValue::Manual: return "Manual";
	case ExposureProgramValue::Normal_program: return "Normal program";
	case ExposureProgramValue::Aperture_priority: return "Aperture priority";
	case ExposureProgramValue::Shutter_priority: return "Shutter priority";
	case ExposureProgramValue::Creative_program: return "Creative program (biased toward depth of field)";
	case ExposureProgramValue::Action_program: return "Action program (biased toward fast shutter speed)";
	case ExposureProgramValue::Portrait_mode: return "Portrait mode (for closeup photos with the background out of focus)";
	case ExposureProgramValue::Landscape_mode: return "Landscape mode (for landscape photos with the background in focus)";
	}
	return "";
}

std::ostream& operator<< (std::ostream& os, MeteringModeValue type)
{
	switch (type)
	{
	case MeteringModeValue::Unknown: return os << "unknown";
	case MeteringModeValue::Average: return os << "average";		// 1: average
	case MeteringModeValue::Center: return os << "center weighted average";			// 2: center weighted average
	case MeteringModeValue::Spot: return os << "spot";			// 3: spot
	case MeteringModeValue::MultiSpot: return os << "multi-spot";		// 4: multi-spot
	case MeteringModeValue::MultiSegment: return os << "multi-segment";	// 5: multi-segment
	}
	return os << "Unkown";
}

const char *JpegEXIF::toString(MeteringModeValue type)
{
	switch (type)
	{
	case MeteringModeValue::Unknown: return "unknown";
	case MeteringModeValue::Average: return "average";		// 1: average
	case MeteringModeValue::Center: return "center weighted average";			// 2: center weighted average
	case MeteringModeValue::Spot: return "spot";			// 3: spot
	case MeteringModeValue::MultiSpot: return "multi-spot";		// 4: multi-spot
	case MeteringModeValue::MultiSegment: return "multi-segment";	// 5: multi-segment
	}
	return "";
}

std::ostream& operator<< (std::ostream& os, ColorSpaceValue type) {
	switch (type)
	{
	case ColorSpaceValue::sRGB: return os << "sRGB";
	case ColorSpaceValue::Adobe_RGB: return os << "Adobe RGB";
	case ColorSpaceValue::Wide_Gamut_RGB: return os << "Wide Gamut RGB";
	case ColorSpaceValue::ICC_Profile: return os << "ICC Profile";
	case ColorSpaceValue::Uncalibrated: return os << "Uncalibrated";
	}
	return os << "Unkown";
}

const char *JpegEXIF::toString(ColorSpaceValue type) {
	switch (type)
	{
	case ColorSpaceValue::sRGB: return " sRGB";
	case ColorSpaceValue::Adobe_RGB: return "Adobe RGB";
	case ColorSpaceValue::Wide_Gamut_RGB: return "Wide Gamut RGB";
	case ColorSpaceValue::ICC_Profile: return "ICC Profile";
	case ColorSpaceValue::Uncalibrated: return "Uncalibrated";
	}
	return "";
}




std::ostream& operator<< (std::ostream& os, SubjectDistanceRangeValue type) {
	switch (type)
	{

	case SubjectDistanceRangeValue::Unknown: return os << "Unknown";
	case SubjectDistanceRangeValue::Macro: return os << "Macro";
	case SubjectDistanceRangeValue::CloseView: return os << "CloseView";
	case SubjectDistanceRangeValue::DistantView: return os << "DistantView";
	
	}
	return os << "Unkown";
}

const char *JpegEXIF::toString(SubjectDistanceRangeValue type) {
	switch (type)
	{
	case SubjectDistanceRangeValue::Unknown: return "Unknown";
	case SubjectDistanceRangeValue::Macro: return "Macro";
	case SubjectDistanceRangeValue::CloseView: return "CloseView";
	case SubjectDistanceRangeValue::DistantView: return "DistantView";
	}
	return "";
}


std::ostream& operator<< (std::ostream& os, SceneCaptureTypeValue type) {
	switch (type)
	{
	case SceneCaptureTypeValue::stardard: return os << "stardard";
	case SceneCaptureTypeValue::Landscape: return os << "Landscape";
	case SceneCaptureTypeValue::Portrait: return os << "Portrait";
	case SceneCaptureTypeValue::Night: return os << "Night";
	}
	return os << "Unkown";
}

const char *JpegEXIF::toString(SceneCaptureTypeValue type) {
	switch (type)
	{
	case SceneCaptureTypeValue::stardard: return "stardard";
	case SceneCaptureTypeValue::Landscape: return "Landscape";
	case SceneCaptureTypeValue::Portrait: return "Portrait";
	case SceneCaptureTypeValue::Night: return "Night";
	}
	return "";
}

std::ostream& operator<< (std::ostream& os, WhiteBalanceValue type) {
	switch (type)
	{
	case WhiteBalanceValue::Auto: return os << "Auto";
	case WhiteBalanceValue::Manual: return os << "Manual";
	}
	return os << "Unkown";
}

const char *JpegEXIF::toString(WhiteBalanceValue type) {
	switch (type)
	{
	case WhiteBalanceValue::Auto: return "Auto";
	case WhiteBalanceValue::Manual: return "Manual";
	}
	return "";
}

std::ostream& operator<< (std::ostream& os, GainControlValue type) {
	switch (type)
	{
	case GainControlValue::None: return os << "None";
	case GainControlValue::Low_gain_up: return os << "Low gain up";
	case GainControlValue::High_gain_up: return os << "High gain up";
	case GainControlValue::Low_gain_down: return os << "Low gain down";
	case GainControlValue::High_gain_down: return os << "High gain down";
	}
	return os << "Unkown";
}
const char *JpegEXIF::toString(GainControlValue type) {
	switch (type)
	{
	case GainControlValue::None: return "None";
	case GainControlValue::Low_gain_up: return "Low gain up";
	case GainControlValue::High_gain_up: return "High gain up";
	case GainControlValue::Low_gain_down: return "Low gain down";
	case GainControlValue::High_gain_down: return "High gain down";
	}
	return "";
}


std::ostream& operator<< (std::ostream& os, ContrastValue type) {
	switch (type)
	{
	case ContrastValue::Normal: return os << "Normal";
	case ContrastValue::Low: return os << "Low";
	case ContrastValue::High: return os << "High";
	}
	return os << "Unkown";
}
const char *JpegEXIF::toString(ContrastValue type) {
	switch (type)
	{
	case ContrastValue::Normal: return "Normal";
	case ContrastValue::Low: return "Low";
	case ContrastValue::High: return "High";
	}
	return "";
}


std::ostream& operator<< (std::ostream& os, SaturationValue type) {
	switch (type)
	{
	case SaturationValue::Normal: return os << "Normal";
	case SaturationValue::Low: return os << "Low";
	case SaturationValue::High: return os << "High";
	}
	return os << "Unkown";
}

const char *JpegEXIF::toString(SaturationValue type) {
	switch (type)
	{
	case SaturationValue::Normal: return "Normal";
	case SaturationValue::Low: return "Low";
	case SaturationValue::High: return "High";
	}
	return "";
}

std::ostream& operator<< (std::ostream& os, SharpnessValue type) {
	switch (type)
	{
	case SharpnessValue::Normal: return os << "Normal";
	case SharpnessValue::Soft: return os << "Soft";
	case SharpnessValue::Hard: return os << "Hard";
	}
	return os << "Unkown";
}

const char *JpegEXIF::toString(SharpnessValue type) {
	switch (type)
	{
	case SharpnessValue::Normal: return "Normal";
	case SharpnessValue::Soft: return "Soft";
	case SharpnessValue::Hard: return "Hard";
	}
	return "";
}

std::ostream& operator<< (std::ostream& os, ResolutionUnitValue type) {
	switch (type)
	{
	case ResolutionUnitValue::None: return os << "None";
	case ResolutionUnitValue::inches: return os << "inches";
	case ResolutionUnitValue::cm: return os << "cm";
	}
	return os << "Unkown";
}

const char *JpegEXIF::toString(ResolutionUnitValue type) {
	switch (type)
	{
	case ResolutionUnitValue::None: return "None";
	case ResolutionUnitValue::inches: return "inches";
	case ResolutionUnitValue::cm: return "cm";
	}
	return "";
}


std::ostream& operator<< (std::ostream& os, PhotometricInterpretationValue type) {
	switch (type)
	{
	case PhotometricInterpretationValue::WhiteIsZero: return os << "WhiteIsZero";
	case PhotometricInterpretationValue::BlackIsZero: return os << "BlackIsZero";
	case PhotometricInterpretationValue::RGB: return os << "RGB";
	case PhotometricInterpretationValue::RGB_Palette: return os << "RGB Palette";
	case PhotometricInterpretationValue::Transparency_Mask: return os << "Transparency Mask";
	case PhotometricInterpretationValue::CMYK: return os << "CMYK";
	case PhotometricInterpretationValue::YCbCr: return os << "YCbCr";
	case PhotometricInterpretationValue::CIELab: return os << "CIELab";
	case PhotometricInterpretationValue::ICCLab: return os << "ICCLab";
	case PhotometricInterpretationValue::ITULab: return os << "ITULab";
	case PhotometricInterpretationValue::Color_Filter_Array: return os << "Color Filter Array";
	case PhotometricInterpretationValue::Pixar_LogL: return os << "Pixar LogL";
	case PhotometricInterpretationValue::Pixar_LogLuv: return os << "Pixar LogLuv";
	case PhotometricInterpretationValue::Linear_Raw: return os << "Linear Raw";
	case PhotometricInterpretationValue::Not_Set: return os << "";
	}
	return os << "Unkown";
}

const char *JpegEXIF::toString(PhotometricInterpretationValue type) {
	switch (type)
	{
	case PhotometricInterpretationValue::WhiteIsZero: return "WhiteIsZero";
	case PhotometricInterpretationValue::BlackIsZero: return "BlackIsZero";
	case PhotometricInterpretationValue::RGB: return "RGB";
	case PhotometricInterpretationValue::RGB_Palette: return "RGB Palette";
	case PhotometricInterpretationValue::Transparency_Mask: return "Transparency Mask";
	case PhotometricInterpretationValue::CMYK: return "CMYK";
	case PhotometricInterpretationValue::YCbCr: return "YCbCr";
	case PhotometricInterpretationValue::CIELab: return "CIELab";
	case PhotometricInterpretationValue::ICCLab: return "ICCLab";
	case PhotometricInterpretationValue::ITULab: return "ITULab";
	case PhotometricInterpretationValue::Color_Filter_Array: return "Color Filter Array";
	case PhotometricInterpretationValue::Pixar_LogL: return "Pixar LogL";
	case PhotometricInterpretationValue::Pixar_LogLuv: return "Pixar LogLuv";
	case PhotometricInterpretationValue::Linear_Raw: return "Linear Raw";
	case PhotometricInterpretationValue::Not_Set: "";
	}
	return "";

}


void JpegEXIF::clear() {
	// Strings
	ImageDescription = "";
	Make = "";
	Model = "";
	Software = "";
	DateTime = "";
	DateTimeOriginal = "";
	DateTimeDigitized = "";
	SubSecTimeOriginal = "";
	copyright = "";

	// Shorts / unsigned / double
	ByteAlign = 0;
	Orientation = 0;

	BitsPerSample = 0;
//	ExposureTime.denominator = 0;
	ExposureTime.numerator = 0;
	ExposureTime.rational = 0;
	FNumber.denominator = 0;
	FNumber.numerator = 0;
	FNumber.rational = 0;
	ISOSpeedRatings = 0;
//	ShutterSpeedValue = 0;
	ExposureBiasValue.denominator = 0;
	ExposureBiasValue.numerator = 0;
	ExposureBiasValue.rational = 0;
	SubjectDistance = 0;
	FocalLength = 0;
	FocalLengthIn35mm = 0;
	Flash = 0;
	flashEnergy = 0;
	meteringMode = 0;
	ImageWidth = 0;
	ImageHeight = 0;
	compression = 6;
	photometricInterpretation = 11,
	// Geolocation
	GeoLocation.Latitude = 0;
	GeoLocation.Longitude = 0;
	GeoLocation.Altitude = 0;
	GeoLocation.AltitudeRef = 0;
	GeoLocation.LatComponents.degrees = 0;
	GeoLocation.LatComponents.minutes = 0;
	GeoLocation.LatComponents.seconds = 0;
	GeoLocation.LatComponents.direction = 0;
	GeoLocation.LonComponents.degrees = 0;
	GeoLocation.LonComponents.minutes = 0;
	GeoLocation.LonComponents.seconds = 0;
	GeoLocation.LonComponents.direction = 0;

}



void JpegEXIF::print() {
	// Strings
	printf("Image Description %s\n", ImageDescription.c_str());
	printf("Make %s\n", Make.c_str());
	printf("Model %s\n", Model.c_str());
	printf("Software %s\n", Software.c_str());
	printf("DateTime %s\n", DateTime.c_str());
	printf("DateTimeOriginal %s\n", DateTimeOriginal.c_str());
	printf("DateTimeDigitized %s\n", DateTimeDigitized.c_str());
	printf("SubSecTimeOriginal %s\n", SubSecTimeOriginal.c_str());
	printf("copyright %s\n", copyright.c_str());

	printf("title %s\n", title.c_str());
	printf("comment %s\n", comment.c_str());
	printf("author %s\n", author.c_str());
	printf("keywords %s\n", keywords.c_str());
	printf("subject %s\n", subject.c_str());
	printf("rating %d\n", rating);
	


	// Shorts / unsigned / double
	printf("ByteAlign %d\n", ByteAlign);
	printf("Orientation %d\n", Orientation);

	printf("BitsPerSample %d\n", BitsPerSample);

	if (ExposureTime.rational < 1) {
		printf("ExposureTime %d/%d\n", ExposureTime.numerator, ExposureTime.denominator);
	}
	else {
		printf("ExposureTime %d\n", ExposureTime.numerator);
	}
	
	printf("F-stop f/%.1f\n", FNumber.rational);
	std::string ep = JpegEXIF::toString((ExposureProgramValue)exposureProgram);
	printf("ExposureProgram %s\n", ep.c_str());
	std::string ls = toString((LightSourceValues)lightSource);
	printf("lightSource %s\n", ls.c_str());
	printf("ISOSpeedRatings %d\n", ISOSpeedRatings);
	printf("ExposureBiasValue %.1f\n", (double)(ExposureBiasValue.numerator/ExposureBiasValue.denominator));
	//printf("%d/%d\n", ExposureBiasValue.numerator, ExposureBiasValue.denominator);
	printf("SubjectDistance %f\n", SubjectDistance);
	printf("FocalLength %f\n", FocalLength);
	printf("FocalLengthIn35mm %d\n", FocalLengthIn35mm);
	std::string f = JpegEXIF::toString((FlashValues)Flash);
	printf("Flash %s\n", f.c_str());
	if (flashEnergy == 0.0) {
		printf("Flash energy\n");
	}
	else {
		printf("Flash energy %f\n", flashEnergy);
	}
	std::string mm = JpegEXIF::toString((MeteringModeValue)meteringMode);
	printf("MeteringMode %s\n", mm.c_str());
	printf("ImageWidth %d\n", ImageWidth);
	printf("ImageHeight %d\n", ImageHeight);
	
	std::string res = JpegEXIF::toString((ResolutionUnitValue)resolutionUnit);
	printf("X Resolution %.0f %s\n", xResolution, res.c_str());
	printf("Y Resolution %.0f %s\n", yResolution, res.c_str());
	printf("copyright %s\n", copyright.c_str());
	std::string cp = JpegEXIF::toString((ColorSpaceValue)colorSpace);
	printf("colorSpace %s\n", cp.c_str());
	if (brightnessValue.numerator == 0xFFFFFFFF) {
		printf("brightnessValue: unknown\n");
	}
	else {
		printf("brightnessValue %.3f\n", brightnessValue.rational);
	}
	std::string sct = toString((SceneCaptureTypeValue)SceneCaptureType);
	printf("SceneCaptureType %s\n", sct.c_str());
	std::string c = JpegEXIF::toString((ContrastValue)Contrast);
	printf("Contrast %s\n", c.c_str());
	std::string sa = toString((SaturationValue)Saturation);
	printf("Saturation %s\n", sa.c_str());
	std::string sh = toString((SharpnessValue)Sharpness);
	printf("SceneCaptureType %s\n", sh.c_str());
	printf("MaxApertureValue %f\n", maxApertureValue.rational);
	printf("Compressed bits per pixel %f\n", compressedBitsPerPixel.rational);
	std::string comp = toString((CompressionValue)compression);
	printf("Compression %s\n", comp.c_str());
	
	printf("Exif version %s\n", exifVersion.c_str());
	std::string pi = toString((PhotometricInterpretationValue)photometricInterpretation);
	printf("Photometric Interpretation %s\n", pi.c_str());
	
/*
	// Geolocation
	GeoLocation.Latitude = 0;
	GeoLocation.Longitude = 0;
	GeoLocation.Altitude = 0;
	GeoLocation.AltitudeRef = 0;
	GeoLocation.LatComponents.degrees = 0;
	GeoLocation.LatComponents.minutes = 0;
	GeoLocation.LatComponents.seconds = 0;
	GeoLocation.LatComponents.direction = 0;
	GeoLocation.LonComponents.degrees = 0;
	GeoLocation.LonComponents.minutes = 0;
	GeoLocation.LonComponents.seconds = 0;
	GeoLocation.LonComponents.direction = 0;
*/
}


/*
unsigned short rating;			  // Short Rating tag used by Windows
unsigned short ratingPercent;	  // Rating tag used by Windows, value in percent
double ExposureTime;              // Exposure time in seconds
double FNumber;                   // F/stop
unsigned short ISOSpeedRatings;   // ISO speed
double ShutterSpeedValue;         // Shutter speed (reciprocal of exposure time)
double ExposureBiasValue;         // Exposure bias value in EV
unsigned short exposureProgram;	  // The class of the program used by the camera to set exposure when the picture is taken.
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


unsigned short  whiteBalance;		// Short This tag indicates the white balance mode set when the image was shot.

double DigitalZoomRatio;			// This tag indicates the digital zoom ratio when the image was shot.If the numerator of the recorded value is 0, this indicates that digital zoom was not used.

TIFF Tag SceneCaptureType
The specification defines these values:

0 = Standard
1 = Landscape
2 = Portrait
3 = Night scene



unsigned short SceneCaptureType;	// This tag indicates the type of scene that was shot.It can also be used to record the mode in which the image was shot.Note that this differs from the <SceneType> tag.

unsigned short Contrast;			// This tag indicates the direction of contrast processing applied by the camera when the image was shot.
unsigned short Saturation;		// This tag indicates the direction of saturation processing applied by the camera when the image was shot.
unsigned short Sharpness;		    // This tag indicates the direction of sharpness processing applied by the camera when the image was shot.

unsigned short SubjectDistanceRange; // This tag indicates the distance to the subject.
*/
