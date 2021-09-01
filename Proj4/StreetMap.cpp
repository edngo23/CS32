#include "provided.h"
#include <string>
#include <vector>
#include <functional>
#include <iostream>
#include <fstream>
#include "ExpandableHashMap.h"
using namespace std;

unsigned int hasher(const GeoCoord& g)
{
    return std::hash<string>()(g.latitudeText + g.longitudeText);
}

class StreetMapImpl
{
public:
    StreetMapImpl();
    ~StreetMapImpl();
    bool load(string mapFile);
    bool getSegmentsThatStartWith(const GeoCoord& gc, vector<StreetSegment>& segs) const;
private:
	ExpandableHashMap<GeoCoord, vector<StreetSegment>> m_mapOfStreets;
	vector<StreetSegment> m_streets;
};

StreetMapImpl::StreetMapImpl()
{
}

StreetMapImpl::~StreetMapImpl()
{
}

bool StreetMapImpl::load(string mapFile)
{
	// Open the file for input
	ifstream inf(mapFile);
	// Test for failure to open
	if (!inf)
	{
		cerr << "Cannot open the map file!" << endl;
		return false;
	}

	// Read each line.  The return value of getline is treated
	// as true if a line was read, false otherwise (e.g., because
	// the end of the file was reached).
	cerr << "loading file \n";
	string streetName;
	while (getline(inf, streetName))
	{
		//cerr << streetName << endl;
		int numOfSegs;
		inf >> numOfSegs;
		//cerr << numOfSegs << endl;

		string startLat, startLong, endLat, endLong;
		for (int i = 1; i <= numOfSegs; i++)
		{
			inf >> startLat >> startLong >> endLat >> endLong;
			GeoCoord start(startLat, startLong);
			GeoCoord end(endLat, endLong);

			StreetSegment street(start, end, streetName);
			StreetSegment streetRev(end, start, streetName);
			if (m_mapOfStreets.find(start) == nullptr)
			{
				vector<StreetSegment> temp;
				temp.push_back(street);
				m_mapOfStreets.associate(start, temp);
			}
			else
				m_mapOfStreets.find(start)->push_back(street);

			m_streets.push_back(street);

			if (m_mapOfStreets.find(end) == nullptr)
			{
				vector<StreetSegment> temp;
				temp.push_back(streetRev);
				m_mapOfStreets.associate(end, temp);
			}
			else
				m_mapOfStreets.find(end)->push_back(streetRev);
		}
		inf.ignore(100000, '\n');
		//cerr << "Success for " << streetName << endl;
	}
	cerr << "num of street segs : " << m_streets.size() << endl;
	return true;
}

bool StreetMapImpl::getSegmentsThatStartWith(const GeoCoord& gc, vector<StreetSegment>& segs) const
{
	if (m_mapOfStreets.find(gc) != nullptr)
	{
		segs = *m_mapOfStreets.find(gc);
		return true;
	}
	return false;
}

//******************** StreetMap functions ************************************

// These functions simply delegate to StreetMapImpl's functions.
// You probably don't want to change any of this code.

StreetMap::StreetMap()
{
    m_impl = new StreetMapImpl;
}

StreetMap::~StreetMap()
{
    delete m_impl;
}

bool StreetMap::load(string mapFile)
{
    return m_impl->load(mapFile);
}

bool StreetMap::getSegmentsThatStartWith(const GeoCoord& gc, vector<StreetSegment>& segs) const
{
    return m_impl->getSegmentsThatStartWith(gc, segs);
}
