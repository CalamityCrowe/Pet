#include <iostream>

using std::cout;
using std::cin;
using std::endl;
using std::string;

struct TAMA // this keeps all the relevant information about the pet in one place and easier to access this in diffrent functions when needed
{
	// these will give a warning of not initialized but in this case it doesn't particularly matter  
	string Name;
	int Levels[4];// 0 = Hunger, 1 = Hydration, 2 = Happiness, 3 = Tiredness
	int HAP;
	bool isDead, PassedOut, isAsleep; // this should probably be an array but this is to make it easier to 
	double duration, Start;
};
