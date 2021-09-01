#include "Map.h"

using namespace std;

Map::Map() {
	m_counter = 0;
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
	if (m_counter < DEFAULT_MAX_ITEMS) {
		if (contains(key))
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
	KeyType tempKey;
	ValueType tempVal;
	int tempSize;
	if (this->size() <= other.size()) {
		for (int i = 0; i < other.size(); i++) {
			tempKey = other.m_pair[i].m_key;
			tempVal = other.m_pair[i].m_val;
			this->get(i, other.m_pair[i].m_key, other.m_pair[i].m_val);
			this->m_pair[i].m_key = tempKey;
			this->m_pair[i].m_val = tempVal;
		}
	}
	else {
		for (int i = 0; i < this->size(); i++) {
			tempKey = this->m_pair[i].m_key;
			tempVal = this->m_pair[i].m_val;
			other.get(i, this->m_pair[i].m_key, this->m_pair[i].m_val);
			other.m_pair[i].m_key = tempKey;
			other.m_pair[i].m_val = tempVal;
		}
	}
	tempSize = this->m_counter;
	this->m_counter = other.m_counter;
	other.m_counter = tempSize;
}