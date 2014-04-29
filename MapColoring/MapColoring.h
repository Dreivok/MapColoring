/********************************************************************/
/* 2008012647 JANG HYUN JUN in Hanyang Univ.
/* MapColoring.cpp: Start k-coloring
/* Output: determine each Point's Color
/* Basic Algorithm:
  ---------------------------------------------------------------------------------------
  |	Reculsive-Backtracking() 															|
  |		if assignment is complete, then return assignment								|
  |		var = Select unassigned virable(variable[csp], assignment, csp)					|
  |		for each value in ORDER-DOMAIN-VALUES(var, assignment, csp) do                  |
  |			if value is consistent with assignment according to COSTARAINTS[csp] then	|
  |			add {val = value} to assignment												|
  |			result <- Reculsive-Backtracking(assignment, csp)							|
  |			if result != failure, return result											|
  |			remove {var = value} from assignment										|
  |		return failure																	|
  ---------------------------------------------------------------------------------------
/*
/* Homework3 for A.I. 
												Date: 2014.04.26
												Make BackTracking and Forward Check
																	*/
/********************************************************************/

#ifndef _MAPCOLORING_H_
#define _MAPCOLORING_H_

#include "globals.h"

#define SUCCESS 1
#define FAILURE 0

int backtrackingSearch(int method);	// reculsive backtracking Search

int selectVariable();	// using MRV(Minimum Remaining Values) and MCV(Most Constraining Variable)
COLORS selectValues(int var);	// using LCV(Least Constraning Value)

void fowardCheck(int, COLORS);
void removeColor(int var, COLORS color);

void arcConsistency(int);	// using MAC(Maintaining arc Consistency)
int valueInconsistency(int, int);
void pushQueue(mapEdge*, mapEdge*, bool);
mapEdge* popQueue(mapEdge*);


bool checkNeighbor(int, COLORS);

#endif