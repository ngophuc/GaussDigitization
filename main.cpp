#include "functions.h"

#ifdef WITH_DGTAL
#include "DGtal/io/boards/Board2D.h"
#include "DGtal/io/Color.h"
#include "DGtal/helpers/StdDefs.h"
#endif

int main(int argc, char** argv)
{
    Polygon2Di poly = readFile2D<int>("pts.2d");
    std::cout<<"poly1.size()="<<poly.size()<<poly<<std::endl;
    std::vector<Point2Di> gd = GaussDigization(poly);
    std::cout<<"gdr.size()="<<gd.size()<<std::endl;
    
#ifdef WITH_DGTAL
    //std::cout<<"WITH_DGTAL"<<std::endl;
    int width = 1;
    int border = 2;
    std::pair<Point2Di, Point2Di> bb = poly.getBoundingBox();
    DGtal::Z2i::Point d1( bb.first.x() - border, bb.first.y() - border );
    DGtal::Z2i::Point d2( bb.second.x() + border , bb.second.y() + border);
    DGtal::Z2i::Domain domain( d1, d2 );
    //Draw the Gauss digitization result
    DGtal::Board2D board;
    board << domain;
    board << DGtal::SetMode("PointVector", "Both");
    for(size_t it=0; it<gd.size(); it++) {
        DGtal::Z2i::Point p(gd.at(it).x(),gd.at(it).y());
        board << p;
    }
    //Draw the polyline
    board.setPenColor(DGtal::Color::Red);
    board.setLineWidth(width);
    for(size_t it=0; it<poly.size()-1; it++) {
        DGtal::Z2i::RealPoint p1 (poly.at(it).x(),poly.at(it).y());
        DGtal::Z2i::RealPoint p2 (poly.at(it+1).x(),poly.at(it+1).y());
        board.drawLine(p1[0],p1[1],p2[0],p2[1]);
    }
    DGtal::Z2i::RealPoint p1 (poly.front().x(),poly.front().y());
    DGtal::Z2i::RealPoint p2 (poly.back().x(),poly.back().y());
    board.drawLine(p1[0],p1[1],p2[0],p2[1]);
    //Save result
    board.saveSVG("gd.svg");
#else
    ;//std::cout<<"WITHOUT_DGTAL"<<std::endl;
#endif
    writeFile2D("gd.txt", gd);
    return EXIT_SUCCESS;
}
