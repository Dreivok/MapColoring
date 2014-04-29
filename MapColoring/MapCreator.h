/********************************************************************/
/* 2008012647 JANG HYUN JUN in Hanyang Univ.
/* MapCreator.cpp: Generation Map Informaiton
/* Output: mapPoints Array having Point and Line Information
/*
/* Homework3 for A.I. 
												Date: 2014.04.24	*/
/********************************************************************/

#ifndef _MAPCREATOR_H_
#define _MAPCREATOR_H_

#include <time.h>
#include <math.h>
#include "globals.h"

void createPoint();	// Random Scattered Points Generation and Initialize mapPoints
void createEdge();	// Random Enough Edges Generation
void createAllEdge();	// Possible All Edge Generation, but Some Problem because Line cross algorithm..

/********** Point Function **********/
bool checkPointEqual(mapPoint A, mapPoint B);	// Check, is Point A already exist?

/********** Edge Function **********/
bool checkLineExist(int A, int B);	// Check, Does Ath Point have a Edge with Bth Point?
bool checkLineCrossed(int A, int B, int C);	// Check, is newEdge(Line AB) intersected other exEdges(C's Edges)?
void insertEdge(int A, int B);

/********** Calculation Function **********/
double distancePoint(int A, int B);

#endif