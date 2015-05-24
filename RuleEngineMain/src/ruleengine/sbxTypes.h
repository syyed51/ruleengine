//============================================================================
// Name        : sbxTypes.cpp
// Author      : xjes
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#ifndef RULEENGINE_SBXTYPES_H_
#define RULEENGINE_SBXTYPES_H_

#include <algorithm>
#include <string>
#include <iostream>

namespace sbx {

std::string toUpper(const std::string& s);
bool toBool(const std::string& s);

enum ComparisonTypes : unsigned short {
	kUnknown	= 0,
	kMin		= 1,
	kMax		= 2,
	kEnum		= 3,
	kEquals		= 4
};

enum ProductElementTypes : unsigned short {
	kUnknownPEType	= 0,
	kLong			= 1,
	kText 			= 2,
	kMonth 			= 3,
	kBool 			= 4,
	kCurr			= 5,
	kDate 			= 6,
	kYear			= 7,
	kPercent		= 8
};

enum ProductOid : unsigned short {
	kUnknownProduct		 	= 0,
	kP_194					= 194,
	kP_217					= 217,
	kP_218					= 218
};

enum ProductElementOid : unsigned short {
			kUnknownProductElement 			= 0,
			kDoedBlGrMin					= 1,
			kDoedBlOblMax					= 2,
			kDoedSpaendBl					= 3,
			kDoedSoliMax					= 4,
			kDoedPctGrMin					= 6,
			kDoedPctOblMax					= 7,
			kDoedSpaendPct					= 9,
			kTaeBlGrMin						= 14,
			kTaeBlOblMax					= 15,
			kTaeSpaendBl					= 16,
			kBoerneUdloebsalder				= 67,
			kUdlobsalder_Pension			= 71,
			kTraditionel_MK					= 72,
			kLink_MK						= 73,
			kMarkedspension_MK				= 74,
			kTidspensionMedGaranti_MK		= 75,
			kDoedReguleringskode			= 82,
			kBoerneRentePctMin				= 85,
			kBoerneRenteBlMin				= 89,
			kBoerneSumBlMin					= 91,
			kStandardProduct				= 95,
			kBoernerenteReguleringstype		= 128,
			kDoedDaekningstype				= 139,
			kLoenDefinition					= 146,
			kLoenRegulering					= 147,
			kBoerneRenteSoliMax				= 183,
			k194							= 194,
			kTidspensionUdenGaranti_MK 		= 210,
			k217							= 217,
			k218							= 218,
			kFravalgRisiko_MK				= 230,
			kFravalgRisikoAlder				= 231
};


enum ValidationCode : unsigned short {
	kUnknownValidationCode			= 0,		// If everything goes wrong, this code is used
	// General validation codes
	kOK 							= 1,		// General OK code
	kWarning 						= 2,		// General warning code
	kFail 							= 3,		// General fail code
	kProductElementNotDefined		= 4,		// If trying to validate a pe that hasn't been defined/set on the TA

	// Validation Codes for values (inside pe's)
	kValueNotAllowed 				= 101,		// value is not allowed i.e. not found in options list
	kValueUnderLimit 				= 103,		// Value is under the allowed limit
	kValueOverLimit 				= 104,		// Value is over the allowed limit

	// Validation Codes for product elements, not the actual value is being validated
	kProductElementNotAllowed		= 201,		// Product element shouldn't be on the TA (and therefore not have a value either)
	kProductElementRequired			= 202, 		// Product element is required to be on the TA (and have a value)

	// muParser Validation Codes
	kTokenNotDefined				= 301		// If an expression (maybe custom) contains a token that hasn't been loaded into the parser
};

} // namespace sbx

#endif // RULEENGINE_SBXTYPES_H_
