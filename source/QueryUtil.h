#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <unordered_map>
#include <queue>

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

  /** Checks if given Grammar object is an underscore.
  *   @param t_grammar grammar object
  *   @return true if an underscore, else return false.
  *   @author jazlyn
  */
  static bool isUnderscore(Grammar t_grammar);

  /** Checks if given Grammar object is a synonym.
  *   @param t_grammar grammar object
  *   @return true if a synonym, else return false.
  *   @author jazlyn
  */
  static bool isSynonym(Grammar t_grammar);

  /** Checks if two given Grammar objects contain the same synonym.
  *   @param t_g1 A grammar object which holds the first parameter of a relation clause.
  *   @param t_g2 A grammar object which holds the second parameter of a relation clause.
  *   @return true if both are the same synonym, else return false.
  *   @author jazlyn
  */
  static bool areBothSameSynonyms(Grammar t_g1, Grammar t_g2);

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
  * A public function to check if the pattern clause is anything with any pattern (_, _).
  * Given the pattern parameters, it will check if both parameters are underscores.
  * @param t_g1 A grammar object which holds the first parameter of a pattern clause.
  * @param t_g2 A grammar object which holds the second parameter of a pattern clause.
  * @return true if both parameters are underscores.
  */
  static bool isAnythingWithAnyPattern(Grammar t_g1, Grammar t_g2);

  /**
  * A public function to check if the pattern clause is anything with an exact pattern (_, "x").
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
  * A public function to check if the pattern clause is anything with a non-exact pattern (_, _"x"_).
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
  * A public function to check if the pattern clause is a variable string with any pattern ("x", _).
  * Given the pattern parameters, it will check if both parameters are underscores.
  * @param t_g1 A grammar object which holds the first parameter of a pattern clause.
  * @param t_g2 A grammar object which holds the second parameter of a pattern clause.
  * @return true if both parameters are underscores.
  */
  static bool isVarWithAnyPattern(Grammar t_g1, Grammar t_g2);

  /**
  * A public function to check if the pattern clause is a variable string with an exact pattern ("x", "x").
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
  * A public function to check if the pattern clause is a variable string with a non-exact pattern ("x", _"x"_).
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
  * A public function to check if the pattern clause is a variable synonym with any pattern (v, _).
  * Given the pattern parameters, it will check if both parameters are underscores.
  * @param t_g1 A grammar object which holds the first parameter of a pattern clause.
  * @param t_g2 A grammar object which holds the second parameter of a pattern clause.
  * @return true if both parameters are underscores.
  */
  static bool isSynonymWithAnyPattern(Grammar t_g1, Grammar t_g2);

  /**
  * A public function to check if the pattern clause is a variable synonym with an exact pattern (v, "x").
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
  * A public function to check if the pattern clause is a variable synonym with a non-exact pattern (v, _"x"_).
  * Given the pattern parameters, it will check if the first parameter is underscore
  * and the second parameter is a string and the third parameter is false.
  * @param t_g1 A grammar object which holds the first parameter of a pattern clause.
  * @param t_g2 A grammar object which holds the second parameter of a pattern clause.
  * @param t_isExact A boolean to indicate whether the pattern has to be an exact match.
  * @return true if the first parameter is underscore and the second parameter is a string
  * and the third parameter is false.
  */
  static bool isSynonymWithSubPattern(Grammar t_g1, Grammar t_g2, bool t_isExact);

  /**
  * A public function to check if the synonym is common with any other clauses.
  * Check if the synonyn is used more than once in the query.
  * @param t_synMap the map to check whether t_synonym is used in other clauses.
  * @param t_synonym the synonym to check if it is used with any other clauses.
  * @return true if the synonym is used more than once in the query
  * else return false if the synonym is used only once in the query.
  */
  static bool isSynonymCommon(MAP_OF_SYNONYMS_TO_COUNTS t_synMap, STRING t_synonym);

  /**
  * A public function to check if the synonym is selected.
  * Check if the synonyn is a synonym selected in the select clause.
  * @param t_selects the queue of selected synonyms to check whether t_synonym is a selected synonym.
  * @param t_synonym the synonym to check if it is selected.
  * @return true if the synonym is selected in the query
  * else return false if the synonym is not selected in the query.
  */
  static bool isSynonymSelected(std::queue<Grammar> t_selects, STRING t_synonym);
};
