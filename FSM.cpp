#include <fstream>
#include <exception>
#include <vector>
#include <iostream>
#include <set>
#include <queue>
#include <map>

#include "FSM.h"

FSM::FSM(const std::string& fileName)
{
	std::ifstream input(fileName);
	std::string str;
	while (std::getline(input, str))
	{
		try
		{
			std::pair<std::pair<State, State>, char> transition = getTransition(str);
			m_graph.insert(
				std::make_pair(std::make_pair(transition.first.first, transition.second), 
							   transition.first.second));
		}
		catch (std::exception e)
		{
			throw e;
		}
	}
	bool deterministic = isDeterministic();
	std::cout << (deterministic ? "Deterministic" : "Nondeterministic") << std::endl;
	if (!deterministic)
	{
		determine();
	}
}

std::pair<std::pair<State, State>, char> FSM::getTransition(const std::string& str)
{
	auto it = str.begin();
	StateType firstStateType;
	StateType secondStateType;
	int firstStateNum;
	int secondStateNum;
	char edge;
	switch (*it)
	{
	case 'q':
		firstStateType = kNormalState;
		break;
	case 'f':
		firstStateType = kFinishState;
		break;
	default:
		throw std::exception(("Illegal transition: " + str).c_str());
		break;
	}
	it++;

	if (it == str.end())
		throw std::exception("Unexpected end of line!!!");
	auto it1 = it;
	while (it != str.end() && *it >= '0' && *it <= '9')
		it++;
	firstStateNum = std::stoi(std::string(it1, it));

	if (it == str.end() || *it != ',')
		throw std::exception("Expected comma!");
	it++;

	if (it == str.end())
		throw std::exception("Unexpected end of line!!!");
	edge = *it;
	it++;
	
	if (it == str.end() || *it != '=')
		throw std::exception("Expected equal sign!");
	it++;


	if (it == str.end())
		throw std::exception("Unexpected end of line!!!");
	switch (*it)
	{
	case 'q':
		secondStateType = kNormalState;
		break;
	case 'f':
		secondStateType = kFinishState;
		break;
	default:
		throw std::exception(("Illegal transition: " + str).c_str());
		break;
	}
	it++;

	if (it == str.end())
		throw std::exception("Unexpected end of line!!!");
	it1 = it;
	while (it != str.end() && *it >= '0' && *it <= '9')
		it++;
	secondStateNum = std::stoi(std::string(it1, it));

	if (it != str.end())
		throw std::exception("Too long line!!!");

	if (firstStateType == kNormalState && firstStateNum == 0)
		firstStateType = kBeginState;
	
	if (secondStateType == kNormalState && secondStateNum == 0)
		secondStateType = kBeginState;

	State firstState(firstStateType, firstStateNum);
	State secondState(secondStateType, secondStateNum);

	return std::make_pair(std::make_pair(firstState, secondState), edge);
}

State FSM::findFirstState() const
{
	State res;
	for (auto it = m_graph.begin(); it != m_graph.end(); it++)
	{
		if (it->first.first.getStateType() == kBeginState)
		{
			res = it->first.first;
			break;
		}
	}
	return res;
}

std::pair<bool, int> FSM::parseString(const std::string& str)
{
	State curState = findFirstState();
	for (auto it = str.begin(); it != str.end(); it++)
	{
		std::pair<State, char> transitionToFind = std::make_pair(curState, *it);
		auto it1 = m_graph.find(transitionToFind);
		if (it1 != m_graph.end())
		{
			curState = it1->second;
		}
		else
		{
			return std::make_pair(false, -1);
		}
	}
	if (curState.getStateType() == kFinishState)
		return std::make_pair(true, curState.getNumOfState());
	else
		return std::make_pair(false, -1);
}

bool FSM::isDeterministic() const
{
	bool res = true;
	for (auto it = m_graph.begin(); it != m_graph.end() && res; it++)
	{
		if (m_graph.count(it->first) > 1)
			res = false;
	}
	return res;
}

void FSM::determine()
{
	std::queue<std::set<State>> q;
	std::multimap<std::pair<State, char>, State> newGraph;
	std::set<State> state;
	std::set<std::set<State>> newStates;
	std::map<std::set<State>, State> fromOldStatesToNewState;
	State start = findFirstState();
	state.insert(start);
	fromOldStatesToNewState[state] = start;
	q.push(state);
	int normalCnt = 1, finishCnt = 0;
	while (!q.empty())
	{
		std::set<State> p = q.front();
		q.pop();
		for (char c = CHAR_MIN; c <= CHAR_MAX; c++)
		{
			bool isFinish = false;
			std::set<State> qd;
			for (auto it = p.begin(); it != p.end(); it++)
			{
				auto its = m_graph.equal_range(std::make_pair(*it, c));
				for (auto it1 = its.first; it1 != its.second; it1++)
				{
					if (it1->second.getStateType() == kFinishState)
						isFinish = true;
					qd.insert(it1->second);
				}
			}
			if (fromOldStatesToNewState.find(qd) == fromOldStatesToNewState.end())
			{
				if (isFinish)
					fromOldStatesToNewState[qd] = State(kFinishState, finishCnt++);
				else
					fromOldStatesToNewState[qd] = State(kNormalState, normalCnt++);
				q.push(qd);
			}
			if (!qd.empty())
				newGraph.insert(std::make_pair(std::make_pair(fromOldStatesToNewState[p], c), fromOldStatesToNewState[qd]));
			if (c == CHAR_MAX)
				break;
		}
	}
	m_graph = newGraph;
	for (auto it = m_graph.begin(); it != m_graph.end(); it++)
	{
		std::cout << getString(*it) << std::endl;
	}
}


std::string FSM::getString(const std::pair<std::pair<State, char>, State>& transition) const
{
	std::string res = "";
	const State& firstState = transition.first.first;
	const State& secondState = transition.second;
	const char& symb = transition.first.second;
	if (firstState.getStateType() == kFinishState)
		res += 'f';
	else
		res += 'q';
	res += std::to_string(firstState.getNumOfState());
	res += ',';
	res += symb;
	res += '=';
	if (secondState.getStateType() == kFinishState)
		res += 'f';
	else
		res += 'q';
	res += std::to_string(secondState.getNumOfState());
	return res;
}