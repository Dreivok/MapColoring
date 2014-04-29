/********************************************************************/
/* 2008012647 JANG HYUN JUN in Hanyang Univ.
/* globals.h: Base Header file
/*
/* Homework3 for A.I. 
												Update: 2014.04.24	
														2014.04.2?	*/
/********************************************************************/

#ifndef _GLOBALS_H_
#define _GLOBALS_H_

#include <stdio.h>
#include <stdlib.h>

/******** ON / OFF Flags *********/
extern bool boolLog;
extern bool boolSave;
extern bool boolShowWindow;
extern bool boolAllEdge;

extern FILE * result; 

/******** Coloring Information Definition *********/
typedef enum COLORS {WHITE = -1, RED, GREEN, YELLOW, BLUE};	// white means no setted color;

extern int kColor;

/******** Map Information Definition *********/
#define X_MAX 10
#define Y_MAX 10
#define POINT_COUNT 50		// Count of scattered point

typedef struct mapEdge {
	int point1;
	int point2;
	struct mapEdge * next;
} MapEdge;

typedef struct mapPoint {
	int x;
	int y;
	int edgeCount;
	struct mapEdge * edges;
	int colorCount;		// if Color is setted, than colorCount is 0, other is Possible Color Count.
	int colorState[4];	// Possiable Color State, each array is Red, Green, Yellow, Blue.
	COLORS color;		// setted Color
} MapPoint;


extern MapPoint mapPoints[POINT_COUNT];

#endif
