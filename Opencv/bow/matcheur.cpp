/*
 * DONE : Extraire les keypoints et descripteurs
 * DONE : Effectuer le matching entre deux images
 * ----------------------------------------------
 * TODO : Effectuer le matching avec les cascades
 *
 *
 */

#include <stdio.h>
#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/core.hpp>
#include <opencv2/xfeatures2d/nonfree.hpp>

using namespace cv;

int main(int argc, char** argv)
{

  if(argc < 2){
      std::cout << "Usage : " << argv[0] << " <image-1> <image-2>" << std::endl;
    }
  else{
      Mat img1 = imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);
      Mat img2 = imread(argv[2], CV_LOAD_IMAGE_GRAYSCALE);

      if(!img1.data && !img2.data) return -1;
      //if(!img1.data) return -1;


      std::vector<KeyPoint> keypoint1;
      std::vector<KeyPoint> keypoint2;
      Mat output1;
      Mat output2;
      Mat descripteur1;
      Mat descripteur2;

      // SIFT detecteur
      Ptr<Feature2D> detector = xfeatures2d::SiftFeatureDetector::create();

      // Detect Kp Image 1
      detector->detect(img1,keypoint1);
      drawKeypoints(img1, keypoint1, output1);

      //imshow("Keypoint Sift Image 1",output1);

      // Detect Kp Image 2
      detector->detect(img2,keypoint2);
      drawKeypoints(img2, keypoint2,output2);

      //imshow("Keypoint Sift Image 2",output2);

      // SIFT extracteur
      Ptr<Feature2D> extractor = xfeatures2d::SiftDescriptorExtractor::create();

      extractor->compute(img1,keypoint1,descripteur1);
      extractor->compute(img2,keypoint2,descripteur2);

      // FLANN matcheur
      std::vector<DMatch>matches;
      Ptr<FlannBasedMatcher> matcheur(new FlannBasedMatcher);
      matcheur->match(descripteur1,descripteur2,matches);

      // Reduce matche by distance
      std::vector<DMatch> goodMatches;


      double min_dist = 10000;
      double max_dist = 0;

/*
      for(std::vector<DMatch>::iterator it = matches.begin(); it != matches.end(); ++it)
        {
          DMatch matche;
          if( matche = *it)
            {
              double dist = matche.distance;
              if(dist < min_dist) min_dist = dist;
              if(dist > max_dist) max_dist = dist;
            }
        }

      std::cout <<" Distances de matche - Max : " << max_dist << " Min : "<< min_dist << std::endl;
      for(std::vector<DMatch>::iterator it = matches.begin(); it != matches.end(); ++it)
        {
          DMatch matche;
          if( matche = *it)
            {
              if(matche.distance < 2*min_dist) goodMatches.push_back(matche);
            }
        }
*/

      for(int i=0; i<matches.size();++i)
        {
          DMatch matche = matches[i];

              double dist = matche.distance;
              if(dist < min_dist) min_dist = dist;
              if(dist > max_dist) max_dist = dist;

        }

      std::cout <<" Distances de matche - Max : " << max_dist << " Min : "<< min_dist << std::endl;
      for(int i=0; i<matches.size();++i)
        {
          DMatch matche = matches[i];

              if(matche.distance < 2*min_dist) goodMatches.push_back(matche);

        }

      Mat img_matches;
      drawMatches(img1,keypoint1,img2,keypoint2,goodMatches,img_matches);
      imshow("MATCHING SIFT + FLANN WITH DISTANCE FILTERING ( MATCH < 2 x MIN_DISTANCE )", img_matches);

      waitKey(0);

      return 0;
    }

}
