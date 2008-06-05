/* FlamingTux 
 * Authors: peacey, jeed890 
 * http://flamingtux.googlecode.com 
 * This code is GPL, licensing soon though ;) */
#include <iostream>
#include <stdexcept>
#include <string>
#include <sstream>

#include "CommonFunctions.h"

// copied from http://www.parashift.com/c++-faq-lite/misc-technical-issues.html#faq-39.1
class BadConversion : public std::runtime_error {
	public:
		BadConversion(const std::string& s)
	: std::runtime_error(s)
		{ }
};
 
std::string stringify(int x)
{
	std::ostringstream o;
	if (!(o << x))
		throw BadConversion("stringify(int)");
	return o.str();
}

