#include <visp/vpConfig.h>
#include <visp/vpImage.h>
#include <visp/vpPose.h>
#include <visp/vpMeLine.h>
#include <visp/vpPixelMeterConversion.h>
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

  //Initialise the line tracker
  vpMeLine line[4];
  vpMe me;
  me.setRange(10) ;
  me.setPointsToTrack(1000) ;
  me.setThreshold(90000) ;
  me.setSampleStep(3);

  for (int i = 0; i < 4; i++)
  {
    line[i].setMe(&me);
    line[i].initTracking(I);
    line[i].display(I, vpColor::green) ;
    vpDisplay::flush(I);
  }

  //Initialise the pose computation
  vpHomogeneousMatrix cMo ;
  vpPoint P[4]  ;
  vpPose pose ;
  pose.clearPoint() ;

  //Set the coordinates of the coorners in the object frame
  P[0].setWorldCoordinates(-0.15,0.105, 0 ) ;
  P[1].setWorldCoordinates(0.15,0.105, 0 ) ;
  P[2].setWorldCoordinates(0.15,-0.105, 0 ) ;
  P[3].setWorldCoordinates(-0.15,-0.105, 0 ) ;

  //Set the camera parameters
  vpCameraParameters cam(558.5309599, 556.055053, 345.0142541, 230.552552);

  //Compute the pose
  for (int i=0 ; i < 4; i++)
  {
    vpImagePoint iP;
    vpMeLine::intersection(line[i], line[(i+1)%4],iP);
    double x = 0;
    double y = 0;
    vpPixelMeterConversion::convertPoint(cam,iP,x,y);
    P[i].set_x(x) ;
    P[i].set_y(y) ;
    pose.addPoint(P[i]) ;
  }
  pose.computePose(vpPose::LAGRANGE, cMo);
  pose.computePose(vpPose::VIRTUAL_VS, cMo);

  vpDisplay::flush(I);
  
  int count = 0;
  
  while ( 1 )
  {
    g.acquire(I);
    vpDisplay::display(I);

    //Track the lines.
    for (int i = 0; i < 4; i++)
    {
      line[i].track(I);
      if(count < 30)
	line[i].display(I, vpColor::green);
    }
    count ++;
    
    //Compute the new pose
    pose.clearPoint();
    for (int i=0 ; i < 4; i++)
    {
      vpImagePoint iP;
      vpMeLine::intersection(line[i], line[(i+1)%4],iP);
      double x = 0;
      double y = 0;
      vpPixelMeterConversion::convertPoint(cam,iP,x,y);
      P[i].set_x(x) ;
      P[i].set_y(y) ;
      pose.addPoint(P[i]) ;
    }
    pose.computePose(vpPose::VIRTUAL_VS, cMo);
    
    //Display the object frame
    vpDisplay::displayFrame(I,cMo,cam,0.1,vpColor::none,2);
    
     vpDisplay::flush(I);
  }
  
  return 0;
}