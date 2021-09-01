#include "provided.h"
#include "ExpandableHashMap.h"
#include <queue>
#include <list>
using namespace std;

//Have a container of node pointers, you have 3 containers, list, priority queue (parent), you keep them all so you can delete later
// < operator to compare the f value (heuristic)

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
    const StreetMap* m_streetMap;

    struct GeoCoordPackage
    {
    public:
        GeoCoordPackage(GeoCoord g, double distance)
        {
            m_geo = g;
            m_totalDistance = distance;
        }
        GeoCoord m_geo;
        double m_totalDistance;
    };

    struct GeoPriorityFunc
    {
    public:
        bool operator()(const GeoCoordPackage& g1, const GeoCoordPackage& g2) const
        {
            return g1.m_totalDistance > g2.m_totalDistance;
        }
    };
};

PointToPointRouterImpl::PointToPointRouterImpl(const StreetMap* sm)
{
    m_streetMap = sm;
}

PointToPointRouterImpl::~PointToPointRouterImpl()
{
}

DeliveryResult PointToPointRouterImpl::generatePointToPointRoute(
    const GeoCoord& start,
    const GeoCoord& end,
    list<StreetSegment>& route,
    double& totalDistanceTravelled) const
{
    vector<StreetSegment> connectedStreets;

    if (!m_streetMap->getSegmentsThatStartWith(start, connectedStreets) ||
        !m_streetMap->getSegmentsThatStartWith(end, connectedStreets))
        return BAD_COORD;

    ExpandableHashMap<GeoCoord, StreetSegment> previousWayPoints;
    //bool is placeholder for something more important?
    priority_queue<GeoCoordPackage, vector<GeoCoordPackage>, GeoPriorityFunc> queueToExplore;
    //priority_queue<GeoCoord> queueToExploreTest;
    
    cerr << "finding route \n";

    GeoCoordPackage geoStartP(start, 0.0);
    queueToExplore.push(geoStartP);
    geoStartP.m_totalDistance = 0;

    double streetLength = 0;

    while (!queueToExplore.empty())
    {
        GeoCoord tempGeoCoord = queueToExplore.top().m_geo;
        double distAlreadyTraveled = queueToExplore.top().m_totalDistance;
        queueToExplore.pop();
        if (tempGeoCoord == end)
        {
            while (tempGeoCoord != start)
            {
                route.push_front(*previousWayPoints.find(tempGeoCoord));
                tempGeoCoord = previousWayPoints.find(tempGeoCoord)->start;
            }
            totalDistanceTravelled = distAlreadyTraveled;
            cerr << "IT WORKED!\n";
            cerr << "Distance : " << totalDistanceTravelled << endl;
            return DELIVERY_SUCCESS;
        }

        if (m_streetMap->getSegmentsThatStartWith(tempGeoCoord, connectedStreets))
        {
            for (int i = 0; i < connectedStreets.size(); i++)
            {
                GeoCoord nextGeoCoord = connectedStreets[i].end;
                if (previousWayPoints.find(nextGeoCoord))
                    continue;
                previousWayPoints.associate(nextGeoCoord, connectedStreets[i]);
                streetLength = distanceEarthMiles(nextGeoCoord, tempGeoCoord);
                GeoCoordPackage tempGeoP(nextGeoCoord, distAlreadyTraveled + streetLength);
                queueToExplore.push(tempGeoP);
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
