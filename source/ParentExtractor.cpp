#include "ParentExtractor.h"
#include "ParentTable.h"

void ParentExtractor::extractDesign() {
  populateParentedByStarMap();
  populateParentStarMap();
  populateParentMatrix();
}

void ParentExtractor::populateParentedByStarMap() {
  ParentTable* parentTable = m_pkb->getParentTable();
  auto parentMap = parentTable->getParentMap();
  for (auto mapItr = parentMap.begin(); mapItr != parentMap.end(); mapItr++) {
    int baseStmtNo = mapItr->first;
    LIST_OF_STMT_NUMS stmtsOfParentedBy;
    stmtsOfParentedBy.push_back(mapItr->second);
    auto nextParentLink = parentTable->getParentMap().find(mapItr->second);
    while (nextParentLink != parentTable->getParentMap().end()) {
      stmtsOfParentedBy.push_back(nextParentLink->second);
      nextParentLink = parentTable->getParentMap().find(nextParentLink->second);
    }
    parentTable->getParentedByStarMap().insert({ baseStmtNo, stmtsOfParentedBy });
  }
}

void ParentExtractor::populateParentStarMap() {
  ParentTable* parentTable = m_pkb->getParentTable();
  for (auto it = parentTable->getChildMap().begin(); it != parentTable->getChildMap().end(); ++it) {
    int parent = it->first;
    LIST_OF_STMT_NUMS children = it->second;
    parentTable->getParentStarMap().emplace(parent, children);
    LIST_OF_STMT_NUMS childrenStar = children;
    for (int i = 0; i < childrenStar.size(); i++) {
      //for every child, if it can be found in the map, append all from it's mapped vector to children
      auto iterator = parentTable->getChildMap().find(childrenStar[i]);
      if (iterator != parentTable->getChildMap().end()) {
        LIST_OF_STMT_NUMS toBeAppended = iterator->second;
        childrenStar.reserve(childrenStar.size() + toBeAppended.size());
        childrenStar.insert(childrenStar.end(), toBeAppended.begin(), toBeAppended.end());
      }
    }
    parentTable->getParentStarMap()[parent] = childrenStar;
  }
}

void ParentExtractor::populateParentMatrix() {
  TOTAL_NUMBER_OF_STMTS number_of_stmts;
  number_of_stmts = m_pkb->getStatementTable()->getNumberOfStatements();
  ParentTable* parentTable = m_pkb->getParentTable();
  parentTable->populateParentMatrix(number_of_stmts);
}