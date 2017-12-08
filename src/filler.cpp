#include "filler.h"

filler::filler(cv::Mat input_image, cv::Mat segmented_img, cv::Rect position){
    this->whole_img = input_image;
    this->output_img = this->whole_img.clone();
    this->binary_segmented = segmented_img;

    cv::Mat kernel_erode = cv::Mat(5, 5, CV_8UC1, 1);
    cv::erode(this->binary_segmented, this->binary_segmented, kernel_erode, cv::Point(-1,-1), 3);

    cv::Mat tmp_img = this->whole_img.clone();
    cv::blur(tmp_img, tmp_img, cv::Size(5, 5), cv::Point(-1,-1));

    cv::Mat noise = this->whole_img.clone();
    cv::Mat kernel = (cv::Mat_<float>(3,3) << 1, 0, 1, 2, -8, 2, 1, 0, 1);
    cv::filter2D(noise, noise, CV_8UC1, kernel);
    cv::cvtColor(noise, noise, CV_BGR2GRAY );
    cv::blur(noise, noise, cv::Size(5, 5), cv::Point(-1,-1));
    cv::blur(noise, noise, cv::Size(5, 5), cv::Point(-1,-1));

    cv::RNG rng;
    for(int y = 0; y < this->binary_segmented.rows; y++){
        for(int x = 0; x < this->binary_segmented.cols; x++){
            if ((int)this->binary_segmented.at<uchar>(y, x) == 0){
                cv::Vec3b first_color, second_color, new_color;
                int first_position, second_position;
                int first_noise, second_noise;
                int k = x;
                while(k < this->binary_segmented.cols && (int)this->binary_segmented.at<uchar>(y, k) == 0){k++;}
                first_position = std::max(position.x + x - 1, 0);
                second_position = std::min(position.x + k, this->whole_img.cols - 1);
                first_color = tmp_img.at<cv::Vec3b>(position.y + y, first_position);
                second_color = tmp_img.at<cv::Vec3b>(position.y + y, second_position);
                first_noise = (int)noise.at<uchar>(position.y + y, first_position)/2;
                second_noise = (int)noise.at<uchar>(position.y + y, second_position)/2;
                int length = second_position - first_position - 1;
                for(int tmp = 0; tmp < length; tmp++){
                    new_color = ((float)length - 1 - tmp)/(length - 1) * first_color + (float)tmp/(length - 1) * second_color;
                    int added_noise = ((float)length - 1 - tmp)/(length - 1) * rng.gaussian(first_noise) + (float)tmp/(length - 1) * rng.gaussian(second_noise);
                    for(int channel = 0; channel < 3; channel++){
                        new_color.val[channel] = std::min(std::max((int)(new_color.val[channel] + added_noise), 0), 255);
                    }
                    this->output_img.at<cv::Vec3b>(position.y + y, first_position + tmp + 1) = new_color;
                }
                x = k;
            }
        }
    }
}

cv::Mat filler::get_output(){
    return this->output_img;
}
