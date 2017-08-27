#pragma once

#include "GNode.h"

class Cfg {
public:
  void createNode(GNode node);
private:
  GNode *m_head;
};