#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <unordered_map>

#include "GlobalTypeDef.h"
#include "Grammar.h"

class QueryUtil {
public:
  /**
  * A public function to check if both parameters are underscores.
  * Given two parameters, it will check if both parameters are underscores.
  * @param t_g1 A grammar object which holds the first parameter of a relation clause.
  * @param t_g2 A grammar object which holds the second parameter of a relation clause.
  * @return true if both parameters are underscores else return false.
  */
  static bool isAllUnderscores(Grammar t_g1, Grammar t_g2);

  /**
  * A public function to check if both parameters are not synonyms.
  * Given two parameters, it will check if both parameters are not synonyms.
  * @param t_g1 A grammar object which holds the first parameter of a relation clause.
  * @param t_g2 A grammar object which holds the second parameter of a relation clause.
  * @return true if both parameters are not synonyms else return false.
  */
  static bool hasNoSynonyms(Grammar t_g1, Grammar t_g2);

  /**
  * A public function to check if only the second paramter is a synonym.
  * Given two parameters, it will check if the first parameter is not a synonym
  * and the second parameter is a synonym.
  * @param t_g1 A grammar object which holds the first parameter of a relation clause.
  * @param t_g2 A grammar object which holds the second parameter of a relation clause.
  * @return true if only the second parameter is a synonym else return false.
  */
  static bool hasOneRightSynonym(Grammar t_g1, Grammar t_g2);

  /**
  * A public function to check if only the first paramter is a synonym.
  * Given two parameters, it will check if the first parameter is a synonym
  * and the second parameter is not a synonym.
  * @param t_g1 A grammar object which holds the first parameter of a relation clause.
  * @param t_g2 A grammar object which holds the second parameter of a relation clause.
  * @return true if only the first parameter is a synonym else return false.
  */
  static bool hasOneLeftSynonym(Grammar t_g1, Grammar t_g2);

  /**
  * A public function to check if both paramters are synonyms.
  * Given two parameters, it will check if both parameters are synonyms.
  * @param t_g1 A grammar object which holds the first parameter of a relation clause.
  * @param t_g2 A grammar object which holds the second parameter of a relation clause.
  * @return true if both parameters are synonyms else return false.
  */
  static bool hasTwoSynonyms(Grammar t_g1, Grammar t_g2);

  /**
  * A public function to check if the pattern clause is anything with any pattern.
  * Given the pattern parameters, it will check if both parameters are underscores.
  * @param t_g1 A grammar object which holds the first parameter of a pattern clause.
  * @param t_g2 A grammar object which holds the second parameter of a pattern clause.
  * @return true if both parameters are underscores.
  */
  static bool isAnythingWithAnyPattern(Grammar t_g1, Grammar t_g2);

  /**
  * A public function to check if the pattern clause is anything with an exact pattern.
  * Given the pattern parameters, it will check if the first parameter is underscore
  * and the second parameter is a string and the third parameter is true.
  * @param t_g1 A grammar object which holds the first parameter of a pattern clause.
  * @param t_g2 A grammar object which holds the second parameter of a pattern clause.
  * @param t_isExact A boolean to indicate whether the pattern has to be an exact match.
  * @return true if the first parameter is underscore and the second parameter is a string
  * and the third parameter is true.
  */
  static bool isAnythingWithExactPattern(Grammar t_g1, Grammar t_g2, bool t_isExact);

  /**
  * A public function to check if the pattern clause is anything with a non-exact pattern.
  * Given the pattern parameters, it will check if the first parameter is underscore
  * and the second parameter is a string and the third parameter is false.
  * @param t_g1 A grammar object which holds the first parameter of a pattern clause.
  * @param t_g2 A grammar object which holds the second parameter of a pattern clause.
  * @param t_isExact A boolean to indicate whether the pattern has to be an exact match.
  * @return true if the first parameter is underscore and the second parameter is a string
  * and the third parameter is false.
  */
  static bool isAnythingWithSubPattern(Grammar t_g1, Grammar t_g2, bool t_isExact);

  /**
  * A public function to check if the pattern clause is anything with any pattern.
  * Given the pattern parameters, it will check if both parameters are underscores.
  * @param t_g1 A grammar object which holds the first parameter of a pattern clause.
  * @param t_g2 A grammar object which holds the second parameter of a pattern clause.
  * @return true if both parameters are underscores.
  */
  static bool isVarWithAnyPattern(Grammar t_g1, Grammar t_g2);

  /**
  * A public function to check if the pattern clause is anything with an exact pattern.
  * Given the pattern parameters, it will check if the first parameter is underscore
  * and the second parameter is a string and the third parameter is true.
  * @param t_g1 A grammar object which holds the first parameter of a pattern clause.
  * @param t_g2 A grammar object which holds the second parameter of a pattern clause.
  * @param t_isExact A boolean to indicate whether the pattern has to be an exact match.
  * @return true if the first parameter is underscore and the second parameter is a string
  * and the third parameter is true.
  */
  static bool isVarWithExactPattern(Grammar t_g1, Grammar t_g2, bool t_isExact);

  /**
  * A public function to check if the pattern clause is anything with a non-exact pattern.
  * Given the pattern parameters, it will check if the first parameter is underscore
  * and the second parameter is a string and the third parameter is false.
  * @param t_g1 A grammar object which holds the first parameter of a pattern clause.
  * @param t_g2 A grammar object which holds the second parameter of a pattern clause.
  * @param t_isExact A boolean to indicate whether the pattern has to be an exact match.
  * @return true if the first parameter is underscore and the second parameter is a string
  * and the third parameter is false.
  */
  static bool isVarWithSubPattern(Grammar t_g1, Grammar t_g2, bool t_isExact);

  /**
  * A public function to check if the pattern clause is anything with any pattern.
  * Given the pattern parameters, it will check if both parameters are underscores.
  * @param t_g1 A grammar object which holds the first parameter of a pattern clause.
  * @param t_g2 A grammar object which holds the second parameter of a pattern clause.
  * @return true if both parameters are underscores.
  */
  static bool isSynonymWithAnyPattern(Grammar t_g1, Grammar t_g2);

  /**
  * A public function to check if the pattern clause is anything with an exact pattern.
  * Given the pattern parameters, it will check if the first parameter is underscore
  * and the second parameter is a string and the third parameter is true.
  * @param t_g1 A grammar object which holds the first parameter of a pattern clause.
  * @param t_g2 A grammar object which holds the second parameter of a pattern clause.
  * @param t_isExact A boolean to indicate whether the pattern has to be an exact match.
  * @return true if the first parameter is underscore and the second parameter is a string
  * and the third parameter is true.
  */
  static bool isSynonymWithExactPattern(Grammar t_g1, Grammar t_g2, bool t_isExact);

  /**
  * A public function to check if the pattern clause is anything with a non-exact pattern.
  * Given the pattern parameters, it will check if the first parameter is underscore
  * and the second parameter is a string and the third parameter is false.
  * @param t_g1 A grammar object which holds the first parameter of a pattern clause.
  * @param t_g2 A grammar object which holds the second parameter of a pattern clause.
  * @param t_isExact A boolean to indicate whether the pattern has to be an exact match.
  * @return true if the first parameter is underscore and the second parameter is a string
  * and the third parameter is false.
  */
  static bool isSynonymWithSubPattern(Grammar t_g1, Grammar t_g2, bool t_isExact);
};
