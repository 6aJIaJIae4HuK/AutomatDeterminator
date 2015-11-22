#include <string>
#include <map>

#include "State.h"

class FSM
{
public:

	explicit FSM(const std::string& fileName);
	std::pair<bool, int> parseString(const std::string& str);
private:
	std::multimap<std::pair<State, char>, State> m_graph;
	std::pair<std::pair<State, State>, char> getTransition(const std::string& str);
	State findFirstState() const;
	bool isDeterministic() const;
	void determine();
	std::string getString(const std::pair<std::pair<State, char>, State>& transition) const;
};