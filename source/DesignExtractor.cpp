#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <unordered_map>
#include <stack>
#include "PKB.h"
#include "DesignExtractor.h"

DesignExtractor::DesignExtractor(PKB* t_pkb) {
  m_pkb = t_pkb;
}

void DesignExtractor::extractRestOfDesignAbstractions() {
  //populate the parent* maps.
  m_pkb->populateParentedByStarMap();
  m_pkb->populateParentStarMap();
}
