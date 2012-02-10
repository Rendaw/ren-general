#include <iostream>
#include <cassert>
#include <array>

#include <ren-general/arrangement.h>

int main(int argc, char **argv)
{
	// Test set
	// TODO
	
	// Test many to one mapper
	{
		std::array<int, 3> Left = {{1, 4, 8}}, Right = {{2, 5, 9}};
		ManyToOneMapper<int, int> Test;
		for (auto &Element : Left) Test.AddA(Element);
		for (auto &Element : Right) Test.AddB(Element);
		Test.RemoveB(Right[2]);
		Test.RemoveA(Left[2]);
		Test.Connect(Left[0], Right[1]);
		Test.Connect(Left[1], Right[1]);
		Test.Connect(Left[1], Right[0]);
		Test.Disconnect(Left[1], Right[0]);
		
		auto FirstMappings = Test.GetBMappings(Right[0]);
		assert(FirstMappings.second.empty());
		
		auto SecondMappings = Test.GetBMappings(Right[1]);
		assert(SecondMappings.second.size() == 2);
	}

	return 0;
}
