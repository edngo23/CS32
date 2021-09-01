#include "provided.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "ExpandableHashMap.h"
using namespace std;

bool loadDeliveryRequests(string deliveriesFile, GeoCoord& depot, vector<DeliveryRequest>& v);
bool parseDelivery(string line, string& lat, string& lon, string& item);

int main(int argc, char* argv[])
{
    //C:\CS32\Proj4Redo\Debug\Proj4Redo.exe C:\CS32\Proj4Redo\mapdata.txt C:\CS32\Proj4Redo\deliveries.txt

    /*ExpandableHashMap<GeoCoord, int> ex;
    GeoCoord geo1;
    GeoCoord geo2;
    GeoCoord geo3;
    GeoCoord geo4;
    GeoCoord geo5;
    GeoCoord geo6;
    GeoCoord geo7;
    GeoCoord geo8;
    GeoCoord geo9;
    geo1.latitudeText = "geo1";
    geo2.latitudeText = "geo2";
    geo3.latitudeText = "geo3";
    geo4.latitudeText = "geo4";
    geo5.latitudeText = "geo5";
    geo6.latitudeText = "geo6";
    geo7.latitudeText = "geo7";
    geo8.latitudeText = "geo8";
    geo9.latitudeText = "geo9";
    ex.associate(geo1, 1);
    ex.associate(geo2, 2);
    ex.associate(geo3, 3);
    ex.associate(geo4, 4);
    ex.associate(geo5, 5);
    ex.associate(geo6, 6);
    ex.associate(geo7, 7);
    ex.associate(geo8, 8);
    ex.associate(geo9, 9);
    cerr << ex.size() << endl;
    if (ex.find(geo9) == nullptr)
        cerr << "not there \n";
    else
        cerr << *(ex.find(geo9)) << endl;*/

    /*StreetMap s;
    s.load("mapdata.txt");

    PointToPointRouter p(&s);
    GeoCoord start("34.0547000", "-118.4794734");
    GeoCoord end("34.0434431", "-118.4918523");
    list<StreetSegment> ls;
    double d;
    p.generatePointToPointRoute(start, end, ls, d);*/

    // testing point to point routing
    //StreetMap s;

    //s.load("mapdata.txt");

    //GeoCoord gs("34.0625329", "-118.4470263");
    ////GeoCoord ge("34.0636671", "-118.4461842");
    //GeoCoord gr("34.0538308", "-118.4949545");
    //GeoCoord gr1("34.0412608", "-118.4396830");
    //// some random coord I picked ^^

    //PointToPointRouter a(&s);
    //list<StreetSegment> ss;
    //double d;
    //a.generatePointToPointRoute(gs, gr1, ss, d);

    //for (auto it = ss.begin(); it != ss.end(); it++)
    //{
    //    cerr << (*it).name << endl;
    //}
    //cerr << "distance of path: " << d << endl;

    /*vector<StreetSegment> v(0);

    GeoCoord geo("34.0411467", "-118.5001646");
    geo.latitude = 34.0411467;
    geo.longitude = -118.5001646;

    cerr << "Starting size: " << v.size() << endl;

    if (s.getSegmentsThatStartWith(geo, v))
    {
        cerr << "Ending size: " << v.size() << endl;
    }*/

    if (argc != 3)
    {
        cout << "Usage: " << argv[0] << " mapdata.txt deliveries.txt" << endl;
        return 1;
    }

    StreetMap sm;

    if (!sm.load(argv[1]))
    {
        cout << "Unable to load map data file " << argv[1] << endl;
        return 1;
    }

    GeoCoord depot;
    vector<DeliveryRequest> deliveries;
    if (!loadDeliveryRequests(argv[2], depot, deliveries))
    {
        cout << "Unable to load delivery request file " << argv[2] << endl;
        return 1;
    }

    cout << "Generating route...\n\n";

    DeliveryPlanner dp(&sm);
    vector<DeliveryCommand> dcs;
    double totalMiles;
    DeliveryResult result = dp.generateDeliveryPlan(depot, deliveries, dcs, totalMiles);
    if (result == BAD_COORD)
    {
        cout << "One or more depot or delivery coordinates are invalid." << endl;
        return 1;
    }
    if (result == NO_ROUTE)
    {
        cout << "No route can be found to deliver all items." << endl;
        return 1;
    }
    cout << "Starting at the depot...\n";
    for (const auto& dc : dcs)
        cout << dc.description() << endl;
    cout << "You are back at the depot and your deliveries are done!\n";
    cout.setf(ios::fixed);
    cout.precision(2);
    cout << totalMiles << " miles travelled for all deliveries." << endl;
}

bool loadDeliveryRequests(string deliveriesFile, GeoCoord& depot, vector<DeliveryRequest>& v)
{
    ifstream inf(deliveriesFile);
    if (!inf)
        return false;
    string lat;
    string lon;
    inf >> lat >> lon;
    inf.ignore(10000, '\n');
    depot = GeoCoord(lat, lon);
    string line;
    while (getline(inf, line))
    {
        string item;
        if (parseDelivery(line, lat, lon, item))
            v.push_back(DeliveryRequest(item, GeoCoord(lat, lon)));
    }
    return true;
}

bool parseDelivery(string line, string& lat, string& lon, string& item)
{
    const size_t colon = line.find(':');
    if (colon == string::npos)
    {
        cout << "Missing colon in deliveries file line: " << line << endl;
        return false;
    }
    istringstream iss(line.substr(0, colon));
    if (!(iss >> lat >> lon))
    {
        cout << "Bad format in deliveries file line: " << line << endl;
        return false;
    }
    item = line.substr(colon + 1);
    if (item.empty())
    {
        cout << "Missing item in deliveries file line: " << line << endl;
        return false;
    }
    return true;
}
