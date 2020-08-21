////
////  teststreetmap.cpp
////  goober eats
////
////  Created by Rodger Lee on 3/9/20.
////  Copyright © 2020 Rodger Lee. All rights reserved.
////
//
//#include "provided.h"
//#include "ExpandableHashMap.h"
//#include <iostream>
//#include <fstream>
//#include <sstream>
//#include <string>
//#include <vector>
//using namespace std;
//
//int main()  //test streetmap.cpp
//{
//    StreetMap s;
//    s.load("/Users/Justin/Desktop/UCLA_TEXT/SECOND YEAR/WINTER20/CS 32/Proj4/mapdata.txt");
//
//    GeoCoord gc = GeoCoord("34.0411322", "-118.4984104");
//    vector<StreetSegment> seg;
////    34.0411322 -118.4984104
//    if(s.getSegmentsThatStartWith(gc, seg))
//        cerr << "exists" << endl;
//    for (int i = 0; i < seg.size(); i++)
//    {
//        cout << "Segment " << i+1 << endl;
//        cout << seg[i].name << " " << seg[i].start.latitude << " " << seg[i].start.longitude << " " << seg[i].end.latitude << " " << seg[i].end.longitude << endl;
//    }
//}
