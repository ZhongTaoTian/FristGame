#ifndef _GeometricRecognizerTypesIncluded_
#define _GeometricRecognizerTypesIncluded_

#include <math.h>
#include <string>
#include <list>
#include <vector>

using namespace std;

namespace DollarRecognizer
{
	class Point2D
	{
	public:
		//--- Wobbrock used doubles for these, not ints
		//int x, y;
		double x, y;
		Point2D() 
		{
			this->x=0; 
			this->y=0;
		}
		Point2D(double x, double y)
		{
			this->x = x;
			this->y = y;
		}
	};

	typedef vector<Point2D>  Path2D;
	typedef Path2D::iterator Path2DIterator;

	class Rectangle
	{
	public:
		double x, y, width, height;
		Rectangle(double x, double y, double width, double height)
		{
			this->x = x;
			this->y = y;
			this->width = width;
			this->height = height;
		}
	};

	class RecognitionResult
	{
	public:
		string name;
		double score;
		RecognitionResult(string name, double score)
		{
			this->name = name;
			this->score = score;
		}
	};
}
#endif
