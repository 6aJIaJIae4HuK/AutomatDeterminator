#include "State.h"

State::State() : m_stateType(kBeginState), m_numOfState(0)
{
}

State::State(StateType type, int num) : m_stateType(type), m_numOfState(num)
{
}

StateType State::getStateType() const
{
	return m_stateType;
}

int State::getNumOfState() const
{
	return m_numOfState;
}

bool State::operator<(const State& state) const
{
	if (this->m_stateType != state.getStateType())
		return this->m_stateType < state.getStateType();
	if (this->m_numOfState != state.getNumOfState())
		return this->m_numOfState < state.getNumOfState();
	return false;
}

bool State::operator==(const State& state) const
{
	return this->m_numOfState == state.getNumOfState() && this->m_stateType == state.getStateType();
}