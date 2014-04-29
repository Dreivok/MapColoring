/********************************************************************/
/* 2008012647 JANG HYUN JUN in Hanyang Univ.
/* MapScene.cpp: Make result to visual scene
/* Output: Visible Result in Window
/*
/* Homework3 for A.I. 
												Update: 2014.04.24	
														2014.04.2?	*/
/********************************************************************/

#include "MapScene.h"

void mapDisplay() {
	glutInitWindowSize(500, 500);
	glutCreateWindow("MAP COLORING RESULT");

	glClearColor(1.0, 1.0, 1.0, 1.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1.0, X_MAX, -1.0, Y_MAX, -1.0, 1.0);
	glutDisplayFunc(myDisplay);
		glutMainLoop();
}

void myDisplay() {
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	
	glColor3f(0.0, 0.0, 0.0);

	glLineWidth(1.0f);
	glBegin(GL_LINES);
	for(int i=0; i<X_MAX; i++) {
		glVertex2i(i, -1);
		glVertex2i(i, Y_MAX);
	}
	for(int j=0; j<Y_MAX; j++) {
		glVertex2i(-1, j);
		glVertex2i(X_MAX, j);
	}
	glEnd();

	glColor3f(0.0, 0.0, 1.0);
	glLineWidth(3.0f);
	for(int i=0; i<POINT_COUNT; i++) {
		glBegin(GL_LINES);
			mapEdge * temp = mapPoints[i].edges;
			while(temp!=NULL) {
				glVertex2f(mapPoints[i].x, mapPoints[i].y);
				glVertex2f(mapPoints[temp->point2].x, mapPoints[temp->point2].y);
				temp = temp->next;
			}
		glEnd();
	}
	
	for(int i=0; i<POINT_COUNT; i++) {
		char ch[POINT_COUNT];

		glPushMatrix();

		switch(mapPoints[i].color)
		{
		case RED: glColor3f(1.0,0.0,0.0); break;
		case GREEN: glColor3f(0.0,1.0,0.0); break;
		case YELLOW: glColor3f(1.0,1.0,0.0); break;
		case BLUE: glColor3f(0.0,0.0,1.0); break;
		default: glColor3f(0.5, 0.5, 0.5);
		}
		//glutBitmapCharacter

		glTranslatef(mapPoints[i].x,mapPoints[i].y,0.0);
		glutSolidCube(0.7);
		glColor3f(0, 0, 0);
		glPopMatrix();

		glRasterPos2f(mapPoints[i].x - 0.2, mapPoints[i].y);
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, itoa(i, ch, 10)[0]);
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, itoa(i, ch, 10)[1]);
	}
	glutSwapBuffers();
	//glFlush();
}
