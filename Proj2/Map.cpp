#include "Map.h"
#include <iostream>

//constructor that is called when creating a new Map object
Map::Map()
	:m_size(0)
{
	dummyNode.prev = nullptr;
	dummyNode.next = nullptr;
}

//destructor that is called when a Map goes out of scope, deleting all dynamically allocated memory of that Map object
Map::~Map()
{
	Node* traverse = &dummyNode; //traverse the list and deleting all dynamically allocated memory
	while (traverse->next != nullptr)
		traverse = traverse->next;
	while (m_size > 0)
	{
		traverse = traverse->prev;
		delete traverse->next;
		m_size--;
	}
}

//a copy constructor that creates a Map object to be a copy of the source Map object
Map::Map(const Map& src) 
{
	m_size = 0;
	dummyNode.prev = nullptr;
	dummyNode.next = nullptr;

	if (src.m_size != 0)
	{
		const Node* traverseSrc = &src.dummyNode;
		while (traverseSrc->next != nullptr) //make the same number of nodes and set all the new nodes to that of the source list
		{
			traverseSrc = traverseSrc->next;
			//insert(traverseSrc->m_key, traverseSrc->m_val);
		}
	}
	m_size = src.m_size;
}

//make an already created Map object be a copy of another Map object
Map& Map::operator=(const Map& src)
{
	if (&src == this)
		return *this;

	const Node* traverseSrc = &src.dummyNode;
	Node* traverseCopy = &dummyNode;

	KeyType k;
	ValueType v;

	//delete all the nodes of the old object, the one that will become a copy of the source object
	while (traverseCopy->next != nullptr)
		traverseCopy = traverseCopy->next;
	while (m_size > 0)
	{
		get(m_size - 1, k, v);
		erase(k);
	}
	dummyNode.next = nullptr;
	traverseCopy = &dummyNode;

	std::cout << m_size;
	m_size = src.m_size;

	std::cout << m_size;
	while (traverseSrc->next != nullptr) //make the same number of nodes and set all the new nodes to that of the source list
	{
		traverseSrc = traverseSrc->next;
		std::cout << "I hate this.\n";
		this->insert(traverseSrc->m_key, traverseSrc->m_val);
		std::cout << "I worked.\n";
		traverseCopy = traverseCopy->next;
	}
	return *this;
}

//description of this and other trivial functions are described in the Map.h header file
int Map::size() const
{
	return m_size;
}

bool Map::empty() const
{
	return size() == 0;
}

bool Map::contains(const KeyType& key) const
{
	const Node* traverse = &dummyNode;
	while (traverse != nullptr) 
	{
		if (traverse->m_key == key)
			return true;
		traverse = traverse->next;
	}
	return false;
}

bool Map::insert(const KeyType& key, const ValueType& value)
{
	if (contains(key))
		return false;
	Node* newNode = new Node; //create a new node and have the pointers point in the correct order
	newNode->m_key = key;
	newNode->m_val = value;
	newNode->next = dummyNode.next;
	newNode->prev = &dummyNode;
	if (dummyNode.next != nullptr)
		dummyNode.next->prev = newNode;
	dummyNode.next = newNode;
	m_size++;
	return true;
}

bool Map::update(const KeyType& key, const ValueType& value)
{
	Node* traverse = &dummyNode;
	while (traverse != nullptr) 
	{
		if (traverse->m_key == key)
		{
			traverse->m_val = value;
			return true;
		}
		traverse = traverse->next;
	}
	return false;
}

bool Map::insertOrUpdate(const KeyType& key, const ValueType& value)
{
	if (update(key, value))
		return true;
	insert(key, value);
	return true;
}

bool Map::erase(const KeyType& key)
{
	Node* traverse = &dummyNode;
	while (traverse != nullptr)
	{
		if (traverse->m_key == key)
		{
			if(traverse->prev != nullptr)
				(traverse->prev)->next = traverse->next; //make the previous node point to the node after
			if(traverse->next != nullptr)
				(traverse->next)->prev = traverse->prev; //make the node after point to the previous node
			delete traverse; //delete the node
			m_size--;
			return true;
		}
		traverse = traverse->next;
	}
	return false;
}

bool Map::get(const KeyType& key, ValueType& value) const
{
	const Node* traverse = &dummyNode;
	while (traverse != nullptr)
	{
		if (traverse->m_key == key)
		{
			value = traverse->m_val;
			return true;
		}
		traverse = traverse->next;
	}
	return false;
}

bool Map::get(int i, KeyType& key, ValueType& value) const
{
	if (i < 0 || i >= m_size)
		return false;
	const Node* traverse = &dummyNode;
	for (int count = 0; count <= i; count++) {
		if (traverse->next != nullptr)
		{
			traverse = traverse->next;
		}
	}
	key = traverse->m_key;
	value = traverse->m_val;
	return true;
}

//switch the dummy nodes and what they point at using a temporary node
void Map::swap(Map& other)
{
	Node temp;
	int tempSize = 0;
	//use a temporary node
	temp.prev = nullptr;
	temp.next = this->dummyNode.next; //have temp's next point to the current Map's first Node
	this->dummyNode.next = other.dummyNode.next; //have the current Map's first node be the node of the other Map's first node
	//have the other Map's first node point to the temporary node's next node, which was previously the current Map's first node
	other.dummyNode.next = temp.next; 
	//make the second node's previous node point to the now switched dummy nodes, if there is a a second node
	if(this->dummyNode.next != nullptr)
		this->dummyNode.next->prev = &dummyNode;  
	if(other.dummyNode.next != nullptr)
		other.dummyNode.next->prev = &other.dummyNode;

	tempSize = m_size; //switch the size of each list using a temporary int
	m_size = other.m_size;
	other.m_size = tempSize;
}

//combine the two maps to put in result
bool combine(const Map& m1, const Map& m2, Map& result)
{
	Map temp;
	KeyType key1;
	ValueType value1;
	ValueType value2;
	bool success = true;

	for (int i = 0; i < m1.size(); i++)
	{
		m1.get(i, key1, value1); //get the key and value of the i-th position in map 1
		m2.get(key1, value2); //set the value 2 to the value corresponding to the key in map 2. if there is one

		if (m2.contains(key1)) //if map 2 does have key 1
		{
			if (value1 != value2) //if the value in map 1 and value of map 2 are not equal that means there are two values for the same key
				success = false;
			else
				temp.insert(key1, value1); //insert to temp if map 1 and map 2 have the same key and same value
		}
		else
			temp.insert(key1, value1); //if map 2 doesn't have that key in one of its nodes, then insert since it is only in map 1
	}
	for (int i = 0; i < m2.size(); i++)
	{
		m2.get(i, key1, value1); //do the same for map 2
		m1.get(key1, value2);

		if (!m1.contains(key1))
			temp.insert(key1, value1); //this inserts to temp if it is in map 2 but not in map 1
	}
	result = temp; //make result be temp, where temp is used just in case result is mapped to map 1 or map 2
	return success;
}

//reassign all nodes values, to be the value of the next node, and the last node value to be what the first node previously had
void reassign(const Map& m, Map& result)
{
	KeyType key1;
	ValueType value1;
	KeyType key2;
	ValueType value2;
	ValueType value3;

	result = m; //make result be a copy of m, so we can forget about m since we can't make changes to it
	result.get(0, key1, value1);
	value3 = value1; //save the value of the first node for later
	for (int i = 1; i < result.size(); i++)
	{
		result.get(i - 1, key1, value1);
		result.get(i, key2, value2);
		result.update(key1, value2); //take the value of the next node and update the current node to the next node's value
	}
	result.get(result.size() - 1, key2, value2); //get the key of the last node
	result.update(key2, value3); //set the last node's value to value 3, which is the first node's previous value
}
