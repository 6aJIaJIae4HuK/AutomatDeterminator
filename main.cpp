#include <iostream>

#include "FSM.h"

int main() 
{
	std::string fileName;
	std::cin >> fileName;
	try
	{
		FSM fsm(fileName);
		std::string str;
		std::cin >> str;
		std::pair<bool, int> ans = fsm.parseString(str);
		std::cout << ans.first << ' ' << ans.second << std::endl;
	}
	catch (std::exception e)
	{
		std::cout << e.what() << std::endl;
	}
	return 0;
}