/********************************************************************/
/* 2008012647 JANG HYUN JUN in Hanyang Univ.
/* MapColoring.cpp: Main Source File
/* Output: Result Scene using OpenGL and saved text file
/* Description: It can Chage the Options, Solving Methods..
				...
/*
/* Homework3 for A.I. 
												Update: 2014.04.24
													Map Generating
														2014.04.26
													BackTracking and Forward Check
														2014.04.28
													Correct Some Error
														2014.04.29
													Arc Consistency
																	*/
/********************************************************************/

#include "globals.h"
#include "MapCreator.h"
#include "MapScene.h"
#include "MapColoring.h"

#include <time.h>

bool boolLog = false;
bool boolSave = true;		// save result to file
bool boolShowWindow = true;	// show result visiably to window
bool boolAllEdge = true;	// it will makes map more clearly

int kColor = 3;

FILE * result;
extern MapPoint mapPoints[POINT_COUNT];

void initColor();
void saveFile();

int main(int argc, char** argv)
{
	int method = 0;
	double start, finish;

	srand((unsigned int)time(NULL));
	
	fopen_s(&result, "mapColoring.txt", "w");
	if(result == NULL)
		printf("ERROR: File Open Failed");

	// Create Map Information
	printf("Start Map Generating..\n");
	createPoint();

	printf("Start Line Generating..\n");
	createEdge();
	if(boolAllEdge)
		createAllEdge();	// Plus Edge incremently..

	if(boolLog) {
		int a = 0;
		for(int i=0; i<POINT_COUNT; i++)
			a += mapPoints[i].edgeCount;
		printf("\n Created Edges Count:%d  \n", a/2);
	}
	
	// Save the Result
	if(boolSave) saveFile();
	
		initColor();
	// Select Color using each strategy
	for(method = 0; method < 3; method++) { // method 0~2
		kColor = 3;
		printf("\nStart map Coloring using %s\n", method == 0? "Backtracking" 
			: method == 1? "Backtracking using Foward check" : "Backtracking using Arc Consistency");
		if(boolSave) fprintf(result, "\nStart map Coloring using %s\n", method == 0? "Backtracking"
			: method == 1? "Backtracking using Foward check" : "Backtracking using Arc Consistency");

		// time check
		start = clock();

		initColor();
		if(!backtrackingSearch(method)) {
			printf("ERROR: It can't make %d-Coloring Map!!\n", kColor);
			if(boolSave) fprintf(result, "ERROR: It can't make %d-Coloring Map!!\n", kColor);
		
			kColor = 4;
			if(boolLog) printf("\nStart %d-Colorings Search\n", kColor);
			if(boolSave) fprintf(result, "\nStart %d-Colorings Search\n", kColor);

			initColor();
			backtrackingSearch(method);	// mapColoring using k = 4
		}
		
		printf("Map Coloring using %s is finished!!\n", method == 0? "Backtracking" 
			: method == 1? "Backtracking using Foward check" : "Backtracking using Arc Consistency");
		if(boolSave) fprintf(result, "Map Coloring using %s is finished!!\n", method == 0? "Backtracking" 
			: method == 1? "Backtracking using Foward check" : "Backtracking using Arc Consistency");

		// end time
		finish = clock();

		printf("time : %.3fs\n", ((finish-start)/CLOCKS_PER_SEC));
		if(boolSave) fprintf(result, "%s time : %.3fs\n"
			, method == 0? "Backtracking" : method == 1? "Backtracking using Foward check" : "Backtracking using Arc Consistency"
			, ((finish-start)/CLOCKS_PER_SEC));
		//printf("time : %f, %f, %.3f\n", start, finish, difftime(finish, start));
	}

	
	fclose(result);
	
	if(boolShowWindow) {
		glutInit(&argc, argv);
		mapDisplay();
	}
	return 0;
}

void initColor()
{
		for(int i=0; i<POINT_COUNT; i++) {
			mapPoints[i].color = WHITE;
			mapPoints[i].colorCount = kColor;
			for(int j=0; j<kColor; j++)
				mapPoints[i].colorState[j] = 0;	// 0 is true
		}
}

void saveFile()
{
	int i, j;
	int countEDGE = 0;
	mapEdge * temp;

	fprintf(result, "--------- OPTION Inforamtion ---------\n");
	fprintf(result, "%% Log ON: %d\n", boolLog? 1 : 0);
	fprintf(result, "%% SAVE ON: %d\n", boolSave? 1 : 0);
	fprintf(result, "%% SHOW WINDOW ON: %d\n", boolShowWindow? 1 : 0);
	fprintf(result, "%% ALL EDGE ON: %d\n", boolAllEdge? 1 : 0);
	fprintf(result, "\n");

	fprintf(result, "%% MAP SIZE: %d X %d\n", X_MAX, Y_MAX);
	//fprintf(result, "%% NUMBER of POINTS: %d\n", POINT_COUNT);
	for(i=0; i<POINT_COUNT; i++)
		countEDGE += mapPoints[i].edgeCount;
	fprintf(result, "%% NUMBER of EDGES: %d\n", countEDGE/2);
	fprintf(result, "%% NUMBER of COLORS: %d\n", kColor);

	
	fprintf(result, "\n--------- POINT & LINE Informaiton ---------\n");
	for(i=0; i<POINT_COUNT; i++) {
		fprintf(result, "Point%d: (%d, %d)\n", i, mapPoints[i].x, mapPoints[i].y);
		//fprintf(result, "EDGE COUNT: %d\n", mapPoints[i].edgeCount);
		temp = mapPoints[i].edges;
		j = 0;
		while(temp!=NULL) {
			fprintf(result, "\tEdge%d: (%d, %d)\n", ++j, temp->point1, temp->point2);
			temp = temp->next;
		}
	}

	fprintf(result, "\n--------- MAP-COLORING Informaiton ---------\n");
}