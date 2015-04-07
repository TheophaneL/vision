#include <visp/vpConfig.h>
#include <visp/vpImage.h>
#include <visp/vpMbEdgeTracker.h>
#include <visp/vpOpenCVGrabber.h>
#include <visp/vpDisplayOpenCV.h>
#include <visp/vpDisplayGDI.h>
#include <visp/vpDisplayX.h>

int main()
{
  vpImage<unsigned char> I;
  
  //Image grabber initialisation
  int device = 0;
  cv::VideoCapture cap(device);
  cv::Mat frame;
  cap.read(frame);
  vpImageConvert::convert(frame,I);
  //vpOpenCVGrabber g ;
  //g.setDeviceType (CV_CAP_ANY);
  //g.open();
  //g.acquire(I);

  //Display initialisation
#if defined VISP_HAVE_X11
  vpDisplayX d;
#elif defined VISP_HAVE_GDI
  vpDisplayGDI d;
#elif defined VISP_HAVE_OPEN_CV
  vpDisplayOpenCV d;
#endif
  d.init(I, I.getHeight(), I.getWidth(), "Cube tracking") ;
  
  vpDisplay::display(I);
  vpDisplay::flush(I);

  vpMbEdgeTracker tracker;
  vpHomogeneousMatrix cMo;

  // Load tracker config file (camera parameters and moving edge settings)
  //tracker.loadConfigFile("cadModel&configFile/mmicro.xml");
  tracker.loadConfigFile("cube.xml");

  // initialise an instance of vpCameraParameters with the parameters from the tracker
  vpCameraParameters cam;
  tracker.getCameraParameters(cam);
  
  // Load the 3D model (either a vrml file or a .cao file)
  //tracker.loadModel("cadModel&configFile/mmicro.wrl");
  tracker.loadModel("cube.cao");

  tracker.setDisplayFeatures(true);
  //Initialize the tracking.
  //tracker.initClick(I, "cadModel&configFile/mmicro", true);
  tracker.initClick(I,"cube.init",true);

  //track the model
  tracker.track(I);
  tracker.getPose(cMo);
  tracker.display(I, cMo, cam, vpColor::red, 1);
  vpDisplay::flush(I);
  
  while ( 1 )
    {
      //g.acquire(I);
      cap.read(frame);
      vpImageConvert::convert(frame,I);
      vpDisplay::display(I);

      //Track the object.
      tracker.track(I);
      tracker.getPose(cMo);
      tracker.display(I, cMo, cam, vpColor::red, 1);
      vpDisplay::displayFrame (I, cMo, cam, 0.05, vpColor::blue);
      vpDisplay::flush(I);
    }
  
  return 0;
}
