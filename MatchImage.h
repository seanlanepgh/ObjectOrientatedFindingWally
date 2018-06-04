#ifndef MatchImage_H
#define MatchImage_H 
#include "BaseImage.h"
//Struct is used to store the coordinates of where the match image is in the scene
struct locationCoords
{
	int locationStartRow;
	int locationEndRow;
	int locationStartCol;
	int locationEndCol;
};
//MatchImage inherits from BaseImage
class MatchImage :public BaseImage {
public:
	//MatchImage Constructor
	MatchImage(BaseImage * baseImagePointer, double score, int startRow, int endRow, int startCol, int endCol);
	//MatchImage Destructor
	~MatchImage();
	//MatchImage Copy Constructor
	MatchImage(MatchImage& copyConstructor);
	// MatchImage assignment operator
	MatchImage* operator=(MatchImage& other);
	// Gets the score from MatchImage
	double Get_Score();
	// Sets the score for MatchImage
	void Set_Score(double newScore);
	//Gets the Coordinates from MatchImage
	locationCoords Get_Coords();
	void Set_Coords(int startRow, int endRow, int startCol, int endCol);
protected:
	locationCoords matchImageCoords;//Stores the match image coordinates using the struct locationCoords
	double nNSScore;// Stores the score of the NNS algorithm


};
#endif