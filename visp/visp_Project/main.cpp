#include <stdio.h>
#include <iostream>
#include "blob-tracking-multiple.hpp"
#include "line-tracking.hpp"
#include "ellipse-tracking.hpp"
#include "model-edge-tracking.hpp"
/*
 * Blop time
 */

int main(int argc, const char** argv)
{
  if(argc < 2){
      std::cout << "Usage :"<< argv[0] << " [Blob] - [Line] - [Ellipse] - [model-edge] " << std::endl;
      return 0;
    }
  else if(std::string(argv[1])== "Blob"){
      std::cout << "Demarrage du Blob detection\n" << std::endl;
      blob_track();
    }
  else if(std::string(argv[1])=="Line"){
      std::cout << "Demarrage du Line tracking\n" << std::endl;
      line_track();
    }
  else if(std::string(argv[1])=="Ellipse"){
      std::cout << "Demarrage du Ellipse tracking\n" << std::endl;
      ellipse_track();
    }
  else if(std::string(argv[1])=="model-edge"){
      std::cout << "Demarrage du Model-Based Edge tracking\n" << std::endl;
      model_edge_tracking(argc,argv);
    }
  else{
      std::cout << "Usage :"<< argv[0] << " [Blob] - [Line] - [Ellipse] - [model-edge] " << std::endl;
      return 0;
    }

}

