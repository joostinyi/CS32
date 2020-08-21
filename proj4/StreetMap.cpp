#include "provided.h"
#include <string>
#include <vector>
#include "ExpandableHashMap.h"
#include <iostream>
#include <fstream>
#include <iomanip>
using namespace std;

class StreetMapImpl
{
public:
    StreetMapImpl();
    ~StreetMapImpl();
    bool load(string mapFile);
    bool getSegmentsThatStartWith(const GeoCoord& gc, vector<StreetSegment>& segs) const;
private:
    ExpandableHashMap<GeoCoord, vector<StreetSegment>> map;
};

StreetMapImpl::StreetMapImpl()
{}

StreetMapImpl::~StreetMapImpl()
{}

unsigned int hasher(const GeoCoord& g)
{
    return hash<string>()(g.latitudeText + g.longitudeText);
}

bool StreetMapImpl::load(string mapFile)
{
    ifstream inf(mapFile);
    // Test for failure to open
    if (!inf)
    {
        cout << "Cannot open map file!" << endl;
        return false;
    }
    string line;
    while (getline(inf, line))
    {
        int num;
        string lat, lon;
        string name = line;
        getline(inf, line);
        num = stoi(line);
        for(int i = 0; i < num; i++)
        {
            getline(inf, line);
            istringstream iss(line);
            iss >> lat >> lon;
            GeoCoord startGC = GeoCoord(lat, lon);
            iss >> lat >> lon;
            GeoCoord endGC = GeoCoord(lat, lon);
            StreetSegment streetSeg = StreetSegment(startGC, endGC, name);
            if(map.find(startGC) == nullptr)
                map.associate(startGC, vector<StreetSegment>(1, streetSeg));
            else
                map.find(startGC)->push_back(streetSeg);
            StreetSegment streetSegRev = StreetSegment(endGC, startGC, name);
            if(map.find(endGC) == nullptr)
                map.associate(endGC, vector<StreetSegment>(1, streetSegRev));
            else
                map.find(endGC)->push_back(streetSegRev);
        }
    }
    return true;
}

bool StreetMapImpl::getSegmentsThatStartWith(const GeoCoord& gc, vector<StreetSegment>& segs) const
{
    if(map.find(gc) == nullptr)
        return false;
    segs = *map.find(gc);
    return true;
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
