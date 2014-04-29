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
													  2014.04.28
												Correct some error..
													  2014.04.29
												Make Arc Consistency
																	*/
/********************************************************************/

#include "MapColoring.h"

int backtrackingSearch(int method)	// reculsive backtracking
{
	int i;
	int var;
	COLORS color;

	// if assignment is complete, then return assignment
	for(i=0; i<POINT_COUNT; i++) {
		if(mapPoints[i].color == WHITE)	// if there is NULL color, It doesn't complete
			break;
	}
	if(i == POINT_COUNT)	// all Point is setted Color, so Complete
		return SUCCESS;
	
	// var = Select unassigned virable(variable[csp], assignment, csp)
	if((var = selectVariable()) < 0) {
		return FAILURE;
	}

	if(boolLog) printf("Selected Variable is %dth Point\n", var);
	if(boolSave) fprintf(result, "Selected Variable is %dth Point\n", var);

	// for each value in ORDER-DOMAIN-VALUES(var, assignment, csp) do
	for(i=0; i<kColor; i++) {
		if(method == 2) arcConsistency(var);	// when using MAC, did It maintain Consistency? Remove Unacceptable values

		// if value is consistent with assignment according to COSTARAINTS[csp] then
		if(method == 0) {
			if(!checkNeighbor(var, (COLORS)i))
				continue;
			color = (COLORS)i;
		}
		else
			color = selectValues(var);

		if(color == WHITE) {
			if(boolLog) printf("\t%dth Point have no Color!!!\n", var);
			break;
		}
		
		// add {val = value} to assignment
		mapPoints[var].color = color;
		if(boolLog) printf("\t%dth Point Select Color %s\n", var
			, color == 0? "RED": color == 1? "GREEN": color == 2? "YELLOW":"BLUE");
		if(boolSave) fprintf(result, "\t%dth Point Select Color %s\n"
			, var, color == 0? "RED": color == 1? "GREEN": color == 2? "YELLOW":"BLUE");

		if(method > 0) fowardCheck(var, color);	// When Using Foward Check or MAC, start Forward Check

		// result <- Reculsive-Backtracking(assignment, csp)
		// if result != failure, return result
		if((backtrackingSearch(method)))
			return SUCCESS;

		// remove {var = value} from assignment	
		if(method == 0) mapPoints[var].color = WHITE;
		else removeColor(var, color);	// When FAILED and Using Foward Check, Recover Previous Status.

		if(boolLog) printf("\t%dth Point Remove Color %s\n", var
			, color == 0? "RED": color == 1? "GREEN": color == 2? "YELLOW":"BLUE");
		if(boolSave) fprintf(result, "\t%dth Point Remove Color %s\n", var
			, color == 0? "RED": color == 1? "GREEN": color == 2? "YELLOW":"BLUE");
	}

	// return failure
	return FAILURE;
}

// using MRV(Minimum Remaining Values) and MCV(Most Constraining Variable)
int selectVariable()
{
	int i, temp;
	int candidate = -1, minColor = kColor, maxEdges = -1;

	// MRV: find Variable using Minimum Possible Color Count.
	for(i=0; i<POINT_COUNT; i++) {
		if(mapPoints[i].color != WHITE)
			continue;
		temp = mapPoints[i].colorCount;
		if((temp > 0) && (temp < minColor))
			minColor = temp;
		else if(temp == 0) {	// All color of this point cannot be set!
			if(boolLog) printf("\tERROR: %dth Point Cannot have any Color!!\n", i);
			return -1;
		}
	}

	// MCV: find Variable using Maximum having Edges.
	for(i=0; i<POINT_COUNT; i++) {
		if(mapPoints[i].color != WHITE)
			continue;
		if(mapPoints[i].colorCount != minColor)
			continue;	// check tie-Breaker

		temp = mapPoints[i].edgeCount;
		if(temp > maxEdges) {
			maxEdges = temp;
			candidate = i;
		}
	}

	return candidate;
}

// using LCV(Least Constraning Value)
COLORS selectValues(int var)
{
	int COLOR, count = 0, itemp = POINT_COUNT;
	COLORS candidate = WHITE;
	for(COLOR=RED; COLOR<kColor; COLOR++) {
		if(mapPoints[var].colorState[COLOR] > 0)
			continue;
		if(!checkNeighbor(var, (COLORS)COLOR))
			continue;

		count = 0;
		MapEdge * temp = mapPoints[var].edges;
		while(temp!=NULL) {
			if((mapPoints[temp->point2].color == WHITE) && (mapPoints[temp->point2].colorState[COLOR] == 0))
				count++;
			temp = temp->next;
		}
		if(count < itemp) {
			itemp = count;
			candidate = (COLORS)COLOR;
		}

	}

	return candidate;
}

void fowardCheck(int var, COLORS color)
{
	mapEdge * temp = mapPoints[var].edges;
	while(temp != NULL) {
		if((mapPoints[temp->point2].color == WHITE) && (mapPoints[temp->point2].colorState[color] == 0)) {
			mapPoints[temp->point2].colorState[color] += 1;
			mapPoints[temp->point2].colorCount--;
			if(boolLog) printf("\tFoward Check: %dth Point Remove Color %s\n", temp->point2
				, color == 0? "RED": color == 1? "GREEN": color == 2? "YELLOW":"BLUE");
			if(boolSave) fprintf(result, "\tFoward Check: %dth Point Remove Color %s\n", temp->point2
				, color == 0? "RED": color == 1? "GREEN": color == 2? "YELLOW":"BLUE");
		}
		temp = temp->next;
	}
}

void removeColor(int var, COLORS color)
{
	mapPoints[var].color = WHITE;
	mapPoints[var].colorState[color] += 1;
	mapPoints[var].colorCount--;

	mapEdge * temp = mapPoints[var].edges;
	while(temp != NULL) {
		if((mapPoints[temp->point2].color == WHITE) && (mapPoints[temp->point2].colorState[color] > 0)) {
			mapPoints[temp->point2].colorState[color] -= 1;
			mapPoints[temp->point2].colorCount++;
			if(boolLog) printf("\tForward Recorver: %dth Point Recover Color %s\n", temp->point2
				, color == 0? "RED": color == 1? "GREEN": color == 2? "YELLOW":"BLUE");
			if(boolSave) fprintf(result, "\tForward Recorver: %dth Point Recover Color %s\n", temp->point2
				, color == 0? "RED": color == 1? "GREEN": color == 2? "YELLOW":"BLUE");
		}
		temp = temp->next;
	}
}

void arcConsistency(int var)
{
	int Xi, Xj, count = 0;

	mapEdge * arcQueue = (mapEdge*)malloc(sizeof(mapEdge));

	mapEdge * front = arcQueue;
	mapEdge * rear = arcQueue;

	// Initialize Queue, all the arcs in csp
	mapEdge * temp = mapPoints[var].edges;
	while(temp != NULL) {
		if(mapPoints[temp->point2].color == WHITE) {
			pushQueue(arcQueue, temp, false);
			count++;
			rear = arcQueue;
			arcQueue = arcQueue->next;
		}
		temp = temp->next;
	}

	// while queue is not empty do
	while(count > 0) {
	// {Xi, Xj} <- REMOVE-FIRSR(queue)
		Xi = front->point1;
		Xj = front->point2;
		front = front->next;
		count--;

	// if RM-INCONSISTENT-VALUE(Xi, Xj) then
		if(valueInconsistency(Xi, Xj)) {
	//		for each Xk in Neighbors[Xi] do
	//			add {Xk <- Xi} to queue
			temp = mapPoints[Xi].edges;
			while(temp!=NULL) {
				if(mapPoints[temp->point2].color == WHITE) {
					rear = rear->next;
					pushQueue(rear, temp, true);
					count++;
				}
				temp = temp->next;
			}
		}
	}

}

int valueInconsistency(int Xi, int Xj) {
	// remove <- false
	int remove = 0;
	int color, possible = 0;

	// for each x in Domain[Xi] do
	for(color = 0; color<kColor; color++) {
		if(mapPoints[Xi].colorState[color] == 0) { // if value x selected in Domain Xi
			mapPoints[Xj].colorState[color] += 1;
			mapPoints[Xj].colorCount--;

			// if no value in y DOMAIN[Xj] allows (x,y) to satisfy consisteny(Xi, Xj)
			for(possible=0; possible<kColor; possible++) {
				if(mapPoints[Xj].colorState[possible] == 0)
					break;
			}
			if(possible == kColor) {	// dont break; -> no Possible color
				// then delete x from domain[Xi], remove <- true
				mapPoints[Xi].colorState[color] += 1;
				mapPoints[Xi].colorCount--;
				if(boolLog) printf("\tArc Consistency Check: %dth Point Remove Color %s\n", Xi
					, color == 0? "RED": color == 1? "GREEN": color == 2? "YELLOW":"BLUE");
				if(boolSave) fprintf(result, "\tArc Consistency Check: %dth Point Remove Color %s\n", Xi
					, color == 0? "RED": color == 1? "GREEN": color == 2? "YELLOW":"BLUE");
			
				remove = 1;
			}

			mapPoints[Xj].colorState[color] -= 1;
			mapPoints[Xj].colorCount++;
		}
	}
	//	return remove
	return remove;
}

void pushQueue(mapEdge* queue, mapEdge* arc, bool reverse) {
	if(!reverse) {
		queue->point1 = arc->point1;
		queue->point2 = arc->point2;
	}
	else {
		queue->point1 = arc->point2;
		queue->point2 = arc->point1;
	}
	queue->next = (mapEdge*)malloc(sizeof(mapEdge));
}

mapEdge* popQueue(mapEdge* queue) {
	mapEdge* temp = (mapEdge*)malloc(sizeof(mapEdge));
	temp->point1 = queue->point1;
	temp->point2 = queue->point2;
	queue = queue->next;

	return temp;
}

// false: this point's color is same with its neighbor
bool checkNeighbor(int point, COLORS color)
{
	mapEdge * temp = mapPoints[point].edges;
	while(temp!=NULL) {
		if(color == mapPoints[temp->point2].color)
			return false;
		temp = temp->next;
	}
	return true;
}
