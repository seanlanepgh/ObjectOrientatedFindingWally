#ifndef BaseImage_H
#define BaseImage_H 
#include <vector>

using namespace std;
class BaseImage {
public:
	//Default Constructor 
	BaseImage();
	//BaseImage Constructor that takes the number of rows and columns
	BaseImage::BaseImage(int rowSize, int columnSize);
	//BaseImage Constructor that takes the number of rows , columns and the data 
	BaseImage(int rowSize, int columnSize, double* inputData);
	//BaseImage Deconstructor
	~BaseImage();
	//BaseImage Copy Constructor
	BaseImage(const BaseImage& baseImageCopy);
	//BaseImage assignment operator
	BaseImage* operator=(const BaseImage& otherBaseImage);
	//getBlock function returns a base object pointer from the scene image that it is called from 
	virtual BaseImage* getBlock(int startRow, int endRow, int startColumn, int endColumn);
	// - operator overload
	BaseImage* operator-(const BaseImage& subtractBaseImage);
	// * operator overload
	BaseImage* operator*(const BaseImage& multiplyBaseImage);
	// () operator overload
	void operator()(const int & row, const int & column, double &element);
	// () operator overload
	double operator()(const int & row, const int & column);
	//Gets the columns
	int GetColumn();
	//Gets the rows
	int GetRow();
	// Gets the baseImage Data
	double* GetData();
	//Sets the rows
	void SetRow(int setRow);
	//Sets the column
	void SetColumn(int setColumn);
	//Sets the base image data
	void SetData(double* setData);
protected:
	int baseImageColumn;//Stores total number of Columns the base image has
	int baseImageRow; //Stores total number of rows the base image has 
	double *baseImageDataPointer;//Pointer that points to a 1D array that contains the pixel data
};

#endif