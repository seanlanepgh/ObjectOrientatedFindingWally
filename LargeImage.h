#ifndef LargeImage_H
#define LargeImage_H 
#include "BaseImage.h"
#include "MatchImage.h"
//LargeImage inherits from BaseImage
class LargeImage :public BaseImage {
public:
	//LargeImage Constructor 
	LargeImage::LargeImage(int rowSize, int columnSize, double *inputData, vector<MatchImage*>Matches);
	//LargeImage Destructor
	LargeImage :: ~LargeImage();
	//LargeImage Copy Constructor
	LargeImage::LargeImage(LargeImage& copyConstructor);
	//getBlock function returns a block object pointer from the scene image that it is  called from 
	BaseImage* getBlock(int startRow, int endRow, int startColumn, int endColumn);
	//Function plots the outlines of the match image on the clutter scene image
	void  LargeImage::plotOutlineOfMatch(int& start_row, int &end_row, int &start_column, int& end_column);
	//Gets the similarity score and returns the sum without using the * and - operator overloads
	double LargeImage::SSD1(BaseImage *wally, BaseImage *block);
	//Gets the similarity score and returns the sum using the * and - operator overloads
	double LargeImage::SSD2(BaseImage* wally, BaseImage* block);
	//Sets the MatchImages
	void LargeImage::Set_MatchImages(vector<MatchImage*>&BestImagePointer);
	//Gets the MatchImages
	vector<MatchImage*> LargeImage::Get_MatchImages();
	//Compares and Sorts the MatchImages
	void LargeImage::CompareandSort(MatchImage* Match,int numOfMatches);
	//Bubble Sort to sort the N vectors in the MatchImages
	void LargeImage::BubbleSort(vector<MatchImage*> & data);
	//Swap function
	void LargeImage::swap(vector<MatchImage*> & data, int matchImage1, int matchImage2);
	//Function used to compare the similarity score
	void  LargeImage::CompareScore(MatchImage*Match, vector<MatchImage*>&MatchImages,int numOfMatches);
protected:
	vector<MatchImage*>MatchImages; //Vector of match images 

};
#endif