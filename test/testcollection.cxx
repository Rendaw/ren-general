#include <iostream>
#include <cassert>

#include <ren-general/collection.h>
#include <ren-general/string.h>

int main(int argc, char **argv)
{
	{
		std::vector<String> Test = { "A", "man", "a", "plan", "a", "c" };
		StringStream Concatenation;
		
		auto Iterable1 = StandardIterable<decltype(Test)>(Test);
		for (String &Element : Iterable1) Concatenation << Element;
		
		auto Iterable2 = StandardReverseIterable<decltype(Test)>(Test);
		for (String &Element : Iterable2) Concatenation << Element;
		
		std::cout << "Concatenation using all iterators is: \"" << Concatenation.str() << "\"." << std::endl;
		assert(Concatenation.str() == "AmanaplanaccaplanamanA");
	}
	
	{
		class Counter
		{
			public:
				int &Count;
				int Value;
				Counter(int Value, int &Count) : Count(Count), Value(Value) { Count += 1; }
				Counter(Counter const &Other) : Count(Other.Count), Value(Other.Value) { Count += 1; }
		};
		int Count = 0;
		std::vector<Counter> Test = { {1, Count}, {2, Count}, {3, Count}, {4, Count}, {5, Count} };
		
		int Sum = 0;

		class Iterable3Type
		{
			private:
				std::vector<Counter> &Base;
			public:
				Iterable3Type(std::vector<Counter> &Base) : Base(Base) {}
				unsigned int GetCount(void) { return Base.size(); }
				Counter &GetValue(unsigned int const &Index) { return Base.at(Index); }
		} Iterable3Base(Test);
		IndexIterable<Iterable3Type> Iterable3(Iterable3Base);
		for (Counter &Element : Iterable3) Sum += Element.Value;
		for (Counter &Element : Iterable3) Sum += Element.Value;
		for (Counter &Element : Iterable3) Sum += Element.Value;
		for (Counter &Element : Iterable3) Sum += Element.Value;
		std::cout << "Sum is " << Sum << ", count is " << Count << std::endl;
		assert(Sum == 60); // 15 per iteration
		assert(Count == 10); // Initialized twice because of uniform init and initializer lists?
	}
	
	{
		std::vector<int> Test = { 1, 2, 3, 4 };
		VectorIteratorRange<int> Iterable(Test.begin() + 1, Test.end());
		int Sum = 0;
		for (auto &Element : Iterable) Sum += Element;
		assert(Sum == 9);
	}
	
	return 0;
}
