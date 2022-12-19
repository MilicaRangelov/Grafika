#include "pch.h"
#include "CGLRenderer.h"

CGLRenderer::CGLRenderer()
{
	this->m_hrc = NULL;

	this->viewR = 20;
	this->viewAngleXY = 30;
	this->viewAngleXZ = 0;

	this->viewPosition = new GLdouble[3]
	{
		this->viewR * cos(M_PI * viewAngleXY / 180) * cos(M_PI * viewAngleXZ / 180),
		this->viewR * sin(M_PI * viewAngleXY / 180),
		this->viewR * cos(M_PI * viewAngleXY / 180) * sin(M_PI * viewAngleXZ / 180)
	};
	this->upVector = new GLdouble[3]{ 0, 1, 0 };
	this->lookingAt = new GLdouble[3]{ 0, 2.5, 0 };

	this->showGrid = this->showAxes = true;

	this->pieceAngle = 0;
	this->sphereR = 1.5;
	this->cylinderH = 2.0;
	this->cylinderR = 0.8;
	this->coneH = 1;
	this->coneR = 0.35;
	this->nSeg = 90;

	this->room = new CGLMaterial(),
		this->pedestal = new CGLMaterial(),
		this->vase = new CGLMaterial(),
		this->light = new CGLMaterial();

	this->pink = true;
	this->red = true;
	this->blue = true;

	this->showNormals = true;
}

CGLRenderer::~CGLRenderer()
{
	if (this->lookingAt)
	{
		delete[] this->lookingAt;
		this->lookingAt = nullptr;
	}

	if (this->viewPosition)
	{
		delete[] this->viewPosition;
		this->viewPosition = nullptr;
	}

	if (this->upVector)
	{
		delete[] this->upVector;
		this->upVector = nullptr;
	}

	if (this->room)
	{
		delete this->room;
		this->room = nullptr;
	}

	if (this->pedestal)
	{
		delete this->pedestal;
		this->pedestal = nullptr;
	}

	if (this->vase)
	{
		delete this->vase;
		this->vase = nullptr;
	}

	if (this->light)
	{
		delete this->light;
		this->light = nullptr;
	}
}

bool CGLRenderer::CreateGLContext(CDC* pDC)
{
	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));

	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 32;
	pfd.iLayerType = PFD_MAIN_PLANE;

	int nPixelFormat = ChoosePixelFormat(pDC->m_hDC, &pfd);

	if (nPixelFormat == 0)
		return false;

	BOOL bResult = SetPixelFormat(pDC->m_hDC, nPixelFormat, &pfd);

	if (bResult == FALSE)
		return false;

	this->m_hrc = wglCreateContext(pDC->m_hDC);

	if (this->m_hrc == NULL)
		return false;

	return true;
}

void CGLRenderer::PrepareScene(CDC* pDC)
{
	wglMakeCurrent(pDC->m_hDC, this->m_hrc);
	// -------------------------------------------

	glClearColor(0, 0, 0, 1);
	glEnable(GL_DEPTH_TEST);

	this->room->SetAmbient(.4, .4, .4, 1);
	this->room->SetDiffuse(.4, .4, .4, 1);

	this->pedestal->SetAmbient(.6, .6, .6, 1);
	this->pedestal->SetDiffuse(.2, .2, .2, 1);

	this->vase->SetShininess(100);

	this->light->SetAmbient(0, 0, 0, 1);
	this->light->SetDiffuse(0, 0, 0, 1);
	this->light->SetSpecular(0, 0, 0, 1);
	this->light->SetShininess(112);

	GLfloat l_model_ambient[] = { .3, .3, .3, 1 };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, l_model_ambient);
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_FALSE);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

	// Direkciono svetlo
	float light_ambient0[] = { .5, .5, .5, 1 };
	float light_diffuse0[] = { 1, 1, 1, 1 };
	float light_specular0[] = { .3, .3, .3, 1 };

	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient0);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse0);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular0);

	// Roze poziciono svetlo
	float light_ambient1[] = { 0.25, 0.0, 0.25, 1 };
	float light_diffuse1[] = { 1, 0, 1, 1 };
	float light_specular1[] = { .35, .0, 0.35, 1 };

	glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient1);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse1);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular1);

	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 25);
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 2);

	glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 1);
	glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0);
	glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0);

	// Crveno poziciono svetlo
	float light_ambient2[] = { .8, 0.0, 0, 1 };
	float light_diffuse2[] = { 1, 0, 0, 1 };
	float light_specular2[] = { .8, 0.0, 0, 1 };

	glLightfv(GL_LIGHT2, GL_AMBIENT, light_ambient2);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, light_diffuse2);
	glLightfv(GL_LIGHT2, GL_SPECULAR, light_specular2);

	glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 25);
	glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, 2);

	glLightf(GL_LIGHT2, GL_CONSTANT_ATTENUATION, 1);
	glLightf(GL_LIGHT2, GL_LINEAR_ATTENUATION, 0);
	glLightf(GL_LIGHT2, GL_QUADRATIC_ATTENUATION, 0);

	// Plavo poziciono svetlo
	float light_ambient3[] = { 0.35, 0.35, 0.0, 1 };
	float light_diffuse3[] = { 1.00, 1.00, 0.00, 1 };
	float light_specular3[] = { 0.35, 0.35, .0, 1 };

	glLightfv(GL_LIGHT3, GL_AMBIENT, light_ambient3);
	glLightfv(GL_LIGHT3, GL_DIFFUSE, light_diffuse3);
	glLightfv(GL_LIGHT3, GL_SPECULAR, light_specular3);

	glLightf(GL_LIGHT3, GL_SPOT_CUTOFF, 25);
	glLightf(GL_LIGHT3, GL_SPOT_EXPONENT, 2);

	glLightf(GL_LIGHT3, GL_CONSTANT_ATTENUATION, 1);
	glLightf(GL_LIGHT3, GL_LINEAR_ATTENUATION, 0);
	glLightf(GL_LIGHT3, GL_QUADRATIC_ATTENUATION, 0);


	// -------------------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DestroyScene(CDC* pDC)
{
	wglMakeCurrent(pDC->m_hDC, this->m_hrc);
	// -------------------------------------------
	wglMakeCurrent(NULL, NULL);

	if (this->m_hrc != NULL)
	{
		wglDeleteContext(this->m_hrc);
		this->m_hrc = NULL;
	}
}

void CGLRenderer::Reshape(CDC* pDC, int w, int h)
{
	wglMakeCurrent(pDC->m_hDC, this->m_hrc);
	// -------------------------------------------

	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(30, (double)w / (double)h, 1, 150);
	glMatrixMode(GL_MODELVIEW);

	// -------------------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DrawScene(CDC* pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------------------
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_DEPTH_TEST);
	glLoadIdentity();

	gluLookAt(this->viewPosition[0], this->viewPosition[1], this->viewPosition[2],
		this->lookingAt[0], this->lookingAt[1], this->lookingAt[2],
		this->upVector[0], this->upVector[1], this->upVector[2]);



	this->Light();

	glEnable(GL_CULL_FACE);
	DrawWalls(100, 50, 50, 50);
	glDisable(GL_CULL_FACE);

	glTranslatef(0.0, -1.5, 0.0);
	this->DrawFigure(this->pieceAngle);
	
	if (this->showAxes)
		this->DrawAxes();
	glFlush();

	//-------------------------------------------------

	SwapBuffers(pDC->m_hDC);
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DrawFigure(double angle)
{
	double heightCylinderSphereDif = this->cylinderH / 2 + this->sphereR/2 - 0.5,
		heightConeSphereDif = this->coneH / 2 + this->sphereR;


	GLdouble eqn2[4] = { 0.0, 1.0, 0.0, 0.0 };
	this->pedestal->Select(GL_FRONT_AND_BACK);

	glPushMatrix(); {

		glClipPlane(GL_CLIP_PLANE0, eqn2);
		glEnable(GL_CLIP_PLANE0);
		glPushMatrix(); {
			glTranslatef(0.0,0.0, -sphereR);
			glRotatef(90.0, 1.0, 0.0, 0.0);
			DrawSphere(sphereR, 15, 15);
		}
		glPopMatrix();

		glDisable(GL_CLIP_PLANE0);

		glTranslatef(0.0, heightCylinderSphereDif, 0.0);
		DrawCylinder(cylinderH, cylinderR, cylinderR, nSeg);
		glTranslatef(0.0, this->cylinderH / 2 + 0.5, 0.0);
		
		glPushMatrix(); {
			glRotatef(45.0, 0.0, 1.0, 0.0);
			//DrawQuadricPrison();
			DrawCuboid(5,5,1,10);
		}
		glPopMatrix();

		glTranslatef(0.0, 0.25 , 0.0);
		DrawHollowCylinderPrisone(0.35, 1.0, 1.4, nSeg, { 1.00, 0.00 ,0.00 }, showNormals);

		glTranslatef(0.0, 0.35, 0.0);
		DrawHollowCylinderPrisone(0.35, 0.7, 1.0, nSeg, { 1.00, 0.00, 1.00 }, showNormals);

		glTranslatef(0.0, 0.35, 0.0);
		DrawHollowCylinderPrisone(0.35, 0.7, 0.7, nSeg, { 0.00, 1.00, 1.00 }, showNormals);

		glTranslatef(0.0, 0.35, 0.0);
		DrawHollowCylinderPrisone(0.35, 0.7, 0.7, nSeg, { 1.00, 0.00, 1.00 }, showNormals);

		glTranslatef(0.0, 0.35, 0.0);
		DrawHollowCylinderPrisone(0.35, 1.0, 0.7, nSeg, { 0.00, 1.00, 1.00 }, showNormals);

		glTranslatef(0.0, 0.35, 0.0);
		DrawHollowCylinderPrisone(0.35, 1.4, 1.0, nSeg, { 1.00, 0.00, 1.00 }, showNormals);

		glTranslatef(0.0, 0.35, 0.0);
		DrawHollowCylinderPrisone(0.35, 1.0, 1.4, nSeg, { 0.00, 1.00, 1.00 }, showNormals);

		glTranslatef(0.0, 0.35, 0.0);
		DrawHollowCylinderPrisone(0.35, 0.7, 1.0, nSeg, { 1.00, 0.00, 1.00 }, showNormals);

		glTranslatef(0.0, 0.35, 0.0);
		DrawHollowCylinderPrisone(0.35, 1.0, 0.7, nSeg, { 0.00, 1.00, 1.00 }, showNormals);

		glTranslatef(0.0, 0.35, 0.0);
		DrawHollowCylinderPrisone(0.35, 0.7, 1.0, nSeg, { 1.00, 0.00, 1.00 }, showNormals);

		glTranslatef(0.0, 0.35, 0.0);
		DrawHollowCylinderPrisone(0.35, 1.0, 0.7, nSeg, { 0.00, 1.00, 1.00 }, showNormals);

		glTranslatef(0.0, 0.35, 0.0);
		DrawHollowCylinderPrisone(0.35, 1.4, 1.0, nSeg, { 1.00, 0.00, 1.00 }, showNormals);
		
	}
	glPopMatrix();


}

void CGLRenderer::DrawCylinder(double h, double rTop, double rBottom, int nSeg, COLOR3F color)
{
	double step = 2 * PI / nSeg;

	//donja strana
	glBegin(GL_TRIANGLE_FAN);
	for (int i = 0; i < nSeg; i++) {
		glVertex3f(cos(step * i) * rBottom, 0, sin(step * i) * rBottom);
	}
	glEnd();

	//gornja strana
	glBegin(GL_TRIANGLE_FAN);

	for (int i = 0; i < nSeg; i++) {
		glVertex3f(cos(step * i) * rTop, h, sin(step * i) * rTop);
	}
	glEnd();

	//stranice
	glBegin(GL_QUAD_STRIP);
	for (int i = 0; i < nSeg + 1; i++) {
		glVertex3f(cos(step * i) * rTop, h, sin(step * i) * rTop);
		glVertex3f(cos(step * i) * rBottom, 0, sin(step * i) * rBottom);
	}
	glEnd();
}



void CGLRenderer::DrawSphere(double r, int nSegAlpha, int nSegBeta, COLOR3F color)
{
	double stepA = 2 * PI / nSegAlpha;
	double stepB = 2 * PI / nSegAlpha;

	//glEnable(GL_NORMALIZE);
	glBegin(GL_QUAD_STRIP);
	this->pedestal->Select();
	for (int i = 0; i < nSegAlpha; i++) {
		for (int j = 0; j < nSegBeta; j++) {
			glNormal3f(cos(stepA * i) * cos(stepB * i), sin(stepA * i), cos(stepA * i) * sin(stepB * i));
			glVertex3f(r * cos(stepA * i) * cos(stepB * j), r + r * sin(stepA * i), r * cos(stepA * i) * sin(stepB * j));
			glVertex3f(r * cos(stepA * (i + 1)) * cos(stepB * j), r + r * sin(stepA * (i + 1)), r * cos(stepA * (i + 1)) * sin(stepB * j));
		}
	}
	glEnd();
	 
	//glDisable(GL_NORMALIZE);
}

void CGLRenderer::DrawHollowCylinderPrisone(double h, double rTop, double rBottom, int nSeg, COLOR3F color , bool showNormal)
{
	double step = 2 * PI / nSeg;

	double dW = M_PI / (180.0 / (double)nSeg),
		hHalf = h / 2,
		rTopHalf = rTop / 2,
		rBottomHalf = rBottom / 2,
		r =  rBottom > rTop? rBottom - rTop : rTop-rBottom,
		L = sqrt(h * h + r * r),
		v = rBottom > rTop? 1 : -1,
		ny = r / L,
		nr = h / L;


	long size = ((nSeg + 1) * 3) << 2 + 12;
	float* vertNorm = new float[size];
	int counter = 0;

	this->vase->SetAmbient(color.R, color.G, color.B, 1);
	this->vase->SetDiffuse(color.R, color.G, color.B, 1);
	this->vase->SetSpecular(.5, 0, 0, 1);
	this->vase->Select(GL_FRONT_AND_BACK);
	
	//glEnable(GL_NORMALIZE);
	//donja strana
	glBegin(GL_QUAD_STRIP);
	for (int i = 0; i < nSeg; i++) {
		glNormal3f(0, -1, 0);
 		glVertex3f(cos(step * i) * rBottom, 0, sin(step * i) * rBottom);
		
	}
	glEnd();

	//gornja strana
	glBegin(GL_QUAD_STRIP);
	for (int i = 0; i < nSeg; i++) {
		glNormal3f(0,1,0);
		glVertex3f(cos(step * i) * rTop, h, sin(step * i) * rTop);
	
	}
	glEnd();

	//stranice
	glBegin(GL_QUAD_STRIP);
	for (int i = 0; i < nSeg + 1; i++) {

		glNormal3f(nr * cos(step * i), ny*v, nr *sin(step * i));
		glVertex3f(cos(step * i) * rTop, h, sin(step * i) * rTop);
		glVertex3f(cos(step * i) * rBottom, 0, sin(step * i) * rBottom);
	
	}
	glEnd();


	if (showNormal)
	{
		glDisable(GL_LIGHTING);
		glLineWidth(1.25);
		glColor3f(0, 1, 0);

		glEnableClientState(GL_VERTEX_ARRAY);

		for (int i = 0; i < nSeg; i+=4) {
		
		
			vertNorm[counter++] = cos(step * i) * rTop;
			vertNorm[counter++] = 0;
			vertNorm[counter++] = sin(step * i)* rTop;
			
			vertNorm[counter++] = cos(step * i) * rTop + cos(step * i) * nr;
			vertNorm[counter++] = ny * v;
			vertNorm[counter++] = sin(step * i)* rTop + sin(step * i) * nr;
	
			vertNorm[counter++] = cos(step * i) * rBottom;
			vertNorm[counter++] = h;
			vertNorm[counter++] = sin(step * i)* rBottom;
			
			vertNorm[counter++] = cos(step * i) * rBottom + cos(step * i) * nr;
			vertNorm[counter++] = h + ny * v;
			vertNorm[counter++] = sin(step * i)* rBottom + sin(step * i) * nr;
		}

		glVertexPointer(3, GL_FLOAT, 0, &vertNorm[0]);

		glDrawArrays(GL_LINES, 0, (counter - 2) / 3);

		glDisableClientState(GL_VERTEX_ARRAY);

		glEnable(GL_LIGHTING);
	}


	if (vertNorm)
	{
		delete[] vertNorm;
		vertNorm = nullptr;
	}

	//glDisable(GL_NORMALIZE);

	
}



void CGLRenderer::DrawWalls(double l, double w, double h, int nStep)
{
	this->room->Select(GL_FRONT_AND_BACK);
	glPushMatrix();
	{
		glTranslatef(0, h / 3, 0);
		DrawCuboid(l, w, h, nStep, false, true);
	}
	glPopMatrix();
}

void CGLRenderer::DrawCuboid(double l, double w, double h, int nStep, bool drawRoof, bool inverted)
{
	double lHalf = l / 2,
		wHalf = w / 2,
		hHalf = h / 2,
		lStep = l / (double)nStep,
		wStep = w / (double)nStep,
		hStep = h / (double)nStep;

	// Desna strana
	if (inverted)
	{
		glPushMatrix();
		glScalef(-1, 1, 1);
	}

	glNormal3f(inverted ? -1 : 1, 0, 0);
	for (double i = hHalf; i > -hHalf; i -= hStep)
	{
		glBegin(GL_QUAD_STRIP);
		{
			for (double j = wHalf; j > -(wHalf + wStep); j -= wStep)
			{
				glVertex3f(lHalf, i, j);
				glVertex3f(lHalf, i - hStep, j);
			}
		}
		glEnd();
	}

	// Leva strana
	glNormal3f(inverted ? 1 : -1, 0, 0);
	for (double i = hHalf; i > -hHalf; i -= hStep)
	{
		glBegin(GL_QUAD_STRIP);
		{
			for (double j = -wHalf; j < (wHalf + wStep); j += wStep)
			{
				glVertex3f(-lHalf, i, j);
				glVertex3f(-lHalf, i - hStep, j);
			}
		}
		glEnd();
	}

	if (inverted)
		glScalef(-1, -1, 1);

	if (drawRoof ^ inverted)
	{
		// Gornja strana
		glNormal3f(0, inverted ? -1 : 1, 0);
		for (double i = -wHalf; i < wHalf; i += wStep)
		{
			glBegin(GL_QUAD_STRIP);
			{
				for (double j = -lHalf; j < (lHalf + lStep); j += lStep)
				{
					glVertex3f(j, hHalf, i);
					glVertex3f(j, hHalf, i + wStep);
				}
			}
			glEnd();
		}
	}

	if (!inverted || drawRoof)
	{
		// Donja strana
		glNormal3f(0, inverted ? 1 : -1, 0);
		for (double i = wHalf; i > -wHalf; i -= wStep)
		{
			glBegin(GL_QUAD_STRIP);
			{
				for (double j = -lHalf; j < (lHalf + lStep); j += lStep)
				{
					glVertex3f(j, -hHalf, i);
					glVertex3f(j, -hHalf, i - wStep);
				}
			}
			glEnd();
		}
	}

	// Prednja strana
	if (inverted)
		glScalef(1, -1, -1);

	glNormal3f(0, 0, inverted ? -1 : 1);
	for (double i = hHalf; i > -hHalf; i -= hStep)
	{
		glBegin(GL_QUAD_STRIP);
		{
			for (double j = -lHalf; j < (lHalf + lStep); j += lStep)
			{
				glVertex3f(j, i, wHalf);
				glVertex3f(j, i - hStep, wHalf);
			}
		}
		glEnd();
	}

	// Zadnja strana
	glNormal3f(0, 0, inverted ? 1 : -1);
	for (double i = hHalf; i > -hHalf; i -= hStep)
	{
		glBegin(GL_QUAD_STRIP);
		{
			for (double j = lHalf; j > -(lHalf + lStep); j -= lStep)
			{
				glVertex3f(j, i, -wHalf);
				glVertex3f(j, i - hStep, -wHalf);
			}
		}
		glEnd();
	}

	if (inverted)
		glPopMatrix();
}



void CGLRenderer::Light()
{
	glEnable(GL_LIGHTING);

	// Direkciono svetlo
	float light_position[] = { .6, 1, .75, 0 };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glEnable(GL_LIGHT0);

	if (this->pink)
	{

		float light_spot_direction1[] = { 0, 0, -1, 1 };
		glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, light_spot_direction1);

		float light_position_pink[] = { 0, 14, 25, 1 };
		glLightfv(GL_LIGHT1, GL_POSITION, light_position_pink);

		this->light->SetEmission(.8, 0, 0.8, 0);
		this->light->SetDiffuse(.8, .0, 0.8, 0);
		this->light->Select(GL_FRONT_AND_BACK);

		glEnable(GL_LIGHT1);

		glPushMatrix(); {
			glTranslatef(0, 14, 25);
			glRotatef(-90, 1, 0, 0);
			DrawSphere(0.6);
		}
		glPopMatrix();
	}
	else
		glDisable(GL_LIGHT1);

	if (this->red)
	{

		float light_spot_direction2[] = { 0, 0, 1, 1 };
		glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, light_spot_direction2);

		float light_position_red[] = { 0, 14, -25, 1 };
		glLightfv(GL_LIGHT2, GL_POSITION, light_position_red);

		this->light->SetEmission(.8, 0.0, 0, 0);
		this->light->SetDiffuse(.8, 0.0, 0, 0);
		this->light->Select(GL_FRONT_AND_BACK);

		glEnable(GL_LIGHT2);

		glPushMatrix(); {
			glTranslatef(0, 14, -25);
			glScalef(1, 1, -1);
			glRotatef(-90, 1, 0, 0);
			DrawSphere(0.6);
			glPopMatrix();
		}
	}
	else
		glDisable(GL_LIGHT2);

	if (this->blue)
	{

		float light_spot_direction3[] = { 0, -1, 0, 1 };
		glLightfv(GL_LIGHT3, GL_SPOT_DIRECTION, light_spot_direction3);

		float ligth_position_blue[] = { 0, 30, 0, 1 };
		glLightfv(GL_LIGHT3, GL_POSITION, ligth_position_blue);

		this->light->SetEmission(.8, 8, .0, 0);
		this->light->SetDiffuse(.8, .8, .0, 0);
		this->light->Select(GL_FRONT_AND_BACK);
		glEnable(GL_LIGHT3);

		glPushMatrix(); {
			glTranslatef(0, 30, 0);
			glScalef(1, -1, 1);
			DrawSphere(0.6);
		}
		glPopMatrix();
	}
	else
		glDisable(GL_LIGHT3);
}

void CGLRenderer::DrawAxes(double lenght)
{
	glLineWidth(lenght);

	glBegin(GL_LINES);
	//x
	glColor3f(1, 0, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(10, 0, 0);
	//y
	glColor3f(0, 1, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 10, 0);
	//z
	glColor3f(0, 0, 1);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, 10);
	glEnd();
}



void CGLRenderer::RotateView(double dXY, double dXZ)
{
	this->viewAngleXY += dXY;
	this->viewAngleXZ += dXZ;

	double dwXY = M_PI * this->viewAngleXY / 180,
		dwXZ = M_PI * this->viewAngleXZ / 180;

	if (dXY)
		this->upVector[1] = signbit(this->viewR * sin(dwXY) - this->viewPosition[1]) ? (signbit(dXY) ? 1 : -1) : (signbit(dXY) ? -1 : 1);

	this->viewPosition[0] = this->viewR * cos(dwXY) * cos(dwXZ);
	this->viewPosition[1] = this->viewR * sin(dwXY);
	this->viewPosition[2] = this->viewR * cos(dwXY) * sin(dwXZ);
}
