#pragma once
#ifndef DESIGNABSTRACTION_H
#define DESIGNABSTRACTION_H
class DesignAbstraction
{
public:
  enum class DAType { FOLLOWS, FOLLOWS_, PARENT, PARENT_, USES, MODIFIES };
  DesignAbstraction();
  DesignAbstraction(string t_type, Grammar t_g1, Grammar t_g2);
  DAType getType() { return m_type; }
  Grammar getG1() { return m_g1; }
  Grammar getG2() { return m_g2; }

private:
  DAType m_type; /**< type of this Design Abstraction object */
  Grammar m_g1; /**< First Grammar object useed by the Design Abstraction */
  Grammar m_g2; /**< Second Grammar object useed by the Design Abstraction */

};

#endif