#include "effect.h"

cv::Mat applyDotsEffect(const cv::Mat& image, const cv::Mat& mask, int idx) 
{
    // Settings

    const double kGlowBrightness = 0.4;
    const double kMinFlickerBrightness = 0.1;
    const int kGlowRadius = 16;
    const int kCircleRadius = 8;
    const int kDistanceBetweenCircles = 30;
    const int fFlickerDuration = 50;

    // Effect

    auto contour = maskContour(mask);
    cv::Mat original_image = image.clone();
    cv::Mat glowing_layer = image.clone();

    cv::Point& prev = contour.front();

    for (auto& point : contour) 
    {
        if (kDistanceBetweenCircles < cv::norm(prev - point)) 
        {
            cv::circle( glowing_layer,
                        point,
                        kGlowRadius,
                        cv::Scalar{0, 255, 0},
                        cv::FILLED);

            cv::circle( image,
                        point,
                        kCircleRadius,
                        cv::Scalar{0, 255, 0},
                        cv::FILLED);

            prev = point;
        }
    }

    cv::addWeighted(glowing_layer, kGlowBrightness, image, 1 - kGlowBrightness, 0, image);

    double brightness_coefficient = std::abs(std::cos(M_PI * (idx*idx) / fFlickerDuration));

    double alpha = kMinFlickerBrightness + (1 - kMinFlickerBrightness) * brightness_coefficient;
    
    cv::addWeighted(image, alpha, original_image, 1 - alpha, 0, image);

    return image;
}

cv::Mat GetPolarImage(const cv::Mat& image, double radius, const cv::Point2d& center) 
{
    cv::Mat polar_image;
    cv::linearPolar(image, polar_image, center, radius, cv::WARP_FILL_OUTLIERS);
    return polar_image;
}

cv::Mat GetImageFromPolar(const cv::Mat& polar_image, double radius, const cv::Point2d& center) 
{
    cv::Mat image;
    cv::linearPolar(polar_image, image, center, radius, cv::WARP_INVERSE_MAP);
    return image;
}

cv::Point getCentroid(cv::InputArray points) 
{
    cv::Point coordinate;
    cv::Moments moment = cv::moments(points, false);
    
    double moment10 = moment.m10;
    double moment01 = moment.m01;
    double moment00 = moment.m00;

    coordinate.x = int(moment10 / moment00);
    coordinate.y = int(moment01 / moment00);
    
    return coordinate;
}

cv::Mat applyRippleEffect(const cv::Mat& image, const cv::Mat& mask, cv::Point2d* center, int idx) 
{
    // Settings

    int kWidth = 150;
    int kAmplitude = 75;
    int kDuration = 25;
    double kColorShiftCoefficient = 0.4;

    // Effect

    if (idx % kDuration == 0) 
    {
        *center = getCentroid(maskContour(mask));
    }

    double radius;
    
    if (center->x > image.size[0] / 2) 
    {
        if (center->y > image.size[0] / 2) 
        {
            radius = (center->x) * (center->x) + (center->y) * (center->y);
        } 
        else 
        {
            radius = (center->x) * (center->x) + (center->y - image.size[1]) * (center->y - image.size[1]);
        }
    } 
    else 
    {
        if (center->y > image.size[0] / 2) 
        {
            radius = (center->x - image.size[0]) * (center->x - image.size[0]) + (center->y) * (center->y);
        } 
        else 
        {
            radius = (center->x - image.size[0]) * (center->x - image.size[0]) +
                     (center->y - image.size[1]) * (center->y - image.size[1]);
        }
    }

    radius = std::sqrt(radius) * 1.1;

    cv::Mat polar_image = GetPolarImage(image, radius, *center);

    std::vector<cv::Mat> polar_image_splited;
    cv::split(polar_image, polar_image_splited);

    int begin_of_wave = polar_image.size[0] * (idx % kDuration) / kDuration;

    for (int x = 0; x < polar_image.size[1]; ++x) 
    {
        for (int y = 0; y < polar_image.size[0]; ++y) 
        {
            if (x > begin_of_wave && x - begin_of_wave < kWidth) 
            {
                for (int i = 0; i < 3; ++i) 
                {
                    double color_shift_coef = (1. + kColorShiftCoefficient * (i - 1));
                    double x_shift = 0.5 - (x / (1. * polar_image.size[0]));
                    double sin_shift = std::sin(M_PI * (x - begin_of_wave) / kWidth);
                    
                    int amplitude = std::max(kAmplitude * color_shift_coef * x_shift, 0.) * sin_shift;
                    int shifted_y = y - amplitude;
                    
                    if (shifted_y < 0) 
                    {
                        shifted_y = polar_image.size[1] + shifted_y;
                    }

                    polar_image_splited[i].at<uchar>(shifted_y, x) = polar_image.at<cv::Vec3b>(y, x)[i];
                }
            }
        }
    }

    cv::merge(polar_image_splited, polar_image);

    return GetImageFromPolar(polar_image, radius, *center);
}

cv::Mat applyToneEffect(const cv::Mat& image, const cv::Mat& mask, int idx) 
{
    // Settings

    const bool kUseMask = true;
    const bool kAnimated = true;
    const int kAnimationDuration = 100;
    const auto kColormap = cv::COLORMAP_HSV;
    // const auto kColormap = cv::COLORMAP_INFERNO;

    // Effect

    cv::Mat colored_image;
    cv::applyColorMap(image, colored_image, kColormap);

    if (kAnimated) 
    {
        cv::Mat hsv;
        cv::cvtColor(colored_image, hsv, cv::COLOR_BGR2HSV);

        for (int y = 0; y < hsv.size[1]; ++y) 
        {
            for (int x = 0; x < hsv.size[0]; ++x) 
            {
                hsv.at<cv::Vec3b>(y, x) = cv::Vec3b((hsv.at<cv::Vec3b>(y, x)[0] + 255 * idx / kAnimationDuration) % 255,
                                                                                        hsv.at<cv::Vec3b>(y, x)[1],
                                                                                        hsv.at<cv::Vec3b>(y, x)[2]);
            }
        }

        cv::cvtColor(hsv, colored_image, cv::COLOR_HSV2BGR);
    }

    if (kUseMask) 
    {
        cv::Mat masked_colored_image;
        cv::bitwise_or(colored_image, colored_image, masked_colored_image, mask);

        cv::Mat inverted_mask;
        cv::Mat masked_image;
        cv::bitwise_not(mask, inverted_mask);
        cv::bitwise_or(image, image, masked_image, inverted_mask);

        return masked_colored_image + masked_image;
    } 
    else 
    {
        return colored_image;
    }
}

cv::Mat shiftImage(cv::Mat image, double offset_x, double offset_y)
{
    cv::Mat matrix = (cv::Mat_<double>(2,3) << 1, 0, offset_x, 0, 1, offset_y);
    warpAffine(image, image, matrix, image.size());
    return image;
}

cv::Mat applyBonusEffect(const cv::Mat& image, const cv::Mat& mask, int idx) 
{
    // Settings

    const bool kAnimated = true;
    const int kOffset = 16;
    const int kMinOffset = 6;
    const int kDurationAnimation = 4;

    // Effect

    cv::Mat masked_image;
    cv::bitwise_or(image, image, masked_image, mask);

    std::vector<cv::Mat> splited_masked_image(3);
    cv::split(masked_image, splited_masked_image);

    double offset = kOffset;
    if (kAnimated) 
    {
        offset = kMinOffset + (kOffset - kMinOffset) * std::sin(M_PI * idx / kDurationAnimation);
    }

    shiftImage(splited_masked_image.at(0), offset, 0);
    shiftImage(splited_masked_image.at(2), -offset, 0);

    cv::merge(splited_masked_image, masked_image);

    cv::bitwise_or(image, masked_image, image);

    return image;
}
