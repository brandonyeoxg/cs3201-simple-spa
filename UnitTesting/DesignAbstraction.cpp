#include "stdafx.h"
#include "DesignAbstraction.h"
#include <string>
#include "Grammar.h"

using namespace std;

/**
*  A class representing the multiple types of Design Abstractions used in the queries. It stores the relevant Grammar objects that are used by the Design Abstraction.
*/
class DesignAbstraction {

public:
  /** Represents the type of Design Abstraction.
  *  An enum type representing the type of Design Abstraction to carry out on the Grammar objects. An "_" is used in place of a "*" for Design Abstractions that require checking more than one line.
  */
  enum class DAType { FOLLOWS, FOLLOWS_, PARENT, PARENT_, USES, MODIFIES };

  DesignAbstraction() {}

  /**
  * A constructor.
  * Constructs a new Design Abstraction object with a DAType corresponding to the specified initial capacity and a string with the specified t_name.
  */
  DesignAbstraction(string t_type, Grammar t_g1, Grammar t_g2);

  /**
  * A Getter that returns the type of this Grammar object
  * Returns the type of this Grammar object. The returned GType is an enum type that identifies the Grammar as either a Statement or a Variable, and is able to further specify a type of Statement or Variable.
  */
  DAType getType() { return m_type; }

  /**
  * A Getter that returns the first Grammar object of the Dersign Abstraction.
  * Returns the first Grammar object of the Dersign Abstraction. The returned Grammar object is the first one used by the Design Abstraction. e.g. Follows(m_g1, 2)
  */
  Grammar getG1() { return m_g1; }

  /**
  * A Getter that returns the second Grammar object of the Dersign Abstraction
  * Returns the second Grammar object of the Dersign Abstraction. The returned Grammar object is the second one used by the Design Abstraction. e.g. Follows(1, m_g2)
  */
  Grammar getG2() { return m_g2; }

private:
  DAType m_type; /**< type of this Design Abstraction object */
  Grammar m_g1; /**< First Grammar object useed by the Design Abstraction */
  Grammar m_g2; /**< Second Grammar object useed by the Design Abstraction */
};

DesignAbstraction::DesignAbstraction(string t_type, Grammar t_g1, Grammar t_g2) {
  if (t_type.compare("Follows") == 0) {
    m_type = DAType::FOLLOWS;
  }
  else if (t_type.compare("Follows*") == 0) {
    m_type = DAType::FOLLOWS_;
  }
  else if (t_type.compare("Parent") == 0) {
    m_type = DAType::PARENT;
  }
  else if (t_type.compare("Parent*") == 0) {
    m_type = DAType::PARENT_;
  }
  else if (t_type.compare("Uses") == 0) {
    m_type = DAType::USES;
  }
  else if (t_type.compare("Modifies") == 0) {
    m_type = DAType::MODIFIES;
  }
  m_g1 = t_g1;
  m_g2 = t_g2;
}