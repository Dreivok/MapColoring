/********************************************************************/
/* 2008012647 JANG HYUN JUN in Hanyang Univ.
/* MapCreator.cpp: Generation Map Informaiton
/* Output: mapPoints Array having Point and Line Information
/*
/* Homework3 for A.I. 
												Date: 2014.04.24
													  2014.04.28
																	*/
/********************************************************************/

#include "MapCreator.h"

MapPoint mapPoints[POINT_COUNT];

void createPoint()
{
	int i, j;
	
	// Random Point Generation
	for(i=0; i<POINT_COUNT; i++) {
		mapPoints[i].x = rand() % X_MAX;
		mapPoints[i].y = rand() % Y_MAX;

		// Check, is Point Already exist?
		for(j=0; j<i; j++) {
			if(checkPointEqual(mapPoints[i], mapPoints[j])) {
				mapPoints[i].x = rand() % X_MAX;
				mapPoints[i].y = rand() % Y_MAX;
				j = -1;	// recheck
			}
		}
		
		// initialize Edge and Color Infomation
		mapPoints[i].edgeCount = 0;
		mapPoints[i].edges = NULL;
		/*mapPoints[i].colorCount = kColor;
		mapPoints[i].color = WHITE;
		for(j=RED; j<kColor; j++)
			mapPoints[i].colorState[j] = 0;*/
	}

	if(boolLog) {
		printf("\t!! Generated Map Points !!\n");
		for(int i=0; i<POINT_COUNT; i++)
			printf("\tPoint%d: %d, %d\n", i, mapPoints[i].x, mapPoints[i].y);
	}
}

void createEdge()
{
	int i, NUM, candidate;
	int count = 0;
	int max = (int)pow((double)POINT_COUNT,3);

	double distance;
	double nearest;

	if(boolLog)
		printf("\n\t!! Random Line Generation !!\n");
	
	while(count < max)
	{
		bool alright = true;

		// Select Random Point
		NUM = rand() % POINT_COUNT;
		candidate = NUM;
		distance = 0.0f;
		nearest = X_MAX*2;

		// Calculate Nearest Point that is not Connected
		for(i=0; i<POINT_COUNT; i++) {

			if(checkLineExist(NUM, i))
				continue;

			distance = distancePoint(NUM, i);
			if(nearest > distance) {
				nearest = distance;
				candidate = i;
			}
		}
		if(candidate == NUM)
			alright = false;

		// Check, is there Cross Line
		for(i=0; i<POINT_COUNT; i++) {
			if(!alright)
				break;

			if((i==NUM) || (i==candidate))
				continue;

			if(checkLineCrossed(NUM, candidate, i))
				alright = false;
		}

		// Alright? then Make Edge!! 
		if(alright) {
			insertEdge(NUM, candidate);
			insertEdge(candidate, NUM);

			if(boolLog) {
				printf("\tEdge %d to %d is created!!\n", NUM, candidate);
			}
		}
		
		// Check, is All Edges made? -> Did Edges make enough?
		count++;
	}
}

void createAllEdge()
{
	int mainPoint = 0;
	int i, j, candidate;

	if(boolLog)
		printf("\n\t!! Incremental Line Generation !!\n");

	while(mainPoint < POINT_COUNT)
	{

		for(i=0; i<POINT_COUNT; i++)
		{ // find Edge mainPoint to candidate, that is doesnt exist.
			if(mainPoint == i)
				continue;
			
			candidate = i;			
			bool alright = true;

			mapEdge * temp = mapPoints[mainPoint].edges;
			while(temp!=NULL) {
				if(temp->point2 == i) {	// already exist edge
					candidate = -1;
					break;
				}
				temp = temp->next;
			}

			//m = (mapPoints[mainPoint].y - mapPoints[candidate].y) / (mapPoints[mainPoint].x - mapPoints[candidate].x);

			if(candidate >= 0) {
				// line mainPoint to candidate isnt exist, so check intersection with all other edges
				for(j=0; j<POINT_COUNT; j++)
				{
					if((j == mainPoint) || (j == candidate))
						continue;

					// when there is Point in Line.. because It doesnt use distance..
					if((mapPoints[mainPoint].x - mapPoints[candidate].x) == 0) { // m = infinite
						if((mapPoints[j].y - mapPoints[candidate].y) * (mapPoints[j].y - mapPoints[mainPoint].y) < 0) {
							if(mapPoints[j].x == mapPoints[candidate].x) {
								alright = false;
								break;
							}
						}
					}
					else if((mapPoints[mainPoint].y - mapPoints[candidate].y) == 0) { // m = 0
						if((mapPoints[j].x - mapPoints[candidate].x) * (mapPoints[j].x - mapPoints[mainPoint].x) < 0) {
							if (mapPoints[j].y == mapPoints[candidate].y) {
								alright = false;
								break;
							}
						}
					}	

					if(checkLineCrossed(mainPoint, candidate, j)) {
						alright = false;
						break;
					}
				}
			}			
			else // candidate < 0
				alright = false;

			// Alright? then Make Edge!! 
			if(alright) {
				insertEdge(mainPoint, candidate);
				insertEdge(candidate, mainPoint);

				if(boolLog) {
					printf("\tEdge %d to %d is created!!\n", mainPoint, candidate);
				}
			}
		}

		mainPoint++;
		
	}
	
}

// Check, is Point A already exist?
bool checkPointEqual(mapPoint A, mapPoint B)
{
	if((A.x == B.x) && (A.y == B.y))
		return true;	// Point is Already Exist
	return false;
}

// Check, Does Ath Point have a Edge with Bth Point?
bool checkLineExist(int A, int B)
{
	mapEdge * temp = mapPoints[A].edges;
	if(A == B)
		return true;	// Cannot be a Line
	while(temp!=NULL) {
		if(temp->point2 == B)
			return true;	// Line is Already Exist
		temp = temp->next;
	}
	return false;
}

// Check, is newEdge(Line AB) intersected other exEdges(C's Edges)?
bool checkLineCrossed(int A, int B, int C)
{
	int Ax = mapPoints[A].x, Ay = mapPoints[A].y;
	int Bx = mapPoints[B].x, By = mapPoints[B].y;
	int Cx = mapPoints[C].x, Cy = mapPoints[C].y;
	int Dx, Dy;
	double cross1 = (Bx-Ax)*(Cy-By) - (By-Ay)*(Cx-Bx);	// if cross1 = 0, POINT C is in LINE AB

	if((cross1 == 0) && ((Cx-Bx) * (Cx-Ax) < 0))
		return true;
	
	mapEdge * temp = mapPoints[C].edges;
	while(temp != NULL) {
		Dx = mapPoints[temp->point2].x;
		Dy = mapPoints[temp->point2].y;
		double det = (Bx-Ax) * (Dy-Cy) - (Dx-Cx) * (By-Ay);	// if det = 0, LINE AB and LINE CD is parallel
		double cross2 = (Bx-Ax)*(Dy-By) - (By-Ay)*(Dx-Bx);	// if cross2 = 0, POINT D is in LINE AB
		
		if(det == 0) {
			// when parallel, check intersection
			if(checkPointEqual(mapPoints[A], mapPoints[temp->point2])) {
				// when colinear, check intersection
				if((Ax == Bx) && (Ax == Cx)) {
					if((Cy-Ay)*(Cy-By) < 0)
						return true;	// intersect occur!
				}
				else {
					if((Cx-Ax)*(Cx-Bx) < 0)
						return true;	// intersect occur!
				}
			}
			// it just parallel
		}
		else if(((cross1 * cross2) < 0))
			return true;	// intersect occur!

		temp = temp->next;
	}
	return false;

}

void insertEdge(int A, int B)
{
	mapEdge * newEdge = (mapEdge*)malloc(sizeof(mapEdge));
	newEdge->point1 = A;
	newEdge->point2 = B;
	newEdge->next = NULL;

	if(mapPoints[A].edges == NULL) {
		mapPoints[A].edges = newEdge;
	} else {
		mapEdge * temp = mapPoints[A].edges;
		while(temp->next != NULL)
			temp = temp->next;
		temp->next = newEdge;
	}
	mapPoints[A].edgeCount++;
}

double distancePoint(int A, int B)
{
	int x1 = mapPoints[A].x;
	int y1 = mapPoints[A].y;
	int x2 = mapPoints[B].x;
	int y2 = mapPoints[B].y;

	return sqrt(pow((double)(x2-x1),2)+pow((double)(y2-y1),2));
}