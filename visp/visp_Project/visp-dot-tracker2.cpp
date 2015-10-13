#include <visp/vpConfig.h>
#include <visp/vp1394TwoGrabber.h>
#include <visp/vpImage.h>
#include <visp/vpDisplay.h>
#include <visp/vpDisplayX.h>
#include <visp/vpDisplayGDI.h>
#include <visp/vpDot2.h>
//#include <visp/vpVideoWriter.h>

int main()
{
#if ((defined (VISP_HAVE_X11) || defined (VISP_HAVE_GDI)) && defined (VISP_HAVE_DC1394_2))
  vpImage<unsigned char> I ;
  bool reset = false;
  
  // Initialize the camera grabber
  vp1394TwoGrabber g(reset);

  g.setVideoMode(vp1394TwoGrabber::vpVIDEO_MODE_640x480_MONO8);
  g.setFramerate(vp1394TwoGrabber::vpFRAMERATE_60);
  // g.setIsoTransmissionSpeed(vp1394TwoGrabber::vpISO_SPEED_800); // 1394b

  g.acquire(I) ;

#if defined VISP_HAVE_X11
  vpDisplayX display;
#elif defined VISP_HAVE_GDI
  vpDisplayGDI display;
#endif
  display.init(I, 800, 100, "Camera view");

  vpDisplay::display(I) ;
  vpDisplay::flush(I);

  // Material to create an mpeg
//   vpImage<vpRGBa> O;
//   vpVideoWriter writer;
//   writer.setBitRate(10000000);
//   writer.setCodec(CODEC_ID_MPEG1VIDEO);
//   writer.setFileName("video.mpeg");
//   writer.open(I);

  vpDot2 dot ;
  dot.setGrayLevelPrecision(0.9);
  dot.setEllipsoidShapePrecision(0); // to allow the tracking of a non ellipoid shape

  std::cout << "Click on a dot..." << std::endl;
  dot.initTracking(I) ;
  vpDisplay::flush(I);

  while ( 1 ) {
    g.acquire(I);
    vpDisplay::display(I);
    
    dot.track(I);
    dot.display(I, vpColor::red, 3);
    vpDisplay::flush(I);

    // Material to create an mpeg
//     vpDisplay::getImage(I, O);
//     writer.saveFrame(O);
  }    

#endif
  return 0;
}
