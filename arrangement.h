#ifndef arrangement_h
#define arrangement_h

#include <cassert>
#include <set>
#include <vector>
#include <algorithm>

template <typename SetType> class Set : public std::set<SetType>
{
	public:
		Set(void) {}
		
		Set(SetType const &Element)
			{ insert(Element); }

		Set(std::initializer_list<SetType> Elements) : std::set<SetType>(Elements) {}
		
		Set<SetType> &And(Set<SetType> const &Object)
		{
			insert(Object.begin(), Object.end());
			return *this;
		}
		
		Set<SetType> &And(SetType const &Element)
		{
			insert(Element);
			return *this;
		}
		
		Set<SetType> &operator=(SetType const &Element)
		{
			std::set<SetType>::clear();
			insert(Element);
			return *this;
		}

		bool Contains(SetType const &Element) const
			{ return find(Element) != std::set<SetType>::end(); }

		bool operator[](SetType const &Element) const
			{ return Contains(Element); }
};

// Only has queries for to-side, but reverse A and B to if the opposite queries are more important.
template <typename AType, typename BType> class ManyToOneMapper
{
	private:
		typedef std::pair<BType *, std::vector<AType *> > Pair;
		std::vector<AType *> A; // Used only for debug checks
		std::vector<Pair> Mappings;
	
		bool HasA(AType const &TestA) const
			{ return std::find(A.begin(), A.end(), &TestA) != A.end(); }
		
		bool HasB(BType const &TestB) const
		{ 
			return std::find_if(Mappings.begin(), Mappings.end(), 
				[&TestB](Pair const &Element) { return Element.first == &TestB; }) != Mappings.end();
		}
		
	public:
		// Construction
		void AddA(AType &NewA)
		{
			assert(!HasA(NewA));
			A.push_back(&NewA);
		}
		
		void AddB(BType &NewB)
		{
			assert(!HasB(NewB));
			Mappings.push_back(Pair(&NewB, std::vector<AType *>()));
		}
		
		void RemoveA(AType &Target)
		{
			assert(HasA(Target));
			
			for (auto &CurrentMapping : Mappings)
			{
				auto APosition = std::find(CurrentMapping.second.begin(), CurrentMapping.second.end(), &Target);
				if (APosition == CurrentMapping.second.end())
					continue;
				
				CurrentMapping.second.erase(APosition);
				
				assert(std::find(CurrentMapping.second.begin(), CurrentMapping.second.end(), &Target) == CurrentMapping.second.end());
			}
			
			A.erase(std::find(A.begin(), A.end(), &Target));
		}
		
		void RemoveB(BType &Target)
		{
			assert(HasB(Target));
			auto TargetPosition = std::find_if(Mappings.begin(), Mappings.end(),
				[&Target](Pair const &Element) { return Element.first == &Target; });
			Mappings.erase(TargetPosition);
		}
		
		void Disconnect(void)
		{
			for (auto &CurrentMapping : Mappings)
				CurrentMapping.second.clear();
		}
		
		void Connect(AType &A, BType &B)
		{
			assert(HasA(A));
			assert(HasB(B));
			auto BMappings = std::find_if(Mappings.begin(), Mappings.end(), 
				[&B](Pair const &Element) { return Element.first == &B; });
			assert(std::find(BMappings->second.begin(), BMappings->second.end(), &A) == BMappings->second.end());
			BMappings->second.push_back(&A);
		}
		
		void Disconnect(AType &A, BType &B)
		{
			assert(HasA(A));
			assert(HasB(B));
			auto BMappings = std::find_if(Mappings.begin(), Mappings.end(),
				[&B](Pair const &Element) { return Element.first == &B; });
			auto APosition = std::find(BMappings->second.begin(), BMappings->second.end(), &A);
			assert(APosition != BMappings->second.end());
			BMappings->second.erase(APosition);
		}
		
		// Access - for range based for.
		// Returns pairs of (To, vector of all connected froms)
		decltype(Mappings.begin()) begin(void) { return Mappings.begin(); }
		decltype(Mappings.end()) end(void) { return Mappings.end(); }
		
		Pair const &GetBMappings(BType const &Target)
		{
			assert(HasB(Target));
			return *std::find_if(Mappings.begin(), Mappings.end(),
				[&Target](Pair const &Element) { return Element.first == &Target; });
		}
};

#endif
