/*! \example tutorial-me-ellipse-tracker.cpp */
#include "ellipse-tracking.hpp"

int ellipse_track()
{
#if (defined(VISP_HAVE_V4L2))
  try {
    vpImage<unsigned char> I;


#if defined(VISP_HAVE_V4L2)
    vpV4l2Grabber g;
#endif
    g.open(I);
    g.acquire(I);

#if defined(VISP_HAVE_X11)
    vpDisplayX d(I, 0, 0, "Camera view");
#elif defined(VISP_HAVE_GDI)
    vpDisplayGDI d(I, 0, 0, "Camera view");
#elif defined(VISP_HAVE_OPENCV)
    vpDisplayOpenCV d(I, 0, 0, "Camera view");
#else
    std::cout << "No image viewer is available..." << std::endl;
#endif

    vpDisplay::display(I);
    vpDisplay::flush(I);

    vpMe me;
    me.setRange(10);
    me.setThreshold(15000);
    me.setSampleStep(5);

    vpMeEllipse ellipse;
    ellipse.setMe(&me);
    ellipse.setDisplay(vpMeSite::RANGE_RESULT);
    ellipse.initTracking(I);

    while(1) {
      g.acquire(I);
      vpDisplay::display(I);
      ellipse.track(I);
      ellipse.display(I, vpColor::red);
      vpDisplay::displayText(I,10,10,"Ellipse time",vpColor::blue);
      vpDisplay::flush(I);

    }
  }
  catch(vpException e) {
    std::cout << "Catch an exception: " << e << std::endl;
  }
#endif
}
