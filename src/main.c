/* Desenvolvimento teste de física de corpos rígidos */

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <time.h>
#include <math.h>

#define SUBDIVISIONS 16
#define RADIUS 0.05
#define FRAMERATE 10
#define MAX_VELOCITY 100
#define MAX_SPHERES 1000
#define SPAWN_RATE 0.1

#define BOX_NEAR -2.0
#define BOX_FAR -4.0
#define BOX_LEFT -1.0
#define BOX_RIGHT 1.0
#define BOX_TOP -1.0
#define BOX_BOTTOM 1.0

#define CELLS_PER_SIDE 4
#define CELL_SIZE 0.5

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

Color red = { 1.0, 0.0, 0.0 };
Color green = { 0.0, 1.0, 0.0 };
Color blue = { 0.0, 0.0, 1.0 };

void keyboard_handler(unsigned char c, int x, int y);
void create_spheres();
void move_objects();
/*float rand_position(char axis);*/
float rand_velocity();
void collision(Sphere *s);
void reshape();
void init(void);
void render_scene(void);
void render_objects(void);
void frame(void);
void set_delta_time(void);

Sphere spheres[1000];
int spheres_count;

float delta_time;
float current_time;


void keyboard_handler(unsigned char c, int x, int y)
{
	switch (c)
	{
	case 'q':
	case 27:
		exit(0);
		break;
  /* Desativação dos comandos para criação de novas esferas

  case '+':
    create_spheres(1 + spheres_count * SPAWN_RATE);
    break;
  case '-':

    spheres_count = spheres_count - spheres_count * SPAWN_RATE < 1 ?
                    0 : (int) spheres_count - spheres_count * SPAWN_RATE;
    break;
  */
	}
}

/*float rand_position(char axis){*/
/*  if (axis == 'x') {*/
/*    if (rand() % 2 == 0){*/
/*      return (float) rand() / RAND_MAX + BOX_LEFT;*/
/*    } else {*/
/*      return (float) -rand() / RAND_MAX + BOX_RIGHT;*/
/*    }*/
/*  } else if (axis == 'y') {*/
/*    if (rand() % 2 == 0){*/
/*      return (float) rand() / RAND_MAX + BOX_TOP;*/
/*    } else {*/
/*      return (float) -rand() / RAND_MAX + BOX_BOTTOM;*/
/*    }*/
/*  } else if (axis == 'z') {*/
/*    if (rand() % 2 == 0){*/
/*      return (float) -rand() / RAND_MAX + BOX_NEAR;*/
/*    } else {*/
/*      return (float) rand() / RAND_MAX + BOX_FAR;*/
/*    }*/
/*  }*/
/*}*/

float rand_velocity(){
  return 0.01 + 0.01 * (rand() % MAX_VELOCITY) ;
}


void create_spheres()
{
	int x;
	for (x = 0; x < CELLS_PER_SIDE; x++)
	{
	  int y;
	  for (y = 0; y < CELLS_PER_SIDE; y++)
	  {
  	  int z;
	    for (z = 0; z < CELLS_PER_SIDE; z++)
	    {
	      Sphere sphere;
        sphere.pos.x = BOX_LEFT + x * CELL_SIZE;
        sphere.pos.y = BOX_TOP + y * CELL_SIZE;
        sphere.pos.z = BOX_FAR + z * CELL_SIZE;
        sphere.radius = RADIUS;

        if (sphere.pos.x - sphere.radius < BOX_LEFT) sphere.pos.x = BOX_LEFT + sphere.radius;
        else if (sphere.pos.x + sphere.radius > BOX_RIGHT) sphere.pos.x = BOX_RIGHT - sphere.radius;
        if (sphere.pos.y - sphere.radius < BOX_TOP) sphere.pos.y = BOX_TOP + sphere.radius;
        else if (sphere.pos.y + sphere.radius > BOX_BOTTOM) sphere.pos.y = BOX_BOTTOM - sphere.radius;
        if (sphere.pos.z - sphere.radius < BOX_NEAR) sphere.pos.z = BOX_NEAR - sphere.radius;
        else if (sphere.pos.z + sphere.radius > BOX_FAR) sphere.pos.z = BOX_FAR + sphere.radius;

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
	}
}

void move_objects(){
  int i;
  for(i = 0; i < spheres_count; i++){
    Sphere *s = &spheres[i];
    collision(s);
    s->pos.x += s->velocity.x * delta_time;
    s->pos.y += s->velocity.y * delta_time;
    s->pos.z += s->velocity.z * delta_time;
  }
}

void collision(Sphere *s){
  Vector *v = &s->velocity;
  Vector *pos = &s->pos;
  if (pos->x + v->x * delta_time - s->radius < BOX_LEFT ||
      pos->x + v->x * delta_time + s->radius > BOX_RIGHT){
    v->x *= -1;
  }
  if (pos->y + v->y * delta_time - s->radius < BOX_TOP ||
      pos->y + v->y * delta_time + s->radius > BOX_BOTTOM){
    v->y *= -1;
  }
  if (pos->z + v->z * delta_time - s->radius < BOX_FAR ||
      pos->z + v->z * delta_time + s->radius > BOX_NEAR){
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

   glutReshapeFunc(reshape);
   glutDisplayFunc(render_scene);
   glutKeyboardFunc(keyboard_handler);

	 create_spheres();

   glutIdleFunc(frame);
}

void render_objects(void)
{

/*   glPushMatrix ();*/

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

/*   glPopMatrix ();*/
}


void render_scene(void)
{
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
/*      glPushMatrix ();*/

      glLoadIdentity();

      render_objects();

/*      glPopMatrix();*/
      glutSwapBuffers();
}

void reshape(int w, int h)
{
   glViewport(0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   if (w <= h) {
      gluPerspective(65.0f, h/w, -BOX_NEAR, -BOX_FAR);
/*      glOrtho (-2.0, 2.0, -2.0*h/w, 2.0*h/w, -10.0, 10.0);*/
   } else {
/*    glOrtho (-2.0*w/h, 2.0*w/h, -2.0, 2.0, -10.0, 10.0);*/
      gluPerspective(65.0f, w/h, -BOX_NEAR, -BOX_FAR);
  }
   glMatrixMode(GL_MODELVIEW);
//   glLoadIdentity();
}


void frame(){
  set_delta_time();
	glutPostRedisplay();
}

void set_delta_time(){
  float previous_time = current_time;
  current_time = glutGet(GLUT_ELAPSED_TIME);
  delta_time = (current_time - previous_time) / 1000;
}

int main(int argc, char** argv)
{
	 srand(time(NULL));
	 spheres_count = 0;
	 delta_time = 0;
	 current_time = 0;

   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
   glutInitWindowSize (800, 600);
   glutInitWindowPosition (100, 100);
   glutCreateWindow (argv[0]);
   init();

   glutMainLoop();
   return 0;
}

