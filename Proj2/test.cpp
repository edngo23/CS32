#include "Map.h"
#include <iostream>
#include <cassert>
using namespace std;

void test()
{
	ValueType v;
	Map m;
	assert(m.insert("Fred", 123));
	assert(m.insert("Ethel", 456));
	Map m2(m);
	assert(m2.contains("Fred"));
	Map m3;
	m3.insert("Bob", 145);
	m3.insert("Fred", 321);
	m3 = m;
	assert(m3.contains("Ethel"));
	assert(m3.get("Fred", v) && v == 123);
	assert(m3.erase("Fred"));
	assert(!m3.contains("Fred"));
	assert(!m3.erase("Bob"));
	Map m4;
	m4.swap(m);
	assert(m.empty());
	assert(m4.contains("Ethel"));
	Map m5;
	combine(m2, m4, m5);
	assert(m5.contains("Ethel"));
	m4.update("Ethel", 1234);
	combine(m2, m4, m5);
	assert(!m5.contains("Ethel"));
	Map m6;
	m6.insert("Fred", 123);
	m6.insert("Bob", 456);
	m6.insert("Ally", 789);
	Map m7(m);
	//m6 = m7;
	reassign(m6, m7);
	assert(m6.get("Bob", v) && v == 123);
}

int main()
{
	test();
	cout << "Passed all tests" << endl;
}
