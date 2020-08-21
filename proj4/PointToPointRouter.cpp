#include "provided.h"
#include "ExpandableHashMap.h"

#include <set>
#include <utility>
#include <list>
#include <queue>
using namespace std;

class PointToPointRouterImpl
{
public:
    PointToPointRouterImpl(const StreetMap* sm);
    ~PointToPointRouterImpl();
    DeliveryResult generatePointToPointRoute(
        const GeoCoord& start,
        const GeoCoord& end,
        list<StreetSegment>& route,
        double& totalDistanceTravelled) const;
private:
    const StreetMap* m_streetmap;
    
    ExpandableHashMap<GeoCoord, GeoCoord> locationofPreviousWayPoint;
};


PointToPointRouterImpl::PointToPointRouterImpl(const StreetMap* sm)
    : m_streetmap(sm) {}

PointToPointRouterImpl::~PointToPointRouterImpl()
{}

DeliveryResult PointToPointRouterImpl::generatePointToPointRoute(
        const GeoCoord& start,
        const GeoCoord& end,
        list<StreetSegment>& route,
        double& totalDistanceTravelled) const
{
    totalDistanceTravelled = 0;
    ExpandableHashMap<GeoCoord, GeoCoord> locationofPreviousWayPoint;
    queue<GeoCoord> coordqueue;
    coordqueue.push(start);
    vector<StreetSegment> segs;
    set<GeoCoord> visited;
    list<GeoCoord> correctpath;
    list<string> streetnames;
    
    if (m_streetmap->getSegmentsThatStartWith(start, segs) == false || m_streetmap->getSegmentsThatStartWith(end, segs) == false)
        return BAD_COORD;
    
    while (!coordqueue.empty())
    {
        //Pop the top coordinate off the stack. This gives you the current
        //        location that your algorithm is exploring.
        GeoCoord current = coordqueue.front();
        coordqueue.pop();
            
            //    If the current coordinate is equal to the ending coordinate,
            //        then we've solved it
        
        //check each street segment (current) stemming from temp GeoCoord
            m_streetmap->getSegmentsThatStartWith(current, segs);
        
        if (current == end)
        {
            GeoCoord* startcoord = locationofPreviousWayPoint.find(current);
            GeoCoord* temp = &current;
            correctpath.push_front(current);
            while ((*startcoord) != start)
            {
//                GeoCoord* temp
                startcoord = locationofPreviousWayPoint.find((*temp));
                correctpath.push_front((*startcoord));
                m_streetmap->getSegmentsThatStartWith((*startcoord), segs);
                for (int i = 0; i < segs.size(); i++)
                {
                    if (segs[i].end == (*temp))
                    {
                        streetnames.push_front(segs[i].name);
                    }
                }
                temp = startcoord;
                
            }
            while (!streetnames.empty())
            {
                GeoCoord s = correctpath.front();
                correctpath.pop_front();
                GeoCoord e = correctpath.front();
                totalDistanceTravelled += distanceEarthMiles(s, e);
                string streetname = streetnames.front();
                streetnames.pop_front();
                route.push_front(StreetSegment(s, e, streetname));
            }
            route.reverse();
            
            return DELIVERY_SUCCESS;
        }
        

        for (int i = 0; i < segs.size(); i++)
        {
            GeoCoord neighbor = segs[i].end;
//            GeoCoord* endptr = locationofPreviousWayPoint.find(neighbor);
            
            if (visited.find(neighbor) == visited.end())
            {
                coordqueue.push(neighbor);
                
                visited.insert(neighbor);
                            
                locationofPreviousWayPoint.associate(neighbor, current);
//                visited.insert(segs[i]); 
            }
        }
            
    }
    
    return NO_ROUTE;
}

//******************** PointToPointRouter functions ***************************

// These functions simply delegate to PointToPointRouterImpl's functions.
// You probably don't want to change any of this code.

PointToPointRouter::PointToPointRouter(const StreetMap* sm)
{
    m_impl = new PointToPointRouterImpl(sm);
}

PointToPointRouter::~PointToPointRouter()
{
    delete m_impl;
}

DeliveryResult PointToPointRouter::generatePointToPointRoute(
        const GeoCoord& start,
        const GeoCoord& end,
        list<StreetSegment>& route,
        double& totalDistanceTravelled) const
{
    return m_impl->generatePointToPointRoute(start, end, route, totalDistanceTravelled);
}
