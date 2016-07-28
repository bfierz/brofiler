#include <iostream>
#include "TestEngine.h"

using namespace std;

int main(int, char **)
{
	cout << "Starting profiler test." << endl;

	Test::Engine engine;
	cout << "Engine successfully created." << endl;

	cout << "Starting main loop update." << endl;
	/*for(int i = 0; i < 10; ++i) {
		std::cout << '.'; cout.flush();
		engine.Update();
	}*/
	while( engine.Update() ) { cout<<'.'; cout.flush(); }

	std::cout << std::endl;
	return 0;
}
