// Tracking-egde on model-based
#include "cube-tracking.hpp"


int cube_model_tracking(int argc, const char** argv)
{

  // On selectionne la caméra par OpenCV, pourquoi ? parce que c'est magique et c'est pour le gameplay!

#if defined(VISP_HAVE_OPENCV) && (VISP_HAVE_OPENCV_VERSION >= 0x020100) || defined(VISP_HAVE_FFMPEG)
  try {


    std::string modele = "cube.cao";
    int device = 0;

    for (int i=0; i<argc; i++) {
      if (std::string(argv[i]) == "--model")
        modele = std::string(argv[i+1]);
      else if(std::string(argv[i]) == "--device"){
          device = atoi(argv[i+1]);
        }
      else if (std::string(argv[i]) == "--help") {
        std::cout << "\nUsage: " << argv[0] << " [--name <video name>] [--help]\n" << std::endl;
        return 0;
      }
    }

    // Ouverture de la cam avec OpenCV
    std::cout << "Use device :" << device << std::endl;
    cv::VideoCapture cap(device);
    if(!cap.isOpened()){
        std::cout << "Open cam Fail!" << std::endl;
        return -1;
      }
    cv::Mat frame;
    cap >> frame;
    //cap.read(frame);
    std::cout << "Frame size : "<< frame.cols << "x" << frame.rows << std::endl;


#if defined(VISP_HAVE_X11)
    vpDisplayX display;
#else
    std::cout << "No image viewer is available..." << std::endl;
    return 0;
#endif


    std::string parentname = vpIoTools::getParent(modele);
    std::string objectname= vpIoTools::getNameWE(modele);

    if(! parentname.empty())
      objectname = parentname + "/" + objectname;

    //std::cout << "Video " << video << std::endl;
    std::cout << "Tracker requested config files: " << objectname
              << ".[init,"
#ifdef VISP_HAVE_XML2
              << "xml,"
#endif
              << "cao or wrl]" << std::endl;
    std::cout << "Tracker optional config files: " << objectname << ".[ppm]" << std::endl;

    //! [Image]
    vpImage<vpRGBa> Iaffiche;
    vpImage<unsigned char> I;
    vpImageConvert::convert(frame,I);
    vpImageConvert::convert(frame,Iaffiche);

    vpCameraParameters cam;
    //! [Image]
    //! [cMo]
    vpHomogeneousMatrix cMo;
    //! [cMo]

    display.init(I, I.getHeight(), I.getWidth(),"Cube model-based tracker");


    //! [Constructor]
    vpMbEdgeTracker tracker;
    vpMbtMeEllipse ellipse;
    //! [Constructor]
    bool usexml = false;
    //! [Load xml]
#ifdef VISP_HAVE_XML2
    if(vpIoTools::checkFilename(objectname + ".xml")) {
      tracker.loadConfigFile(objectname + ".xml");

      usexml = true;
    }
#endif
    //! [Load xml]
    if (! usexml) {
      //! [Set parameters]
      vpMe me;
      me.setMaskSize(5);
      me.setMaskNumber(180);
      me.setRange(8);
      me.setThreshold(10000);
      me.setMu1(0.5);
      me.setMu2(0.5);
      me.setSampleStep(4);
      me.setNbTotalSample(250);
      tracker.setMovingEdge(me);
      cam.initPersProjWithoutDistortion(839, 839, 325, 243);
      tracker.setCameraParameters(cam);
      //! [Set angles]
      tracker.setAngleAppear( vpMath::rad(70) );
      tracker.setAngleDisappear( vpMath::rad(80) );
      //! [Set angles]
      //! [Set clipping distance]
      tracker.setNearClippingDistance(0.1);
      tracker.setFarClippingDistance(100.0);
      //! [Set clipping distance]
      //! [Set clipping fov]
      tracker.setClipping(tracker.getClipping() | vpMbtPolygon::FOV_CLIPPING);
      //! [Set clipping fov]
      //! [Set parameters]
    }
    //! [Set ogre]
    tracker.setOgreVisibilityTest(false);
    //! [Set ogre]
    //! [Load cao]
    if(vpIoTools::checkFilename(objectname + ".cao")){
      std::cout << "Ouveture du "<<objectname<<".cao" << std::endl;
      tracker.loadModel(objectname + ".cao");
      }
    //! [Load cao]
    //! [Load wrl]
    else if(vpIoTools::checkFilename(objectname + ".wrl"))
      tracker.loadModel(objectname + ".wrl");
    //! [Load wrl]
    //! [Set display]
    tracker.setDisplayFeatures(true);
    //! [Set display]
    //! [Init]
    tracker.initClick(I, objectname + ".init", true);
    //! [Init]
    while(1){
        cap >> frame;
        //cap.read(frame);
        vpImageConvert::convert(frame,I);
        vpImageConvert::convert(frame,Iaffiche);

        vpDisplay::display(I);
        //vpDisplay::display(Iaffiche);
        //! [Track]
        tracker.track(I);
        //! [Track]
        //! [Get pose]
        tracker.getPose(cMo);
        //! [Get pose]
        //! [Display]
        tracker.getCameraParameters(cam);
        tracker.display(I, cMo, cam, vpColor::red, 2);
        //! [Display]
        vpDisplay::displayFrame(I, cMo, cam, 0.025, vpColor::none, 3);
        vpDisplay::displayText(I, 10, 10, "A click to exit...", vpColor::red);
        vpDisplay::flush(I);
        if (vpDisplay::getClick(I, false))
          break;
      }
    vpDisplay::getClick(I);
    //! [Cleanup]
#ifdef VISP_HAVE_XML2
    vpXmlParser::cleanup();
#endif
#if defined(VISP_HAVE_COIN) && (COIN_MAJOR_VERSION == 3)
    SoDB::finish();
#endif
    //! [Cleanup]
  }
  catch(vpException e) {
    std::cout << "Catch an exception: " << e << std::endl;
  }
#else
  //(void)argc;
  //(void)argv;
  std::cout << "Install OpenCV or ffmpeg and rebuild ViSP." << std::endl;
#endif
}

int main(int argc, const char** argv)
{
  if(argc < 4){
      std::cout << "Usage :" << argv[0] << " [--device <numero>] [--model <file-model-name>]" << std::endl;
    }
  cube_model_tracking(argc, argv);

  return 0;
}


/*
// Affichage statique d'un modèle d'après une matrice homogène
#include <visp/vpMbEdgeTracker.h>
#include <visp/vpImage.h>
#include <visp/vpImageIo.h>
#include <visp/vpHomogeneousMatrix.h>
#include <visp/vpCameraParameters.h>
#include <visp/vpDisplayX.h>

int main()
{
  vpMbEdgeTracker tracker; // Create a model based tracker.
  vpImage<unsigned char> I;
  vpHomogeneousMatrix cMo; // Pose used to display the model.
  vpCameraParameters cam;

  // Acquire an image
  cv::VideoCapture cap(1);
  if(!cap.isOpened()){
      std::cout << "Open cam Fail!" << std::endl;
      return -1;
    }
  cv::Mat frame;
  cap >> frame;
  vpImageConvert::convert(frame,I);

#if defined VISP_HAVE_X11
  vpDisplayX display;
  display.init(I,100,100,"Mb Edge Tracker");
#endif

#if defined VISP_HAVE_XML2
  tracker.loadConfigFile("cube1.xml"); // Load the configuration of the tracker
#endif
  tracker.getCameraParameters(cam); // Get the camera parameters used by the tracker (from the configuration file).
  tracker.loadModel("cube1.cao"); // load the 3d model, to read .wrl model coin is required, if coin is not installed .cao file can be used.
  std::ifstream f("cubeCMO.dat");
  cMo. load(f);
  while(true){
    // acquire a new image
    // Get the pose using any method
    cap >> frame;
    vpImageConvert::convert(frame,I);
    vpDisplay::display(I);
    tracker.display(I, cMo, cam, vpColor::yellow, 1, true); // Display the model at the computed pose.
    vpDisplay::displayFrame(I, cMo, cam, 0.025, vpColor::none, 3);
    vpDisplay::displayText(I, 10, 10, "A click to exit...", vpColor::red);
    vpDisplay::flush(I);
    if (vpDisplay::getClick(I, false))
      break;
  }

  // Cleanup memory allocated by xml library used to parse the xml config file in vpMbEdgeTracker::loadConfigFile()
  vpXmlParser::cleanup();

  return 0;
}
*/
