#ifdef _WIN32
#include <windows.h>
#include <VideoIM.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#ifndef __APPLE__
#include <GL/gl.h>
#include <GL/glut.h>
#else
#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#endif
#include <AR/gsub.h>
#include <AR/video.h>
#include <AR/param.h>
#include <AR/ar.h>

//
// Camera configuration.
//
#ifdef _WIN32
char			*vconf = "Data\\WDM_camera_flipV.xml";
#else
char			*vconf = "";
#endif

int             fantasma = 0;

int             xsize, ysize;
int             thresh = 100;
int             count = 0;


char           *cparam_name    = "Data/camera_para.dat";
ARParam         cparam;

//char           *patt1_name      = "Data/multi/patt.a";
char           *patt1_name     = "Data/patt.Hiro";
char           *patt2_name     = "Data/patt.Kanji";
int             patt1_id, patt2_id;

double          patt1_width     = 80.0;
double          patt1_center[2] = {0.0, 0.0};
double          patt1_trans[3][4];

double          patt2_width     = 80.0;
double          patt2_center[2] = {0.0, 0.0};
double          patt2_trans[3][4];

int             patt1_marker, patt2_marker;

static void   init(void);
static void   cleanup(void);
static void   keyEvent( unsigned char key, int x, int y);
static void   mainLoop(void);
static void   draw( void );

int main(int argc, char **argv)
{

    printf ("Diretorio corrente:");
#ifdef WIN32
    system("cd");
#else
    system("pwd");
#endif
    printf("\n");

    glutInit(&argc, argv);
    init();

    arVideoCapStart();
    argMainLoop( NULL, keyEvent, mainLoop );
    return (0);
}

static void keyEvent( unsigned char key, int x, int y)
{
    /* quit if the ESC key is pressed */
    if( key == 0x1b )
    {
        printf("*** %f (frame/sec)\n", (double)count/arUtilTimer());
        cleanup();
        exit(0);
    }

    if (key == 'f')
    {
printf("Mudou Fanstama..");
        fantasma = !fantasma;

    }
}

/* main loop */
static void mainLoop(void)
{

    ARUint8         *dataPtr;
    ARMarkerInfo    *marker_info;
    int             marker_num;
    int             j, k;


    //arVideoCapNext();
    /* grab a vide frame */
    if( (dataPtr = (ARUint8 *)arVideoGetImage()) == NULL )
    {
        arUtilSleep(2);
        return;
    }
    if( count == 0 )
        arUtilTimerReset();
    count++;

    argDrawMode2D();

    argDispImage( dataPtr, 0,0 );

    /* detect the markers in the video frame */
    if( arDetectMarker(dataPtr, thresh, &marker_info, &marker_num) < 0 )
    {
        cleanup();
        exit(0);
    }

    arVideoCapNext();

    patt1_marker = -1;
    patt2_marker = -1;

    /* check for object visibility */
    for( j = 0; j < marker_num; j++ )
    {
        if( patt1_id == marker_info[j].id )
        {
            if( patt1_marker == -1 ) patt1_marker = j;
            else if( marker_info[patt1_marker].cf < marker_info[j].cf ) patt1_marker = j;
        }
        if( patt2_id == marker_info[j].id )
        {
            if( patt2_marker == -1 ) patt2_marker = j;
            else if( marker_info[patt2_marker].cf < marker_info[j].cf ) patt2_marker = j;
        }
    }
    if((patt1_marker == -1) && (patt2_marker == -1))
    {
        argSwapBuffers();
        return;
    }

    if (patt1_marker != -1)
        arGetTransMat(&marker_info[patt1_marker], patt1_center, patt1_width, patt1_trans);

    if (patt2_marker != -1)
        arGetTransMat(&marker_info[patt2_marker], patt2_center, patt2_width, patt2_trans);

    draw();

    argSwapBuffers();
}

static void init( void )
{
    ARParam  wparam;

    /* open the video path */
    if( arVideoOpen( vconf ) < 0 )
        exit(0);
    /* find the size of the window */
    if( arVideoInqSize(&xsize, &ysize) < 0 ) exit(0);
    printf("Image size (x,y) = (%d,%d)\n", xsize, ysize);

    /* set the initial camera parameters */
    if( arParamLoad(cparam_name, 1, &wparam) < 0 )
    {
        printf("Camera parameter load error !!\n");
        getchar();
        exit(0);
    }
    arParamChangeSize( &wparam, xsize, ysize, &cparam );
    arInitCparam( &cparam );
    printf("*** Camera Parameter ***\n");
    arParamDisp( &cparam );

    if( (patt1_id=arLoadPatt(patt1_name)) < 0 )
    {
        printf("pattern 1 load error !!\n");
        getchar();
        exit(0);
    }

    if( (patt2_id=arLoadPatt(patt2_name)) < 0 )
    {
        printf("pattern 2 load error !!\n");
        getchar();
        exit(0);
    }

    /* open the graphics window */
    argInit( &cparam, 1.0, 0, 0, 0, 0 );
}

/* cleanup function called when program exits */
static void cleanup(void)
{
    arVideoCapStop();
    arVideoClose();
    argCleanup();
}

static void draw( void )
{
    double    gl_para[16];
    GLfloat   mat_ambient[]     = {0.0, 0.0, 1.0, 1.0};
    GLfloat   mat2_ambient[]     = {1.0, 0.0, 0.0, 1.0};
    GLfloat   mat_flash[]       = {0.0, 0.0, 1.0, 1.0};
    GLfloat   mat2_flash[]       = {1.0, 0.0, 0.0, 1.0};
    GLfloat   mat_flash_shiny[] = {50.0};
    GLfloat   mat2_flash_shiny[] = {50.0};
    GLfloat   light_position[]  = {100.0,-200.0,200.0,0.0};
    GLfloat   ambi[]            = {0.1, 0.1, 0.1, 0.1};
    GLfloat   lightZeroColor[]  = {0.9, 0.9, 0.9, 0.1};

    argDrawMode3D();
    argDrawMode3D();
    argDraw3dCamera( 0, 0 );
    //glClearDepth( 1.0 );
    glClear(GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambi);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightZeroColor);
    glMatrixMode(GL_MODELVIEW);

    if (patt1_marker != -1)  // se o Pattern 1 FOI localizado
    {
        glPushMatrix ();

        /* load the camera transformation matrix */
        argConvGlpara(patt1_trans, gl_para);
        glMatrixMode(GL_MODELVIEW);
        glEnable(GL_DEPTH_TEST);

        if (fantasma)
        {
            glColorMask (GL_FALSE, GL_FALSE, GL_FALSE, GL_TRUE);
        }

        glMaterialfv(GL_FRONT, GL_SPECULAR, mat_flash);
        glMaterialfv(GL_FRONT, GL_SHININESS, mat_flash_shiny);
        glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
        glLoadMatrixd( gl_para );
        glTranslatef( 0.0, 0.0, -67.0 );
        glutSolidCube(134.0);
        //glTranslatef( 0.0, 0.0, -50.0 );
        //glutSolidCube(100.0);

        if (fantasma)
        {
            glColorMask (GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
        }

        if (patt2_marker == -1) // se o Pattern 2 NÃO FOI localizado
        {
            glTranslatef (0.0f, -185.0f, -67.0f); // translada para desenhar o cone a partir do cubo
        }
        else    // Se o Pattern 2 FOI localizado
        {       // Desenha o Cone a partir dele mesmo
            glPopMatrix ();
            glPushMatrix ();
            argConvGlpara(patt2_trans, gl_para);
            glMatrixMode(GL_MODELVIEW);
            glEnable(GL_DEPTH_TEST);
            glLoadMatrixd( gl_para );
        }

        glMaterialfv(GL_FRONT, GL_SPECULAR, mat2_flash);
        glMaterialfv(GL_FRONT, GL_SHININESS, mat2_flash_shiny);
        glMaterialfv(GL_FRONT, GL_AMBIENT, mat2_ambient);
        glutSolidCone (67.0, 134.0, 20, 20);

        glPopMatrix ();

    }

    else  // Se o padrao  1 NAO FOI localizado
        if (patt2_marker != -1)  // se o Pattern 2 FOI localizado
    {

        glPushMatrix ();

        /* load the camera transformation matrix */
        argConvGlpara(patt2_trans, gl_para);
        glMatrixMode(GL_MODELVIEW);
        glEnable(GL_DEPTH_TEST);

        glMaterialfv(GL_FRONT, GL_SPECULAR, mat2_flash);
        glMaterialfv(GL_FRONT, GL_SHININESS, mat2_flash_shiny);
        glMaterialfv(GL_FRONT, GL_AMBIENT, mat2_ambient);
        glLoadMatrixd( gl_para );
        glutSolidCone (67.0, 134.0, 20, 20);

        if (patt1_marker == -1) // Se o padrao  1 NAO FOI localizado
        {
            glTranslatef (0.0f, 185.0f, 67.0f); // translada para desenhar o cubo a partir do cone
        }
        else
        {
            glPopMatrix ();
            glPushMatrix ();
            argConvGlpara(patt1_trans, gl_para);
            glMatrixMode(GL_MODELVIEW);
            glEnable(GL_DEPTH_TEST);
            glLoadMatrixd( gl_para );
            glTranslatef( 0.0, 0.0, 67.0 );
        }

        glMaterialfv(GL_FRONT, GL_SPECULAR, mat_flash);
        glMaterialfv(GL_FRONT, GL_SHININESS, mat_flash_shiny);
        glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
        glutSolidCube(134.0);

        glPopMatrix ();
    }

    glDisable( GL_LIGHTING );

    glDisable( GL_DEPTH_TEST );
}
