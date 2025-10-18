#include "contour.h"

std::vector<Contour> maskContours(const cv::Mat& binMask) 
{
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;

    cv::findContours(binMask, contours, hierarchy, cv::RETR_CCOMP, cv::CHAIN_APPROX_NONE);

    if (contours.empty()) 
    {
        return {};
    }

    std::set<size_t> outerContoursSet;
    
    int next = 0;
    while(next != -1) 
    {
        outerContoursSet.insert(next);
        next = hierarchy.at(next)[0];
    }

    std::vector<Contour> outerContours;
    std::vector<Contour> innerContours;

    for (int i = 0; i < contours.size(); ++i) 
    {
        Contour cnt;
        cnt.points = std::move(contours[i]);
        auto outer = outerContoursSet.count(i);

        if (outer) 
        {
            cnt.inner = false;
            outerContours.push_back(std::move(cnt));
        } 
        else 
        {
            cnt.inner = true;
            innerContours.push_back(std::move(cnt));
        }
    }

    auto maxContourIt = std::max_element(contours.begin(), contours.end(), [](auto& a, auto& b) 
    {
        return a.size() < b.size();
    });

    auto maxContourIdx = std::distance(contours.begin(), maxContourIt);

    if (outerContoursSet.count(maxContourIdx) == 0) 
    {
        std::swap(outerContours, innerContours);
        for (auto& cnt : outerContours) 
        {
            cnt.inner = !cnt.inner;
        }
        for (auto& cnt : innerContours) 
        {
            cnt.inner = !cnt.inner;
        }
    }

    std::sort(outerContours.begin(), outerContours.end(), [](auto& a, auto& b) 
    {
        return a.points.size() > b.points.size();
    });

    std::sort(innerContours.begin(), innerContours.end(), [](auto& a, auto& b) 
    {
        return a.points.size() > b.points.size();
    });

    outerContours.insert(outerContours.end(), innerContours.begin(), innerContours.end());

    return outerContours;
}

std::vector<cv::Point> maskContour(const cv::Mat& binMask) 
{
    auto contours = maskContours(binMask);
    if (contours.empty()) 
    {
        return {};
    }

    return contours.front().points;
}
