//#include "provided.h"
//#include "ExpandableHashMap.h"
//#include <iostream>
//#include <fstream>
//#include <sstream>
//#include <string>
//#include <vector>
//using namespace std;
//
//int main()  // p2p test
//{
//    StreetMap* s = new StreetMap;
//    s->load("/Users/Justin/Desktop/UCLA_TEXT/SECOND YEAR/WINTER20/CS 32/Proj4/mapdata.txt");
//
//    GeoCoord end = GeoCoord("34.0625329", "-118.4470263");
//    GeoCoord start = GeoCoord("34.0636671", "-118.4461842");
////    34.0625329 -118.4470263
////    34.0636671 -118.4461842
////    vector<StreetSegment> seg;
//    list<StreetSegment> seg;
//    double totalDistanceTravelled;
//    PointToPointRouter currentroute(s);
//    currentroute.generatePointToPointRoute(start, end, seg, totalDistanceTravelled);
////    34.0411322 -118.4984104
//
////    s->getSegmentsThatStartWith(start, seg);
//    list<StreetSegment>::iterator it = seg.begin();
//    while(it != seg.end())
//    {
//
//        cout << (*it).name << " " << (*it).start.latitude << " " << (*it).start.longitude << " " << (*it).end.latitude << " " << (*it).end.longitude << endl;
//        cout << endl;
//        it++;
//    }
//    cout << totalDistanceTravelled << " mi" << endl;
//}
//
//
////34.0625329 -118.4470263 34.0632405 -118.4470467 Broxton Ave
////34.0632405 -118.4470467 34.0636533 -118.4470480 Broxton Ave
////34.0636533 -118.4470480 34.0636625 -118.4464708 Le Conte Ave
////34.0636625 -118.4464708 34.0636671 -118.4461842 Le Conte Ave
