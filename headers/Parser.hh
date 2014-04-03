#ifndef		ESIE_PARSER_HH_
#define		ESIE_PARSER_HH_

#include	<list>
#include	<string>

#include	"Engine.hh"

namespace	esie
{
  enum		elem
    {
      RULES = 0,
      FACTS = 1,
      QUERY = 2,
      ENULL = 3
    };

  class		Parser
  {
  public:
    Parser(std::string const & file);
    Parser(char * const & file);
    ~Parser();

    void	parse();
  private:
    Parser();

    std::string		_file;
    std::list<Values *>	_atoms;

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
