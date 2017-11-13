#pragma once
#include "Extractor.h"
#include "../pkb/relationshipTables/AffectsTable.h"
#include <algorithm>

/**
* Handles extractions for the Affects relation.
*
* @author Brandon
* @date 28/10/2017
*/
class AffectsExtractor : public Extractor
{
public:
  AffectsExtractor(PkbTablesOnly* t_pkb) : Extractor(t_pkb) {};
  ~AffectsExtractor() {};
  void extractDesign();
  ///////////////////////////////////////////////////////
  //  Affects
  ///////////////////////////////////////////////////////
  /**
  * Extracts and returns all affects values, for example affects(a1,a2).
  *
  */
  MAP_OF_STMT_NUM_TO_LIST_OF_STMT_NUMS extractAllAffects();
  
  /**
  * Extracts and returns the assignment statement that affects the uses line, for example affects(a,12)
  * @param t_usesLine the line that is using the modified variables.
  *
  */
  LIST_OF_AFFECTS_STMTS extractAffects(STMT_NUM t_usesLine);
  
  /**
  * Extracts and returns the assignment statement that is affected by the modifies line, for example affects(2,a)
  * @param t_modifiesLine the line that modifies the uses line.
  *
  */
  LIST_OF_AFFECTS_STMTS extractAffectedBy(STMT_NUM t_modifiesLine); // affects(2,a)

  /**
  * Extracts and returns true if the modifie line affects the uses line, for example affects(1,12).
  * @param t_modifiesLine the modifies line.
  * @param t_usesLine the uses line.
  *
  */
  BOOLEAN extractIsAffects(STMT_NUM t_modifiesLine, STMT_NUM t_usesLine);

  /**
  * Extracts and find if there are any affects relationship, for example affects(_,_).
  *
  */
  BOOLEAN extractHasAffectsRelationship();
  
  /**
  * Extracts and returns the list of statements that are modifies, for example affects(a,_)
  *
  */
  LIST_OF_AFFECTS_STMTS extractAffectsAnything();
  
  /**
  * Extracts and returns the list of statements that are uses, for example affects(_,a)
  *
  */
  LIST_OF_AFFECTS_STMTS extractAffectedByAnything();

  /**
  * Extracts and returns true if there is any affects relation starting from the uses line, for example affects(1,_).
  * @param t_usesLine the line uses.
  *
  */
  BOOLEAN extractIsAffectsAnything(STMT_NUM t_usesLine);
  
  /**
  * Extracts and returns true if there is any affects relation ending at the modifies line, for example affects(_,12).
  * @param t_modifiesLine the line that is modifying.
  *
  */
  BOOLEAN extractIsAffectedByAnything(STMT_NUM t_modifiesLine); // affects(_,12)

  ///////////////////////////////////////////////////////
  //  Affects* Extractor
  ///////////////////////////////////////////////////////
  /**
  * Extracts and returns all affects* values, for example affects*(a1,a2).
  *
  */
  MAP_OF_STMT_NUM_TO_LIST_OF_STMT_NUMS extractAllAffectsStar();

  /**
  * Extracts and returns the assignment statement that affects* the modifies line, for example affects*(2,a)
  * @param t_modifiesLine the line that is using the modified variables.
  *
  */
  LIST_OF_AFFECTS_STMTS extractAffectedByStar(STMT_NUM t_modifiesLine); // affects*(2,a)

  /**
  * Extracts and returns the assignment statement that affects* the uses line, for example affects*(a,12)
  * @param t_usesLine the line that is using the modified variables.
  *
  */
  LIST_OF_AFFECTS_STMTS extractAffectsStar(STMT_NUM t_usesLine); // affects*(a,12)

  /**
  * Extracts and returns true if the modifie line affects the uses line, for example affects*(1,12).
  * @param t_modifiesLine the modifies line.
  * @param t_usesLine the uses line.
  *
  */
  BOOLEAN extractIsAffectsStar(STMT_NUM t_modifiesLine, STMT_NUM t_usesLine); // affects*(1,12)

  /**
  * Extracts and find if there are any affects relationship, for example affects*(_,_).
  *
  */
  BOOLEAN extractHasAffectsRelationshipStar(); // affects*(_,_)

  /**
  * Extracts and returns the list of statement that are modifies, for example affects*(a,_)
  *
  */
  LIST_OF_AFFECTS_STMTS extractAffectsAnythingStar();  // affects*(a,_)

  /**
  * Extracts and returns the list of statement that are uses, for example affects*(_,a)
  *
  */
  LIST_OF_AFFECTS_STMTS extractAffectedByAnythingStar(); // affects*(_,a)

  /**
  * Extracts and returns true if there is any affects* relation starting from the uses line, for example affects*(1,_).
  * @param t_modifiesLine the line uses.
  *
  */
  BOOLEAN extractIsAffectsAnythingStar(STMT_NUM t_modifiesLine); // affects*(1,_)

  /**
  * Extracts and returns true if there is any affects* relation ending at the modifies line, for example affects*(_,12).
  * @param t_usesLines the line that is modifying.
  *
  */
  BOOLEAN extractIsAffectedByAnythingStar(STMT_NUM t_usesLines); // affects*(_,12)

private:
  /**
  * A helper method to obtain the list of statements of a procedure from a particular statement number that resides in the procedure.
  * It will return the vector of stmts that belongs to that procedure.
  * @param t_t_stmtNum a statement number that belongs to the target procedure.
  * @return a vector of stmts that belongs to the target procedure.
  */
  LIST_OF_STMT_NUMS getListOfStmtsFromStmtNum(STMT_NUM t_stmtNum);
  MAP_OF_STMT_NUM_TO_LIST_OF_STMT_NUMS appendAffectsList(MAP_OF_STMT_NUM_TO_SET_OF_STMT_NUMS toAdd, MAP_OF_STMT_NUM_TO_LIST_OF_STMT_NUMS result);
};

