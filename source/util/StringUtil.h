#pragma once
#include <iostream>
#include <vector>
#include "../GlobalTypeDef.h"
/**
* Handles utility methods for manipulation of strings.
*
* @author Brandon
* @date 8/10/2017
*/
class StringUtil
{
public:
  /**
  * Splits the string based on the delimiter provided. The default delimiter is ' '.
  *
  * @param t_line is the string that you wish to split.
  * @param t_delim is the delimiter to be provided. Defaults to ' '.
  */
  static LIST_OF_RESULTS splitString(STRING t_line, STRING_CHARACTER t_delim = ' ');

  /**
  * Returns a string with its ends trimmed based on the delimiter provided. The default delimiter is " " and "\t".
  *
  * @param t_line is the string that you wish to trim.
  * @param t_whitespace is the delimiter to be provided. Defaults to " " and "\t".
  */
  static STRING trimString(const STRING& t_str, const STRING& t_whitespace = " \t");

  /**
  * Returns a string with that has been reduced to a single " " only. "\t" and additional " " are removed.
  *
  * @param t_str is the string that you wish to reduce.
  * @param t_fill is the string to be filled in place of the whitespaces. Defaults to " ".
  * @param t_whitespace is the delimiter to be provided. Defaults to " " and "\t".
  */
  static STRING reduceString(const STRING& t_str, const STRING& t_fill = " ", const STRING& t_whitespace = " \t");

  /**
  * Removes white spaces from the string provided, this mutates the param provided.
  *
  * @param t_str is the string that you wish to remove white spaces.
  */
  static void removeWhiteSpace(STRING& t_str);

  /**
  * Changes the string to lowercase, this mutates the param provided.
  *
  * @param t_str is the string that you wish to change to lower case.
  */
  static void toLower(STRING& t_str);

  /**
  * Create a string with the given character repeated in the given number of times.
  * @param t_repeatedChar The character to repeat in a string
  * @param t_numOfTimesToRepeat The number of times to repeat the character in a string
  * @return a string with the given character repeated in the given number of times.
  */
  static STRING createStringWithRepeatedChar(STRING t_repeatedChar, INTEGER t_numOfTimesToRepeat);

  /**
  * A public function to check whether the string is "_".
  * It checks whether the given string is "_".
  * @param t_str The string to be checked.
  * @return true if the given string is "_" else return false.
  */
  static BOOLEAN isUnderscore(STRING t_str);
};

