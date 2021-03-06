#include <iostream>
#include <stdlib.h>

#include "ProductElementValue_sbx.h"
#include "Utils.h"

using namespace std;

namespace sbx {

ProductElementValue::ProductElementValue(void)
		: _productElementOid { 0 },
		  _stringValue { "" }
{
	// empty constructor needed for vector initialisation.
}
ProductElementValue::ProductElementValue(const sbx::ProductElementValue& otherProductElementValue)
		: _productElementOid { otherProductElementValue._productElementOid },
		  _stringValue { otherProductElementValue._stringValue }
{}

ProductElementValue::ProductElementValue(unsigned short peOid, const std::string& stringValue)
		: _productElementOid { peOid },
		  _stringValue { stringValue }
{}

/**
 * Get the constant value as a string
 */
std::string ProductElementValue::stringValue() const
{
	return _stringValue;
}

/**
 * Get the constant value as a long value
 */
long ProductElementValue::longValue() const
{
	// convert string into a long value
	return atol(_stringValue.c_str());
}

/**
 * Gets the constant value as a double
 */
double ProductElementValue::doubleValue() const
{
	// convert string into a double value
	return atof(_stringValue.c_str());
}

bool ProductElementValue::boolValue() const{ return sbx::utils::toBool(_stringValue); }

unsigned short ProductElementValue::getProductElementOid() const
{
	return _productElementOid;
}

void sbx::ProductElementValue::setValue(const std::string& value)
{
	_stringValue = value;
}

/**
 *
 */
ProductElementValue::~ProductElementValue(void)
{}

} // namespace sbx

