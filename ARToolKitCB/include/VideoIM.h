
#include "im_capture.h"


int  arVideoOpen(char *config);
int arVideoClose(void);
int arVideoInqSize( int *x, int *y );
unsigned char *arVideoGetImage( void );
int arVideoCapStart( void );
int arVideoCapStop( void );
int arVideoCapNext( void );
void error_exit(void);

