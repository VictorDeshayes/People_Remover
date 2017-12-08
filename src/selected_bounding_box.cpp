#include "selected_bounding_box.h"

selected_bounding_box::selected_bounding_box(){
    this->x = -1;
    this->y = -1;
    this->w = -1;
    this->h = -1;
    this->arrow = -1;
    this->is_set = false;
}

void selected_bounding_box::set(simple_bounding_box selected){
    this->x = selected.get_x();
    this->y = selected.get_y();
    this->w = selected.get_w();
    this->h = selected.get_h();
    this->is_set = true;
}

void selected_bounding_box::set_arrow(int key){
    this->arrow = key;
}

bool selected_bounding_box::selected_arrow(){
    if (this->arrow != -1){
        return true;
    }
    else{
        return false;
    }
}

void selected_bounding_box::move_arrow(int click_x, int click_y){
    if (this->arrow == global::left)
        this->move_left(click_x);
    if (this->arrow == global::up)
        this->move_up(click_y);
    if (this->arrow == global::right)
        this->move_right(click_x);
    if (this->arrow == global::down)
        this->move_down(click_y);
    if (this->w == 0 || this->h == 0){
        this->clear();
        return;
    }
    if (this->w < 0){
        this->swap_x();
    }
    if (this->h < 0){
        this->swap_y();
    }
}

void selected_bounding_box::move_left(int click_x){
    this->w = this->w + this->x - click_x;
    this->x = click_x;
}

void selected_bounding_box::move_up(int click_y){
    this->h = this->h + this->y - click_y;
    this->y = click_y;
}

void selected_bounding_box::move_right(int click_x){
    this->w = click_x - this->x;
}

void selected_bounding_box::move_down(int click_y){
    this->h = click_y - this->y;
}

void selected_bounding_box::swap_x(){
    this->x = this->x + this->w;
    this->w = -this->w;
    this->swap_arrow();
}

void selected_bounding_box::swap_y(){
    this->y = this->y + this->h;
    this->h = -this->h;
    this->swap_arrow();
}

void selected_bounding_box::swap_arrow(){
    if (this->arrow == global::left)
        this->arrow = global::right;
    else if (this->arrow == global::up)
        this->arrow = global::down;
    else if (this->arrow == global::right)
        this->arrow = global::left;
    else if (this->arrow == global::down)
        this->arrow = global::up;
}

bool selected_bounding_box::remove_arrow(){
    if (this->arrow != -1){
        this->arrow = -1;
        return true;
    }
    else{
        return false;
    }
}

void selected_bounding_box::clear(){
    this->x = -1;
    this->y = -1;
    this->w = -1;
    this->h = -1;
    this->arrow = -1;
    this->is_set = false;
}

bool selected_bounding_box::already_set(){
    return this->is_set;
}

void selected_bounding_box::draw(cv::Mat img){
    cv::rectangle(img, cv::Point(this->x, this->y), cv::Point(this->x + this->w, this->y + this->h), cv::Scalar(0, 255, 0), 2);
    if (this->selected_arrow() == true){
        cv::line(img, this->pt1(), this->pt2(), cv::Scalar(0, 255, 255), 2);
    }
}

cv::Point selected_bounding_box::pt1(){
    if (this->arrow == 81 || this->arrow == 82)
        return cv::Point(this->x, this->y);
    else
        return cv::Point(this->x + this->w, this->y + this->h);
}

cv::Point selected_bounding_box::pt2(){
    if (this->arrow == 82 || this->arrow == 83)
        return cv::Point(this->x + this->w, this->y);
    else
        return cv::Point(this->x, this->y + this->h);
}

void selected_bounding_box::print(){std::cout << "Selected: " << this->x << " " << this->y << " " << this->w << " " << this->h << std::endl;}

int selected_bounding_box::get_x(){return this->x;}
int selected_bounding_box::get_y(){return this->y;}
int selected_bounding_box::get_w(){return this->w;}
int selected_bounding_box::get_h(){return this->h;}

