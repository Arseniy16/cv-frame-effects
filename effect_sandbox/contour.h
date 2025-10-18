#ifndef _CONTOUR_H_
#define _CONTOUR_H_

#include <opencv2/opencv.hpp>

struct Contour 
{
    std::vector<cv::Point> points;
    bool inner{};
};

std::vector<cv::Point> maskContour(const cv::Mat& binMask);

#endif  // _CONTOUR_H_
