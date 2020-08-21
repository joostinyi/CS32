#include "provided.h"
#include <vector>
#include <iterator>
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
    string proceedDir(double angle) const; // generates direction of angle for proceed
    void proceed(list<StreetSegment> route, list<StreetSegment>::iterator& it) const; // return iterator at position of turn
    string turnDir(double angle) const; // generates direction of angle for turn
    void convertToCommands(list<StreetSegment> route, vector<DeliveryCommand>& commands) const; // converts route StreetSegments to commands
    const DeliveryOptimizer dOp;
    const PointToPointRouter p2p;
    const StreetMap* map;
};

DeliveryPlannerImpl::DeliveryPlannerImpl(const StreetMap* sm)
: dOp(sm), p2p(sm), map(sm){}

DeliveryPlannerImpl::~DeliveryPlannerImpl()
{
    delete map;
}

string DeliveryPlannerImpl::proceedDir(double angle) const
{
    if(0 <= angle && angle < 22.5)
        return "east";
    else if(angle < 67.5)
        return "northeast";
    else if(angle < 112.5)
        return "north";
    else if(angle < 157.5)
        return "northwest";
    else if(angle < 202.5)
        return "west";
    else if(angle < 247.5)
        return "southwest";
    else if(angle < 292.5)
        return "south";
    else if(angle < 337.5)
        return "southeast";
    else
        return "east";
}

string DeliveryPlannerImpl::turnDir(double angle) const
{
    if(angle >= 1 && angle < 180)
        return "left";
    else if (angle >= 180 && angle <= 359)
        return "right";
    return "turn dir error";
}

void DeliveryPlannerImpl::proceed(list<StreetSegment> route, list<StreetSegment>::iterator& it) const
{
    string prevStreet = it->name;
    while(it != route.end())
    {
        if(it->name == prevStreet)
            it++;
        else
        {
            if(it == route.begin())
            {
                prevStreet = it->name;
                it++;
            }
            double angle = angleBetween2Lines(*it, *prev(it,1));;
            if(angle < 1 || angle > 359)
                it++;
            else
                return;
        }
    }
}

void DeliveryPlannerImpl::convertToCommands(list<StreetSegment> route, vector<DeliveryCommand>& commands) const
{
    auto it = route.begin();
    while(it != route.end())
    {
        DeliveryCommand curr = DeliveryCommand();
        GeoCoord startCoord = it->start;
        proceed(route, it);
        if(it != route.end())
        {
            curr.initAsProceedCommand(proceedDir(angleOfLine(StreetSegment(startCoord,it->start, prev(it,1)->name))), prev(it,1)->name, distanceEarthMiles(startCoord, prev(it,1)->end));
            commands.push_back(curr);
            double angle = angleBetween2Lines(*prev(it,1), *it);
            curr.initAsTurnCommand(turnDir(angle), it->name);
            commands.push_back(curr);
        }
        else
        {
            curr.initAsProceedCommand(proceedDir(angleOfLine(StreetSegment(startCoord,prev(it,1)->end, prev(it,1)->name))),prev(it,1)->name, distanceEarthMiles(startCoord, prev(it,1)->end));
            commands.push_back(curr);
            return;
        }
    }
}


DeliveryResult DeliveryPlannerImpl::generateDeliveryPlan(
                                                         const GeoCoord& depot,
                                                         const vector<DeliveryRequest>& deliveries,
                                                         vector<DeliveryCommand>& commands,
                                                         double& totalDistanceTravelled) const
{
    double oldCrowDistance, newCrowDistance;
    auto optDeliveries = deliveries;
    dOp.optimizeDeliveryOrder(depot, optDeliveries, oldCrowDistance, newCrowDistance);
    list<StreetSegment> deliveryRoute;
    DeliveryCommand end = DeliveryCommand();
    double startDist;
    DeliveryResult startFlag = p2p.generatePointToPointRoute(depot, deliveries[0].location, deliveryRoute, startDist);
    if(startFlag == NO_ROUTE || startFlag == BAD_COORD)
        return NO_ROUTE;
    totalDistanceTravelled += startDist;
    vector<DeliveryCommand> tempCommands;
    convertToCommands(deliveryRoute, tempCommands);
    commands.insert(commands.end(), tempCommands.begin(), tempCommands.end());
    end.initAsDeliverCommand(deliveries[0].item);
    commands.push_back(end);
    tempCommands.clear();
    deliveryRoute.clear();
    for(int i = 1; i < deliveries.size(); i++)
    {
        deliveryRoute.clear();
        tempCommands.clear();
        double tempDist;
        DeliveryResult tempFlag = p2p.generatePointToPointRoute(deliveries[i-1].location, deliveries[i].location, deliveryRoute, tempDist);
        totalDistanceTravelled += tempDist;
        if(tempFlag == NO_ROUTE || tempFlag == BAD_COORD)
            return NO_ROUTE;
        convertToCommands(deliveryRoute, tempCommands);
        commands.insert(commands.end(), tempCommands.begin(), tempCommands.end());
        end.initAsDeliverCommand(deliveries[i].item);
        commands.push_back(end);
    }
    list<StreetSegment> endRoute;
    double endDist;
    DeliveryResult endFlag = p2p.generatePointToPointRoute(deliveries[deliveries.size()-1].location, depot, endRoute, endDist);
    if(endFlag == NO_ROUTE || endFlag == BAD_COORD)
        return NO_ROUTE;
    totalDistanceTravelled += endDist;
    convertToCommands(endRoute, tempCommands);
    commands.insert(commands.end(), tempCommands.begin(), tempCommands.end());
    tempCommands.clear();
    return DELIVERY_SUCCESS;
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
