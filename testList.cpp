#define CATCH_CONFIG_RUNNER
#include "catch.hpp"
#include "list.hpp"
#include <vector>
#include <iostream>

TEST_CASE("list is empty","[aufgabe1]")
{
	List<int> Liste{};

	REQUIRE(Liste.empty());
	REQUIRE(Liste.size() == 0);
}

TEST_CASE("add an element with push_front", "[push_front]")
{
	List<int> listofint; 
	listofint.push_front(42); 
	REQUIRE(42 == listofint.front());
	REQUIRE(1 == listofint.size());
}

TEST_CASE("add an element with push_back and access the List from front and back", "[push_back-front-back]")
{
	List<int> listofint;
    for (int i=1; i<10; i++)
	{
		listofint.push_back(i);
		REQUIRE(i==listofint.size());
	}
	REQUIRE(9==listofint.back());
	REQUIRE(1==listofint.front());
}

TEST_CASE("should be empty after clearing", "[clear]")
{
	List<int> list; 
	list.push_front(1); 
	list.push_front(2); 
	list.push_front(3); 
	list.push_front(4); 
	REQUIRE(4 == list.size());
	list.clear(); 
	REQUIRE(list.empty());
	REQUIRE(list.size()==0);
}

TEST_CASE("itterator implementation", "[itterator]")
{
	List<int> list;
	for (int i=0; i<=5; i++) list.push_back(i);
	int count = 0;
	for (auto const& itt:list) REQUIRE(count++==itt);
	REQUIRE(count==6);
	REQUIRE(0==*list.begin());
	REQUIRE(list.end()==nullptr);

	auto iter = list.begin();
	++iter;
	--iter;
	iter++;
	iter--;
	REQUIRE(list.begin()==iter);
}

TEST_CASE("should be an empty range after default construction", "[begin_end]")
{
	List<int> list;
	auto b = list.begin(); 
	auto e = list.end(); 
	REQUIRE(b == e);
}

TEST_CASE("provide acces to the first element with begin", "[begin]")
{
	List<int> list; 
	list.push_front(42); 
	REQUIRE(42 == *list.begin());
	REQUIRE(1==list.size());
}

TEST_CASE("nullptr if empty","[begin]")
{
	List<int> list;
	REQUIRE(list.begin()==nullptr);
	REQUIRE(0==list.size());

	list.push_front(1);
	list.push_back(2);
	list.pop_back();
	list.pop_front();
	REQUIRE(list.begin()==nullptr);
	REQUIRE(0==list.size());

	list.push_front(1); 
	list.push_front(2);
	REQUIRE(2==list.size()); 
	list.push_front(3); 
	list.push_front(4);
	list.clear();
	REQUIRE(list.begin()==nullptr);
	REQUIRE(list.end()==nullptr);
	REQUIRE(0==list.size());
}

TEST_CASE("list should be the same", "[equal]")
{
	List<int> list; 
	list.push_front(1); 
	list.push_front(2); 
	list.push_front(3); 
	list.push_front(4); 
		List<int> list2; 
	list2.push_front(1); 
	list2.push_front(2); 
	list2.push_front(3); 
	list2.push_front(4); 
	REQUIRE(list == list2);
}

TEST_CASE("list shouldn't be the same", "[notequal]")
{
	List<int> list; 
	list.push_front(1); 
	list.push_back(2); 
	list.push_front(3); 
	list.push_front(4); 
		List<int> list2; 
	list2.push_front(1); 
	list2.push_back(2); 
	list2.push_front(938848); 
	list2.push_front(4); 
	REQUIRE(list != list2);
}

TEST_CASE("copy constructor", "[constructor]")
{
	List<int> list;
	list.push_front(1);
	list.push_front(2);
	list.push_front(3);
	list.push_front(4);
	list.push_front(9);
	List<int> list2(list);

	REQUIRE(5==list.size());
	REQUIRE(5==list2.size());
	REQUIRE(list == list2);
}

TEST_CASE("move constructor", "[constructor]")
{
	List<int> list;
	list.push_front(1);
	list.push_front(2);
	list.push_front(3);
	list.push_front(4);
	List<int> list2(std::move(list));
	REQUIRE(0 == list.size());
	REQUIRE(list.empty());
	REQUIRE(4 == list2.size());
}

TEST_CASE("insert value at itterator", "[insert]")
{
	List<int> list;
	list.push_back(1);
	list.push_back(2);
	list.push_back(3);
	list.push_back(4);
	ListIterator<int> iter = list.begin();
	++iter;
	iter = list.insert(iter,99);
	REQUIRE(99==*iter);
	REQUIRE(5==list.size());
	++iter;
	REQUIRE(2==*iter);
	iter = list.insert(list.end(),8);
	REQUIRE(8==list.back());
}

TEST_CASE("reverse","[reverse]")
{
	List<int> list;
	list.push_back(1);
	list.push_back(2);
	list.push_back(3);
	list.push_back(4);
	
	List<int> list2(list);
	list.reverse();
	// std::cout << list << list2;
	REQUIRE(list.front()==list2.back());

	reverse(list);
	REQUIRE(list2==list);
	// std::cout << std::endl << list << list2;
	reverse(list); list2.reverse();
	// std::cout << std::endl << list << list2;
	REQUIRE(list2==list);
}

TEST_CASE("std::copy list into vector","copy")
{
	List<int> list;
	list.push_back(1);
	list.push_back(2);
	list.push_back(3);
	list.push_back(4);
	std::vector<int> vect;
	std::copy(list.begin(),list.end(),std::back_inserter(vect));
	auto list_iter = list.begin();
	for(auto const& item:vect) {
		REQUIRE(item==*list_iter++);
	}
}

int main (int argc, char* argv[])
{ 	// Tests ausführen.
	return Catch::Session().run(argc,argv);
}