enum StateType 
{
	kBeginState,
	kNormalState,
	kFinishState
};

class State
{
public:
	State();
	State(StateType type, int num);
	StateType getStateType() const;
	int getNumOfState() const;
	bool operator<(const State& s) const;
	bool operator==(const State& s) const;
private:
	StateType m_stateType;
	int m_numOfState;
};