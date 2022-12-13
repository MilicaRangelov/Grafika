#pragma once
#pragma comment(lib, "OpenGL32.lib")
#pragma comment(lib, "glu32.lib")

#include <GL\gl.h>
#include <GL\glu.h>

#define _USE_MATH_DEFINES
#include <math.h>

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
	CGLRenderer();
	~CGLRenderer();

	bool CreateGLContext(CDC* pDC);
	void PrepareScene(CDC* pDC);
	void DestroyScene(CDC* pDC);
	void Reshape(CDC* pDC, int w, int h);
	void DrawScene(CDC* pDC);


	void DrawFigure(double angle);
	void DrawCylinder(double h = 1, double rTop = 1, double rBottom = -1, int nSeg = 90, COLOR3F color = { 0.51, 0.92, 0.31 });
	void DrawCone(double h = 1, double r = 1, int nSeg = 90, COLOR3F color = { 0.51, 0.92, 0.31 });
	void DrawSphere(double r = 1, int nSegAlpha = 90, int nSegBeta = 90, COLOR3F color = { 0.14, 0.74, 0.32 });

	void DrawAxes(double lenght = 10, bool useNegative = false);
	void DrawGrid(double width = 10, double height = 10, int nSegW = 10, int nSegH = 10, COLOR3F color = { 1.0, 1.0,1.0 });
	void RotatePiece(double dAngle) { this->pieceAngle += dAngle; }
	void RotateView(double dXY, double dXZ);
};

