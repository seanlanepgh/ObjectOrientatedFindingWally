#include <stdlib.h>
#include <array>
#include <sstream> // stringstream
#include <iostream> // cout, cerr
#include <fstream> // ifstream
#include <istream>
#include "BaseImage.h"
#include "MatchImage.h"
#include "LargeImage.h"
using namespace std;
// Use to read from a text file
double* readTXT(char *fileName, int sizeRow, int sizeCol);
// use to write to create a pgm file
void WritePGM(char *fileName, double *Data, int sizeRow, int sizeCol, int greyScale);
int main() {
	//Stores the rows and columns of the cluttered_scene
	int sceneRow = 768; int sceneCol = 1024;
	//Stores the rows and columns of the Wally_grey 
	int wallyRow = 49;  int wallyCol = 36;
	double*sceneInputData = 0;// Stores the data recieved by  reading from cluttered_scene
	double*wallyInputData = 0;//Stores the data recieved by reading from the Wally_grey
	char* sceneInputFileName = "Cluttered_scene.txt";//Stores file path of cluttered_scene
	char* wallyInputFileName = "Wally_grey.txt";//Stores file path of wally_grey
	double SDDScore = 0; //Stores the score of the SSD1 function that uses only the () operator overload to compare the two images
	double SDDScore2 = 0;//Stores the score of the SSD2 function that uses the * , - and () operator overloads to compare the two images
	int greyScale = 255;//Used in the writing to the pgm file function
	int numOfMatches = 0;//Used to store how many matches the user wants
	int input = 0;//Used so you can see the ranking, scores and coordinates
	//Create two arrays to store the 10 best match image file names. Used to output each of the two function results to pgm files
	array<char*, 10>BestMatchFileName{
		"BestMatchImage1_1.pgm","BestMatchImage1_2.pgm",
		"BestMatchImage1_3.pgm","BestMatchImage1_4.pgm",
		"BestMatchImage1_5.pgm","BestMatchImage1_6.pgm",
		"BestMatchImage1_7.pgm","BestMatchImage1_8.pgm",
		"BestMatchImage1_9.pgm","BestMatchImage1_10.pgm"
	}; array<char*, 10>BestMatchFileName2{
		"BestMatchImage2_1.pgm","BestMatchImage2_2.pgm",
		"BestMatchImage2_3.pgm","BestMatchImage2_4.pgm",
		"BestMatchImage2_5.pgm","BestMatchImage2_6.pgm",
		"BestMatchImage2_7.pgm","BestMatchImage2_8.pgm",
		"BestMatchImage2_9.pgm","BestMatchImage2_10.pgm"
	};
	locationCoords matchCoords;//Used to store the matches coordinates 
	int matchRow = 49; //Stores the match image rows
	int matchCol = 36;//Stores the match image columns
	char*sceneOutputFileName;//Stores the cluttered scene filename
	//Gets data from each image text files by calling  readTXT function 
	sceneInputData = readTXT(sceneInputFileName, sceneRow, sceneCol);
	wallyInputData = readTXT(wallyInputFileName, wallyRow, wallyCol);
	vector<MatchImage*>matches;//Empty vector to store the match images  object pointers
	BaseImage* wallyObjectPointer = new BaseImage(wallyRow, wallyCol, wallyInputData);//Creates an base image object pointer to store the wally image matrix used to compare
	LargeImage *sceneObjectPointer = new LargeImage(sceneRow, sceneCol, sceneInputData, matches);//Create an object to store the cluttered scene image matrix
	//I have another scene object of the cluttered scene as i am comparing my results from two different SSD functions.
	LargeImage *sceneObject2Pointer = new LargeImage(sceneRow, sceneCol, sceneInputData, matches);
	LargeImage sceneObjectCopy = LargeImage(sceneRow, sceneCol, sceneInputData, matches); //copy of the scene image just to use the virtual function
	BaseImage * baseImageObjectPointer = &sceneObjectCopy;//Used to call the virtual get block function 
	cout << " Enter the number of matches you want to identify ?" << endl;
	cin >> numOfMatches;
	//Two Loops that go through the rows and column of the cluttered scene image
	for (int row = 0; row <= 713; row += 6) {
		for (int col = 0; col <= 982; col += 6) {
			//Calls the get block function. Stores the returned  block object pointer
			BaseImage* blockObjectPointer = baseImageObjectPointer->getBlock(row, row + 49, col, col + 36);
			// Resets the scores
			SDDScore = 0;
			SDDScore2 = 0;
			//Both SSD1 and SSD2 uses the NNS algorithm
			//Calls the SSD1 function that uses only the () operator overload and returns the score
			SDDScore = sceneObjectPointer->SSD1(wallyObjectPointer, blockObjectPointer);
			//Calls the SSD2 function that uses the * , - and () operator overload and returns a score
			SDDScore2 = sceneObject2Pointer->SSD2(wallyObjectPointer, blockObjectPointer);
			//Creates the MatchImage objects using the Block object pointer, the score and its coords in the scene Object
			MatchImage* Match = new  MatchImage(blockObjectPointer, SDDScore, row, row + 49, col, col + 36);
			MatchImage* Match2 = new MatchImage(blockObjectPointer, SDDScore2, row, row + 49, col, col + 36);
			//Calls the compare and sort function for each scene Object
			sceneObjectPointer->CompareandSort(Match , numOfMatches);
			sceneObject2Pointer->CompareandSort(Match2, numOfMatches);
		}
	}
	//Loop the vectors of the matchimages and plot the coordinates of the N best matches
	for (int index = 0; index < numOfMatches; index++) {
			//Gets the best matches for the SSD1  function
			matches = sceneObjectPointer->Get_MatchImages();
			//Gets the Coordinates from the vector that stored the results to from SSD1.
			matchCoords = matches[index]->Get_Coords();
			//Call this function to draw the outline of each of the results from SSD1 on to the cluttered scene
			sceneObjectPointer->plotOutlineOfMatch(matchCoords.locationStartRow, matchCoords.locationEndRow, matchCoords.locationStartCol, matchCoords.locationEndCol);
			//Print out the rank , score and coordinates for the SSD1 function
			cout << "SSD1 Best Match Image Rank: " << index +1 << " Score: " << matches[index]->Get_Score() << " Start Row: " << matchCoords.locationStartRow << " End Row: " << matchCoords.locationEndRow << " Start Column: " << matchCoords.locationStartCol << " EndColumn: " << matchCoords.locationEndCol << endl;
			cout << " " << endl;
			//Gets the best matches for the SSD2 function
			matches = sceneObject2Pointer->Get_MatchImages();
			//Gets the Coordinates from the vector that stored the results to from SSD2.
			matchCoords = matches[index]->Get_Coords();
			//Call this function to draw the outline of each of the results from SSD2 on to the cluttered scene
			sceneObject2Pointer->plotOutlineOfMatch(matchCoords.locationStartRow, matchCoords.locationEndRow, matchCoords.locationStartCol, matchCoords.locationEndCol);
			//Print out the rank , score and coordinates for the SSD2 function
			cout << "SSD2 Best Match Image Rank: " << index + 1 << " Score: " << matches[index]->Get_Score() << " Start Row: " << matchCoords.locationStartRow << " End Row: " << matchCoords.locationEndRow << " Start Column: " << matchCoords.locationStartCol << " EndColumn: " << matchCoords.locationEndCol << endl;
			cout << " " << endl;
	}
	cout << "Enter 1 to continue ..." << endl;
	cin >> input;
	//If the num of match is less than 10 or equal 10  write the 10 best  matches to separate pgm files
	if (numOfMatches <= 10) {
		for (int index = 0; index < numOfMatches; index++) {
			//Gets the best matches for the SSD1  function
			matches = sceneObjectPointer->Get_MatchImages();
			//Gets the Coordinates from the vector that stored the results to from SSD1.
			matchCoords = matches[index]->Get_Coords();
			//Call WritePGM and create the PGM file using the best match file name ,the data from match image, the number of rows,columns and then the greyscale 
			WritePGM(BestMatchFileName[index], matches[index]->GetData(), matchRow, matchCol, greyScale);
			//Gets the best matches for the SSD2 function
			matches = sceneObject2Pointer->Get_MatchImages();
			//Gets the Coordinates from the vector that stored the results to from SSD2.
			matchCoords = matches[index]->Get_Coords();
			//Call WritePGM and create the PGM file using the best match file name 2 ,the data from match image, the number of rows,columns and then the greyscale 
			WritePGM(BestMatchFileName2[index], matches[index]->GetData(), matchRow, matchCol, greyScale);
		}
	}
	// writes data back to .pgm file stored in outputFileName
	//Creates the orginial cluttered_scene.pgm
	sceneOutputFileName = "Cluttered_scene.pgm";
	WritePGM(sceneOutputFileName, sceneInputData, sceneRow, sceneCol, greyScale);
	//Creates the  image that shows the SSD1 result locations on the cluttered scene
	sceneOutputFileName = "Cluttered_Scene_SSD1_Results.pgm";
	WritePGM(sceneOutputFileName, sceneObjectPointer->GetData(), sceneRow, sceneCol, greyScale);
	//Creates the  image that shows the SSD2 result locations on the cluttered scene
	sceneOutputFileName = "Cluttered_Scene_SSD2_Results.pgm";
	WritePGM(sceneOutputFileName, sceneObject2Pointer->GetData(), sceneRow, sceneCol, greyScale);
}
// Read .txt file with image of size RxC, and convert to an array of doubles
double* readTXT(char *fileName, int sizeRow, int sizeCol)
{
	double* fileData = new double[sizeRow*sizeCol];
	int i = 0;
	ifstream myFile(fileName);
	if (myFile.is_open())
	{

		while (myFile.good())
		{
			if (i>sizeRow*sizeCol - 1) break;
			myFile >> *(fileData + i);
			i++;
		}
		myFile.close();
	}

	else cout << "Unable to open file";


	return fileData;
}

// convert data from double to .pgm stored in filename
void WritePGM(char *fileName, double *Data, int sizeR, int sizeC, int Q)
{

	int i, j;
	unsigned char *image;
	ofstream myFile;

	image = (unsigned char *) new unsigned char[sizeR*sizeC];

	// convert the integer values to unsigned char

	for (i = 0; i<sizeR*sizeC; i++)
		image[i] = (unsigned char)Data[i];

	myFile.open(fileName, ios::out | ios::binary | ios::trunc);

	if (!myFile) {
		cout << "Can't open file: " << fileName << endl;
		exit(1);
	}

	myFile << "P5" << endl;
	myFile << sizeC << " " << sizeR << endl;
	myFile << Q << endl;

	myFile.write(reinterpret_cast<char *>(image), (sizeR*sizeC) * sizeof(unsigned char));

	if (myFile.fail()) {
		cout << "Can't write image " << fileName << endl;
		exit(0);
	}

	myFile.close();

	delete[] image;

}

