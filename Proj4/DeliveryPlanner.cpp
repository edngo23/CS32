#include "provided.h"
#include <vector>
using namespace std;

class DeliveryPlannerImpl
{
public:
    DeliveryPlannerImpl(const StreetMap* sm);
    ~DeliveryPlannerImpl();
    DeliveryResult generateDeliveryPlan(
        const GeoCoord& depot,
        const vector<DeliveryRequest>& deliveries,
        vector<DeliveryCommand>& commands,
        double& totalDistanceTravelled) const;
private:
    PointToPointRouter m_pToPObj;
    DeliveryOptimizer m_DelivOptimizer;
    string angleOfSegment(const StreetSegment& s) const;
};

DeliveryPlannerImpl::DeliveryPlannerImpl(const StreetMap* sm)
    :m_pToPObj(sm), m_DelivOptimizer(sm)
{
}

DeliveryPlannerImpl::~DeliveryPlannerImpl()
{
}

DeliveryResult DeliveryPlannerImpl::generateDeliveryPlan(
    const GeoCoord& depot,
    const vector<DeliveryRequest>& deliveries,
    vector<DeliveryCommand>& commands,
    double& totalDistanceTravelled) const
{
    totalDistanceTravelled = 0;
    GeoCoord start = depot;
    double distance = 0, angleBetweenTwoSegs = 0, distanceProceeded = 0, oldCrowDist, newCrowDist;
    DeliveryResult result;
    vector<DeliveryRequest> deliveriesAndDepot(deliveries);
    deliveriesAndDepot.push_back(DeliveryRequest("", depot));
    m_DelivOptimizer.optimizeDeliveryOrder(depot, deliveriesAndDepot, oldCrowDist, newCrowDist);
    cerr << "old crow dist: " << oldCrowDist << endl; 
    cerr << "new crow dist: " << newCrowDist << endl;
    DeliveryCommand cmd;
    for (int i = 0; i < deliveriesAndDepot.size(); i++)
    {
        list<StreetSegment> route;
        result = m_pToPObj.generatePointToPointRoute(start, deliveriesAndDepot[i].location, route, distance);
        if (result != DELIVERY_SUCCESS)
            return result;
        auto it = route.begin();
        while (it != route.end())
        {
            auto secondIterator = it;
            secondIterator++;
            cerr << "it name: " << (*it).name << endl;
            cerr << "second name: " << (*secondIterator).name << endl;
            //do proceeds
            if (it != route.end() && secondIterator != route.end())
            {
                cerr << "We shall proceed \n";
                secondIterator--;
                cmd.initAsProceedCommand(angleOfSegment(*it), (*it).name, distanceProceeded);
                while (secondIterator != route.end() && (*it).name == (*secondIterator).name)
                {
                    cmd.increaseDistance(distanceEarthMiles((*secondIterator).start, (*secondIterator).end));
                    secondIterator++;
                    cerr << "Increased distance \n";
                }
                commands.push_back(cmd);
            }
            //do turns (repeated if statement to make sure I don't dereference a nullptr
            if(it != route.end() && secondIterator != route.end())
            {
                angleBetweenTwoSegs = angleBetween2Lines(*it, *secondIterator);
                cerr << "Got angle: " << angleBetweenTwoSegs << endl;
                if (angleBetweenTwoSegs >= 1 && angleBetweenTwoSegs < 180)
                {
                    cmd.initAsTurnCommand("left", (*secondIterator).name);
                    cerr << "Turn left \n";
                }
                else if (angleBetweenTwoSegs >= 180 && angleBetweenTwoSegs <= 359)
                {
                    cmd.initAsTurnCommand("right", (*secondIterator).name);
                    cerr << "Turn right \n";
                }
                commands.push_back(cmd);
            }
            it = secondIterator;
        }
        cerr << "add this distance please: " << distance << endl;
        if (deliveriesAndDepot[i].location != depot)
        {
            cmd.initAsDeliverCommand(deliveriesAndDepot[i].item);
            commands.push_back(cmd);
            cerr << "Delivered something \n";
        }
        totalDistanceTravelled += distance;
        start = deliveriesAndDepot[i].location;
    }
    return DELIVERY_SUCCESS;
}

inline
string DeliveryPlannerImpl::angleOfSegment(const StreetSegment& s) const
{
    double angle = angleOfLine(s);
    if (angle >= 0 && angle < 22.5)
        return "east";
    else if (angle >= 22.5 && angle < 67.5)
        return "northeast";
    else if (angle >= 67.5 && angle < 112.5)
        return "north";
    else if (angle >= 112.5 && angle < 157.5)
        return "northwest";
    else if (angle >= 157.5 && angle < 202.5)
        return "west";
    else if (angle >= 202.5 && angle < 247.5)
        return "southwest";
    else if (angle >= 247.5 && angle < 292.5)
        return "south";
    else if (angle >= 292.5 && angle < 337.5)
        return "southeast";
    else if (angle >= 337.5)
        return "east";
    return "BAD";
}

//******************** DeliveryPlanner functions ******************************

// These functions simply delegate to DeliveryPlannerImpl's functions.
// You probably don't want to change any of this code.

DeliveryPlanner::DeliveryPlanner(const StreetMap* sm)
{
    m_impl = new DeliveryPlannerImpl(sm);
}

DeliveryPlanner::~DeliveryPlanner()
{
    delete m_impl;
}

DeliveryResult DeliveryPlanner::generateDeliveryPlan(
    const GeoCoord& depot,
    const vector<DeliveryRequest>& deliveries,
    vector<DeliveryCommand>& commands,
    double& totalDistanceTravelled) const
{
    return m_impl->generateDeliveryPlan(depot, deliveries, commands, totalDistanceTravelled);
}
