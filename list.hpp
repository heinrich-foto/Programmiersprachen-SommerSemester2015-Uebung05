#ifndef BUW_LIST_HPP 
#define BUW_LIST_HPP

//#include <cstddef > 
#include <algorithm>
#include <iostream>
// List.hpp

template <typename T> 
class List;

template <typename T> 
struct ListNode
{
	ListNode() : m_value(), m_prev(nullptr), m_next(nullptr) {}
	ListNode(T const& v, ListNode* prev, ListNode* next) : m_value(v), m_prev(prev), m_next(next) {}
	
	T m_value;
	ListNode* m_prev;
	ListNode* m_next;
};

template <typename T> 
struct ListIterator
{
	typedef ListIterator <T> Self;
	typedef ListNode <T> Node;
	typedef T value_type;
	typedef T* pointer;
	typedef T& reference;
	typedef ptrdiff_t difference_type;
	typedef std::forward_iterator_tag iterator_category;

	friend class List<T>;
	
	ListIterator():m_node{nullptr} {}
	ListIterator(ListNode<T>* n):m_node{n} {}

	reference operator*() const { return m_node->m_value; }
	pointer  operator->() const { return (m_node) ? &m_node : nullptr; }
	const Self& operator++() {
		// (preincrement) neuer Zustand ist jetzt vorhanden.
		if (m_node) m_node = m_node->m_next;
		return *this;
	}
	const Self& operator--() {
		if (m_node) m_node = m_node->m_prev;
		return *this;
	}
	Self operator++(int) {
		// incrementiert sich gibt aber erneut seinen alten Zustand zurück. (postincrement)
		Self result{*this}; ++(*this); return result;
	} 
	Self operator--(int) {
		Self result{*this}; --(*this); return result;
	}
	bool operator==(const Self& x) const { return m_node == x.m_node; } 
	bool operator!=(const Self& x) const { return m_node != x.m_node; } 

	Self next () const {
		return (m_node) ? ListIterator(m_node->m_next) : ListIterator(nullptr);
	} 

	private:
	// The Node the iterator is pointing to
	ListNode <T>* m_node;
};

template <typename T> 
struct ListConstIterator 
{
	public:
	// not implemented yet private:
	ListNode <T>* m_node; 
};

template <typename T> 
class List
{
	public:
		// Default Constructor
		List (): m_size(0), m_first(nullptr), m_last(nullptr) {};
		~List() { clear(); };

		List(List const& other) : m_first(nullptr), m_last(nullptr), m_size(0) // Copy Construktor
		{	// used for lvalues
			for (auto const& x: other) this->push_back(x);
		};

		List(List && other): m_first(other.m_first), m_last(other.m_last), m_size(other.m_size)  // Move Construktor
		{	// used for rvalues
			other.m_first = nullptr;
			other.m_last = nullptr;
			other.m_size = 0;
        };
        void swap(List & other) // swaps content of X with *this
        {	
        	for (auto const& x: other) this->push_back(x);
        	std::swap(m_first , other.m_first); 
        	std::swap(m_last , other.m_last);
        };
        
        friend void swap(List& a, List& b) { a.swap(b); };
        
        List& operator=(List  rhs) { // copy assignment
        	swap(rhs);
        	return *this;
        };

        // auto operator=(List<T>&& rhs) & noexcept -> List<T>& // move assignement
        // // && R‑Value-Referenzen
        // /* Die Move-Operationen sind in der Regel sehr effizient. 
        // Denn im Gegensatz zu den Copy-Operationen muss lediglich eine 
        // flache Kopie gemacht werden, wobei gleichzeitig das Argument 
        // in eine Art Leer- beziehungsweise Initialzustand versetzt wird. */
        // {};

		// http://en.cppreference.com/w/cpp/container/list/empty 
		bool empty() const { return (m_first== nullptr) && (m_last==nullptr); };
		// http://en.cppreference.com/w/cpp/container/list/size 
		std::size_t size() const { return m_size; };

		void push_front(T const& value) 
		{ 
			ListNode <T>* pointer = new ListNode <T> (value, nullptr, m_first);
			(m_first) ?  m_first->m_prev = pointer : m_last = pointer;
			m_first = pointer; 
			++m_size; 
        };

		void push_back(T const& value) 
		{ 
			ListNode <T>* pointer = new ListNode <T> (value, m_last, nullptr);
			(m_last) ? (*m_last).m_next = pointer : m_first = pointer;
			m_last = pointer; 
			++m_size; 
        };

		void pop_front()
		{ 
		 	auto* pointer_prev = m_first->m_prev;
			auto* pointer_next = m_first->m_next;
			delete m_first;
			m_size--;
			m_first = pointer_next;
			(m_first) ? m_first->m_prev = pointer_prev :  m_last = nullptr;
        };

		void pop_back() 
		{
			auto* pointer_prev = m_last->m_prev;
		 	auto* pointer_next = m_last->m_next;
		 	delete m_last;
		 	m_size--;
		 	m_last = pointer_prev;
		 	(m_last) ? m_last->m_next = pointer_next :  m_first = nullptr;
		};
    
    	T front() const { return m_first->m_value; };
    
    	T back() const { return m_last->m_value; };

    	ListIterator<T> begin() const { return (m_first!=nullptr)?  ListIterator <T>( m_first ) : nullptr; };

    	ListIterator<T> end() const { return nullptr; };

    	void clear() { while (m_first != nullptr) pop_back(); };

    	ListIterator<T> insert(ListIterator<T> iter, T value) 
    	{	if (iter == nullptr) { 
    			push_back(value);
    			return m_last;
    		}
    		else {
    			ListNode<T>* after = iter.m_node;
				ListNode<T>* before = after->m_prev;
				ListNode<T>* newNode = new ListNode<T>{value, before, after};
				
				after->m_prev = newNode;
				if (before == nullptr) /* insert at beginning */
   					m_first = newNode;
				else
   					before->m_next = newNode;
				++m_size;
				return ListIterator<T>(newNode);
    		}
    	};

    	void reverse () 
		{
			for (auto itter = ListIterator<T> (begin()); itter != ListIterator<T> (end()); --itter)
			{ 
				std::swap(itter.m_node->m_next,itter.m_node->m_prev);
			}
			std::swap(m_first,m_last);
		};

    friend bool operator==(List<T> const& xs, List<T> const& ys)
	{
		if (xs.size() != ys.size())
		    return false;
		auto yit = ys.begin();
		for ( auto xit = xs.begin(); xit!=xs.end(); ++xit)
		{
			if (*xit != *yit) return false;
			++yit;
		}
		return true;
	};
	friend bool operator!=(List<T> const& xs, List<T> const& ys) 
	{ 
		return !(xs == ys); 
	};

	friend std::ostream& operator<<(std::ostream& os, List<T> const& list) 
  	{
  		os << "[ ";
    	for (auto iter = list.begin(); iter!=list.end(); ++iter) {
    		os << *iter;
    		if (iter.m_node->m_next!=nullptr) os << ", ";
    	}
    	os << "]";
    	return  os; 
    };
	
	private:
	std::size_t m_size; 
	ListNode <T>* m_first; 
	ListNode <T>* m_last;
};

template <typename T> 
void reverse(List<T> & other)
{
	other.reverse();
};

#endif // #define BUW_LIST_HPP