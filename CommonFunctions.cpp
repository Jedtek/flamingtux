/* FlamingTux 
 * Authors: peacey, jeed890 
 * http://flamingtux.googlecode.com 
 *  This code is protected by GPLv3, please see LICENSE file for more information */
#include <iostream>
#include <stdexcept>
#include <string>
#include <sstream>
#include <pangomm.h>

#include "CommonFunctions.h"

// copied from http://www.parashift.com/c++-faq-lite/misc-technical-issues.html#faq-39.1
class BadConversion : public std::runtime_error {
	public:
		BadConversion(const std::string& s)
	: std::runtime_error(s)
		{ }
};
 
std::string stringify(int x) {
	std::ostringstream o;
	if (!(o << x))
		throw BadConversion("stringify(int)");
	return o.str();
}

int intify(std::string x) {
	std::istringstream o(x);
	int a;
	if (!(o >> a))
		throw BadConversion("intify(string)");
	return a;
}

Glib::ustring parseMarkup(Glib::ustring markup) {
	if(!pango_parse_markup(markup.c_str(), -1, 0, NULL, NULL, NULL, NULL)) {
		Glib::RefPtr<Glib::Regex> regexp = Glib::Regex::create("&");
		markup = regexp->replace(markup, 0, "&amp;", 
				       static_cast<Glib::RegexMatchFlags>(0));
		regexp = Glib::Regex::create("<");
		markup = regexp->replace(markup, 0, "&lt;", 
				       static_cast<Glib::RegexMatchFlags>(0));
		regexp = Glib::Regex::create(">");
		markup = regexp->replace(markup, 0, "&gt;", 
				       static_cast<Glib::RegexMatchFlags>(0));
	}
	return markup;
}

