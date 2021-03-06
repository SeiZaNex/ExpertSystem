#ifndef		ESIE_ENGINE_HH_
#define		ESIE_ENGINE_HH_

#include	<list>
#include	<string>

namespace	esie
{
  class		AObject
  {
  public:
    AObject();
    virtual ~AObject();

    std::list<AObject *>	getDep() const;

    void			addDep(AObject *obj);

  protected:
    std::list<AObject *>	_dep;
  };

  enum	op
    {
      NOT = 1 << 0,
      AND = 1 << 1,
      OR = 1 << 2,
      EQU = 1 << 3
    };

  class		Rules : public AObject
  {
  public:
    Rules(enum op oper);
    ~Rules();

    enum op		getOper() const;

  private:
    enum op		_oper;
  };

  enum	val
    {
      FALSE = 1 << 0,
      TRUE = 1 << 1,
      UNKNOWN = FALSE | TRUE
    };

  class		Values : public AObject
  {
  public:
    Values(std::string const &name);
    ~Values();

    std::string		getName() const;
    enum val		getVal() const;

    void		setVal(enum val value);

  private:
    std::string		_name;
    enum val		_val;
  };
};

#endif		/* !ESIE_ENGINE_HH_ */
