#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <unordered_map>
#include <stack>
#include "DesignExtractor.h"

DesignExtractor::DesignExtractor(PkbWriteOnly* t_pkb) {
  m_pkb = t_pkb;
}

void DesignExtractor::extractRestOfDesignAbstractions() {
  //populate the parent* maps.
  m_pkb->populateParentedByStarMap();
  m_pkb->populateParentStarMap();
  m_pkb->populateAssignTableAbstractions();
}
