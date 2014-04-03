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
    ~AObject();

    std::list<AObject *>	getImpl() const;
    std::list<AObject *>	getDep() const;

    void			addImpl(AObject *obj);
    void			addDep(AObject *ojb);

  protected:
    std::list<AObject *>	_impl;
    std::list<AObject *>	_dep;

  private:
  };

  enum	op
    {
      NOT,
      AND,
      OR,
      EQU
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
      FALSE,
      TRUE,
      UNKNOWN
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
