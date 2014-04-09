#include	<list>
#include	<stack>
#include	<vector>
#include	<string>
#include	<fstream>
#include	<sstream>
#include	<iostream>
#include	<algorithm>

#include	"Parser.hh"

using namespace		esie;

Parser::Parser(std::string const &file) : _file(file) {}
Parser::Parser(char * const &file) : _file(file) {}
Parser::Parser() {}
Parser::~Parser() {}

bool		Parser::_isDup(std::string &str)
{
  return std::any_of(_facts.begin(), _facts.end(), [str](Values *i){ return (i->getName() == str); });
}

Values *	Parser::_getDup(std::string &str)
{
  std::list<Values *>::iterator it;
  Values			*ptr;

  if (_isDup(str))
    for (it = _facts.begin(); it != _facts.end(); ++it)
      {
	ptr = *it;
	if (ptr->getName() == str)
	  return ptr;
      }
  return NULL;
}

void	Parser::_gatherRules()
{
  Rules		*rl;

  while (!_oper.empty())
    {
      rl = _oper.top();
      _oper.pop();
      if (!_isDup(_fact.top()))
	this->_facts.push_back(new Values(_fact.top()));
      rl->addDep(_getDup(_fact.top()));
      _fact.pop();
      if (_stack.empty())
	{
	  if (!_isDup(_fact.top()))
	    this->_facts.push_back(new Values(_fact.top()));
	  rl->addDep(_getDup(_fact.top()));
	  _fact.pop();
	}
      else
	rl->addDep(_stack.top());
      _stack.push(rl);
    }
}

AObject *	Parser::_parseDeps(std::string &deps)
{
  std::stringstream	ss(deps);
  std::string		str;
  int			ops;

  while (ss.good())
    {
      ss >> str;
      ops = _isOper(str);
      if (-1 != ops)
	_oper.push(new Rules(oper[ops].rule));
      else
	_fact.push(str);
    }
  _gatherRules();
  while (!_fact.empty())
    {
      if (!_isDup(_fact.top()))
	this->_facts.push_back(new Values(_fact.top()));
      this->_stack.push(_getDup(_fact.top()));
      _fact.pop();
    }
  return _stack.top();
}

void	Parser::_printFacts()
{
  std::list<Values *>::iterator vit;
  Values			*vtp;
  enum val			val;

  for (vit = _facts.begin(); vit != _facts.end(); ++vit)
    {
      vtp = *vit;
      std::cout << vtp->getName() << ": ";

      val = vtp->getVal();
      if (val == TRUE)
	std::cout << "TRUE";
      else if (val == FALSE)
	std::cout << "FALSE";
      else
	std::cout << "UNKNOWN";
      if (!vtp->getDep().empty())
	{
	  // Values	*val;
	  // Rules	*rul;
	  std::cout << " => Dependencies";
	  std::cout << " of length: " << vtp->getDep().size();

	}
      std::cout << std::endl;
    }
}

void	Parser::_parseRules(std::string &buff)
{
  std::string	impl;
  std::string	deps;
  AObject	*obj;
  std::string	equ = "->";

  if (std::string::npos != buff.find(equ))
    {
      impl = buff.substr(buff.find(equ) + 3);
      deps = buff.substr(0, buff.find(equ) - 1);

      obj = _parseDeps(impl);
      obj->addDep(_parseDeps(deps));
    }
  else
    std::cerr << "Warning: Invalid rule set: no implication" << std::endl;
  return;
}

void	Parser::_parseFacts(std::string &buff)
{
  std::stringstream	ss(buff);
  std::string		str;
  bool			flag = true;
  Values		*val;

  while (ss.good())
    {
      ss >> str;
      if (std::string::npos != str.find("!"))
	{
	  str = str.substr(str.find("!"));
	  flag = false;
	}
      if (std::string::npos != str.find(","))
	str = str.substr(0, str.find(","));
      val = _getDup(str);
      if (NULL != val)
	val->setVal((!flag) ? FALSE : TRUE);
      else
	{
	  std::cerr << "Warning: Unknown use of a fact in the [facts]: ";
	  std::cerr << str << std::endl;
	}
    }
}

void	Parser::_parseQuery(std::string &buff)
{
  (void) buff;
  //std::cout << buff << std::endl;
}

void		Parser::parse()
{
  std::ifstream		ifile(this->_file.c_str());
  std::string		buff;
  static enum elem	type = ENULL;

  if (ifile.is_open())
    {
      while (ifile.good())
	{
	  std::getline(ifile, buff);
	  if (0 != buff.size())
	    {
	      if (ENULL != _isParseMode(buff))
		{
		  type = _isParseMode(buff);
		  continue;
		}
	      if (type != ENULL)
		((*this).*_func[type])(buff);
	    }
	}
      ifile.close();
    }
  std::cout << "Facts: " << std::endl;
  _printFacts();
}
