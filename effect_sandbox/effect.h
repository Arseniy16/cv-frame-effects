#ifndef _EFFECT_H_
#define _EFFECT_H_

#include "contour.h"

#include <opencv2/opencv.hpp>

enum Effect 
{
    kDots   = 1,
    kRipple = 2,
    kTone   = 3,
    kBonus  = 4,
};

cv::Mat applyDotsEffect(const cv::Mat& image, const cv::Mat& mask, int idx);
cv::Mat applyRippleEffect(const cv::Mat& image, const cv::Mat& mask, cv::Point2d* center, int idx = 0);
cv::Mat applyToneEffect(const cv::Mat& image, const cv::Mat& mask, int idx = 0);
cv::Mat applyBonusEffect(const cv::Mat& image, const cv::Mat& mask, int idx = 0);

#endif  // _EFFECT_H_
