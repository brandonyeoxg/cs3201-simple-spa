#pragma once
#ifndef QRP_H
#define QRP_H
#include <string>
#include <list>

class QueryResultProjector
{
public:
  void formatResult();
  std::string printResult(std::list<std::string>);
};

#endif