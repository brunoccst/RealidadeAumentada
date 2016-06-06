#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <VideoIM.h>

static imVideoCapture* myVideoCap;
static int height, width;
static unsigned char *imageBuffer;
static unsigned char *ImgLineTemp;

#define PIXEL_SIZE 3

// *****************************************************
// void SwapImage(ARUint8 *Img)
//
//
// *****************************************************
void SwapImage()
{
	unsigned long InicioLinhaA, InicioLinhaB, TamLinha;

	TamLinha = width * PIXEL_SIZE;
	InicioLinhaA = 0;
	InicioLinhaB = (height-1) * TamLinha;
    int line;
	for( line = 0; line < height/2; line++ )
	{
		//InicioLinhaA = line * xSize * 4;
		//InicioLinhaB = (ySize-line-1) * xSize * 4;
		memcpy(ImgLineTemp, &imageBuffer[InicioLinhaA],TamLinha);
		memcpy(&imageBuffer[InicioLinhaA], &imageBuffer[InicioLinhaB], TamLinha);
		memcpy(&imageBuffer[InicioLinhaB], ImgLineTemp, TamLinha);
		InicioLinhaA += TamLinha;
		InicioLinhaB -= TamLinha;
	}
}
// *****************************************************
// void SwapLine(ARUint8 *Img, unsigned int LinhaA, unsigned int LinhaB)
//		Esta função pode ser usada conforme o código abaixo
//
//	for( int line = 0; line < ySize/2; line++ )
//	{
//		SwapLine(dataPtr, line, ySize-line-1);
//	}
//
//
// *****************************************************
void SwapLine(unsigned char *Img, unsigned int LinhaA, unsigned int LinhaB)
{
	unsigned long InicioLinhaA, InicioLinhaB;

	InicioLinhaA = LinhaA * width * PIXEL_SIZE;
	InicioLinhaB = LinhaB * width * PIXEL_SIZE;

	memcpy(ImgLineTemp, &Img[InicioLinhaA],width * PIXEL_SIZE);
	memcpy(&Img[InicioLinhaA], &Img[InicioLinhaB], width * PIXEL_SIZE);
	memcpy(&Img[InicioLinhaB], ImgLineTemp, width * PIXEL_SIZE);

//
//
//  Abaixo tem-se uma versão mais simples da rotina

/*	for(int i=0;i < xSize*4;i++)
	{
		ImgLineTemp[i] = Img[InicioLinhaA + i];
		Img[InicioLinhaA + i] = Img[InicioLinhaB + i];
		Img[InicioLinhaB + i] = ImgLineTemp[i];
	}
*/
}

int  arVideoOpen(char *config)
{

    height = width = 0;

    myVideoCap = imVideoCaptureCreate();
    if (!myVideoCap)
    {
        printf("No Capture Device!\n");
        system("pause");
        exit(1);
    }

    imVideoCaptureConnect(myVideoCap, 0);
    imVideoCaptureLive(myVideoCap, 1);

    //if (ShowDialog)
    //imVideoCaptureShowDialog(myVideoCap, 0, NULL); // o indice 0 escolhe qual o dialogo a ser mostrado.

    imVideoCaptureGetImageSize(myVideoCap, &width, &height);
    imageBuffer = (unsigned char*) malloc (sizeof(unsigned char)*width*height*PIXEL_SIZE);

    if (!imageBuffer)
    {
        printf("No memory available memory for image buffer !\n");
        return 0;
    }

	ImgLineTemp = (unsigned char*) malloc(sizeof(unsigned char)* width * PIXEL_SIZE);
	if (ImgLineTemp == NULL)
	{
		printf("Unable to alocate memory !!!\n");
		exit(0);
	}

    return 1;
}

int arVideoClose(void)
{
    free (imageBuffer);
    return 1;
}

int arVideoInqSize( int *x, int *y )
{
    imVideoCaptureGetImageSize(myVideoCap, x, y);
    return 1;
}

int arVideoCapStart( void )
{
    return 0;
}

int arVideoCapStop( void )

{
    imVideoCaptureDisconnect(myVideoCap);
    imVideoCaptureDestroy(myVideoCap);
    return 0;
}

int arVideoCapNext( void )
{

    if (!imVideoCaptureLive(myVideoCap, -1))
    {
        printf("Retornando....\n");
        return 0;
    }
    imVideoCaptureFrame(myVideoCap, imageBuffer, IM_RGB|IM_PACKED, 1000);
    SwapImage();

    return 0;

}

unsigned char *arVideoGetImage( void )
{
    return imageBuffer;
}

void error_exit(void)
{
    printf("Error_exit in Win32video_sub\n");
    exit(0);
}


