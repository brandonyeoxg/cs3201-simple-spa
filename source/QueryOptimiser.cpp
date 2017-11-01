#pragma once

#include "QueryOptimiser.h"

void QueryOptimiser::divideClausesIntoGroups(std::priority_queue<Clause> &t_noSyns, std::priority_queue<std::priority_queue<Clause>> &t_noSelectSyns, std::priority_queue<std::priority_queue<Clause>> &t_hasSelectSyns) {

}

void QueryOptimiser::sortBetweenGroups(std::priority_queue<std::priority_queue<Clause>> &t_groups) {

}

void QueryOptimiser::sortWithinGroups(std::priority_queue<Clause> &t_clauses) {

}