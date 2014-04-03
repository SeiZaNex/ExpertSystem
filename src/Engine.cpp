#include	<list>
#include	<string>

#include	"Engine.hh"

using namespace	esie;

AObject::AObject() {}
AObject::~AObject() {}

std::list<AObject *>	AObject::getImpl() const { return this->_impl; }
std::list<AObject *>	AObject::getDep() const { return this->_dep; }

void		AObject::addImpl(AObject *obj) { this->_impl.push_back(obj); }
void		AObject::addDep(AObject *obj) { this->_dep.push_back(obj); }


Rules::Rules(enum op oper) : AObject(), _oper(oper) {}
Rules::~Rules() {}

enum op		Rules::getOper() const { return this->_oper; }


Values::Values(std::string const &name) : AObject(), _name(name), _val(UNKNOWN) {}
Values::~Values() {}

std::string	Values::getName() const { return this->_name; }
enum val	Values::getVal() const { return this->_val; }
void		Values::setVal(enum val value) { this->_val = value; }
