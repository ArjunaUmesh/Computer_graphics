/*
 * GLUT Shapes Demo
 *
 * Written by Nigel Stewart November 2003
 *
 * This program is test harness for the sphere, cone
 * and torus shapes in GLUT.
 *
 * Spinning wireframe and smooth shaded shapes are
 * displayed until the ESC or q key is pressed.  The
 * number of geometry stacks and slices can be adjusted
 * using the + and - keys.
 */

#include <windows.h>
#include<cmath>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include<iostream>
#include<vector>
#include <stdlib.h>
using namespace std;

static int slices = 16;
static int stacks = 16;

static int x_length=720;
static int y_length=640;

/* GLUT callback Handlers */

class Point
{
    public:
        double X;
        double Y;

        Point()
        {
            X=0;
            Y=0;
        }
        Point(double x,double y)
        {
            this->X=x;
            this->Y=y;
        }

        void xy(double x,double y)
        {
            this->X=x;
            this->Y=y;
        }
        double x()
        {
            return X;
        }
        double y()
        {
            return Y;
        }
};

void drawLine(Point A,Point B,double r,double g,double b);
void Liang_Barsky(Point bl,Point tr,Point a,Point B);
void drawLine(Point A,Point B,double r,double g, double b);

static void resize(int width, int height)
{
    const float ar = (float) width / (float) height;

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-ar, ar, -1.0, 1.0, 2.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity() ;
}

void init()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0,x_length,0,y_length);
    glClearColor(0,0,0,0);
}

//
//Point p1(100,100);
//Point p2(200,200);

vector<Point> point;
int count=0;

void display()
{
//    glClearColor(0, 0, 0.502, 1);
    glClear(GL_COLOR_BUFFER_BIT); // clear display window

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    const double w = glutGet( GLUT_WINDOW_WIDTH );
    const double h = glutGet( GLUT_WINDOW_HEIGHT );
    gluOrtho2D(0.0, w, 0.0, h);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glColor3f(1, 1, 1);
    glPointSize(3.0);

    Point bl(-100,0);
    Point tr(200,200);
    Point A(-50,100);
    Point B(100,100);

    if(point.size()>1)
    {
        bl=point[0];
        tr=point[1];
        glBegin(GL_LINES);
            glVertex2i(bl.x()+x_length/2,bl.y()+y_length/2);
            glVertex2i(bl.x()+x_length/2,tr.y()+y_length/2);

            glVertex2i(bl.x()+x_length/2,tr.y()+y_length/2);
            glVertex2i(tr.x()+x_length/2,tr.y()+y_length/2);

            glVertex2i(tr.x()+x_length/2,tr.y()+y_length/2);
            glVertex2i(tr.x()+x_length/2,bl.y()+y_length/2);

            glVertex2i(tr.x()+x_length/2,bl.y()+y_length/2);
            glVertex2i(bl.x()+x_length/2,bl.y()+y_length/2);

        glEnd();
    }
    if(point.size()>3)
    {
        Point A=point[2];
        Point B=point[3];
        //drawLine(A,B,0.502,1,1);
        Liang_Barsky(bl,tr,A,B);
    }

    glFlush();
}

void Liang_Barsky(Point bl,Point tr,Point A,Point B)
{

    glBegin(GL_LINES);
        glVertex2i(bl.x()+x_length/2,bl.y()+y_length/2);
        glVertex2i(bl.x()+x_length/2,tr.y()+y_length/2);

        glVertex2i(bl.x()+x_length/2,tr.y()+y_length/2);
        glVertex2i(tr.x()+x_length/2,tr.y()+y_length/2);

        glVertex2i(tr.x()+x_length/2,tr.y()+y_length/2);
        glVertex2i(tr.x()+x_length/2,bl.y()+y_length/2);

        glVertex2i(tr.x()+x_length/2,bl.y()+y_length/2);
        glVertex2i(bl.x()+x_length/2,bl.y()+y_length/2);

    glEnd();
    double delX=B.x()-A.x();
    double delY=B.y()-A.y();

    vector<double> p(4);
    vector<double> q(4);
    p[0]=-delX;
    p[1]=delX;
    p[2]=-delY;
    p[3]=delY;

    q[0]=A.x()-bl.x();
    q[1]=tr.x()-A.x();
    q[2]=A.y()-bl.y();
    q[3]=tr.y()-A.y();

    double t1=0.0;
    double t2=1.0;
    int out=0;

    for(int i=0;i<4;i++)
    {
        if(p[i]==0.0)
        {
            if(q[i]<0)
            {
                cout<<"Outside window \n";
                out=1;
                break;
            }
        }else
        {
            double ri=q[i]/p[i];
            if(p[i]<0)
            {
                t1=(ri>t1)?ri:t1;
            }
            if(p[i]>0)
            {
                t2=(ri<t2)?ri:t2;
            }
        }

    }
    cout<<t1<<" "<<t2<<endl;

    double ax=A.x();
    double ay=A.y();
    Point Aclip(ax+t1*delX,ay+t1*delY);
    Point Bclip(ax+t2*delX,ay+t2*delY);

    if(t1>t2 || out==1)
    {
        drawLine(A,B,0,1,0.2);
    }else
    {
        drawLine(Aclip,A,0,1,0.2);
        drawLine(Bclip,B,0,1,0.2);
        drawLine(Aclip,Bclip,0.502,0,0);

    }
}

void drawLine(Point A,Point B,double r,double g, double b)
{
    glColor3f(r,g,b);
    glBegin(GL_LINES);
        glVertex2i(A.x()+x_length/2,A.y()+y_length/2);
        glVertex2i(B.x()+x_length/2,B.y()+y_length/2);
    glEnd();
    glFlush();
}

static void key(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 27 :
        case 'q':
            exit(0);
            break;

        case '+':
            slices++;
            stacks++;
            break;

        case '-':
            if (slices>3 && stacks>3)
            {
                slices--;
                stacks--;
            }
            break;

    }

    glutPostRedisplay();
}

void mouse(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        Point p(x-x_length/2,(y_length-y)-y_length/2);
        if(point.size()>=4)
        {
            Point bl=point[0];
            Point tr=point[1];
            point.clear();
            point.push_back(bl);
            point.push_back(tr);
        }
        point.push_back(p);


    }else if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
    {
        point.clear();
    }
    glutPostRedisplay();
}

static void idle(void)
{
    glutPostRedisplay();
}

const GLfloat light_ambient[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
const GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_position[] = { 2.0f, 5.0f, 5.0f, 0.0f };

const GLfloat mat_ambient[]    = { 0.7f, 0.7f, 0.7f, 1.0f };
const GLfloat mat_diffuse[]    = { 0.8f, 0.8f, 0.8f, 1.0f };
const GLfloat mat_specular[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat high_shininess[] = { 100.0f };

/* Program entry point */

int main(int argc, char *argv[])
{
//    vector<vector<vector<vector<int>>>> arr = {{{{1, 2, 3}, {4, 5, 6}, {7, 8, 9, 10}}, {{1, 2, 3}, {4, 5, 6}, {7, 8, 9, 10}}}, {{{1, 2, 3}, {4, 5, 6}, {7, 8, 9, 10}}, {{1, 2, 3}, {4, 5, 6}, {7, 8, 9, 10}}}};
//    cout << arr;
    glutInit(&argc, argv);
    glutInitWindowSize(x_length,y_length);
    glutInitWindowPosition(250,10);
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);

    glutCreateWindow("<WINDOW_TITLE>");
    glutMouseFunc(mouse);
    glutDisplayFunc(display);

    init();


    glutMainLoop();

    return EXIT_SUCCESS;
}
