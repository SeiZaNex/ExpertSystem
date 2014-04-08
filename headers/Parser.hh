#ifndef		ESIE_PARSER_HH_
#define		ESIE_PARSER_HH_

#include	<list>
#include	<stack>
#include	<string>

#include	"Engine.hh"

#define		OPER_SYMBOL	8

namespace	esie
{
  enum		elem
    {
      RULES = 0,
      FACTS = 1,
      QUERY = 2,
      ENULL = 3
    };

  struct	s_op
  {
    const char	*sym;
    enum op	rule;
    int		param;
  };

  static const struct s_op	oper[OPER_SYMBOL]=
    {
      { "!", NOT, 1 },
      { "~", NOT, 1 },
      { "+", AND, 2 },
      { "^", AND, 2 },
      { "*", OR, 2 },
      { "v", OR, 2 },
      { "V", OR, 2 },
      { "->", EQU, 1 }
    };

  class		Parser
  {
  public:
    Parser(std::string const &file);
    Parser(char * const &file);
    ~Parser();

    void	parse();
  private:
    Parser();

    std::string		_file;
    std::list<Values *>	_facts;
    std::list<Rules *>	_rules;

    std::stack<std::string>	_fact;
    std::stack<Rules *>		_oper;
    std::stack<Rules *>		_stack;

    bool	_isDup(std::string &str);
    Values *	_getDup(std::string &str);

    void	_gatherRules();
    //    std::stack<AObject *>	_parseDeps(std::string &deps);

    void	_parseDeps(std::string &deps);
    void	_parseRules(std::string &buff);
    void	_parseFacts(std::string &buff);
    void	_parseQuery(std::string &buff);

    typedef void (Parser::*fnc)(std::string &);

    const fnc	_func[3]=
      {
	&Parser::_parseRules,
	&Parser::_parseFacts,
	&Parser::_parseQuery
      };
  };
};

#endif		/* !ESIE_PARSER_HH_ */
