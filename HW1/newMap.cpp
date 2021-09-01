#include "newMap.h"

using namespace std;

Map::Map() {
	m_counter = 0;
	m_pair = new mapPair[DEFAULT_MAX_ITEMS];
	m_size = DEFAULT_MAX_ITEMS;
}

Map::Map(int size) {
	m_counter = 0;
	m_pair = new mapPair[size];
	m_size = size;
}

Map::~Map() {
	delete[] m_pair;
}

Map& Map::operator= (const Map& src) {
	if (&src == this)
		return *this;
	delete[] m_pair;
	m_counter = src.m_counter;
	for (int i = 0; i < m_counter; i++) {
		m_pair[i].m_key = src.m_pair[i].m_key;
		m_pair[i].m_val = src.m_pair[i].m_val;
	}
	return *this;
}

Map::Map(const Map& src) {
	m_counter = src.m_counter;
	m_pair = new mapPair[m_counter];
	for (int i = 0; i < m_counter; i++) {
		m_pair[i].m_key = src.m_pair[i].m_key;
		m_pair[i].m_val = src.m_pair[i].m_val;
	}
}

bool Map::empty() const {
	if (m_counter == 0)
		return true;
	return false;
}

int Map::size() const {
	return m_counter;
}

bool Map::insert(const KeyType& key, const ValueType& value) {
	if (m_counter < m_size) {
		if(contains(key))
			return false;
		m_pair[m_counter].m_key = key;
		m_pair[m_counter].m_val = value;
		m_counter++;
		return true;
	}
	return false;
}

bool Map::update(const KeyType& key, const ValueType& value) {
	for (int i = 0; i < m_counter; i++) {
		if (m_pair[i].m_key == key) {
			m_pair[i].m_val = value;
			return true;
		}
	}
	return false;
}

bool Map::insertOrUpdate(const KeyType& key, const ValueType& value) {
	if (update(key, value))
		return true;
	else if (insert(key, value))
		return true;
	return false;
}

bool Map::erase(const KeyType& key) {
	for (int i = 0; i < m_counter; i++) {
		if (m_pair[i].m_key == key) {
			for (int j = i; j < m_counter - 1; j++) {
				m_pair[j].m_key = m_pair[j + 1].m_key;
				m_pair[j].m_val = m_pair[j + 1].m_val;
				m_counter--;
				return true;
			}
		}
	}
	return false;
}

bool Map::contains(const KeyType& key) const {
	for (int i = 0; i < m_counter; i++) {
		if (m_pair[i].m_key == key) {
			return true;
		}
	}
	return false;
}

bool Map::get(const KeyType& key, ValueType& value) const {
	for (int i = 0; i < m_counter; i++) {
		if (m_pair[i].m_key == key) {
			value = m_pair[i].m_val;
			return true;
		}
	}
	return false;
}

bool Map::get(int i, KeyType& key, ValueType& value) const {
	if (i >= 0 && i < m_counter) {
		key = m_pair[i].m_key;
		value = m_pair[i].m_val;
		return true;
	}
	return false;
}

void Map::swap(Map& other) {
	Map tempMap = *this;
	*this = other;
	other = tempMap;
}
