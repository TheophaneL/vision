#include <visp/vpConfig.h>
#include <visp/vpImage.h>
#include <visp/vpMeEllipse.h>
#include <visp/vpOpenCVGrabber.h>
#include <visp/vpDisplayOpenCV.h>
#include <visp/vpDisplayGDI.h>
#include <visp/vpDisplayX.h>

int main()
{
  vpImage<unsigned char> I;
  
  //Image grabber initialisation
  vpOpenCVGrabber g ;
  g.setDeviceType (CV_CAP_ANY);
  g.open();
  g.acquire(I);
  
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

  vpMeEllipse ellipse;
  vpMe me;

  //Set the tracking parameters.
  me.setRange(20);
  me.setSampleStep(2);
  me.setPointsToTrack(500);
  me.setThreshold(5000);
  ellipse.setMe(&me);

  //Initialize the tracking.
  ellipse.initTracking(I);
  ellipse.display(I, vpColor::green) ;
  vpDisplay::flush(I);
  
  while ( 1 )
  {
    g.acquire(I);
    vpDisplay::display(I);

    //Track the ellipse.
    ellipse.track(I);
    ellipse.display(I, vpColor::green) ;
    vpDisplay::flush(I);
  }
  
  return 0;
}