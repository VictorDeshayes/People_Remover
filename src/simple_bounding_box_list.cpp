#include "simple_bounding_box_list.h"

simple_bounding_box_list::simple_bounding_box_list(){
    this->bbs;
}

void simple_bounding_box_list::add_bb(int new_x, int new_y, int new_w, int new_h){
    simple_bounding_box new_bb(new_x, new_y, new_w, new_h);
    this->bbs.push_back(new_bb);
    this->empty = false;
}

simple_bounding_box simple_bounding_box_list::select_bb(int click_x, int click_y){
    std::list<simple_bounding_box>::iterator it;
    for (it = this->bbs.begin(); it != this->bbs.end(); ++it){
        if (click_x > (*it).get_x() && click_x < (*it).get_x() + (*it).get_w() && click_y > (*it).get_y() && click_y < (*it).get_y() + (*it).get_h()){
            simple_bounding_box out((*it).get_x(), (*it).get_y(), (*it).get_w(), (*it).get_h());
            this->bbs.erase(it);
            return out;
        }
    }
    return simple_bounding_box(-1, -1, -1, -1);
}

simple_bounding_box simple_bounding_box_list::first_bb(){
    if(!this->bbs.empty()){
        simple_bounding_box out((*this->bbs.begin()).get_x(), (*this->bbs.begin()).get_y(), (*this->bbs.begin()).get_w(), (*this->bbs.begin()).get_h());
        this->bbs.pop_front();
        return out;
    }
    return simple_bounding_box(-1, -1, -1, -1);
}

bool simple_bounding_box_list::draw(cv::Mat img){
    std::list<simple_bounding_box>::iterator it;
    for (it = this->bbs.begin(); it != this->bbs.end(); ++it){
        (*it).draw(img);
    }
}

void simple_bounding_box_list::print(){
    std::list<simple_bounding_box>::iterator it;
    for (it = this->bbs.begin(); it != this->bbs.end(); ++it){
        (*it).print();
    }
}
