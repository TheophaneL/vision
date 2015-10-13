#include <visp/vpConfig.h>
#include <visp/vpImage.h>
#include <visp/vpImageConvert.h>
#include <visp/vpKltOpencv.h>
#include <visp/vpOpenCVGrabber.h>
#include <visp/vpDisplayOpenCV.h>
#include <visp/vpDisplayGDI.h>
#include <visp/vpDisplayX.h>

int main()
{
  vpImage<unsigned char> I;
  IplImage * cvI = NULL;
  
  //Image grabber initialisation
  vpOpenCVGrabber g ;
  g.setDeviceType (CV_CAP_ANY);
  g.open();
  g.acquire(I);
  vpImageConvert::convert(I,cvI);
  
  //Display initialisation
  #if defined VISP_HAVE_X11
  vpDisplayX d;
  #elif defined VISP_HAVE_GDI
  vpDisplayGDI d;
  #elif defined VISP_HAVE_OPEN_CV
  vpDisplayOpenCV d;
  #endif
  d.init(I, 0, 0, "") ;
  
  vpDisplay::display(I);
  vpDisplay::flush(I);
  
  vpKltOpencv tracker;
  //Set tracker parameters
  tracker.setTrackerId(1);
  tracker.setMaxFeatures(200);
  tracker.setWindowSize(10);
  tracker.setQuality(0.01);
  tracker.setMinDistance(15);
  tracker.setHarrisFreeParameter(0.04);
  tracker.setBlockSize(9);
  tracker.setUseHarris(1);
  tracker.setPyramidLevels(3);
  
  //Initialise the tracking
  tracker.initTracking(cvI);
  
  while ( 1 )
  {
    g.acquire(I);
    vpImageConvert::convert(I,cvI);
    vpDisplay::display(I);
    
    tracker.track(cvI);
    tracker.display(I, vpColor::red);
    
    vpDisplay::flush(I);
  }
  return 0;
}
