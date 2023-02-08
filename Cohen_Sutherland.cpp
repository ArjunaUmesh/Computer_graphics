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

template <class T>
 ostream& operator <<(ostream& out,const vector<T> &arr)
 {
     out << "\b";
     for(auto i:arr)
     {
         out<<i<<" ";
     }
     out << endl;
     return out;
 }

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
void regionCode(vector<int> &rc,Point A,Point br,Point tl);
void cohen_sutherland(Point A,Point B,Point br,Point tl);
int trivial_accept(vector<int> A,vector<int> B);
int trivial_reject(vector<int> A,vector<int> B);
void clip(vector<int> &rcA,vector<int> &rcB,Point br,Point tl,Point &A,Point &B,double M);
void drawLine(Point A,Point B,double r,double g,double b);

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
    Point br,tl;
    if(point.size()>1)
    {
        br=point[0];
        tl=point[1];
        glBegin(GL_LINE_LOOP);
            glColor3f(0.8,0.7,0.8);
            glVertex2i(br.x()+x_length/2,br.y()+y_length/2);
            glVertex2i(br.x()+x_length/2,tl.y()+y_length/2);
            glVertex2i(tl.x()+x_length/2,tl.y()+y_length/2);
            glVertex2i(tl.x()+x_length/2,br.y()+y_length/2);
        glEnd();
    }
    if(point.size()>3)
    {
        Point A=point[2];
        Point B=point[3];
        cohen_sutherland(A,B,br,tl);
    }
    glFlush();
}

void cohen_sutherland(Point A,Point B,Point br,Point tl)
{
    //drawLine(A,B);
    vector<int> rcA;
    vector<int> rcB;
    regionCode(rcA,A,br,tl);
    regionCode(rcB,B,br,tl);
    for(int i=0;i<4;i++)
    {
        cout<<rcA[i]<<" ";
    }

    cout<<endl;
    for(int i=0;i<4;i++)
    {
        cout<<rcB[i]<<" ";
    }
    cout<<endl;
    double delx=B.x()-A.x();
    double dely=B.y()-A.y();
    double m;
    if(delx!=0)
    {
        m=dely/delx;
    }else{
        m=INT_MAX;
    }
    //drawLine(A,B);
    clip(rcA,rcB,br,tl,A,B,m);
    //clip(rcB,rcA,br,tl,B,A,m);
    //circle_drawing_algorithm(100,-50,100);

}

void clip(vector<int> &rcA,vector<int> &rcB,Point br,Point tl,Point &A,Point &B,double m)
{
    Point A_=A;
    while(true)
    {
        if(trivial_accept(rcA,rcB) || trivial_reject(rcA,rcB))
        {
            cout<<"Trivial\n";
            break;
        }
        if(rcA[3]==1)
        {
            //double ynew=m*(br.x()-A.x())+A.y();
            A.xy(br.x(),m*(br.x()-A.x())+A.y());
        }
        if(rcA[2]==1)
        {
            //double ynew=m*(tl.x()-A.x())+A.y();
            A.xy(tl.x(),m*(tl.x()-A.x())+A.y());
        }
        if(rcA[1]==1)
        {
            double xnew=(br.y()-A.y())/m+A.x();
            A.xy(xnew,br.y());
        }
        if(rcA[0]==1)
        {
            double xnew=(tl.y()-A.y())/m+A.x();
            A.xy(xnew,tl.y());
        }

        regionCode(rcA,A,br,tl);
        break;
    }
    Point B_=B;
    while(true)
    {
        if(trivial_accept(rcA,rcB) || trivial_reject(rcA,rcB))
        {
            cout<<"Trivial\n";
            break;
        }
        if(rcB[3]==1)
        {
            double ynew=m*(br.x()-B.x())+B.y();
            B.xy(br.x(),ynew);
        }
        if(rcB[2]==1)
        {
            double ynew=m*(tl.x()-B.x())+B.y();
            B.xy(tl.x(),ynew);
        }
        if(rcB[1]==1)
        {
            double xnew=(br.y()-B.y())/m+B.x();
            B.xy(xnew,br.y());
        }
        if(rcB[0]==1)
        {
            double xnew=(tl.y()-B.y())/m+B.x();
            B.xy(xnew,tl.y());
        }
        break;
    }
    if(!trivial_reject(rcA,rcB))
    {
        drawLine(A_,A,0,1,0);
        drawLine(A,B,1,0,0);
        drawLine(B,B_,0,1,0);
    }else
    {
        drawLine(A_,B_,0,1,0);
    }

}

void drawLine(Point A,Point B,double r,double g,double b)
{
    glPointSize(3.0);
    glBegin(GL_LINES);
        glColor3f(r,g,b);
        glVertex2i(A.x()+x_length/2,A.y()+y_length/2);
        glVertex2i(B.x()+x_length/2,B.y()+y_length/2);
    glEnd();
    glFlush();
}

void regionCode(vector<int> &rc,Point A,Point br,Point tl)
{
    rc.clear();
    rc.push_back(tl.y()<A.y());
    rc.push_back(br.y()>A.y());
    rc.push_back(tl.x()<A.x());
    rc.push_back(br.x()>A.x());
}

int trivial_accept(vector<int> A,vector<int> B)
{
    for(int i=0;i<4;i++)
    {
        if(A[i]==1 || B[i]==1)
        {
            return 0;
        }
    }
    return 1;
}
int trivial_reject(vector<int> A,vector<int> B)
{
    for(int i=0;i<4;i++)
    {
        if(A[i]==1 && B[i]==1)
        {
            return 1;
        }
    }
    return 0;
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
        if(point.size()<4)
        {
            point.push_back(p);
        }else
        {
            Point br=point[0];
            Point tl=point[1];
            if(br.x()>tl.x() && br.y()>tl.y())
            {
                Point temp=br;
                br=tl;
                tl=temp;
            }
            point.clear();
            point.push_back(br);
            point.push_back(tl);
            point.push_back(p);
        }
    }
    if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
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
