/** \file
 * \brief Video Capture
 *
 * See Copyright Notice in im.h
 * $Id: Exp $
 */

#ifndef __IM_CAPTURE_H
#define __IM_CAPTURE_H

#if	defined(__cplusplus)
extern "C" {
#endif

/** \defgroup capture Image Capture 
 * \par
 * Captures images from video devices.
 * \par
 * You must link the application with "im_capture.lib/.a/.so". \n
 * Depends also on the Direct Show 9 SDK, you must link with "strmiids.lib".
 * When using the "im_capture.dll" this extra library is not necessary.
 * Since DX uses COM, CoInitialize(NULL) is called when the devices are enumerated.
 * \par
 * For more information: \n
 * http://msdn.microsoft.com/library/en-us/directx9_c/directX/htm/directshow.asp
 * \par
 * See \ref im_capture.h
 */

#define IM_RGB 0
#define IM_PACKED 0x200


typedef struct _imVideoCapture imVideoCapture;

/** Returns the number of available devices.
 * \ingroup capture */
int imVideoCaptureDeviceCount(void);

/** Returns the device description. Returns NULL in the last device.
 * \ingroup capture */
const char* imVideoCaptureDeviceDesc(int device);

/** Reload the device list. The devices can be dynamically removed or added to the system.
 * \ingroup capture */
int imVideoCaptureReloadDevices(void);

/** Creates a new imVideoCapture object. \n
 * Returns NULL if there is no capture device available. \n
 * In Windows returns NULL if DirectX version is older than 8.
 * \ingroup capture */
imVideoCapture* imVideoCaptureCreate(void);

/** Destroys a imVideoCapture object.
 * \ingroup capture */
void imVideoCaptureDestroy(imVideoCapture* vc);

/** Connects to a capture device. 
 * More than one imVideoCapture object can be created
 * but they must be connected to different devices.  \n
 * If the object is conected it will disconnect first. \n
 * Use -1 to return the current connected device, 
 * in this case returns -1 if not connected. \n
 * Returns zero if failed.
 * \ingroup capture */
int imVideoCaptureConnect(imVideoCapture* vc, int device);

/** Disconnect from a capture device.
 * \ingroup capture */
void imVideoCaptureDisconnect(imVideoCapture* vc);

/** Displays a configuration modal dialog of the connected device. \n
 * In Windows, the capturing will be stopped in some cases. \n
 * In Windows parent is a HWND of a parent window, it can be NULL. \n
 * Returns zero if failed.
 * \ingroup capture */
int imVideoCaptureShowDialog(imVideoCapture* vc, int dialog, void* parent);

/** Returns the number of available configuration dialogs.
 * \ingroup capture */
int imVideoCaptureDialogCount(imVideoCapture* vc);

/** Returns the description of a configuration dialog.
 * \ingroup capture */
const char* imVideoCaptureDialogDesc(imVideoCapture* vc, int dialog);

/** Returns the current image size of the connected device. \n
 *  width and height returns 0 if not connected.
 * \ingroup capture */
void imVideoCaptureGetImageSize(imVideoCapture* vc, int *width, int *height);

/** Changes the image size of the connected device. \n 
 * Returns zero if failed.
 * \ingroup capture */
int imVideoCaptureSetImageSize(imVideoCapture* vc, int width, int height);

/** Returns a new captured frame. Use -1 for infinite timeout. \n
 * Color space can be IM_RGB or IM_GRAY, and mode can be packed (IM_PACKED) or not. \n
 * It can not have an alpha channel and orientation is always bottom up. \n
 * Returns zero if failed or timeout expired, the buffer is not changed.
 * \ingroup capture */
int imVideoCaptureFrame(imVideoCapture* vc, unsigned char* data, int color_mode, int timeout);

/** Start capturing, returns the new captured frame and stop capturing.  \n
 * This is more usefull if you are switching between devices. \n
 * Data format is the same as imVideoCaptureFrame. \n
 * Returns zero if failed.
 * \ingroup capture */
int imVideoCaptureOneFrame(imVideoCapture* vc, unsigned char* data, int color_mode);

/** Start capturing. \n
 * Use -1 to return the current state. \n
 * Returns zero if failed.
 * \ingroup capture */
int imVideoCaptureLive(imVideoCapture* vc, int live);

/** Resets a camera or video attribute to the default value or 
 * to the automatic setting. \n
 * Not all attributes support automatic modes. \n
 * Returns zero if failed.
 * \ingroup capture */
int imVideoCaptureResetAttribute(imVideoCapture* vc, const char* attrib, int fauto);

/** Returns a camera or video attribute in percentage of the valid range value. \n
 * Returns zero if failed.
 * \ingroup capture */
int imVideoCaptureGetAttribute(imVideoCapture* vc, const char* attrib, float *percent);

/** Changes a camera or video attribute in percentage of the valid range value. \n
 * Returns zero if failed.
 * \ingroup capture */
int imVideoCaptureSetAttribute(imVideoCapture* vc, const char* attrib, float percent);

/** Returns a list of the description of the valid attributes.
 * \ingroup capture */
const char** imVideoCaptureGetAttributeList(imVideoCapture* vc, int *num_attrib);


/** \defgroup winattrib Windows Attributes
\verbatim
  VideoBrightness - Specifies the brightness, also called the black level. 
  VideoContrast - Specifies the contrast, expressed as gain factor. 
  VideoHue - Specifies the hue angle. 
  VideoSaturation - Specifies the saturation.
  VideoSharpness - Specifies the sharpness. 
  VideoGamma - Specifies the gamma. 
  VideoColorEnable - Specifies the color enable setting. (0/100)
  VideoWhiteBalance - Specifies the white balance, as a color temperature in degrees Kelvin. 
  VideoBacklightCompensation - Specifies the backlight compensation setting. (0/100)
  VideoGain - Specifies the gain adjustment.
  CameraPanAngle - Specifies the camera's pan angle. To 100 rotate right, To 0 rotate left (view from above).
  CameraTiltAngle - Specifies the camera's tilt angle.  To 100 rotate up, To 0 rotate down.
  CameraRollAngle - Specifies the camera's roll angle. To 100 rotate right, To 0 rotate left.
  CameraLensZoom - Specifies the camera's zoom setting. 
  CameraExposure - Specifies the exposure setting. 
  CameraIris - Specifies the camera's iris setting. 
  CameraFocus - Specifies the camera's focus setting, as the distance to the optimally focused target. 
  FlipHorizontal - Specifies the video will be flipped in the horizontal direction.
  FlipVertical - Specifies the video will be flipped in the vertical direction.
  AnalogFormat - Specifies the video format standard NTSC, PAL, etc
      NTSC_M     = 0 
      NTSC_M_J   = 1  
      NTSC_433   = 2
      PAL_B      = 3
      PAL_D      = 4
      PAL_H      = 5
      PAL_I      = 6
      PAL_M      = 7
      PAL_N      = 8
      PAL_60     = 9
      SECAM_B    = 10
      SECAM_D    = 11
      SECAM_G    = 12
      SECAM_H    = 13
      SECAM_K    = 14
      SECAM_K1   = 15
      SECAM_L    = 16
      SECAM_L1   = 17
      PAL_N_COMBO = 18
\endverbatim
 * \ingroup capture */


#if defined(__cplusplus)
}

/** A C++ Wrapper for the imVideoCapture structure functions.
 * \ingroup capture */
class imCapture
{
public:
  imCapture() 
    { vc = imVideoCaptureCreate(); }
  
  ~imCapture() 
    { if (vc) imVideoCaptureDestroy(vc); }
  
  int Failed() 
    { if (!vc) return 0; else return 1; }
  
  int Connect(int device) 
    { return imVideoCaptureConnect(vc, device); }
  
  void Disconnect() 
    { imVideoCaptureDisconnect(vc); }
  
  int ShowDialog(int dialog, void* parent) 
    { return imVideoCaptureShowDialog(vc, dialog, parent); }
  
  int DialogCount() 
    { return imVideoCaptureDialogCount(vc); }
  
  const char* DialogDescription(int dialog) 
    { return imVideoCaptureDialogDesc(vc, dialog); }
  
  void GetImageSize(int *width, int *height) 
    { imVideoCaptureGetImageSize(vc, width, height); }
  
  int SetImageSize(int width, int height) 
    { return imVideoCaptureSetImageSize(vc, width, height); }
  
  int GetFrame(unsigned char* data, int color_mode, int timeout) 
    { return imVideoCaptureFrame(vc, data, color_mode, timeout); }
  
  int GetOneFrame(unsigned char* data, int color_mode) 
    { return imVideoCaptureOneFrame(vc, data, color_mode); }
  
  int Live(int live) 
    { return imVideoCaptureLive(vc, live); }
  
  int ResetAttribute(const char* attrib, int fauto) 
    { return imVideoCaptureResetAttribute(vc, attrib, fauto); }
  
  int GetAttribute(const char* attrib, float *percent) 
    { return imVideoCaptureGetAttribute(vc, attrib, percent); }
  
  int SetAttribute(const char* attrib, float percent) 
    { return imVideoCaptureSetAttribute(vc, attrib, percent); }
  
  const char** GetAttributeList(int *num_attrib) 
    { return imVideoCaptureGetAttributeList(vc, num_attrib); }
  
protected:
  imVideoCapture* vc;
};

#endif

#endif
