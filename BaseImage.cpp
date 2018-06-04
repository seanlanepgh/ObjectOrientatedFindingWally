#include "BaseImage.h"
//Default Constructor 
BaseImage::BaseImage() {};
//BaseImage Constructor that takes the number of rows and columns
BaseImage::BaseImage(int rowSize, int columnSize) {
	baseImageRow = rowSize;
	baseImageColumn = columnSize;
}
//BaseImage Constructor that takes the number of rows , columns and the data 
BaseImage::BaseImage(int rowSize, int columnSize, double* inputData) {
	baseImageRow = rowSize;
	baseImageColumn = columnSize;
	baseImageDataPointer = new double[baseImageRow*baseImageColumn];
	//Goes through and put inputData into  baseImage data
	for (int index = 0; index < baseImageRow * baseImageColumn; index++)
	{
		baseImageDataPointer[index] = inputData[index];
	}
}
//BaseImage Deconstructor
BaseImage::~BaseImage()
{
	delete[] baseImageDataPointer;
}
//BaseImage Copy Constructor
BaseImage::BaseImage(const BaseImage& baseImageCopy)
{
	//Copies  rows and columns
	baseImageRow = baseImageCopy.baseImageRow;
	baseImageColumn = baseImageCopy.baseImageColumn;
	//Create a new baseImageDataPointer for the new baseimage
	baseImageDataPointer = new double[baseImageRow*baseImageColumn];
	//Goes through and copies the data from the orignial baseimage into the new baseimage
	for (int row = 0; row < baseImageRow; row++)
	{
		for (int column = 0; column < baseImageColumn; column++)
		{
			// calcuates the index using this formula
			int index = row *baseImageColumn + column;
			baseImageDataPointer[index] = baseImageCopy.baseImageDataPointer[index];
		}
	}
}
//BaseImage assignment operator
BaseImage* BaseImage:: operator=(const BaseImage& otherBaseImage)
{
	delete[] baseImageDataPointer;
	//Get the row and column of  the other base image
	baseImageRow = otherBaseImage.baseImageRow;
	baseImageColumn = otherBaseImage.baseImageColumn;
	//reserve memory for new array
	baseImageDataPointer = new double[baseImageRow*baseImageColumn];
	//Goes through and assigns the data from the other baseimage into the new baseimage
	for (int row = 0; row < baseImageRow; row++)
	{
		for (int column = 0; column <baseImageColumn; column++)
		{
			// calcuates the index using this formula
			int index = row *baseImageColumn + column;
			this->baseImageDataPointer[index] = otherBaseImage.baseImageDataPointer[index];
		}
	}
	return this;
}
//getBlock function returns a base image object pointer from the scene image that it is  called from 
BaseImage* BaseImage::getBlock(int startRow, int endRow, int startColumn, int endColumn) {
	//Calcutate rows and columns 
	int rows = endRow - startRow;
	int columns = endColumn - startColumn;
	//Get the sceneColumn size
	int sceneColumn = this->GetColumn();
	//Create new BaseImage object pointer using rows and columns
	BaseImage *blockObject = new BaseImage(rows, columns);
	double* largeImageData = this->GetData();//Get the data from the cluttered scene
	double* block = new double[rows*columns];
	blockObject->SetData(block);
	/*Loops through the rows and columns and gets elements from the
	large image and store them in the block object data */
	for (int row = 0; row < rows; row++)
	{
		for (int column = 0; column < columns; column++)
		{
			// calcuates the index using this formula
			int index = (startRow + row) * sceneColumn + (startColumn + column);
			//Use operator overload to get the element from large image and store it in the block objects data
			blockObject->operator()(row, column, largeImageData[index]);
		}
	}
	return  blockObject;
};
// - operator overload 
BaseImage* BaseImage :: operator-(const BaseImage& subtractBaseImage) {
	//Gets rows and columns from the subtractBaseImage
	int rows = subtractBaseImage.baseImageRow;
	int columns = subtractBaseImage.baseImageColumn;
	//Creates a new baseImage object pointer
	BaseImage* temp = new BaseImage(rows, columns);
	temp->baseImageDataPointer = new double[temp->baseImageRow*temp->baseImageColumn];
	//Two loops through and substracts both of the matrices away from each other and stores them in temp
	for (int row = 0; row < rows; row++)
	{
		for (int column = 0; column < columns; column++)
		{
			// calcuates the index using this formula
			int index = row*columns + column;
			temp->baseImageDataPointer[index] = this->baseImageDataPointer[index] - subtractBaseImage.baseImageDataPointer[index];
		}
	}
	return temp;
}
// * operator overload
BaseImage* BaseImage ::operator*(const BaseImage& multiplyBaseImage) {
	//Gets rows and columns from the multiplyBaseImage
	int rows = multiplyBaseImage.baseImageRow;
	int columns = multiplyBaseImage.baseImageColumn;
	//Creates a new BaseImage Object pointer
	BaseImage* temp = new BaseImage(rows, columns);
	temp->baseImageDataPointer = new double[temp->baseImageRow*temp->baseImageColumn];
	//Two loops through and multiplies both of the matrices away from each other and stores them in temp
	for (int row = 0; row < rows; row++)
	{
		for (int column = 0; column < columns; column++)
		{
			// calcuates the index using this formula
			int index = row*columns + column;
			temp->baseImageDataPointer[index] = this->baseImageDataPointer[index] * multiplyBaseImage.baseImageDataPointer[index];
		}
	}

	return temp;
}
// () operator overload
void BaseImage :: operator()(const int & row, const int & column, double &element) {
	// calcuates the index using this formula
	int index = row*this->baseImageColumn + column;
	//Gets the index and assignment the element value in that index of baseImageData
	baseImageDataPointer[index] = element;
}
// () operator overload
double BaseImage :: operator()(const int & row, const int & column) {
	// calcuates the index using this formula
	int index = row*this->baseImageColumn + column;
	//Gets the index and assignment the element value in that index of baseImageDataPointer
	return baseImageDataPointer[index];
}
//Gets the columns
int BaseImage::GetColumn()
{
	return baseImageColumn;
}
//Gets the rows
int BaseImage::GetRow() {
	return baseImageRow;
}
// Gets the baseImageDataPointer
double* BaseImage::GetData()
{
	return baseImageDataPointer;
}
//Sets the rows
void  BaseImage::SetRow(int setRow)
{
	baseImageRow = setRow;
}
//Sets the columns
void BaseImage::SetColumn(int setColumn)
{
	baseImageColumn = setColumn;
}
//Sets the base image data
void  BaseImage::SetData(double* setData)
{
	baseImageDataPointer = setData;
}
