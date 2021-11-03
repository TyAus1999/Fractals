#include "inc.h"

void idleFunction();
void displayFunction();
void reshapeFunction(int w, int h);
void keyDown(unsigned char key, int x, int y);
void keyUp(unsigned char key, int x, int y);
void mouseClick(int button, int state, int x, int y);

int currentWidth=600, currentHeight=600;
int threads=1;
double viewXMin, viewXMax, viewYMin, viewYMax;
double zoom=1;
int x,y,endX,endY,startX,startY;
unsigned char *pixels;
u64 maxCount=0xff;
mtex drawLock;

int main(int argc, char **argv){
    fpTest();
    //return 0;

    u64 testCount=fractalModifiedNonOp(-1,0,maxCount);
    double *xs=aligned_alloc(32, sizeof(double)*4);
    double *ys=aligned_alloc(32, sizeof(double)*4);
    u64 *counts=aligned_alloc(32, sizeof(u64)*4);
    for(int i=0;i<4;i++){
        xs[i]=i+1;
        ys[i]=-1-i;
        counts[i]=0;
    }
    //fractalIntrin(xs,ys,counts,maxCount);
    for(u64 i=0;i<4;i++){
        //printf("%llu, Count: %llu, X: %f, Y: %f\n", i, counts[i], xs[i], ys[i]);
    }
    //printf("Test Count: %llu\n", testCount);
    free(xs);
    free(ys);
    free(counts);
    //return 0;

    viewXMin=-2;
    viewXMax=2;
    viewYMin=-2;
    viewYMax=2;
    pixels=malloc(sizeof(unsigned char)*currentHeight*currentWidth*3);
    /*
    for(unsigned y=0;y<currentWidth;y++)
        for(unsigned x=0;x<currentHeight;x++){
            unsigned index=y*currentHeight;
            index+=x;
            index*=3;
            pixels[index]=0xff;
        }
    */
    for(unsigned i=0;i<sizeof(unsigned char)*currentHeight*currentWidth*3;i++){
        pixels[i]=0;
    }
    endX=currentWidth/2;
    endY=currentHeight/2;
    startX=endX-currentWidth;
    startY=endY-currentHeight;
    x=startX;
    y=startY;
    
    int mutexInit=pthread_mutex_init(&drawLock, NULL);
    if(mutexInit!=0){
        printf("Mutex not init\n");
        return 1;
    }

    startThreadsFractal(&drawLock,pixels,2,startX,endX,startY,endY,
    currentWidth,currentHeight,viewXMin,viewXMax,viewYMin,viewYMax);

	glutInit(&argc, argv);
    glutSetOption(GLUT_MULTISAMPLE, 12);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_MULTISAMPLE | GLUT_DOUBLE);
    glutInitWindowPosition(500,0);
    glutInitWindowSize(currentWidth,currentHeight);
    glutCreateWindow("Fractals");
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
    //glutIdleFunc(idleFunction);
    glutDisplayFunc(displayFunction);
    glutReshapeFunc(reshapeFunction);
    glutKeyboardFunc(keyDown);
    glutKeyboardUpFunc(keyUp);
    glutMouseFunc(mouseClick);
    glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);
    glutMainLoop();
    pthread_mutex_destroy(&drawLock);
    free(pixels);
}

unsigned getIndex(int x, int y){
    int newX=x+endX;
    int newY=y+endY;
    unsigned out=newY*currentHeight;
    out+=newX;
    out*=3;
    //printf("X: %i\t\tY: %i\t\tNewX: %i\t\tNewY: %i\t\tOut: %u\n", x,y,newX,newY,out);
    return out;
}

void idleFunction(){
    sleep(0.16);
    glutPostRedisplay();
}

void displayFunction(){
    glClearColor(0.1,0.1,0.1, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    lockMutex(&drawLock);
    glDrawPixels(currentWidth, currentHeight, GL_RGB, GL_UNSIGNED_BYTE, pixels);
    pthread_mutex_unlock(&drawLock);

    glutSwapBuffers();
}

void reshapeFunction(int w, int h){
    glViewport(0,0,w,h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-10,10,-10,10,1,100);
    glMatrixMode(GL_MODELVIEW);
    glutPostRedisplay();
}

void keyDown(unsigned char key, int x, int y){

}

void keyUp(unsigned char key, int x, int y){

}

void mouseClick(int button, int state, int x, int y){
    printf("Button: %i, State: %i, X: %i, Y: %i, ", button, state, x, y);
    int newX=x,newY=y;
    newX-=currentWidth/2;
    newY-=currentHeight/2;
    newY*=-1;
    double mappedX=map((double)newX, (double)startX, (double)endX, viewXMin, viewXMax);
    double mappedY=map((double)newY, (double)startY, (double)endY, viewYMin, viewYMax);
    printf("NewX: %i, NewY %i, mX: %f, mY: %f\n", newX, newY, mappedX, mappedY);
    if(state==0){
        viewXMin=mappedX-zoom;
        viewXMax=mappedX+zoom;
        viewYMin=mappedY-zoom;
        viewYMax=mappedY+zoom;
        zoom-=zoom/2;

        startThreadsFractal(&drawLock,pixels,5,startX,endX,startY,endY,
        currentWidth,currentHeight,viewXMin,viewXMax,viewYMin,viewYMax);
        
        glutPostRedisplay();
    }
}