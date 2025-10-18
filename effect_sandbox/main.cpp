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

//using namespace cv;
//using namespace std;

// #include <opencv2/highgui.hpp>

#if 0
static void paintAlphaMat(Mat &mat)
{
    CV_Assert(mat.channels() == 4);
    for (int i = 0; i < mat.rows; ++i)
    {
        for (int j = 0; j < mat.cols; ++j)
        {
            Vec4b& bgra = mat.at<Vec4b>(i, j);
            bgra[0] = UCHAR_MAX; // Blue
            bgra[1] = saturate_cast<uchar>((float (mat.cols - j)) / ((float)mat.cols) * UCHAR_MAX); // Green
            bgra[2] = saturate_cast<uchar>((float (mat.rows - i)) / ((float)mat.rows) * UCHAR_MAX); // Red
            bgra[3] = saturate_cast<uchar>(0.5 * (bgra[1] + bgra[2])); // Alpha
        }
    }
}

int main()
{
    Mat mat(480, 640, CV_8UC4); // Create a matrix with alpha channel
    paintAlphaMat(mat);
    vector<int> compression_params;
    compression_params.push_back(IMWRITE_PNG_COMPRESSION);
    compression_params.push_back(9);
    bool result = false;
    try
    {
        result = imwrite("alpha.png", mat, compression_params);
    }
    catch (const cv::Exception& ex)
    {
        fprintf(stderr, "Exception converting image to PNG format: %s\n", ex.what());
    }
    if (result)
        printf("Saved PNG file with alpha data.\n");
    else
        printf("ERROR: Can't save PNG file.\n");
    vector<Mat> imgs;
    imgs.push_back(mat);
    imgs.push_back(~mat);
    imgs.push_back(mat(Rect(0, 0, mat.cols / 2, mat.rows / 2)));
    imwrite("iff", imgs);
    printf("Multiple files saved in iff\n");
    return result ? 0 : 1;
}

#endif 
#if 0 //goood

int main(int argc, char* argv[])
{
    Mat img = imread("../good.jpg"); // Открытие файла
    // Отобразить на изображении белую пунктирную сетку
    for (int i = 0; i < img.rows; i++)
        for (int j = 0; j < img.cols; j++)
            if ((i % 20 == 10 && j % 2 == 1) ||
                (j % 50 == 25 && i % 2 == 1))
            {
                img.at<Vec3b>(i, j)[0] = 255;
                img.at<Vec3b>(i, j)[1] = 255;
                img.at<Vec3b>(i, j)[2] = 255;
            }
    imwrite("image01_res.jpg", img);

    return 0;
}
#endif 

#if 0 // treshold

int main(int argc, char* argv[]) 
{

    cv::Mat original_image;//declaring a matrix to load the original image//
    cv::Mat grayscale_image;//declaring a matrix to store converted image//
    cv::Mat binary_image;//declaring a matrix to store the binary image
    cv::Mat inverted_binary_image;//declaring a matrix to store inverted binary image
    

    //vector<int> compression_params;
    //compression_params.push_back(IMWRITE_PNG_COMPRESSION);
    //compression_params.push_back(9);

    //namedWindow("Binary Image");//declaring window to show binary image//
    //namedWindow("Inverted Binary Image");//declaring window to show inverted binary image//
    original_image = cv::imread("../good.jpg", 0);//loading image into matrix//
    
    //string image_path = samples::findFile("../good.jpg");
    //original_image = imread(image_path, IMREAD_COLOR);

    //cv::cvtColor(original_image, grayscale_image, cv::COLOR_BGR2GRAY);//Converting BGR to Grayscale image and storing it into 'converted' matrix//
    for (int i = 0; i < 255; i++)
    {

        // cv::threshold(grayscale_image, binary_image, i, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);//converting grayscale image stored in 'converted' matrix into binary image//
        // cv::threshold(grayscale_image, binary_image, i, 255, cv::THRESH_OTSU);//converting grayscale image stored in 'converted' matrix into binary image//
        cv::threshold(original_image, binary_image, i, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);//converting grayscale image stored in 'converted' matrix into binary image//
        
        //cv::bitwise_not(binary_image, inverted_binary_image);//inverting the binary image and storing it in inverted_binary_image matrix//
        
        // imwrite("output.jpg", inverted_binary_image);

        std::ostringstream prefix;

        prefix << "photo/output_" << i << "_.jpg";
        std::string str = prefix.str();

        // cv::imwrite(str, inverted_binary_image); //compression_params);
        cv::imwrite(str, binary_image);
    }
    return 0;
}
#endif

#if 0
int main(int argc, char* argv[]) 
{
    cv::VideoCapture video_capture("../Boy.mp4");
    
    if (!video_capture.isOpened())
    {
        std::cout << "Error opening video stream or file" << std::endl;
    } 
/*
    else
    {
        int fps = video_capture.get(cv::CAP_PROP_FPS);
        std::cout << "Frames per second :" << fps << std::endl;
        
        int frame_count = video_capture.get(cv::CAP_PROP_FRAME_COUNT);
        std::cout << "Frame count :" << frame_count << std;
    }
*/

    while (video_capture.isOpened())
    {
        cv::Mat frame;

        bool isSuccess = video_capture.read(frame);

        if(isSuccess == true)
        {
            cv::imshow("Frame", frame);
        }

        if (isSuccess == false)
        {
            std::cout << "Video camera is disconnected" << std::endl;
            break;
        }        
        
        int key = cv::waitKey(20);
        
        if (key == 'q')
        {
            std::cout << "q key is pressed by the user. Stopping the video" << std::endl;
            break;
        }
    }

    video_capture.release();
    //destroyAllWindows();

    return 0;
}
#endif


#if 1 //best
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
    switch (name_index) 
    {
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
    outPath += ".avi";

    int effect;

    std::cout << "Choose effect (1 - Dots, 2 - Ripple, 3 - Tone, 4 - bonus): ";
    std::cin >> effect;

    if (effect < 1 || effect > 4) 
    {
        std::cout << "Incorrect input." << std::endl;
        return 1;
    }

    auto fourcc = cv::VideoWriter::fourcc('M', 'J', 'P', 'G');

    if (!writer.open(outPath, fourcc, 30, frameSize)) 
    {
        std::cout << ("Video writer failed");
        return -1;
    }

#define vch_timestamp() std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count()

    auto startT = vch_timestamp();

    auto bar = cli::ProgressBar(std::cout, frames.size());
    
    cv::Point2d center;

    for (int i = 0; i < frames.size(); ++i) 
    {
        cv::Mat img = cv::imread(frames[i]);
        cv::resize(img, img, frameSize);

        cv::Mat mask = cv::imread(masks[i], cv::IMREAD_GRAYSCALE);
        cv::resize(mask, mask, img.size());

        switch (effect) 
        {
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

#endif