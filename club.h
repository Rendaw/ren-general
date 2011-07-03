#ifndef club_h
#define club_h

#include <cstdlib>
#include <list>
#include <cassert>

/*
The club manages objects but not their lifespans.  It simple dereferences them when they are dead.

Registering a potential member with the club causes the club to pass a membership to the
new member.  When the member dies, it deletes its membership, thereby leaving the club.

The lifespan of a club must be greater than that of its members.
*/

template <class MemberType> class Club;

// This is an indicator of membership in a club.  When it is deleted, it
// removes its membership from the owning club.
class Membership
{
	public:
		Membership(bool &MemberStatus) : Active(MemberStatus) {}
		~Membership(void) { Active = false; }
	private: 
		bool &Active;
};

// This is the type of object managed by clubs.  They handle
// keeping track of membership in the club.
template <class MemberType> class Member
{
	public:
		virtual ~Member(void) {}
	protected:
		friend class Club<MemberType>;
		virtual void Join(Membership *NewMembership) = 0;
};

// This class maintains a collection of members, which may remove themselves
// from the club. Old club members may be pruned together, or, dependant on the
// implementation, may be pruned implicitly at some other time.
template <class MemberType> class Club
{
	public:
		virtual ~Club(void)
		{
			// Check that all members have already left.  The rest must be
			// leftover records ready to be cleaned.
			for (typename std::list<std::pair<bool, MemberType *> *>::iterator CurrentMember = 
				Members.begin(); CurrentMember != Members.end(); CurrentMember++)
			{
				assert(!(*CurrentMember)->first);
				delete *CurrentMember;
			}
		};

		void Register(MemberType *Inductee)
		{
			// Add a record for the member
			Members.push_back(new std::pair<bool, MemberType *>(true, Inductee));
			Inductee->Join(new Membership(Members.back()->first));
		}
		
		void Clean(void)
		{
			// Go through the membership list, deleting stale records.
			typename std::list<std::pair<bool, MemberType *> *>::iterator DeleteMember;
			for (typename std::list<std::pair<bool, MemberType *> *>::iterator CurrentMember = 
				Members.begin(); CurrentMember != Members.end();)
			{
				if ((*CurrentMember)->first) CurrentMember++;
				else
				{
					// This member has left the club, so clean up
					// the record.
					delete *CurrentMember;

					DeleteMember = CurrentMember;
					CurrentMember++;
					Members.erase(DeleteMember);
				}
			}
		}

		MemberType *Front(void)
		{
			//if (Members.empty()) return;
			if (Members.front()->first == false) return NULL;
			return Members.front()->second;
		}
		
		MemberType *Back(void)
		{
			//if (Members.empty()) return;
			if (Members.back()->first == false) return NULL;
			return Members.back()->second;
		}
		
		typedef typename std::list<std::pair<bool, MemberType *> *>::iterator Iterator;
		
		Iterator Begin(void) { return Members.begin(); }
		Iterator End(void) { return Members.end(); }
		bool Empty(void) { return Members.empty(); }
		
	protected:
		std::list<std::pair<bool, MemberType *> *> Members;
};

// ========================================================================
// Some implementations of things

// This club type provides an update function, which handles cleaning and performing
// an undefined UpdateMember function on each member.  The record cleaning is included
// in the update step.
template <class MemberType> class ActiveClub : public Club<MemberType>
{
	public:
		virtual ~ActiveClub(void) {}
		
		virtual void Update(void)
		{
			// Update all the members, while cleaning the membership list
			// of stale records.
			typename std::list<std::pair<bool, MemberType *> *>::iterator DeleteMember;
			for (typename std::list<std::pair<bool, MemberType *> *>::iterator CurrentMember = 
				Club<MemberType>::Members.begin(); CurrentMember != Club<MemberType>::Members.end();)
			{
				if ((*CurrentMember)->first)
				{
					// THe member is still active them.  Update
					// and advance.
					UpdateMember((*CurrentMember)->second);

					CurrentMember++;
				}
				else
				{
					// This member has left the club, so clean up
					// the record.
					delete *CurrentMember;

					DeleteMember = CurrentMember;
					CurrentMember++;
					Club<MemberType>::Members.erase(DeleteMember);
				}
			}
		}

	protected:
		virtual void UpdateMember(MemberType *CurrentMember) = 0;
};

// The simple member is a simple one-club member.
template <class MemberType> class SimpleMember : public Member<MemberType>
{
	public: 
		SimpleMember(void) : SingleMembership(NULL) {}
		~SimpleMember(void) { delete SingleMembership; }
		
	protected:
		friend class Club<MemberType>;
		void Join(Membership *NewMembership) 
			{ assert(SingleMembership == NULL); SingleMembership = NewMembership; }
		
	private: 
		Membership *SingleMembership;
};

// The multi-member can be a member of any number of clubs.  When the member dies,
// the member leaves each club.
template <class MemberType> class MultiMember : public Member<MemberType>
{
	public:
		~MultiMember(void)
		{
			for (std::list<Membership *>::iterator CurrentMembership = Memberships.begin();
				CurrentMembership != Memberships.end(); CurrentMembership++)
				delete *CurrentMembership;
		}
		
	protected:
		friend class Club<MemberType>;
		void Join(Membership *NewMembership) { Memberships.push_back(NewMembership); }

	private:
		std::list<Membership *> Memberships;
};

#endif
