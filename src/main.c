#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <time.h>
#include <math.h>

#define DEBUG true
#define SUBDIVISIONS 100
#define RADIUS 0.05
#define FRAMERATE 10

typedef struct
{
	float x;
	float y;
	float z;
} Vector;

typedef struct
{
	float r;
	float g;
	float b;
} Color;

typedef struct
{
	Vector pos;
	Vector velocity;
	float radius;
	Color color;
} Sphere;

Color red =
{ 1.0, 0.0, 0.0 };
Color green =
{ 0.0, 1.0, 0.0 };
Color blue =
{ 0.0, 0.0, 1.0 };

void paint();
void keyboard_handler(unsigned char c, int x, int y);
void renderSphere(float x, float y, float z, float radius, int subdivisions);
void create_spheres(int n);
void frame();
void animate();
float magnite(Vector v);

Sphere spheres[1000];
int spheres_count;

int main(int argc, char** argv)
{
	srand(time(NULL));
	spheres_count = 0;
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800, 800);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Bouncing Balls");

#if DEBUG
	glClearColor(1.0, 0.167, 1.0, 1.0);
#else
	glClearColor(0.0, 0.0, 0.0, 1.0);
#endif

	glutKeyboardFunc(keyboard_handler);
	glutDisplayFunc(paint);
	glutTimerFunc(FRAMERATE, frame, 0);
	//glutMouseFunc(mouse);

	create_spheres(10);

	glutMainLoop();

	return 0;
}

void frame(int id)
{
	glutPostRedisplay();
	glutTimerFunc(FRAMERATE, frame, 0);
}

void keyboard_handler(unsigned char c, int x, int y)
{
	switch (c)
	{
	case 'q':
		exit(0);
		break;
	}
}

void paint()
{
  animate();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	int i;
	for (i = 0; i < spheres_count; ++i)
	{
		glColor3f(spheres[i].color.r, spheres[i].color.g, spheres[i].color.b);
		renderSphere(spheres[i].pos.x, spheres[i].pos.y, spheres[i].pos.z,
				spheres[i].radius, SUBDIVISIONS);
	}

	glutSwapBuffers();
}

void renderSphere(float x, float y, float z, float radius, int subdivisions)
{
#if DEBUG
	printf("x:%d; y:%d; z:%d\n", x, y, z);
#endif
	glPushMatrix();
	glTranslatef(x, y, z);
	glutSolidSphere(radius, subdivisions, subdivisions);
	glPopMatrix();
}

void create_spheres(int n)
{
	int i;
	for (i = 0; i < n; i++)
	{
		Sphere sphere;
		sphere.pos.x = (float) rand() / RAND_MAX;
		sphere.pos.y = (float) rand() / RAND_MAX;
    sphere.pos.z = (float) rand() / RAND_MAX;
		sphere.velocity.x = -0.01;//(float) rand() / RAND_MAX;
		sphere.velocity.y = 0.02;//(float) rand() / RAND_MAX;
    sphere.velocity.z = 0.01;//(float) rand() / RAND_MAX;
		sphere.radius = RADIUS;
		switch (i % 3)
		{
		case 0:
			sphere.color = red;
			break;
		case 1:
			sphere.color = green;
			break;
		case 2:
			sphere.color = blue;
			break;
		}
		spheres[spheres_count++] = sphere;
	}
}

void animate(){
  int i;
  for(i = 0; i < spheres_count; i++){
    Sphere *s = &spheres[i];
    s->pos.x += s->velocity.x;
    s->pos.y += s->velocity.y;
    s->pos.z += s->velocity.z;
  }
}

