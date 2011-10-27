#ifndef arrangement_h
#define arrangement_h

#include <cassert>
#include <set>
#include <vector>
#include <algorithm>

template <typename SetType> class Set : public std::set<SetType>
{
	public:
		Set<SetType> &And(Set<SetType> const &Object)
		{
			insert(Object.begin(), Object.end());
			return *this;
		}
		
		Set<SetType> &And(SetType const &Object)
		{
			insert(Object);
			return *this;
		}
};

template <typename FromType, typename ToType> class ManyToOneMapper
{
	public:
		void AddStart(FromType &NewStart)
		{
			assert(!HasStart(NewStart));
			Starts.push_back(&NewStart);
		}
		
		void AddEnd(ToType &NewStop)
		{
			assert(!HasStop(NewStop));
			Stops.push_back(&NewStop);
			Mappings.push_back(std::vector<FromType *>());
		}
		
		void RemoveStart(FromType &OldStart)
		{
			assert(HasStart(OldStart));
			
			for (unsigned int CurrentMapping = 0; CurrentMapping < Mappings.size(); CurrentMapping++)
			{
				typename std::vector<FromType *>::iterator StartPosition = std::find(Mappings[CurrentMapping].begin(), Mappings[CurrentMapping].end(), &OldStart);
				if (StartPosition != Mappings[CurrentMapping].end())
				{
					Mappings[CurrentMapping].erase(StartPosition);
					assert(std::find(Mappings[CurrentMapping].begin(), Mappings[CurrentMapping].end(), &OldStart) == Mappings[CurrentMapping].end());
				}
			}
			
			Starts.erase(std::find(Starts.begin(), Starts.end(), &OldStart));
		}
		
		void RemoveEnd(ToType &OldStop)
		{
			assert(HasStop(OldStop));
			
			typename std::vector<ToType *>::iterator OldStopPosition = std::find(Stops.begin(), Stops.end(), &OldStop);
			Mappings.erase(Mappings.begin() + (OldStopPosition - Stops.begin()));
			Stops.erase(OldStopPosition);
		}
		
		unsigned int GetMappingCount(void) const
			{ return Mappings.size(); }
			
		std::vector<FromType *> const &GetMappings(unsigned int Index, ToType *&ToOutput) const
		{
			assert(Index < Stops.size());
			ToOutput = Stops[Index];
			return Mappings[Index];
		}
		
		void Connect(FromType &Start, ToType &Stop)
		{
			assert(HasStart(Start));
			assert(HasStop(Stop));
			unsigned int StopIndex = std::find(Stops.begin(), Stops.end(), &Stop) - Stops.begin();
			assert(std::find(Mappings[StopIndex].begin(), Mappings[StopIndex].end(), &Start) == Mappings[StopIndex].end());
			Mappings[StopIndex].push_back(&Start);
		}
		
		void Disconnect(FromType &Start, ToType &Stop)
		{
			assert(HasStart(Start));
			assert(HasStop(Stop));
			unsigned int StopIndex = std::find(Stops.begin(), Stops.end(), &Stop) - Stops.begin();
			typename std::vector<FromType *>::iterator StartPosition = std::find(Mappings[StopIndex].begin(), Mappings[StopIndex].end(), &Start);
			assert(StartPosition != Mappings[StopIndex].end());
			Mappings[StopIndex].erase(StartPosition);
		}
		
	private:
		std::vector<FromType *> Starts;
		std::vector<ToType *> Stops;
		std::vector<std::vector<FromType *> > Mappings;
	
		bool HasStart(FromType const &Start) const
			{ return std::find(Starts.begin(), Starts.end(), &Start) != Starts.end(); }
		
		bool HasStop(ToType const &Stop) const
			{ return std::find(Stops.begin(), Stops.end(), &Stop) != Stops.end(); }
};

#endif
