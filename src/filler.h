#ifndef FILLER_H
#define FILLER_H

#include <opencv2/opencv.hpp>

class filler
{
private:
    cv::Mat whole_img;
    cv::Mat output_img;
    cv::Mat binary_segmented;
public:
    filler(cv::Mat input_image, cv::Mat segmented_img, cv::Rect position);
    cv::Mat get_output();
};

#endif // FILLER_H
