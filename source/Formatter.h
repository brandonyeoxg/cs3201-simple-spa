#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <unordered_map>

#include "GlobalTypeDef.h"

class Formatter {
public:
  /**
  * A public function to format integer vector to string vector.
  * Given a vector of integers, it will format it into a vector of strings.
  * @return the contents of a vector of integers in a vector of strings.
  */
  static std::vector<std::string> formatVectorIntToVectorStr(std::vector<int> t_vectorInt);

  /**
  * A public function to format string list to string vector.
  * Given a list of strings, it will format it into a vector of strings.
  * @return the contents of a list of strings in a vector of strings.
  */
  static std::vector<std::string> formatListStrToVectorStr(std::list<std::string> t_listStr);

  /**
  * A public function to format integer list to string vector.
  * Given a list of integers, it will format it into a vector of strings.
  * @return the contents of a list of integers in a vector of strings.
  */
  static std::vector<std::string> formatListIntToVectorStr(std::list<int> t_listInt);

  /**
  * A public function to format map<int, str> to map<str, vector<str>>.
  * Given a map<int, str>, it will format it into a map<str, vector<str>>.
  * @return the contents of a map<int, str> in a map<str, vector<str>>.
  */
  static SET_OF_RESULTS formatMapIntStrToMapStrVectorStr(std::unordered_map<int, std::string> t_mapIntStr);

  /**
  * A public function to format string vector to map<str, vector<str>>.
  * Given a string vector, it will format it into a map<str, vector<str>>
  * by duplicating the string vector for the key and value.
  * @return the contents of a string vector in a map<str, vector<str>>.
  */
  static SET_OF_RESULTS formatVectorStrToMapStrVectorStr(LIST_OF_RESULTS t_vectorStr);
};

