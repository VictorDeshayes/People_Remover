#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/dnn.hpp>
#include <opencv2/dnn/shape_utils.hpp>

#include <fstream>
#include <iostream>
#include <algorithm>
#include <cstdlib>

#include "api.h"
#include "global.h"


void CallBackFunc(int event, int x, int y, int flags, void* userdata){
     if  (event == cv::EVENT_LBUTTONDOWN){
          global::click_x = x;
          global::click_y = y;
     }
}

int main(int argc, char** argv){    
    // LOAD IMAGE -------------------------------------------------------------------
    std::string path_to_img;
    cv::Mat img;
    if (argc > 1){
        path_to_img = argv[1];
    }
    else{
        std::cout << "Enter the path to the image: ";
        std::cin >> path_to_img;
        img = cv::imread(path_to_img);
    }
    img = cv::imread(path_to_img);
    while (img.empty()){
        std::cout << "Error loading image: " << path_to_img << std::endl;
        std::cout << "Enter the path to the image: ";
        std::cin >> path_to_img;
        img = cv::imread(path_to_img);
    }
    global::width = img.size().width;
    global::height = img.size().height;
    // ------------------------------------------------------------------------------

    int key = -1;
    api people_remover(img);

    // PROCESS THE PEOPLE DETECTION -------------------------------------------------
    people_remover.process_people_detection();
    // ------------------------------------------------------------------------------

    // DISPLAY AND INTERACT TO REMOVE PEOPLE ----------------------------------------
    cv::namedWindow("Display");
    while(people_remover.input(global::click_x, global::click_y, key)){
        key = -1;
        global::clear_click();
        cv::imshow("Display", people_remover.display());
        cv::setMouseCallback("Display", CallBackFunc, NULL);
        key = cv::waitKey(10);
    }
    cv::destroyAllWindows();
    // ------------------------------------------------------------------------------
    return 0;
}

