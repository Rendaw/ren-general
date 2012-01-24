#ifndef collection_h
#define collection_h

#include <functional>

// Iterable provides a way to limit access to a container only to iterating.  Pass lambdas that return the proper iterators, then the iterable can be used for standard or range-based for loops.
template <typename ElementType, typename IteratorType> class SimpleIterable
{
	private:
		typedef std::function<IteratorType(void)> IteratorGetFunction;
		IteratorGetFunction BeginImplementation;
		IteratorGetFunction EndImplementation;
	
	public:
		SimpleIterable(IteratorGetFunction const &BeginImplementation, IteratorGetFunction const &EndImplementation) :
			BeginImplementation(BeginImplementation), EndImplementation(EndImplementation)
			{}

		IteratorType begin(void)
			{ return BeginImplementation(); }
			
		IteratorType end(void)
			{ return EndImplementation(); }
};

// IndexIterable requires a count method and a value getter method and iterates based on those.
template <typename CoreType> class IndexIterable
{
	private:
		CoreType Base;
		typedef decltype(Base.GetCount()) IndexType;
		typedef decltype(Base.GetValue(0)) ValueType;
	public:
		class Iterator
		{
			private:
				CoreType &Base;
				IndexType Position;
				
			public:					
				Iterator(CoreType &Base) : Base(Base), Position(0) {}
				void operator++(void) { Position++; }
				bool operator!=(Iterator const &Other) { return Position < Base.GetCount(); }
				ValueType &operator*(void) { return Base.GetValue(Position); }
		};
			
		IndexIterable(CoreType const &Base) : Base(Base) {}
		virtual ~IndexIterable(void) {}
		
		Iterator begin(void) { return Iterator(Base); }
		
		Iterator end(void) { return Iterator(Base); } // Pointless, end based on iterator internal counter
};

// StandardIterable and StandardReverseIterable are convenience implementations for most standard collections (std::list, std::vector, etc).  Also: cast the collection to const in itialization (it might do something cool)?
template <typename CollectionType> class StandardIterable
{
	private:
		CollectionType &Base;
		
	public:
		StandardIterable(CollectionType &Base) : Base(Base) {}
		decltype(Base.begin()) begin(void) { return Base.begin(); }
		decltype(Base.end()) end(void) { return Base.end(); }
};

template <typename CollectionType> class StandardReverseIterable
{
	private:
		CollectionType &Base;
		
	public:
		StandardReverseIterable(CollectionType &Base) : Base(Base) {}
		decltype(Base.rbegin()) begin(void) { return Base.rbegin(); }
		decltype(Base.rend()) end(void) { return Base.rend(); }
};

#endif
