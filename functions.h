#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <fstream>
#include <sstream>      // std::istringstream
#include <array>
#include <list>

#include "polygon.h"
#include "point.h"

#include "DGtal/helpers/StdDefs.h"
using namespace DGtal;

/* Source adapted from : https://www.geeksforgeeks.org/how-to-check-if-a-given-point-lies-inside-a-polygon/ */
// Given three colinear points p, q, r, the function checks if
// point q lies on line segment 'pr'
template<typename T>
T max(T v1, T v2) {
    return v1 > v2 ? v1 : v2;
}
template<typename T>
T min(T v1, T v2) {
    return v1 > v2 ? v2 : v1;
}
template<typename TPoint>
bool onSegment(TPoint p, TPoint q, TPoint r)
{
    if (q.x() <= max(p.x(), r.x()) && q.x() >= min(p.x(), r.x()) && q.y() <= max(p.y(), r.y()) && q.y() >= min(p.y(), r.y()))
        return true;
    return false;
}

// To find orientation of ordered triplet (p, q, r).
// The function returns following values
// 0 --> p, q and r are colinear
// 1 --> Clockwise
// 2 --> Counterclockwise
template<typename TPoint>
int orientation(TPoint p, TPoint q, TPoint r)
{
    int val = (q.y() - p.y()) * (r.x() - q.x()) - (q.x() - p.x()) * (r.y() - q.y());
    if (val == 0) return 0; // colinear
    return (val > 0)? 1: 2; // clock or counterclock wise
}

// The function that returns true if line segment 'p1q1'
// and 'p2q2' intersect.
template<typename TPoint>
bool doIntersect(TPoint p1, TPoint q1, TPoint p2, TPoint q2)
{
    // Find the four orientations needed for general and
    // special cases
    int o1 = orientation(p1, q1, p2);
    int o2 = orientation(p1, q1, q2);
    int o3 = orientation(p2, q2, p1);
    int o4 = orientation(p2, q2, q1);
    
    // General case
    if (o1 != o2 && o3 != o4)
        return true;
    
    // Special Cases
    // p1, q1 and p2 are colinear and p2 lies on segment p1q1
    if (o1 == 0 && onSegment(p1, p2, q1)) return true;
    
    // p1, q1 and p2 are colinear and q2 lies on segment p1q1
    if (o2 == 0 && onSegment(p1, q2, q1)) return true;
    
    // p2, q2 and p1 are colinear and p1 lies on segment p2q2
    if (o3 == 0 && onSegment(p2, p1, q2)) return true;
    
    // p2, q2 and q1 are colinear and q1 lies on segment p2q2
    if (o4 == 0 && onSegment(p2, q1, q2)) return true;
    
    return false; // Doesn't fall in any of the above cases
}

//Find direction that does not pass through any vertex of the polygon
template<typename T>
Point2D<T> findDirection(Polygon2D<T> polygon, Point2D<T> p, Point2D<T> d1, Point2D<T> d2 )
{
    Point2D<T> d = d1;
    bool belong = true;
    //Find the ray that doest pass by any vertices of polygon
    do {
        belong = false;
        for(size_t it=0; it<polygon.size(); it++) {
            // Check p is a vertex of polygon, then it is inside
            if(polygon.at(it)==p)
                return p;
            else {
                // Check if 'polygon[i]' belongs to the line segment from 'p' to 'extreme',
                // if it is then launch another ray using dichotomy search
                if(orientation(p, polygon.at(it), d)==0) {
                    d[0] = (d[0] + d2[0])/2;
                    d[1] = (d[1] + d2[1])/2;
                    belong = true;
                }
            }
        }
    } while (belong);
    return d;
}

// Returns true if the point p lies inside the polygon[] with n vertices
template<typename T>
bool isInsidePolygon(Polygon2D<T> polygon, Point2D<T> p)
{
    typedef Point2D<T> TPoint;
    int n = polygon.size();
    // There must be at least 3 vertices in polygon[]
    if (n < 3) return false;
    
    // Create a point for line segment from p to infinite
    // Find the direction without containing any polygon vertices
    TPoint extreme1 (p.x(), INT_MAX/1e6);
    TPoint extreme2 (INT_MAX/1e6, p.y());
    TPoint extreme = findDirection(polygon, p, extreme1, extreme2);
    
    // Check p is a vertex of polygon, then it is inside
    if(extreme==p)
        return true;
    // Count intersections of the above line with sides of polygon
    int count = 0, i = 0;
    do
    {
        int next = (i+1)%n;
        
        // Check if the line segment from 'p' to 'extreme' intersects
        // with the line segment from 'polygon[i]' to 'polygon[next]'
        if (doIntersect(polygon[i], polygon[next], p, extreme))
        {
            // If the point 'p' is colinear with line segment 'i-next',
            // then check if it lies on segment. If it lies, return true,
            // otherwise false
            if (orientation(polygon[i], p, polygon[next]) == 0)
                return onSegment(polygon[i], p, polygon[next]);
            
            count++;
        }
        i = next;
    } while (i != 0);
    
    // Return true if count is odd, false otherwise
    return count&1; // Same as (count%2 == 1)
}

template<typename T>
std::vector<Point2Di> GaussDigization(Polygon2D<T> polygon)
{
    typedef Point2D<T> TPoint;
    std::vector<Point2Di> gd;
    std::pair<TPoint, TPoint> bb = polygon.getBoundingBox();
    int minx = int(bb.first.x())-1;
    int miny = int(bb.first.y())-1;
    int maxx = int(bb.second.x())+1;
    int maxy = int(bb.second.y())+1;
    for(int x = minx; x<maxx; x++) {
        for(int y = miny; y<maxy; y++) {
            TPoint p(x,y);
            if(polygon.isInside(p)) {
                Point2Di pi(int(p.x()),int(p.y()));
                gd.push_back(pi);
            }
        }
    }
    return gd;
}

template<typename T>
Polygon2D<T> readFile2D(const std::string filename)
{
    typedef Point2D<T> TPoint;
    Polygon2D<T> polyline;
    std::string line;
    std::ifstream file (filename);
    if (file.is_open())
    {
        T x, y;
        while ( file >> x >> y )
        {
            TPoint p(x,y);
            polyline.push_back(p);
        }
        file.close();
    }
    else {
        std::cout << "Unable to open file"<<std::endl;
        exit(EXIT_FAILURE);
    }
    return polyline;
}

template<typename T >
Mesh3D<T> readFile3D(const std::string filename)
{
    typedef Point3D<T> TPoint;
    Mesh3D<T> mesh;
    std::ifstream infile (filename);
    if (infile.is_open())
    {
        std::string str;
        getline( infile, str );
        if ( ! infile.good() )
        {
            std::cout << "Unable to read file"<<std::endl;
            exit(EXIT_FAILURE);
        }
        if ( str.substr(0,3) != "OFF" && str.substr(0,4) != "NOFF")
        {
            std::cerr <<"*" <<str<<"*"<< std::endl;
            std::cout << "No OFF or NOFF format in " << filename << std::endl;
        }
        if ( str.substr(0,4) == "NOFF")
        {
            std::cout << "Reading NOFF format (normal vectors will be ignored)..." << std::endl;
        }
        // Processing comments
        do
        {
            getline( infile, str );
            if ( ! infile.good() ){
                std::cout << "Invalid format in " << filename << std::endl;
            }
        }
        while ( str[ 0 ] == '#' || str=="");
        std::istringstream str_in( str );
        int nbPoints, nbFaces, nbEdges;
        str_in >> nbPoints;
        str_in >> nbFaces;
        str_in >> nbEdges;
        
        // Reading mesh vertex
        for(int i=0; i<nbPoints; i++){
            TPoint p;
            infile >> p[0];
            infile >> p[1];
            infile >> p[2];
            mesh.addVertex(p);
            // Needed since a line can also contain vertex colors
            getline(infile, str);
        }
        
        // Reading mesh faces
        for(int i=0; i<nbFaces; i++){
            // Reading the number of face vertex
            unsigned int aNbFaceVertex;
            infile >> aNbFaceVertex;
            std::vector<unsigned int> aFace;
            for (unsigned int j=0; j< aNbFaceVertex; j++){
                unsigned int anIndex;
                infile >> anIndex;
                aFace.push_back(anIndex);
            }
            // Needed since a can also contain vertex colors
            getline(infile, str);
            mesh.addFace(aFace);
        }
        infile.close();
    }
    else {
        std::cout << "Unable to open file"<<std::endl;
        exit(EXIT_FAILURE);
    }
    return mesh;
}

template<typename T>
void writeFile2D(std::string filename, std::vector<Point2D<T> > pts)
{
    typedef Point2D<T> TPoint;
    
    std::string line;
    std::ofstream file (filename);
    if (file.is_open())
    {
        for(size_t it=0; it<pts.size(); it++) {
            TPoint p = pts.at(it);
            file << p.x() << " " << p.y() << std::endl;
        }
    }
    else {
        std::cout << "Unable to open file"<<std::endl;
        exit(EXIT_FAILURE);
    }
}

template<typename T>
void writeFile3D(std::string filename, std::vector<Point3D<T> > pts)
{
    typedef Point3D<T> TPoint;
    
    std::string line;
    std::ofstream file (filename);
    if (file.is_open())
    {
        for(size_t it=0; it<pts.size(); it++) {
            TPoint p = pts.at(it);
            file << p.x() << " " << p.y() << " " << p.z() << std::endl;
        }
    }
    else {
        std::cout << "Unable to open file"<<std::endl;
        exit(EXIT_FAILURE);
    }
}

template<typename TPoint>
TPoint produitVect(TPoint vect1,TPoint vect2){
    return TPoint(vect1[1]*vect2[2]-vect1[2]*vect2[1],vect1[2]*vect2[0]-vect1[0]*vect2[2],vect1[0]*vect2[1]-vect1[1]*vect2[0]);
}

template<typename TPoint>
double produitCart(TPoint vect1,TPoint vect2){
    return vect1[0]*vect2[0]+vect1[1]*vect2[1]+vect1[2]*vect2[2];
}

template<typename TPoint>
double norm(TPoint vect){
    return produitCart(vect,vect);
}

//Verify whether P is belong to the plan (P1P2P3)
template<typename TPoint>
bool isInside(TPoint P1, TPoint P2, TPoint P3, Point3Dd P){
    Point3Dd P1P2 (P2[0]-P1[0],P2[1]-P1[1],P2[2]-P1[2]);
    Point3Dd P1P3 (P3[0]-P1[0],P3[1]-P1[1],P3[2]-P1[2]);
    Point3Dd P2P3 (P3[0]-P2[0],P3[1]-P2[1],P3[2]-P2[2]);
    Point3Dd P3P1 (P1[0]-P3[0],P1[1]-P3[1],P1[2]-P3[2]);
    Point3Dd P1P (P[0]-P1[0],P[1]-P1[1],P[2]-P1[2]);
    Point3Dd P2P (P[0]-P2[0],P[1]-P2[1],P[2]-P2[2]);
    Point3Dd P3P (P[0]-P3[0],P[1]-P3[1],P[2]-P3[2]);
    TPoint n = produitVect(P1P2,P1P3);
    TPoint n1 = produitVect(P2P3,P2P);
    TPoint n2 = produitVect(P3P1,P3P);
    TPoint n3 = produitVect(P1P2,P1P);
    double alpha = produitCart(n,n1)/norm(n);
    double beta = produitCart(n,n2)/norm(n);
    double gamma = produitCart(n,n3)/norm(n);
    if(alpha >=0 and alpha <=1 and beta >=0 and beta <=1 and gamma >=0 and gamma <=1)
        return true;
    return false;
}

//Get param (A,B,C,D) of the plan (P1P2P3) : Ax+By+Cz=D
template<typename T>
std::array<T,4> getPlanEquation(Point3D<T> P1, Point3D<T> P2, Point3D<T> P3){
    Point3D<T> P1P2 (P2[0]-P1[0],P2[1]-P1[1],P2[2]-P1[2]);
    Point3D<T> P1P3 (P3[0]-P1[0],P3[1]-P1[1],P3[2]-P1[2]);
    Point3D<T> produit = produitVect(P1P2,P1P3);
    T d = produitCart(produit,P1);
    return {produit[0],produit[1],produit[2],d};
}

//Return the intersecting point between Plan and the ray (PV)
template<typename T>
bool intersection(std::array<T,4> Plan, Point3D<T> P, Point3D<T> V, Point3Dd& PI){
    double a = Plan[0];
    double b = Plan[1];
    double c = Plan[2];
    double d = Plan[3];
    double det = (a*V[0]+b*V[1]+c*V[2]);
    if (det == 0)
        return  false;
    else{
        double t = (d-(a*P[0]+b*P[1]+c*P[2]))/det;
        double x = P[0]+t*V[0];
        double y = P[1]+t*V[1];
        double z = P[2]+t*V[2];
        PI = Point3Dd(x,y,z);
        return true;
    }
}

struct FacePoint
{
    unsigned int id1;
    unsigned int id2;
    unsigned int id3;
    Point3Dd point;
};

//Get the intersecting points inside a face together with the intersecting face (of Faces)
template<typename T>
std::list<FacePoint> listFaces(std::vector<Point3D<T> > Vertices,std::vector<std::vector<unsigned int > > Faces, Point3D<T> P, Point3D<T> V){
    typedef Point3D<T> TPoint;
    std::list<FacePoint> listDeFaces;
    FacePoint myFacePoint;
    for (auto face = Faces.begin(); face != Faces.end(); ++face){
        myFacePoint.id1 = (*face)[0];
        myFacePoint.id2 = (*face)[1];
        myFacePoint.id3 = (*face)[2];
        TPoint P1 = Vertices[myFacePoint.id1];
        TPoint P2 = Vertices[myFacePoint.id2];
        TPoint P3 = Vertices[myFacePoint.id3];
        std::array<T,4> Plan = getPlanEquation(P1,P2,P3);
        Point3Dd PI;
        bool res = intersection(Plan,P,V,PI);
        if(res) {
            myFacePoint.point = PI;
            if(isInside(P1,P2,P3,myFacePoint.point))
                listDeFaces.push_back(myFacePoint);
        }
    }
    return listDeFaces;
}

std::pair<Z3i::RealPoint,Z3i::RealPoint> getBoundingBox(std::vector<Z3i::RealPoint> Points){
    double minX,minY,minZ,maxX,maxY,maxZ;
    auto point = Points.begin();
    minX = (*point)[0]; minY = (*point)[1]; minZ = (*point)[2];
    maxX = (*point)[0]; maxY = (*point)[1]; maxZ = (*point)[2];
    ++point;
    for (; point != Points.end(); ++point){
        if ((*point)[0]<minX) minX = (*point)[0];
        if ((*point)[0]>maxX) maxX = (*point)[0];
        
        if ((*point)[1]<minY) minY = (*point)[1];
        if ((*point)[1]>maxY) maxY = (*point)[1];
        
        if ((*point)[2]<minZ) minZ = (*point)[2];
        if ((*point)[2]>maxZ) maxZ = (*point)[2];
    }
    Z3i::RealPoint p1(minX,minY,minZ);
    Z3i::RealPoint p2(maxX,maxY,maxZ);
    return std::make_pair(p1, p2);
}

//Get all intersecting rays to mesh
template<typename TPoint>
std::vector<Point3Dd> getRays(std::pair<TPoint,TPoint> bb,TPoint P,TPoint V,double alpha){//Plan normal of direction V passing through P (what is alpha ?)
    std::vector<Point3Dd> rays;
    double x0 = P[0];
    double y0 = P[1];
    double z0 = P[2];
    double xmi,xma,ymi,yma,zmi,zma;
    double d = produitCart(V,P);
    xmi = bb.first[0];
    ymi = bb.first[1];
    zmi = bb.first[2];
    xma = bb.second[0];
    yma = bb.second[1];
    zma = bb.second[2];
    if( V[0]!=0){ //V=(1,0,0) : x direction
        double ry0 = (y0 - ((int) y0/alpha )*alpha); double rz0 = (z0 - ((int) z0/alpha )*alpha);
        for (double y = ((int) ((ymi - ry0 )/alpha) ) * alpha + ry0; y < yma; y += alpha ) {
            for (double z = ((int) ((zmi - rz0 )/alpha) ) * alpha + rz0; z < zma; z += alpha ) {
                Point3Dd p(0,y,z);
                rays.push_back(Point3Dd(d-produitCart(V,p),y,z));
            }
        }
        return rays;
    } else if(V[1]!=0) {//V=(0,1,0) : y direction
        double rx0 = (x0 - ((int) x0/alpha )*alpha); double rz0 = (z0 - ((int) z0/alpha )*alpha);
        for (double x = ((int) ((xmi - rx0 )/alpha) ) * alpha + rx0; x < xma; x += alpha ) {
            for (double z = ((int) ((zmi - rz0 )/alpha) ) * alpha + rz0; z < zma; z += alpha ) {
                Point3Dd p(x,0,z);
                rays.push_back(Point3Dd(x,d-produitCart(V,p),z));
            }
        }
        return rays;
    } else { //V=(0,0,1) : z direction
        double rx0 = (x0 - ((int) x0/alpha )*alpha); double ry0 = (y0 - ((int) y0/alpha )*alpha);
        for (double x = ((int) ((xmi - rx0 )/alpha) ) * alpha + rx0; x < xma; x += alpha ) {
            for (double y = ((int) ((ymi - ry0 )/alpha) ) * alpha + ry0; y < yma; y += alpha ) {
                Point3Dd p(x,y,0);
                rays.push_back(Point3Dd(x,y,d-produitCart(V,p)));
            }
        }
        return rays;
    }
}

bool compareFaceX(FacePoint& face1, FacePoint& face2){
    return ( face1.point[0] < face2.point[0] );
}

bool compareFaceY(FacePoint& face1, FacePoint& face2){
    return ( face1.point[1] < face2.point[1] );
}

bool compareFaceZ(FacePoint& face1, FacePoint& face2){
    return ( face1.point[2] < face2.point[2] );
}

//Get all integer points in between p1 and p2
template<typename TPoint>
std::vector<Point3Di> intBetweenPoints(TPoint p1, TPoint p2)
{
    TPoint v (p2[0]-p1[0],p2[1]-p1[1],p2[2]-p1[2]);
    std::vector<Point3Di> Points;
    int x0 = 0, y0 = 0, z0 = 0;
    if(p1[0] == (int)p1[0]) x0 = (int)p1[0]; else if(p1[0] < 0) x0 = (int) p1[0];else x0 = (int) p1[0] + 1;
    if(p1[1] == (int)p1[1]) y0 = (int)p1[1]; else if(p1[1] < 0) y0 = (int) p1[1];else y0 = (int) p1[1] + 1;
    if(p1[2] == (int)p1[2]) z0 = (int)p1[2]; else if(p1[2] < 0) z0 = (int) p1[2];else z0 = (int) p1[2] + 1;
    if(v[0] != 0) {
        for (int x = x0; x <= p2[0]; x++) {
            double y = p1[1] + (x-p1[0])/v[0]*v[1];
            double z = p1[2] + (x-p1[0])/v[0]*v[2];
            if(y == (int)y && z == (int)z)
                Points.push_back(Point3Di(x,y,z));
        }
    } else if(v[1] != 0) {
        for (double y = y0; y <= p2[1]; y++) {
            double x = p1[0] + (y-p1[1])/v[1]*v[0];
            double z = p1[2] + (y-p1[1])/v[1]*v[2];
            if(x == (int)x && z == (int)z)
                Points.push_back(Point3Di(x,y,z));
        }
    } else {
        for (double z = z0; z <= p2[2]; z++) {
            double x = p1[0] + (z-p1[2])/v[2]*v[0];
            double y = p1[1] + (z-p1[2])/v[2]*v[1];
            if(x == (int)x && y == (int)y)
                Points.push_back(Point3Di(x,y,z));
        }
    }
    return Points;
}

template<typename T>
std::vector<Point3Di> GaussDigization(std::vector<Point3D<T> > Vertices, std::vector<std::vector<unsigned int> > Faces)
{
    typedef Point3D<T> TPoint;
    std::vector<Point3Di> gd;
    Mesh3D<T> mesh(Vertices,Faces);
    std::pair<TPoint, TPoint> bb = mesh.getBoundingBox();
    
    std::list<FacePoint> myListe;
    std::vector<Point3Dd> listePoints;
    Point3Dd V (0,1,0);
    Point3Dd P0 (0,0,0);
    std::vector<Point3Dd> rayons = getRays(bb,P0,V,1);
    unsigned long nbrRays = rayons.size();
    unsigned long taille;
    int i = 1;
    Point3Dd p1,p2;
    for (auto &rayon : rayons) {
        myListe = listFaces(Vertices,Faces,rayon,V);
        if(V[2] != 0) //x direction
            myListe.sort(compareFaceZ);
        else {
            if(V[1] != 0) //y direction
                myListe.sort(compareFaceY);
            else //z direction
                myListe.sort(compareFaceX);
        }
        for (auto &it : myListe) {
            listePoints.push_back(it.point);
        }
        taille = listePoints.size();
        if( taille%2 == 1) {
            exit(0);
        }
        else
            if(taille!=0)
                for (int j = 0; j < taille/2; ++j){
                    p1 = listePoints[2*j];
                    p2 = listePoints[2*j+1];
                    std::vector<Point3Di> pointsEntiers = intBetweenPoints(p1,p2);
                    for (auto &pointsEntier : pointsEntiers)
                        gd.push_back(pointsEntier);
                }
        myListe.clear();
        listePoints.clear();
        i++;
    }
    return gd;
}

#endif // FUNCTIONS_H
