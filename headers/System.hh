#ifndef ESIE_SYSTEM_HH_
#define ESIE_SYSTEM_HH_

#include	<list>
#include	<string>
#include	<iostream>
#include	<algorithm>

#include	"Engine.hh"
#include	"Parser.hh"

inline bool		onAll(std::list<esie::val> &l, esie::val x);
inline bool		onAny(std::list<esie::val> &l, esie::val x);
inline esie::val	valSet(esie::Values *&ptr, esie::val val);

esie::val	flagVal(esie::Values *&val, std::list<esie::val> &flag);
esie::val	flagRule(esie::Rules *rule, std::list<esie::val> &flag);
esie::val	flagOut(esie::AObject *ptr, std::list<esie::val> &flag);

esie::val	askUserAbout(esie::Values *ptr);
esie::val	recDep(esie::AObject *ptr);

#endif /* !ESIE_SYSTEM_HH_ */
