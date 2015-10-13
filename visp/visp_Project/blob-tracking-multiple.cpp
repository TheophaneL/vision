#include "blob-tracking-multiple.hpp"

/*
 * Blop time
 */

int blob_track()
{
  try{
    bool learn = true;

    vpImage<unsigned char> I;  // image gray level

    cv::VideoCapture g(0);
    cv::Mat frame;
    g >> frame;
    vpImageConvert::convert(frame,I);


#if defined(VISP_HAVE_X11)
    vpDisplayX d(I,0,0,"Camera view");
#elif defined(VISP_HAVE_OPENCV)
    vpDisplayOpenCV d(I,0,0,"Camera view");
#endif

    vpDot2 blob;
    std::list<vpDot2> blob_list;

    vpImagePoint germ;
    bool init_done = false;

    std::cout << "Click!!!!" << std::endl;

    while(1) {
        try{
          //if(vpDisplay::getKeyboardEvent(I)) init_done = false;
          g >> frame;
          vpImageConvert::convert(frame,I);

          vpDisplay::display(I);

          if(! init_done){
              vpDisplay::displayText(I,vpImagePoint(10,10), "Click on blob to initialize the tracker",vpColor::red);
              if(vpDisplay::getClick(I,germ,false)){
                  if(learn){
                      // Apprentissage des caractéristiques du blob pour l'auto-detect
                      blob.setGraphics(true);
                      blob.setGraphicsThickness(2);
                      blob.initTracking(I,germ);
                      blob.track(I);
                      std::cout << "Blob carac : " << std::endl;
                      std::cout << "Width : " << blob.getWidth() << std::endl;
                      std::cout << "Height : "<< blob.getHeight() << std::endl;

                      std::cout << "area : " << blob.getArea() << std::endl;
                      std::cout << "gray level min :" << blob.getGrayLevelMin() << std::endl;
                      std::cout << "gray level max : " << blob.getGrayLevelMax() << std::endl;
                      std::cout << "gray level precision : " << blob.getGrayLevelPrecision() << std::endl;
                      std::cout << "size precision : " << blob.getSizePrecision() << std::endl;
                      std::cout << "Ellipsoïde précision : " << blob.getEllipsoidShapePrecision() << std::endl;
                    }
                  else{
                     // Définition des paramètres a reconnaitre
                      blob.setWidth(50);
                      blob.setHeight(50);
                      blob.setArea(1700);
                      blob.setGrayLevelMin(0);
                      blob.setGrayLevelMax(30);
                      blob.setGrayLevelPrecision(0.8);
                      blob.setSizePrecision(0.65);
                      blob.setEllipsoidShapePrecision(0.65);
                    }
                  blob.searchDotsInArea(I,0,0,I.getWidth(),I.getHeight(), blob_list);
                  if(learn){
                      blob_list.push_back(blob);
                    }
                  std::cout << "Number of blob detect : " << blob_list.size() << std::endl;
                  init_done = true;
                  }
            }
          else{
              for(std::list<vpDot2>::iterator it=blob_list.begin(); it !=blob_list.end(); ++it)
                {
                  (*it).setGraphics(true);
                  (*it).setGraphicsThickness(3);
                  (*it).track(I);
                }
            }
          vpDisplay::flush(I);
          /*
        if (vpDisplay::getClick(I, false))
          break;
      */
        }
        catch(...) {
          init_done = false;
        }
      }
  }
  catch(vpException e){
    std::cout << "Catch an exception" << std::endl;
  }
}


