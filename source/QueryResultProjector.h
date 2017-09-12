#pragma once
#ifndef QRP_H
#define QRP_H
#include <string>
#include <list>
#include <vector>

class QueryResultProjector
{
public:
  std::list<std::string> formatResult(std::vector<std::string>);
};

#endif