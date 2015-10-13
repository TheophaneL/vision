/*! \example tutorial-me-line-tracker.cpp */
#include "line-tracking.hpp"

int line_track()
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

    std::cout << "Width/heigth : "<< g.getWidth()<<"x"<< g.getHeight() << std::endl;
    vpDisplay::display(I);
    vpDisplay::flush(I);

    vpMe me;
    me.setRange(15);
    me.setThreshold(15000);
    me.setSampleStep(5);

    vpMeLine line;
    line.setMe(&me);
    line.setDisplay(vpMeSite::RANGE_RESULT);
    line.initTracking(I);

    while(1) {
      g.acquire(I);
      vpDisplay::display(I);
      line.track(I);
      line.display(I, vpColor::red);
      vpDisplay::flush(I);
    }
  }
  catch(vpException e) {
    std::cout << "Catch an exception: " << e << std::endl;
  }
#endif
}
