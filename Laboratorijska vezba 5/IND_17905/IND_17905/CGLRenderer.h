#pragma once
#pragma comment(lib, "OpenGL32.lib")
#pragma comment(lib, "glu32.lib")

#include <GL\gl.h>
#include <GL\glu.h>

#define _USE_MATH_DEFINES
#include <math.h>
#include "CGLMaterial.h"
#define PI 3.1416


struct COLOR3F {
	float R;
	float G;
	float B;
};

class CGLRenderer
{
private:
	HGLRC m_hrc;

	GLdouble* lookingAt;
	GLdouble* viewPosition;
	GLdouble* upVector;


	CGLMaterial* room;
	CGLMaterial* pedestal;
	CGLMaterial* vase;
	CGLMaterial* light;


	double viewR;
	double viewAngleXY;
	double viewAngleXZ;

	bool showGrid;
	bool showAxes;

	double pieceAngle;

	double sphereR;
	double cylinderH;
	double cylinderR;
	double coneH;
	double coneR;
	int nSeg;



public:
	bool pink, red, blue;
	bool showNormals;

	CGLRenderer();
	~CGLRenderer();



	bool CreateGLContext(CDC* pDC);
	void PrepareScene(CDC* pDC);
	void DestroyScene(CDC* pDC);
	void Reshape(CDC* pDC, int w, int h);
	void DrawScene(CDC* pDC);


	void DrawFigure(double angle);
	void DrawCylinder(double h = 1, double rTop = 1, double rBottom = -1, int nSeg = 90, COLOR3F color = { 0.76, 0.76, 0.76 });
	void DrawSphere(double r = 1, int nSegAlpha = 90, int nSegBeta = 90, COLOR3F color = { 0.76, 0.76, 0.76 });
	void DrawHollowCylinderPrisone(double h = 1, double rTop = 1, double rBottom = -1, int nSeg = 90, COLOR3F color = { 0.76, 0.76, 0.76 }, bool showNormal = true);

	void DrawWalls(double l, double w, double h, int nStep);
	void DrawCuboid(double l, double w, double h, int nStep, bool drawRoof = true, bool inverted = false);
	void Light();
	void DrawAxes(double lenght = 2);
	void RotatePiece(double dAngle) { this->pieceAngle += dAngle; }
	void RotateView(double dXY, double dXZ);
};

