#include "ParentExtractor.h"
#include "ParentTable.h"

void ParentExtractor::extractDesign() {
  populateParentedByStarMap();
  populateParentStarMap();
}

void ParentExtractor::populateParentedByStarMap() {
  ParentTable* parentTable = m_pkb->getParentTable();
  auto parentMap = parentTable->getParentMap();
  std::unordered_map<STMT_NUM, std::list<LIST_OF_STMT_NUMS>> parentStarMap;
  for (auto mapItr = parentMap.begin(); mapItr != parentMap.end(); mapItr++) {
    int baseStmtNo = mapItr->first;
    std::vector<int> stmtsOfParentedBy;
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
    std::vector<int> children = it->second;
    parentTable->getParentStarMap().emplace(parent, children);
    std::vector<int> childrenStar = children;
    for (int i = 0; i < childrenStar.size(); i++) {
      //for every child, if it can be found in the map, append all from it's mapped vector to children
      auto iterator = parentTable->getChildMap().find(childrenStar[i]);
      if (iterator != parentTable->getChildMap().end()) {
        std::vector<int> toBeAppended = iterator->second;
        childrenStar.reserve(childrenStar.size() + toBeAppended.size());
        childrenStar.insert(childrenStar.end(), toBeAppended.begin(), toBeAppended.end());
      }
    }
    parentTable->getParentStarMap()[parent] = childrenStar;
  }
}