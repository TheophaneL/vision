#include <visp/vpConfig.h>
#include <visp/vpImage.h>
#include <visp/vpMeLine.h>
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
  
  int nbLines = 2;

  vpMeLine line[nbLines];
  vpMe me;

  //Set the tracking parameters.
  me.setRange(10);
  me.setSampleStep(2);
  me.setPointsToTrack(500);
  me.setThreshold(50000);

  //Initialize the tracking.
  for (int i =0; i < nbLines; i++)
  {
    line[i].setMe(&me);
    line[i].initTracking(I);
    line[i].display(I, vpColor::green) ;
  }
  vpDisplay::flush(I);
  
  while ( 1 )
  {
    g.acquire(I);
    vpDisplay::display(I);

    //Track the line.
    for (int i =0; i < nbLines; i++)
    {
      line[i].track(I);
      line[i].display(I, vpColor::green) ;
    }
    vpDisplay::flush(I);
  }
  
  return 0;
}