#include "BaseImage.h"
#include "MatchImage.h"
//MatchImage Constructor
MatchImage::MatchImage(BaseImage* baseImagePointer, double score, int startRow, int endRow, int startCol, int endCol) {
	//Get the variables of row and columns from BaseImage
	int row = baseImagePointer->GetRow();
	int column = baseImagePointer->GetColumn();
	//Gets the data from baseImagePointer
	double* baseImageData = baseImagePointer->GetData();
	//Set the rows and Columns of the MatchImage 
	this->SetRow(endRow - startRow);
	this->SetColumn(endCol - startCol);
	double * tempData = new double[row*column];
	//Goes through  and gets the data from baseimage pointer and puts it in tempData
	for (int index = 0; index < row*column; index++)
	{
		tempData[index] = baseImageData[index];
	}
	//tempData is then put into MatchImage
	this->SetData(tempData);
	//Set the Coordinates of MatchImage
	this->Set_Coords(startRow, endRow, startCol, endCol);
	//Set the score of MatchImage
	this->Set_Score(score);
};
//MatchImage Destructor
MatchImage::~MatchImage() {

};
//MatchImage Copy Constructor
MatchImage::MatchImage(MatchImage& copyConstructor)
{
	int row = copyConstructor.GetRow();
	int col = copyConstructor.GetColumn();
	this->SetRow(row);
	this->SetColumn(col);
	//Copys the baseImageData over using the baseImageDataPointer
	for (int i = 0; i < row*col; i++) {
		this->baseImageDataPointer[i] = copyConstructor.baseImageDataPointer[i];
	}
	locationCoords copyCoords = copyConstructor.Get_Coords();
	//Set the Coordinates of MatchImage
	this->Set_Coords(copyCoords.locationStartRow, copyCoords.locationEndRow, copyCoords.locationStartCol, copyCoords.locationEndCol);
	//Set the score of MatchImage
	this->Set_Score(copyConstructor.Get_Score());
}
// MatchImage assignment operator
MatchImage* MatchImage :: operator=(MatchImage& other)
{
	delete[] baseImageDataPointer;
	baseImageRow = other.GetRow();
	baseImageColumn = other.GetColumn();
	//reserve memory for new array
	baseImageDataPointer = new double[baseImageRow*baseImageColumn];
	for (int x = 0; x < (baseImageRow*baseImageColumn); x++)
	{
		this->baseImageDataPointer[x] = other.baseImageDataPointer[x];
	}
	//Get coordinates from the other MatchImage
	locationCoords copyCoords = other.Get_Coords();
	//Set the Coordinates of MatchImage
	this->Set_Coords(copyCoords.locationStartRow, copyCoords.locationEndRow, copyCoords.locationStartCol, copyCoords.locationEndCol);
	//Set the score of MatchImage
	this->Set_Score(other.Get_Score());
	return this;
}
// Gets the score from MatchImage
double MatchImage::Get_Score() {
	return nNSScore;
};
// Sets the score for MatchImage
void MatchImage::Set_Score(double newScore) {
	nNSScore = newScore;
};
//Gets the Coordinates from MatchImage
locationCoords MatchImage::Get_Coords() {
	locationCoords getCoords;
	getCoords.locationStartRow = this->matchImageCoords.locationStartRow;
	getCoords.locationEndRow = this->matchImageCoords.locationEndRow;
	getCoords.locationStartCol = this->matchImageCoords.locationStartCol;
	getCoords.locationEndCol = this->matchImageCoords.locationEndCol;
	return getCoords;
}
//Sets the Coordinates of MatchImage
void MatchImage::Set_Coords(int startRow, int endRow, int startCol, int endCol) {
	matchImageCoords.locationStartRow = startRow;
	matchImageCoords.locationEndRow = endRow;
	matchImageCoords.locationStartCol = startCol;
	matchImageCoords.locationEndCol = endCol;
}