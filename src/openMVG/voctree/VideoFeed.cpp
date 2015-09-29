/* 
 * File:   VideoFeed.cpp
 * Author: sgaspari
 * 
 * Created on September 28, 2015, 10:35 AM
 */

#include "VideoFeed.hpp"

#include <openMVG/image/image_converter.hpp>

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core/eigen.hpp>
#include <opencv2/highgui.hpp>

#include <iostream>
#include <exception>

namespace openMVG{
namespace dataio{

class VideoFeed::FeederImpl
{
public:
  FeederImpl() : _isInit(false) { }
  
  FeederImpl(const std::string &videoPath, const std::string &calibPath);
  
  bool isInit() const {return _isInit;}
  
  bool next(image::Image<unsigned char> &imageGray,
                     cameras::Pinhole_Intrinsic_Radial_K3 &camIntrinsics,
                     bool &hasIntrinsics);
  
private:
  bool _isInit;
  cv::VideoCapture _videoCapture;
  cameras::Pinhole_Intrinsic_Radial_K3 _camIntrinsics;
  
};


VideoFeed::FeederImpl::FeederImpl(const std::string &videoPath, const std::string &calibPath)
{
  _isInit = false;

  // load the video
  _videoCapture.open(videoPath);
  if (!_videoCapture.isOpened())
  {
    std::cerr << "Unable to open the video : " << videoPath ;
    throw std::invalid_argument("Unable to open the video : "+videoPath);
  }

  // load the calibration path
  _isInit = readCalibrationFromFile(calibPath, _camIntrinsics);
}



bool VideoFeed::FeederImpl::next(image::Image<unsigned char> &imageGray,
                   cameras::Pinhole_Intrinsic_Radial_K3 &camIntrinsics,
                   bool &hasIntrinsics)
{
  cv::Mat frame;
  _videoCapture >> frame;

  if(!frame.data)
  {
    return false;
  }

  if(frame.channels() == 3)
  {
    // convert to gray
    cv::Mat grey;
    cv::cvtColor(frame, grey, CV_BGR2GRAY);
    imageGray.resize(grey.cols, grey.rows);
    cv::cv2eigen(grey, imageGray);
//      std::cout << grey.channels() << " " << grey.rows << " " << grey.cols << std::endl;
//      std::cout << imageGray.Depth() << " " << imageGray.Height() << " " << imageGray.Width() << std::endl;
  }
  else
  {
    cv::cv2eigen(frame, imageGray);
  }

  camIntrinsics = _camIntrinsics;
  hasIntrinsics = true;

  return true;
}


/*******************************************************************************/
/*                                 VideoFeed                                   */
/*******************************************************************************/

VideoFeed::VideoFeed() : _feeder(new FeederImpl()) { }

VideoFeed::VideoFeed(const std::string &videoPath, const std::string &calibPath) 
  : _feeder(new FeederImpl(videoPath, calibPath))
{ }


bool VideoFeed::next(image::Image<unsigned char> &imageGray,
                     cameras::Pinhole_Intrinsic_Radial_K3 &camIntrinsics,
                     bool &hasIntrinsics)
{
  return(_feeder->next(imageGray, camIntrinsics, hasIntrinsics));
}

bool VideoFeed::isInit() const {return(_feeder->isInit()); }

VideoFeed::~VideoFeed() { }

}//namespace dataio 
}//namespace openMVG