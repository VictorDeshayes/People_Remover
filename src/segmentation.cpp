#include "segmentation.h"

segmentation::segmentation(cv::Mat input_crop_img){
    this->crop_img = input_crop_img;
    this->transformed_img = cv::Mat::zeros(this->crop_img.rows, this->crop_img.cols, CV_8UC1);
}

cv::Mat segmentation::process(bool auto_seg){
    if(global::segmentation == true){
        return segmentation::watershed(auto_seg);
    }
    else{
        return segmentation::full_rect();
    }
}

void watershedCallBackFunc(int event, int x, int y, int flags, void* userdata){
     if  (event == cv::EVENT_LBUTTONDOWN){
          global::click_x = x;
          global::click_y = y;
     }
}

cv::Mat segmentation::watershed(bool auto_seg){
    cv::Mat src, grad, bw, dist, dist_8u, dst;
    src = this->crop_img.clone();
    cv::blur(src, src, cv::Size(5, 5), cv::Point(-1,-1));
    cv::blur(src, src, cv::Size(5, 5), cv::Point(-1,-1));

    cv::Laplacian(src, grad, CV_32F, 3);
    grad.convertTo(grad, CV_8UC3);
    cv::cvtColor(grad, grad, CV_BGR2GRAY );
    cv::threshold(grad, bw, 40, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
    cv::bitwise_not(bw, bw);
    cv::distanceTransform(bw, dist, CV_DIST_L2, 5); // Perform the distance transform algorithm
    cv::normalize(dist, dist, 0, 1., cv::NORM_MINMAX);
    cv::threshold(dist, dist, .4, 1., CV_THRESH_BINARY); // The markers for the foreground objects
    cv::Mat kernel = cv::Mat::ones(3, 3, CV_8UC1);
    cv::dilate(dist, dist, kernel);
    dist.convertTo(dist_8u, CV_8U);
    std::vector<std::vector<cv::Point> > contours;
    cv::findContours(dist_8u, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
    cv::Mat markers = cv::Mat::zeros(dist.size(), CV_32SC1); // Create the marker image for the watershed algorithm
    for (size_t i = 0; i < contours.size(); i++){
        cv::drawContours(markers, contours, static_cast<int>(i), cv::Scalar::all(static_cast<int>(i)+1), -1);
    }
    cv::watershed(src, markers);
    cv::Mat mark = cv::Mat::zeros(markers.size(), CV_8UC1);
    markers.convertTo(mark, CV_8UC1);
    cv::bitwise_not(mark, mark);
    std::vector<cv::Vec3b> colors;
    for (size_t i = 0; i < contours.size(); i++){ // Random color generator: mainly not blue
        int b = cv::theRNG().uniform(0, 128);
        int g = cv::theRNG().uniform(0, 255);
        int r = cv::theRNG().uniform(0, 255);
        colors.push_back(cv::Vec3b((uchar)b, (uchar)g, (uchar)r));
    }
    dst = this->crop_img.clone();
    int lenght = contours.size();
    bool selected_indexes[lenght+1] = {};
    for (int i = 1./6 * markers.rows; i <= 1./2 * markers.rows; i++){
        for (int j = 1./3 * markers.cols; j <= 2./3 * markers.cols; j++){
            int index = markers.at<int>(i, j);
            selected_indexes[index] = true;
        }
    }
    bool reload = true;
    global::clear_click();
    cv::namedWindow("Watershed", CV_WINDOW_NORMAL);
    cv::resizeWindow("Watershed", dst.cols*1.6, dst.rows*1.6);
    cv::setMouseCallback("Watershed", watershedCallBackFunc, NULL);
    int key = -1;
    while(key != global::enter && auto_seg == false){
        if(reload == true){
            dst = this->crop_img.clone();
            for (int i = 0; i < markers.rows; i++){
                for (int j = 0; j < markers.cols; j++){
                    int index = markers.at<int>(i, j);
                    if (index > 0 && index <= static_cast<int>(contours.size())){
                        if(selected_indexes[index] == true){
                            dst.at<cv::Vec3b>(i, j) = 0.3 * dst.at<cv::Vec3b>(i, j) + 0.7 * cv::Vec3b(255, 0, 0);
                        }
                        else{
                            dst.at<cv::Vec3b>(i, j) = 0.7 * dst.at<cv::Vec3b>(i, j) + 0.3 * colors[index-1];
                        }
                    }
                    else{
                        dst.at<cv::Vec3b>(i, j) = cv::Vec3b(0, 0, 255);
                    }
                }
            }
            reload = false;
        }
        cv::imshow("Watershed", dst);
        if (global::click_x != -1){
            reload = true;
            int index = markers.at<int>(global::click_y, global::click_x);
            selected_indexes[index] = !selected_indexes[index];

            global::clear_click();
        }
        key = cv::waitKey(10);
    }
    cv::destroyWindow("Watershed");
    global::clear_click();
    for(int i = 0; i < this->transformed_img.rows; i++){
        for(int j = 0; j < this->transformed_img.cols; j++){
            int index = markers.at<int>(i, j);
            if(selected_indexes[index]){
                this->transformed_img.at<uchar>(i, j) = 0;
            }
            else{
                this->transformed_img.at<uchar>(i, j) = 255;
            }
        }
    }
    cv::erode(this->transformed_img, this->transformed_img, kernel);
    return this->transformed_img;
}

cv::Mat segmentation::active_snake(){
    cv::Mat src = this->crop_img.clone();
    cv::cvtColor(src, src, CV_BGR2GRAY );
    cv::blur(src, src, cv::Size(5, 5), cv::Point(-1,-1));
    cv::blur(src, src, cv::Size(5, 5), cv::Point(-1,-1));
    cv::Laplacian(src, src, CV_8UC3, 3);
    cv::imshow( "Gradient", src );
    cv::waitKey();

//    cv::Mat imgLaplacian;
//    cv::Mat sharp = src.clone(); // copy source image to another temporary one

//    cv::blur(sharp, sharp, cv::Size(5, 5), cv::Point(-1,-1));
//    cv::blur(sharp, sharp, cv::Size(5, 5), cv::Point(-1,-1));

//    cv::Laplacian(sharp, imgLaplacian, CV_8UC3, 3);
//    cv::Mat tmp = sharp.clone();
//    src.convertTo(sharp, CV_32F);
//    cv::Mat imgResult;
//    //imgResult = sharp - imgLaplacian;



//    //cv::Laplacian(tmp, tmp, CV_8UC3, 3);
//    //cv::cvtColor(tmp, tmp, CV_BGR2GRAY );

//    // convert back to 8bits gray scale
//    imgResult.convertTo(imgResult, CV_8UC3);
//    imgResult = tmp.clone();
//    //imgLaplacian.convertTo(imgLaplacian, CV_8UC3);
//    // imshow( "Laplace Filtered Image", imgLaplacian );
//    cv::imshow( "New Sharped Image", imgLaplacian );
//    cv::waitKey();
    segmentation::full_rect();

    return this->transformed_img;
}

cv::Mat segmentation::full_rect(){
    for(int i = 0; i < this->transformed_img.rows; i++){
        for(int j = 0; j < this->transformed_img.cols; j++){
            this->transformed_img.at<uchar>(i, j) = 0;
        }
    }
    return this->transformed_img;
}
