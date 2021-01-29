#include "functions.h"

int main(int argc, char** argv)
{
    Point2Di p1 (0,0);
    Point2Di p2 (10,0);
    Point2Di p3 (0,10);
    Point2Di p4 (10,10);
    Point2Dd r1(1.0,1.0);

    Polygon2Di p;
    std::cout<<"size="<<p.size()<<std::endl;
    p.push_back(p1);
    p.push_back(p2);
    p.push_back(p3);
    std::pair<Point2Di, Point2Di> bb = p.getBoundingBox();
    std::cout<<bb.first<<" and "<<bb.second<<std::endl;

    //bool res= isInsidePolygon(p, p4);
    bool res= p.isInside(p4);
    std::cout<<res<<std::endl;
    
    std::vector<Point2Di> gd = GaussDigization(p);
    std::cout<<"gd.size()="<<gd.size()<<std::endl;
    return EXIT_SUCCESS;
}
