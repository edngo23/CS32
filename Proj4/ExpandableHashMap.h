// ExpandableHashMap.h
#ifndef EXPANDABLE_INCLUDED
#define EXPANDABLE_INCLUDED
	#include<vector>
	#include<list>

	const int STARTING_BUCKET_SIZE = 8;
	// Skeleton for the ExpandableHashMap class template.  You must implement the first six
	// member functions.

	template<typename KeyType, typename ValueType>
	class ExpandableHashMap
	{
	public:
		ExpandableHashMap(double maximumLoadFactor = 0.5);
		~ExpandableHashMap();
		void reset();
		int size() const;
		void associate(const KeyType& key, const ValueType& value);

		// for a map that can't be modified, return a pointer to const ValueType
		const ValueType* find(const KeyType& key) const;

		// for a modifiable map, return a pointer to modifiable ValueType
		ValueType* find(const KeyType& key)
		{
			return const_cast<ValueType*>(const_cast<const ExpandableHashMap*>(this)->find(key));
		}

		// C++11 syntax for preventing copying and assignment
		ExpandableHashMap(const ExpandableHashMap&) = delete;
		ExpandableHashMap& operator=(const ExpandableHashMap&) = delete;

	private:
		struct NodePair
		{
		public:
			NodePair(KeyType k, ValueType v)
			{
				key = k;
				value = v;
			}
			KeyType key;
			ValueType value;
		};
		int m_numOfAssociations, m_bucketSize;
		double m_maxLoadFactor;
		std::vector<std::list<NodePair>> m_vec{ STARTING_BUCKET_SIZE };
		unsigned int getBucketNumber(const KeyType& key) const;
	};

	template<typename KeyType, typename ValueType>
	ExpandableHashMap<KeyType, ValueType>::ExpandableHashMap(double maximumLoadFactor)
	{
		m_numOfAssociations = 0;
		m_bucketSize = STARTING_BUCKET_SIZE;
		if (maximumLoadFactor < 0)
			maximumLoadFactor = 0.5;
		m_maxLoadFactor = maximumLoadFactor;
	}

	template<typename KeyType, typename ValueType>
	ExpandableHashMap<KeyType, ValueType>::~ExpandableHashMap()
	{
	}

	template<typename KeyType, typename ValueType>
	void ExpandableHashMap<KeyType, ValueType>::reset()
	{
		for (int i = 0; i < m_vec.size(); i++)
		{
			std::list<NodePair> tempList;
			m_vec[i].push_back(tempList);
		}
		m_bucketSize = STARTING_BUCKET_SIZE;
	}

	template<typename KeyType, typename ValueType>
	int ExpandableHashMap<KeyType, ValueType>::size() const
	{
		return m_numOfAssociations;
	}

	template<typename KeyType, typename ValueType>
	void ExpandableHashMap<KeyType, ValueType>::associate(const KeyType& key, const ValueType& value)
	{
		//std::cerr << "Load: " << m_maxLoadFactor << std::endl;
		if (find(key) != nullptr)
		{
			*(find(key)) = value;
		}
		unsigned int bucketNum = getBucketNumber(key);
		NodePair temp(key, value);
		m_vec[bucketNum].push_back(temp);
		m_numOfAssociations++;
		if (static_cast<double>(m_numOfAssociations) / m_bucketSize > m_maxLoadFactor)
		{
			m_bucketSize *= 2;
			std::vector<std::list<NodePair>> newVec(m_bucketSize);
			unsigned int newBucketNum;
			for (int i = 0; i < m_vec.size(); i++)
			{
				for (auto it = m_vec[i].begin(); it != m_vec[i].end(); it++)
				{
					newBucketNum = getBucketNumber((*it).key);
					newVec[newBucketNum].push_back(NodePair((*it).key, (*it).value));
				}
			}
			m_vec = newVec;
			std::cerr << "rehashed \n";
		}
		//std::cerr << m_bucketSize << std::endl;
	}

	template<typename KeyType, typename ValueType>
	const ValueType* ExpandableHashMap<KeyType, ValueType>::find(const KeyType& key) const
	{
		unsigned int buckNum = getBucketNumber(key);
		for (auto it = m_vec[buckNum].begin(); it != m_vec[buckNum].end(); it++)
		{
			if ((*it).key == key)
				return &((*it).value);
		}
		return nullptr;
	}

	template<typename KeyType, typename ValueType>
	unsigned int ExpandableHashMap<KeyType, ValueType>::getBucketNumber(const KeyType& key) const
	{
		unsigned int hasher(const KeyType & key);
		unsigned int h = hasher(key);
		h = h % m_bucketSize;
		return h;
	}

#endif // EXPANDABLE_INCLUDED

