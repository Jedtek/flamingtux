/* FlamingTux 
 * Authors: peacey, jeed890 
 * http://flamingtux.googlecode.com 
 *  This code is protected by GPLv3, please see LICENSE file for more information */
#ifndef __COMMONFUNCTIONS__H
#define __COMMONFUNCTIONS__H

#include <glibmm.h>

std::string stringify(int x);
int intify(std::string x);
Glib::ustring parseMarkup(Glib::ustring markup);
bool checkFileExists(Glib::ustring filename);
void playFile(Glib::ustring command, Glib::ustring file);

#endif

