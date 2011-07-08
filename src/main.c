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

typedef struct
{
  Sphere *spheres[125];
  int num;
  Vector pos;
} Cell;

Color red = { 1.0, 0.0, 0.0 };
Color green = { 0.0, 1.0, 0.0 };
Color blue = { 0.0, 0.0, 1.0 };

void keyboard_handler(unsigned char c, int x, int y);
void create_spheres();
void move_objects();
/*float rand_position(char axis);*/
float rand_velocity();
void collision(Sphere *s);
void box_collision(Sphere *s);
void spheres_collision(Sphere *s);
void reshape();
void init(void);
void render_scene(void);
void render_objects(void);
void frame(void);
void set_delta_time(void);
void init_cells(void);
Cell * get_cell(Sphere *s);
void remove_from_cell(Sphere *s);
void add_to_cell(Sphere *s);
int find_sphere_in_cell(Sphere *s, Cell *c);
void sphere_sphere_collision(Sphere *s1, Sphere *s2);
void in_cell_collision(Cell *c, Sphere *s);
float dot_v(Vector *v1, Vector *v2);
Vector normalize_v(Vector *v);
float magnitude_v(Vector *v);
Vector subtract_v(Vector *v1, Vector *v2);
Vector add_v(Vector *v1, Vector *v2);
Vector multiplicate_v_e(Vector *v, float e);

Sphere spheres[1000];
int spheres_count;

float delta_time;
float current_time;
Cell cells[4][4][4];

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

void init_cells()
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
	      Cell cell;
        cell.pos.x = BOX_LEFT + x * CELL_SIZE;
        cell.pos.y = BOX_TOP + y * CELL_SIZE;
        cell.pos.z = BOX_FAR + z * CELL_SIZE;
        cell.num = 0;

/*	      Sphere sphere;*/
/*        sphere.pos.x = BOX_LEFT + x * CELL_SIZE;*/
/*        sphere.pos.y = BOX_TOP + y * CELL_SIZE;*/
/*        sphere.pos.z = BOX_FAR + z * CELL_SIZE;*/
/*        sphere.radius = RADIUS;*/

/*        if (sphere.pos.x - sphere.radius < BOX_LEFT) sphere.pos.x = BOX_LEFT + sphere.radius;*/
/*        else if (sphere.pos.x + sphere.radius > BOX_RIGHT) sphere.pos.x = BOX_RIGHT - sphere.radius;*/
/*        if (sphere.pos.y - sphere.radius < BOX_TOP) sphere.pos.y = BOX_TOP + sphere.radius;*/
/*        else if (sphere.pos.y + sphere.radius > BOX_BOTTOM) sphere.pos.y = BOX_BOTTOM - sphere.radius;*/
/*        if (sphere.pos.z - sphere.radius < BOX_NEAR) sphere.pos.z = BOX_NEAR - sphere.radius;*/
/*        else if (sphere.pos.z + sphere.radius > BOX_FAR) sphere.pos.z = BOX_FAR + sphere.radius;*/

/*    		sphere.velocity.x = rand_velocity();*/
/*    		sphere.velocity.y = rand_velocity();*/
/*        sphere.velocity.z = rand_velocity();*/

/*    		switch (rand() % 3)*/
/*		    {*/
/*    		case 0:*/
/*    			sphere.color = red;*/
/*			    break;*/
/*      		case 1:*/
/*    			sphere.color = green;*/
/*    			break;*/
/*    		case 2:*/
/*			    sphere.color = blue;*/
/*    			break;*/
/*    		}*/

/*    		spheres[spheres_count] = sphere;*/
/*    		cell.spheres[cell.num++] = &spheres[spheres_count++];*/
    		cells[x][y][z] = cell;
	    }
	  }
	}
  int i;
  for(i=0; i< 4; i++){
    int j;
    for(j=0; j<4; j++){
    int k;
    for(k=0; k < 4; k++){
    	Sphere s;
    	s.pos.x = BOX_LEFT + i * CELL_SIZE;
    	s.pos.y = BOX_TOP + j * CELL_SIZE;
    	s.pos.z = BOX_FAR + k * CELL_SIZE;
    	s.velocity.x = rand_velocity();
    	s.velocity.y = rand_velocity();
	    s.velocity.z = rand_velocity();
    	s.color = red;
	    s.radius = RADIUS;
    		switch (rand() % 3)
		    {
    		case 0:
    			s.color = red;
			    break;
      		case 1:
    			s.color = green;
    			break;
    		case 2:
			    s.color = blue;
    			break;
    		}
    	spheres[spheres_count++] = s;
    }}
  }


/* Colisão entre duas partículas */
/*  Sphere s;*/
/*  s.radius = RADIUS;*/
/*  s.pos.x = BOX_LEFT + 1.0f;*/
/*  s.pos.y = BOX_TOP + 1.5f;*/
/*  s.pos.z = BOX_FAR + 1.0f;*/
/*  s.velocity.x = 0.5f;*/
/*  s.velocity.y = -0.9f;*/
/*  s.velocity.z = 0.0f;*/
/*  s.color = red;*/
/*  spheres[spheres_count++] = s;*/

/*  Sphere s2;*/
/*  s2.radius = RADIUS;*/
/*  s2.pos.x = BOX_LEFT + 1.5f;*/
/*  s2.pos.y = BOX_TOP + 1.0f;*/
/*  s2.pos.z = BOX_FAR + 1.0f;*/
/*  s2.velocity.x = -0.5f;*/
/*  s2.velocity.y = 0.0f;*/
/*  s2.velocity.z = 0.0f;*/
/*  s2.color = blue;*/
/*  spheres[spheres_count++] = s2;*/
}

Cell * get_cell(Sphere *s){
  int x = CELL_SIZE * CELLS_PER_SIDE * s->pos.x;
  int y = CELL_SIZE * CELLS_PER_SIDE * s->pos.y;
  int z = CELL_SIZE * CELLS_PER_SIDE * s->pos.z;

  return &cells[x][y][z];
}

int find_sphere_in_cell(Sphere *s, Cell *c){
  int i = 0;
  int found = 0;
  while (!found && i < c->num){
    if (c->spheres[i] == s){
      found = 1;
    } else {
      i++;
    }
  }
  if (found){
    return i;
  } else {
    return -1;
  }
}

void remove_from_cell(Sphere *s){
  Cell *cell = get_cell(s);
  int pos = find_sphere_in_cell(s, cell);
  if (pos != -1) {
    cell->spheres[pos] = NULL;
    if (pos == cell->num) {
      cell->num--;
    }
  }
}

void add_to_cell(Sphere *s){
  Cell *cell = get_cell(s);
  int pos = find_sphere_in_cell(NULL, cell);
  if (pos != -1) {
    cell->spheres[pos] = s;
    if (pos == cell->num) {
      cell->num++;
    }
  }
}

void move_objects(){
  int i;
  for(i = 0; i < spheres_count; i++){
    Sphere *s = &spheres[i];
/*    printf("%p:%f,%f,%f\n", s, s->velocity.x, s->velocity.y, s->velocity.z);*/
    collision(s);
    remove_from_cell(s);
    s->pos.x += s->velocity.x * delta_time;
    s->pos.y += s->velocity.y * delta_time;
    s->pos.z += s->velocity.z * delta_time;
    add_to_cell(s);
  }
}

void box_collision(Sphere *s){
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

float dot_v(Vector *v1, Vector *v2){
  return (v1->x * v2->x) + (v1->y * v2->y) + (v1->z * v2->z);
}

Vector normalize_v(Vector *v){
  float length = magnitude_v(v);
  Vector n;
  n.x = v->x / length;
  n.y = v->y / length;
  n.z = v->z / length;
  return n;
}

float magnitude_v(Vector *v){
  return sqrtf(pow(v->x, 2) + pow(v->y, 2) + pow(v->z, 2));
}

Vector subtract_v(Vector *v1, Vector *v2){
  Vector sub;
  sub.x = v1->x - v2->x;
  sub.y = v1->y - v2->y;
  sub.z = v1->z - v2->z;
  return sub;
}

Vector add_v(Vector *v1, Vector *v2) {
  Vector add;
  add.x = v1->x - v2->x;
  add.y = v1->y - v2->y;
  add.z = v1->z - v2->z;
  return add;
}

Vector multiplicate_v_e(Vector *v, float e) {
  Vector mul;
  mul.x = v->x * e;
  mul.y = v->y * e;
  mul.z = v->z * e;
  return mul;
}

void sphere_sphere_collision(Sphere *s1, Sphere *s2){
/*  Vector vel1 = multiplicate_v_e(&s1->velocity, delta_time);*/
/*  Vector vel2 = multiplicate_v_e(&s2->velocity, delta_time);*/
/*  Vector pos1 = add_v(&s1->pos, &vel1);*/
/*  Vector pos2 = add_v(&s2->pos, &vel2);*/
/*  Vector distance = subtract_v(&pos1, &pos2);*/
/*printf("%f\n", magnitude_v(&distance));*/

Vector s = subtract_v(&s1->pos, &s2->pos); // vector between the centers of each sphere
    Vector v = subtract_v(&s1->velocity, &s2->velocity); // relative velocity between spheres

    float r = s1->radius + s2->radius;
    float c1 = dot_v(&s, &s) - r*r; // if negative, they overlap

    float a1 = dot_v(&v,&v);
    if (a1 < 0.0001f)
        return; // does not move towards each other

    float b1 = dot_v(&v,&s);
    if (b1 >= 0.0)
        return ; // does not move towards each other

    float d1 = b1*b1 - a1*c1;
    if (d1 < 0.0)
        return ; // no real roots ... no collision


    if (c1 > 0.0)
      return;

/*  if (magnitude_v(&distance) < s1->radius + s2->radius) {*/

/*  printf("Colidindo: %p, %p\n", s1, s2);*/

    Vector x = subtract_v(&s1->pos, &s2->pos);
    x = normalize_v(&x);
    Vector v1 = s1->velocity;
    float x1 = dot_v(&x, &v1);
    Vector v1x = multiplicate_v_e(&x, x1);

    Vector v1y = subtract_v(&v1, &v1x);
    float m1 = s1->radius;

    x = multiplicate_v_e(&x, -1);
    Vector v2 = s2->velocity;
    float x2 = dot_v(&x, &v2);
    Vector v2x = multiplicate_v_e(&x, x2);
    Vector v2y = subtract_v(&v2, &v2x);
    float m2 = s2->radius;

/*    printf("%p: %f,%f,%f\n", s1, s1->velocity.x, s1->velocity.y, s1->velocity.z);*/
    s1->velocity = add_v(&v2x, &v1y);
    s2->velocity = add_v(&v1x, &v2y);
/*        printf("%p: %f,%f,%f\n\n", s1, s1->velocity.x, s1->velocity.y, s1->velocity.z);*/
/*  }*/
}

void in_cell_collision(Cell *cell, Sphere *s){
  Vector *v = &s->velocity;
  Vector *pos = &s->pos;
  int i;
  for (i = 0; i < cell->num; i++){
    if(s != cell->spheres[i]) {
      sphere_sphere_collision(s, cell->spheres[i]);
    }
  }
}

void spheres_collision(Sphere *s){

/* Testando método óbvio */
  int i;
  for (i = 0; i < spheres_count; i++) {
    int j;
    for (j = 0; j < spheres_count; j++) {
      if (i != j) {
        sphere_sphere_collision(&spheres[i], &spheres[j]);
      }
    }
  }
  return;

  /* Colisão por grid a partir daqui */

  int x = CELL_SIZE * CELLS_PER_SIDE * s->pos.x;
  int y = CELL_SIZE * CELLS_PER_SIDE * s->pos.y;
  int z = CELL_SIZE * CELLS_PER_SIDE * s->pos.z;

  Cell *cell = &cells[x][y][z];
  in_cell_collision(cell, s);

  int new_x = x;
  int new_y = y;
  int new_z = z;

  if (s->pos.x - s->radius < cell->pos.x && x > 0){
    new_x = x-1;
    in_cell_collision(&cells[new_x][y][z], s);
  } else if (s->pos.x + s->radius > cell->pos.x + CELL_SIZE && x < CELLS_PER_SIDE) {
    new_x = x+1;
    in_cell_collision(&cells[new_x][y][z], s);
  }

  if (s->pos.y - s->radius < cell->pos.y && y > 0){
    new_y = y-1;
    in_cell_collision(&cells[x][new_y][z], s);
    if (new_x != x) {
      in_cell_collision(&cells[new_x][new_y][z], s);
    }
  } else if (s->pos.y + s->radius > cell->pos.y + CELL_SIZE && y < CELLS_PER_SIDE) {
    new_y = y+1;
    in_cell_collision(&cells[x][new_y][z], s);
    if (new_x != x) {
      in_cell_collision(&cells[new_x][new_y][z], s);
    }
  }

  if (s->pos.z - s->radius < cell->pos.z && z > 0){
    new_z = z-1;
    in_cell_collision(&cells[x][y][new_z], s);
    if (new_x != x) {
      in_cell_collision(&cells[new_x][y][new_z], s);
      if (new_y != y) {
        in_cell_collision(&cells[new_x][new_y][new_z], s);
      }
    }
    if (new_y != y) {
      in_cell_collision(&cells[x][new_y][new_z], s);
    }
  } else if (s->pos.z + s->radius > cell->pos.z + CELL_SIZE && z < CELLS_PER_SIDE) {
    new_z = z+1;
    in_cell_collision(&cells[x][y][new_z], s);
  }
}

void collision(Sphere *s){
  box_collision(s);
  spheres_collision(s);
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

	 init_cells();

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

