#pragma once
#include <iostream>
#include <vector>

/*
* Handles utility methods for manipulation of strings.
*/
class StringUtil
{
public:
  static std::vector<std::string> splitString(std::string t_line, char t_delim = ' ');
  static std::string trimString(const std::string &str, const std::string &whitespace = " \t");
  static std::string reduceString(const std::string &str, const std::string& fill = " ", const std::string& whitespace = " \t");
  static void removeWhiteSpace(std::string &str);
  static void toLower(std::string &str);
};

