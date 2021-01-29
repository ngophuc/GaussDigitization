#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <fstream>

#include "polygon.h"
#include "point.h"

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
Polygon2D<T> readFile2D(std::string filename)
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
Polygon3D<T> readFile3D(std::string filename)
{
    typedef Point3D<T> TPoint;
    Polygon3D<T> polyline;
    std::string line;
    std::ifstream file (filename);
    if (file.is_open())
    {
        T x, y, z;
        while ( file >> x >> y >> z)
        {
            TPoint p(x,y, z);
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

#endif // FUNCTIONS_H
