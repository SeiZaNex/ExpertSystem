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

inline enum elem	_isRules(std::string const &line)
{
  if ("[rules]" == line)
    return RULES;
  return ENULL;
}

inline enum elem	_isFacts(std::string const &line)
{
  if ("[facts]" == line)
    return FACTS;
  return ENULL;
}

inline enum elem	_isQuery(std::string const &line)
{
  if ("[query]" == line)
    return QUERY;
  return ENULL;
}

inline enum elem	_isParseMode(std::string const &line)
{
  enum elem	type = ENULL;

  for (int i = 0; i < 3 && ENULL == type; ++i)
    switch (i)
      {
      case 0:
	type = _isRules(line);
	break;
      case 1:
	type = _isFacts(line);
	break;
      case 2:
	type = _isQuery(line);
	break;
      default:
	break;
      }
  return type;
}

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

inline int	_isOper(std::string &str)
{
  for (int i = 0; i < OPER_SYMBOL; ++i)
    if (oper[i].sym == str)
      return i;
  return -1;
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

void	Parser::_parseDeps(std::string &deps)
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
      _fact.pop();
    }
  //  return _stack;
}

void	Parser::_parseRules(std::string &buff)
{
  std::string	impl;
  std::string	deps;
  //  Rules		*rl;

  if (std::string::npos != buff.find("->"))
    {
      impl = buff.substr(buff.find("->") + 3);
      deps = buff.substr(0, buff.find("->") - 1);

      _parseDeps(deps);
      _parseDeps(impl);
      //std::cout << impl << std::endl;
    }
  else
    std::cerr << "Warning: Invalid rule set: no implication" << std::endl;

  std::list<Values *>::iterator vit;
  std::list<Rules *>::iterator rit;
  Values	*vtp;
  Rules		*rtp;
  enum op	op;

  std::cout << "Facts: " << std::endl;
  for (vit = _facts.begin(); vit != _facts.end(); ++vit)
    {
      vtp = *vit;
      std::cout << vtp->getName() << std::endl;
    }

  std::cout << "Rules: " << std::endl;
  for (rit = _rules.begin(); rit != _rules.end(); ++rit)
    {
      rtp = *rit;
      op = rtp->getOper();
      if (op == NOT)
	std::cout << "NOT: ";
      else if (op == AND)
	std::cout << "AND: ";
      else if (op == OR)
	std::cout << "OR";
      else
	std::cout << "EQU" << std::endl;
      std::cout << rtp->getDep().size() << std::endl;
    }
  return;
}

void	Parser::_parseFacts(std::string &buff)
{
  (void) buff;
  //  std::cout << buff << std::endl;
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
}
