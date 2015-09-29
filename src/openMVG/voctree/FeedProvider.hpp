/* 
 * File:   FeedProvider.hpp
 * Author: sgaspari
 *
 * Created on September 28, 2015, 6:48 PM
 */

#pragma once

#include "IFeed.hpp"

#include <string>
#include <memory>

namespace openMVG{
namespace dataio{

class FeedProvider
{
public:
  
  FeedProvider(const std::string &feedPath, const std::string &calibPath);
  
  /**
   * @brief Provide a new image from the feed.
   * 
   * @param[out] imageGray The new image from the feed.
   * @param[out] camIntrinsics The associated camera intrinsics.
   * @param[out] hasIntrinsics True if \p camIntrinsics is valid, otherwise there
   * is no intrinsics associated to \p imageGray.
   * @return True if there is a new image, false otherwise.
   */  
  bool next(image::Image<unsigned char> &imageGray,
        cameras::Pinhole_Intrinsic_Radial_K3 &camIntrinsics,
        bool &hasIntrinsics);
  /**
   * @brief Return true if the feed is correctly initialized.
   * 
   * @return True if the feed is correctly initialized.
   */  
  bool isInit() const;

  virtual ~FeedProvider();
    
private:
  std::unique_ptr<IFeed> _feeder;

};

}//namespace dataio 
}//namespace openMVG

