#include "functions.h"

#ifdef WITH_DGTAL
#include "DGtal/io/boards/Board2D.h"
#include "DGtal/io/Color.h"
#include "DGtal/helpers/StdDefs.h"

#include "DGtal/io/readers/MeshReader.h"
#include "DGtal/io/writers/MeshWriter.h"
#include "DGtal/io/viewers/Viewer3D.h"
#endif

void test2D(std::string filename) {
    //Read input polygon
    Polygon2Di poly = readFile2D<int>(filename);
    //Compute Gauss digitization of the polygon
    std::vector<Point2Di> gd = GaussDigization<int>(poly);
    
#ifdef WITH_DGTAL
    //std::cout<<"WITH_DGTAL"<<std::endl;
    int width = 5;
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
    board.saveSVG("out.svg");
#else
    std::cout<<"WITHOUT_DGTAL"<<std::endl;
#endif
    //Save result
    writeFile2D("out.txt", gd);
}

void test3D(std::string filename,int argc, char** argv) {
    
#ifdef WITH_DGTAL
    DGtal::Mesh<DGtal::Z3i::RealPoint> aMesh;
    DGtal::MeshReader<DGtal::Z3i::RealPoint>::importOFFFile(filename,aMesh);
    QApplication application(argc,argv);
    DGtal::Viewer3D<> viewer;
    viewer.show();
#endif
    
    std::vector<Point3Dd> Points;
    std::vector<std::vector<unsigned int> > Faces;
    Mesh3Dd mesh = readFile3D<double>(filename);
    Points = mesh.getVertices();
    Faces = mesh.getFaces();
    
    std::vector<Point3Di> gd = GaussDigization<double>(Points,Faces);
    
#ifdef WITH_DGTAL
    for (size_t it = 0; it < gd.size(); it++) {
        DGtal::Z3i::RealPoint voxel(gd.at(it).x(),gd.at(it).y(),gd.at(it).z());
        viewer.addCube(voxel,1);
    }

    viewer << DGtal::Viewer3D<>::updateDisplay;
    application.exec();
#endif
    //Save result
    writeFile3D("out.txt", gd);
}

int main(int argc, char** argv)
{
    std::string filename = "../Samples/circle10.2d";
    bool is2D = true;
    if(argc>=2) {
        filename = argv[1];//input
        if(argc==3) {
            std::string is3D = argv[2];
            if(is3D.compare("3D")==0)
               is2D = false;
        }
    }
    else
        std::cout<<"Usage: ./GaussDigitization input_filename [3D]"<<std::endl;
    
    if(!is2D)
        test3D(filename,argc,argv);
    else
        test2D(filename);
    
    return EXIT_SUCCESS;
}
