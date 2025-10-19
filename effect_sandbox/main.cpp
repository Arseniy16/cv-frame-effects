#include <chrono>
#include <fstream>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/filesystem.hpp>
#include <stdexcept>
#include <functional>
#include <string>

#include "cli.h"
#include "effect.h"

// #include <opencv2/highgui.hpp>

int main(int argc, char* argv[]) {
    std::vector<std::string> frames;
    std::vector<std::string> masks;

    int name_index;
    std::cout << "Choose video (1 - Andy, 2 - Carl, 3 - Hope, 4 - Naomi): ";
    std::cin >> name_index;
    if (name_index < 1 || name_index > 4) {
        std::cout << "Incorrect input." << std::endl;
        return 1;
    }

    std::string name;
    switch (name_index) {
        case 1: 
            name = "Andy";
            break;
        case 2: 
            name = "Carl";
            break;
        case 3: 
            name = "Hope";
            break;
        case 4:
            name = "Naomi";
            break;
        default: break;
    }

    auto framespath = "frames/" + name + "/frames";
    auto maskspath = "frames/" + name + "/masks";

    cv::utils::fs::glob(framespath, "*.png", frames);
    if (frames.empty()) 
        cv::utils::fs::glob(framespath, "*.jpg", frames);

    cv::utils::fs::glob(maskspath, "*.png", masks);
    if (masks.empty())
        cv::utils::fs::glob(maskspath, "*.jpg", masks);

    std::sort(frames.begin(), frames.end());
    std::sort(masks.begin(), masks.end());

    cv::Size frameSize = cv::imread(frames[0]).size();

    cv::VideoWriter writer;

    std::string outPath;
    std::cout << "Enter name of file: ";
    std::cin >> outPath;
    outPath += ".mp4";

    int effect;

    std::cout << "Choose effect (1 - Dots, 2 - Ripple, 3 - Tone, 4 - bonus): ";
    std::cin >> effect;

    if (effect < 1 || effect > 4) {
        std::cout << "Incorrect input." << std::endl;
        return 1;
    }

    auto fourcc = cv::VideoWriter::fourcc('M', 'J', 'P', 'G');

    if (!writer.open(outPath, fourcc, 30, frameSize)) {
        std::cout << ("Video writer failed");
        return -1;
    }

#define vch_timestamp() std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count()

    auto startT = vch_timestamp();

    auto bar = cli::ProgressBar(std::cout, frames.size());
    
    cv::Point2d center;

    for (int i = 0; i < frames.size(); ++i) {
        cv::Mat img = cv::imread(frames[i]);
        cv::resize(img, img, frameSize);

        cv::Mat mask = cv::imread(masks[i], cv::IMREAD_GRAYSCALE);
        cv::resize(mask, mask, img.size());

        switch (effect) {
            case Effect::kDots: 
                img = applyDotsEffect(img, mask, i);
                break;
            case Effect::kRipple:
                img = applyRippleEffect(img, mask, &center, i);
                break;
            case Effect::kTone:
                img = applyToneEffect(img, mask, i);
                break;
            case Effect::kBonus:
                img = applyBonusEffect(img, mask, i);
                break;
            default: break;
        }

        writer.write(img);
        bar.update();
    }

    bar.close();

    auto perFrameT = (vch_timestamp() - startT) / frames.size();

    std::cout << "average frame time, ms: " << perFrameT << std::endl;

    if (writer.isOpened()) writer.release();

    return 0;
}
