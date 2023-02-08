
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
void drawHouse();

class Point
{
    public:
        int X;
        int Y;

        Point()
        {
            X=0;
            Y=0;
        }
        Point(int x,int y)
        {
            this->X=x;
            this->Y=y;
        }

        void xy(int x,int y)
        {
            this->X=x;
            this->Y=y;
        }
        int x()
        {
            return X;
        }
        int y()
        {
            return Y;
        }
};


void circle_drawing_algorithm(int,int,int);
void plot_octants(int xc,int yc,int x,int y);

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
    glPointSize(1.5);
    glBegin(GL_LINES);


        glVertex2i(0,y_length/2);
        glVertex2i(x_length,y_length/2);

        glVertex2i(x_length/2,0);
        glVertex2i(x_length/2,y_length);

    glEnd();

    for(int i=0;i<count-1;i+=2)
    {

        Point c=point[i];
        Point r=point[i+1];
        double radius=round(sqrt(pow((r.y()-c.y()),2)+pow((r.x()-c.x()),2)));
        circle_drawing_algorithm(c.x(),c.y(),int(radius));
    }
    //circle_drawing_algorithm(100,-50,100);
    glFlush();


}

void circle_drawing_algorithm(int xc,int yc,int r)
{
    int x,y,p;
    x=0;
    y=r;
    p=1-r;
    cout<<x<<","<<y<<endl;

    glBegin(GL_POINTS);

        glPointSize(5);
        glColor3f(0.502, 0, 0);
        plot_octants(xc,yc,x,y);
//        glVertex2i(xc+x+x_length/2,yc+y+y_length/2);
//        glVertex2i(xc+y+x_length/2,yc+x+y_length/2);
//        glVertex2i(xc-x+x_length/2,yc+y+y_length/2);
//        glVertex2i(xc-y+x_length/2,yc+x+y_length/2);
//        glVertex2i(xc-y+x_length/2,yc-x+y_length/2);
//        glVertex2i(xc-x+x_length/2,yc-y+y_length/2);
//        glVertex2i(xc+x+x_length/2,yc-y+y_length/2);
//        glVertex2i(xc+y+x_length/2,yc-x+y_length/2);

        while(x<=y)
        {
            if(p<0)
            {
                x+=1;
                p+=2*x+1;

            }else
            {
                x+=1;
                y-=1;
                p+=2*(x-y)+1;
            }
            cout<<x<<","<<y<<endl;
            plot_octants(xc,yc,x,y);
//            glVertex2i(xc+x+x_length/2,yc+y+y_length/2);
//            glVertex2i(xc+y+x_length/2,yc+x+y_length/2);
//            glVertex2i(xc-x+x_length/2,yc+y+y_length/2);
//            glVertex2i(xc-y+x_length/2,yc+x+y_length/2);
//            glVertex2i(xc-y+x_length/2,yc-x+y_length/2);
//            glVertex2i(xc-x+x_length/2,yc-y+y_length/2);
//            glVertex2i(xc+x+x_length/2,yc-y+y_length/2);
//            glVertex2i(xc+y+x_length/2,yc-x+y_length/2);

        }
    glEnd();


}
void plot_octants(int xc,int yc,int x,int y)
{
    glBegin(GL_POINTS);
        glColor3f(0.502, 0, 0);
        //glVertex2i(xc+x_length/2,yc+y_length/2);
        glVertex2i(xc+x+x_length/2,yc+y+y_length/2);
        glVertex2i(xc+y+x_length/2,yc+x+y_length/2);
        glVertex2i(xc-x+x_length/2,yc+y+y_length/2);
        glVertex2i(xc-y+x_length/2,yc+x+y_length/2);
        glVertex2i(xc-y+x_length/2,yc-x+y_length/2);
        glVertex2i(xc-x+x_length/2,yc-y+y_length/2);
        glVertex2i(xc+x+x_length/2,yc-y+y_length/2);
        glVertex2i(xc+y+x_length/2,yc-x+y_length/2);
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
    //static vector<Point> point;
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        //digital_differential_analyser(Point(5,6),Point(600,400));
        Point p(x-x_length/2,(y_length-y)-y_length/2);
        point.push_back(p);
        //cout<<p.x()<<" "<<p.y()<<"\n";
        count++;
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
