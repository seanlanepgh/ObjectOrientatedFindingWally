#include "LargeImage.h"
//LargeImage Constructor 
LargeImage::LargeImage(int rowSize, int columnSize, double *inputData, vector<MatchImage*>Matches) :BaseImage(rowSize, columnSize, inputData) {
	MatchImages = Matches;
}
//LargeImage Destructor
LargeImage :: ~LargeImage() {};
//LargeImage Copy Constructor
LargeImage::LargeImage(LargeImage& copyConstructor)
{
	int row = copyConstructor.GetRow();
	int col = copyConstructor.GetColumn();
	this->SetRow(row);
	this->SetColumn(col);
	//Copys the baseImagedata over using the baseImageDataPointer
	for (int i = 0; i < row*col; i++) {
		this->baseImageDataPointer[i] = copyConstructor.baseImageDataPointer[i];
	}
	//Copys the vector of MatchImages
	MatchImages = copyConstructor.MatchImages;
}
//getBlock function returns a block object pointer from the scene image that it is called  by
BaseImage* LargeImage::getBlock(int startRow, int endRow, int startColumn, int endColumn) {
	//Calculate rows and columns 
	int rows = endRow - startRow;
	int columns = endColumn - startColumn;
	//Get the sceneColumn size
	int sceneColumn = this->GetColumn();
	//Create new BaseImage block object pointer using rows and columns
	BaseImage *blockObjectPointer = new BaseImage(rows, columns);
	double* largeImageData = this->GetData();//Get the data from the large image
	double* block = new double[rows*columns];
	blockObjectPointer->SetData(block);
	/*Loops through the rows and columns and gets elements from the
	large image and store them in the block object pointer data */
	for (int row = 0; row < rows; row++)
	{
		for (int column = 0; column < columns; column++)
		{
			// calcuates the index using this formula
			int index = (startRow + row) * sceneColumn + (startColumn + column);
			//Use operator overload to get the element from large image and store it in the block object pointer data
			blockObjectPointer->operator()(row, column, largeImageData[index]);
		}
	}
	return  blockObjectPointer;
};
//Function plots the outlines of the match image on the clutter scene image
void  LargeImage::plotOutlineOfMatch(int& startRow, int &endRow, int &startColumn, int& endColumn) {
	//Get the number of columns of the scene
	int largeColumn = this->GetColumn();
	double*largeImageData = this->GetData();
	//Loops through and does the left and right side of the outline of the match image and adds a 3 pixel width on these sides
	for (int row = startRow; row <= endRow; row++)
	{
		// calcuates the index using this formula
		int index = row * largeColumn + startColumn;
		//Set the value to 0 so it would be black on the image
		largeImageData[index] = 0;
		largeImageData[index + 1] = 0;
		largeImageData[index + 2] = 0;
		index = row * largeColumn + endColumn;
		largeImageData[index] = 0;
		largeImageData[index - 1] = 0;
		largeImageData[index - 2] = 0;
	}
	//Loops through and does the top and the bottom side of the outline of the match image
	for (int column = startColumn; column < endColumn; column++)
	{
		int index = startRow * largeColumn + column;
		largeImageData[index] = 0;
		index = endRow* largeColumn + column;
		largeImageData[index] = 0;
	}
	this->SetData(largeImageData);
};
//Gets the disimilarity score and returns the sum without using the * and - operator overloads
double LargeImage::SSD1(BaseImage *wally, BaseImage *block) {
	//This function finds wally in the cluttered scene
	double sum = 0;
	int rows = wally->GetRow();
	int columns = wally->GetColumn();
	/* Loops through the rows and columns of the wally compare image object pointer
	and the current block object pointer*/
	for (int row = 0; row < rows; row++) {
		for (int column = 0; column < columns; column++) {
			//Get the element value from the wally object pointer
			double elementValue = wally->operator()(row, column);
			/*Checks if the value is white. If is not white then do the calculation
			of the disimilarity score.This allows us to get a more accurate score as
			the wally compare image has lots of white in the background.
			Where as the location of wally in the cluttered scene has not got a white background.
			*/
			if (elementValue != 255) {
				/*Get the element value of from the block object pointer
				and substracts it from the wally object pointer element value*/
				elementValue -= block->operator()(row, column);
				//Square the result of the substraction and adds it to create a disimilarity score
				elementValue *= elementValue;
				sum += elementValue;
			}
		}
	}
	return sum;
}
//Gets the disimilarity score and returns the sum using the * and - operator overloads
double LargeImage::SSD2(BaseImage* wally, BaseImage* block) {
	//This function doesn't find wally in the cluttered scene
	double sum = 0;
	//Uses the - operator overload to take away the data in the wally object pointer by the block object pointer  and create a new  base image object pointer called diff
	BaseImage* diff = *wally - *block;
	//Uses the * operator overload to multiply the new  base image object pointer by itself
	diff = *diff *  *diff;
	int rows = diff->GetRow();
	int columns = diff->GetColumn();
	//Loop through the rows and columns to get the sum of each value in the new  base image object pointer
	for (int row = 0; row < rows; row++)
	{
		for (int column = 0; column < columns; column++)
		{
			sum += diff->operator()(row, column);
		}
	}
	return sum;
};
//Sets the MatchImages
void LargeImage::Set_MatchImages(vector<MatchImage*>&vectorOfMatchImages) {
	MatchImages = vectorOfMatchImages;
};
//Gets the MatchImages
vector<MatchImage*> LargeImage::Get_MatchImages() {
	return this->MatchImages;
}
//Compares and Sorts the Match Image object pointers
void LargeImage::CompareandSort(MatchImage* Match , int numOfMatches) {
	//if the MatchImages vector  has not reached the size of N matches
	if (MatchImages.size() < numOfMatches) {
		//Then push it into the MatchImages vector
		MatchImages.push_back(Match);
	}
	else {
		//If the  MatchImages vector has N matches already then 
		//Call CompareScore and then Bubble Sort functions
		CompareScore(Match, MatchImages,numOfMatches);
		BubbleSort(MatchImages);
	}
}
//Bubble Sort to sort the N match image object pointers by NNSScore  in the MatchImages vector
void LargeImage::BubbleSort(vector<MatchImage*> & data)
{
	int length = data.size();
	//Goes through and compares each of the pairs of the indexs 
	for (int index = 0; index < length; ++index)
	{
		bool swapped = false;//Used bool to check if it data has been swapped 
		for (int index2 = 0; index2 < length - (index + 1); ++index2)
		{
			double matchImageScore = data[index2]->Get_Score();
			double matchImageScore2 = data[index2 + 1]->Get_Score();
			//Compares the current index and the next indexs scores
			if (matchImageScore > matchImageScore2)
			{
				//If the first score is greater then call swap function
				swap(data, index2, index2 + 1);
				swapped = true;//Also  use bool to show there was a swap
			}
		}
		if (!swapped) break;
	}
}
//Swap function
void LargeImage::swap(vector<MatchImage*> & data, int matchImage1, int matchImage2)
{
	//Swaps over the match image object pointers
	MatchImage* tempMatchImage = data[matchImage1];
	data[matchImage1] = data[matchImage2];
	data[matchImage2] = tempMatchImage;
}
//Function used to compare the disimilarity score
void  LargeImage::CompareScore(MatchImage*Match, vector<MatchImage*>&MatchImages ,int numOfMatches) {
	//If the new match score is less than the last score in the vector of MatchImages
	if (Match->Get_Score() < MatchImages[numOfMatches-1]->Get_Score()) {
		//Then add it to the last score position and replace it
		MatchImages[numOfMatches-1] = Match;
	}
	else {
		//else do nothing with it
	}
}


