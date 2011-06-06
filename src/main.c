#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <time.h>
#include <math.h>

#define SUBDIVISIONS 16
#define RADIUS 0.05
#define FRAMERATE 10
#define MAX_VELOCITY 2
#define MAX_SPHERES 1000
#define SPAWN_RATE 0.1

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

void keyboard_handler(unsigned char c, int x, int y);
void create_spheres(int n);
void move_objects();
float rand_position();
float rand_velocity();
void collision(Sphere *s);
void reshape();
void init(void);
void render_scene(void);
void render_objects(void);
void frame(int id);

Sphere spheres[1000];
int spheres_count;


void keyboard_handler(unsigned char c, int x, int y)
{
	switch (c)
	{
	case 'q':
	case 27:
		exit(0);
		break;
  case '+':
    create_spheres(1 + spheres_count * SPAWN_RATE);
    break;
  case '-':

    spheres_count = spheres_count - spheres_count * SPAWN_RATE < 1 ?
                    0 : (int) spheres_count - spheres_count * SPAWN_RATE;
    break;
	}
}

float rand_position(){
  return (float) rand() / RAND_MAX * (rand() % 2 == 0 ? 1 : -1);
}

float rand_velocity(){
  return 0.01 + 0.01 * (rand() % MAX_VELOCITY) ;
}


void create_spheres(int n)
{
	int i;
	for (i = 0; i < n && spheres_count + 1 < MAX_SPHERES; i++)
	{
		Sphere sphere;
		sphere.pos.x = rand_position();
		sphere.pos.y = rand_position();
    sphere.pos.z = rand_position();
		sphere.radius = RADIUS;

    if (sphere.pos.x - sphere.radius < -1.0) sphere.pos.x = -1.0 + sphere.radius;
    else if (sphere.pos.x + sphere.radius > 1.0) sphere.pos.x = 1.0 - sphere.radius;
    if (sphere.pos.y - sphere.radius < -1.0) sphere.pos.y = -1.0 + sphere.radius;
    else if (sphere.pos.y + sphere.radius > 1.0) sphere.pos.y = 1.0 - sphere.radius;
    if (sphere.pos.z - sphere.radius < -1.0) sphere.pos.z = -1.0 + sphere.radius;
    else if (sphere.pos.z + sphere.radius > 1.0) sphere.pos.z = 1.0 - sphere.radius;

		sphere.velocity.x = rand_velocity();
		sphere.velocity.y = rand_velocity();
    sphere.velocity.z = rand_velocity();
		switch (rand() % 3)
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

void move_objects(){
  int i;
  for(i = 0; i < spheres_count; i++){
    Sphere *s = &spheres[i];
    collision(s);
    s->pos.x += s->velocity.x;
    s->pos.y += s->velocity.y;
    s->pos.z += s->velocity.z;
  }
}

void collision(Sphere *s){
  Vector *v = &s->velocity;
  Vector *pos = &s->pos;
  if (pos->x + v->x - s->radius < -1.0 ||
      pos->x + v->x + s->radius > 1.0){
    v->x *= -1;
  }
  if (pos->y + v->y - s->radius < -1.0 ||
      pos->y + v->y + s->radius > 1.0){
    v->y *= -1;
  }
  if (pos->z + v->z - s->radius < -1.0 ||
      pos->z + v->z + s->radius > 1.0){
    v->z *= -1;
  }
}

void init(void)
{
   GLfloat mat_ambient[] = { 1.0, 1.0, 1.0, 1.0 };
   GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
   GLfloat light_position[] = { 5.0, 5.0, 10.0, 1.0 };
   GLfloat lm_ambient[] = { 0.2, 0.2, 0.2, 1.0 };

   glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
   glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
   glMaterialf(GL_FRONT, GL_SHININESS, 50.0);
   glLightfv(GL_LIGHT0, GL_POSITION, light_position);
   glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lm_ambient);

   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);
   glEnable(GL_DEPTH_TEST);
   glShadeModel (GL_FLAT);

   glClearColor(0.0, 0.0, 0.0, 0.0);
}

void render_objects(void)
{

   glPushMatrix ();

   glMatrixMode(GL_PROJECTION);
/*   glRotatef (40.0, 0.5, 0.5, 0.5);*/
  gluPerspective(65.0, 1.0, 1.2, 1000);


  move_objects();

	int i;
	for (i = 0; i < spheres_count; ++i)
	{

    glPushMatrix ();
    GLfloat sphere_diffuse[] = { spheres[i].color.r, spheres[i].color.g, spheres[i].color.b };
    glTranslatef (spheres[i].pos.x, spheres[i].pos.y, spheres[i].pos.z);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, sphere_diffuse);
    glutSolidSphere (spheres[i].radius, SUBDIVISIONS, SUBDIVISIONS);
    glPopMatrix ();
	}

   glPopMatrix ();
}


void render_scene(void)
{
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      glPushMatrix ();

      render_objects();
      glPopMatrix();
      glutSwapBuffers();
}

void reshape(int w, int h)
{
   glViewport(0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   if (w <= h)
      glOrtho (-2.0, 2.0, -2.0*h/w, 2.0*h/w, -10.0, 10.0);
   else
      glOrtho (-2.0*w/h, 2.0*w/h, -2.0, 2.0, -10.0, 10.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}


void frame(int id){
	glutPostRedisplay();
	glutTimerFunc(FRAMERATE, frame, id);
}

int main(int argc, char** argv)
{
	 srand(time(NULL));
	 spheres_count = 0;

   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
   glutInitWindowSize (800, 600);
   glutInitWindowPosition (100, 100);
   glutCreateWindow (argv[0]);
   init();
   glutReshapeFunc(reshape);
   glutDisplayFunc(render_scene);
   glutKeyboardFunc(keyboard_handler);


	 create_spheres(10);

   glutTimerFunc(FRAMERATE, frame, 0);
   glutMainLoop();
   return 0;
}

