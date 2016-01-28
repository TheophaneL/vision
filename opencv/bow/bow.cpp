/*
 * Creation d'un Bag-Of-Word a partir d'une collection d'images
 *
 */

#include <stdio.h>
#include <vector>
#include <opencv2/opencv.hpp>
//#include <opencv2/features2d/features2d.hpp>
#include <opencv2/core.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <opencv2/opencv_modules.hpp>

using namespace cv;

int main(int argc, char** argv )
{
  //Step 1 - Obtain the set of bags of features.
  /*
  if(argc< 3){
      std::cout <<"Usage : "<< argv[0] <<"[--files ] " << std::endl;
    }
    */
  // File name where
  char * filename = new char[100];
  //to store the current input image
  Mat input;

  //To store the keypoints that will be extracted by SIFT
  std::vector<KeyPoint> keypoints;
  //To store the SIFT descriptor of current image
  Mat descriptor;
  //To store all the descriptors that are extracted from all the images.
  Mat featuresUnclustered;

  //The SIFT feature extractor and descriptor
  Ptr<Feature2D> f2D = xfeatures2d::SIFT::create();

  //Select a Stack of X images
  //feature descriptors and build the vocabulary
  for(int f=1;f<=26;++f){
      //create the file name of an image
      sprintf(filename,"../../../images/%i.jpg",f);
      std::cout << filename << std::endl;
      //open the file
      input = imread(filename,CV_LOAD_IMAGE_GRAYSCALE);//CV_LOAD_IMAGE_GRAYSCALE); //Load as grayscale
      //detect feature points
      f2D->detect(input,keypoints);
      std::cout <<" Keypoint "<<f<<" : "<< keypoints.size() << std::endl;

      //compute the descriptors for each keypoint
      f2D->compute(input,keypoints,descriptor);
      //detector.compute(input, keypoints,descriptor);

      //put the all feature descriptors in a single Mat object
      //std::cout <<" Descripteur "<<f<<" : "<< descriptor << std::endl;
      featuresUnclustered.push_back(descriptor);
      //print the percentage
      printf("%i percent done\n",f);
    }

  //Construct BOWKMeansTrainer
  //the number of bags
  int dictionarySize=100;
  //define Term Criteria
  TermCriteria tc(CV_TERMCRIT_ITER,100,0.001);
  //retries number
  int retries=1;
  //necessary flags
  int flags=KMEANS_PP_CENTERS;
  //Create the BoW (or BoF) trainer
  //BOWKMeansTrainer bowTrainer(dictionarySize,tc,retries,flags);
  BOWKMeansTrainer bowTrainer(dictionarySize);

  //cluster the feature vectors
  //std::cout <<" Check data..." << std::endl;
  //std::cout <<featuresUnclustered<< std::endl;

  std::cout <<" Start bow training..." << std::endl;
  cv::Mat featuresUnclusteredConvert(featuresUnclustered.rows,featuresUnclustered.cols,CV_32F);
  featuresUnclustered.convertTo(featuresUnclusteredConvert,CV_32F);
  Mat dictionary=bowTrainer.cluster(featuresUnclusteredConvert);
  //store the vocabulary
  FileStorage fs("dictionary.yml", FileStorage::WRITE);
  fs << "vocabulary" << dictionary;
  fs.release();


}
