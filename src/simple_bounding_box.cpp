#include "simple_bounding_box.h"

simple_bounding_box::simple_bounding_box(int new_x, int new_y, int new_w, int new_h){
    this->x = new_x;
    this->y = new_y;
    this->w = new_w;
    this->h = new_h;
}

void simple_bounding_box::draw(cv::Mat img){
    cv::rectangle(img, cv::Point(this->x, this->y), cv::Point(this->x + this->w, this->y + this->h), cv::Scalar(0, 0, 255), 2);
}

void simple_bounding_box::print(){std::cout << "Simple: " << this->x << " " << this->y << " " << this->w << " " << this->h << std::endl;}

int simple_bounding_box::get_x(){return this->x;}
int simple_bounding_box::get_y(){return this->y;}
int simple_bounding_box::get_w(){return this->w;}
int simple_bounding_box::get_h(){return this->h;}
