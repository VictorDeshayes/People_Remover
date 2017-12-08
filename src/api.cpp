#include "api.h"

api::api(cv::Mat input_img){
    this->img = input_img.clone();
    this->first_pt.x = -1;
    this->first_pt.y = -1;
    this->new_bb = false;
}

void api::process_people_detection(){
    people_detector ppl_det;
    this->add_simple_bb_list(ppl_det.process(this->img));
}

void api::draw(){
    this->simple_bb.draw(this->display_img);
    this->selected_bb.draw(this->display_img);
    if (this->first_pt.x != -1){
        cv::line(this->display_img, this->first_pt, this->first_pt, cv::Scalar(0, 0, 255), 5);
    }
}

cv::Mat api::display(){
    if (this->reload_display){
        this->display_img = this->img.clone();
        this->draw();
        this->reload_display = false;
    }
    return this->display_img;
}

void api::print(){
    this->simple_bb.print();
    this->selected_bb.print();
}

void  api::add_simple_bb(int new_x, int new_y, int new_w, int new_h){
    this->simple_bb.add_bb(new_x, new_y, new_w, new_h);
}

void api::add_simple_bb_list(simple_bounding_box_list people_det){
    simple_bounding_box tmp_bb = people_det.first_bb();
    while (tmp_bb.get_x() != -1){
        this->simple_bb.add_bb(tmp_bb.get_x(), tmp_bb.get_y(), tmp_bb.get_w(),tmp_bb.get_h());
        tmp_bb = people_det.first_bb();
    }
}

bool api::select_bb(int click_x, int click_y){
    simple_bounding_box out = this->simple_bb.select_bb(click_x, click_y);
    if (out.get_x() == -1){
        return false;
    }
    else{
        this->selected_bb.set(out);
        return true;
    }    
}

bool api::select_first_bb(){
    simple_bounding_box out = this->simple_bb.first_bb();
    if (out.get_x() == -1){
        return false;
    }
    else{
        this->selected_bb.set(out);
        return true;
    }
}

bool api::input(int click_x, int click_y, int key){
    if (click_x != -1){
        this->reload_display = this->input_click(click_x, click_y);
        return true;
    }
    else if(key == global::enter){ // ENTER: remove the person(/thing) inside the selected bounding box
        if(this->selected_bb.already_set()){
            segmentation seg(this->img(cv::Rect(this->selected_bb.get_x(), this->selected_bb.get_y(), this->selected_bb.get_w(),this->selected_bb.get_h())));
            cv::Mat segmented = seg.process(false);
            filler f(this->img, segmented, cv::Rect(this->selected_bb.get_x(), this->selected_bb.get_y(), this->selected_bb.get_w(),this->selected_bb.get_h()));
            this->img = f.get_output().clone();
            this->selected_bb.clear();
            this->reload_display = true;
            return true;
        }
    }
    else if(key == global::a){ // A: remove all the people inside all bounding boxes
        if(this->selected_bb.already_set()){
            segmentation seg(this->img(cv::Rect(this->selected_bb.get_x(), this->selected_bb.get_y(), this->selected_bb.get_w(),this->selected_bb.get_h())));
            cv::Mat segmented = seg.process(true);
            filler f(this->img, segmented, cv::Rect(this->selected_bb.get_x(), this->selected_bb.get_y(), this->selected_bb.get_w(),this->selected_bb.get_h()));
            this->img = f.get_output().clone();
            this->selected_bb.clear();

        }
        while(api::select_first_bb()){
            segmentation seg(this->img(cv::Rect(this->selected_bb.get_x(), this->selected_bb.get_y(), this->selected_bb.get_w(),this->selected_bb.get_h())));
            cv::Mat segmented = seg.process(true);
            filler f(this->img, segmented, cv::Rect(this->selected_bb.get_x(), this->selected_bb.get_y(), this->selected_bb.get_w(),this->selected_bb.get_h()));
            this->img = f.get_output().clone();
            this->selected_bb.clear();
        }
        this->reload_display = true;
    }
    else if(key == global::esc){ // ESC: unselect the selected bounding box or remove the side selection on the selected bounding box
        if(this->selected_bb.already_set()){
            if(!this->selected_bb.remove_arrow()){
                this->simple_bb.add_bb(this->selected_bb.get_x(), this->selected_bb.get_y(), this->selected_bb.get_w(), this->selected_bb.get_h());
                this->selected_bb.clear();
            }
            this->reload_display = true;
            return true;
        }
        else if(this->new_bb){
            this->first_pt.x = -1;
            this->first_pt.y = -1;
            this->new_bb = false;
            this->reload_display = true;
            return true;
        }

    }
    else if(key == global::suppr && this->selected_bb.already_set() == true){ // SUPPR: delete the selected bounding box
        this->selected_bb.clear();
        this->reload_display = true;
        return true;
    }
    else if(key == global::left || key == global::up || key == global::right || key == global::down){ // ARROW: select a size on the selected bounding box (in order to move it)
        this->selected_bb.set_arrow(key);
        this->reload_display = true;
        return true;
    }
    else if(key == global::n){ // N: To create a new bounding box
        std::cout << "New bounding box" << std::endl;
        this->new_bb = true;
    }
    else if(key == global::q){ // Q
        return false;
    }
    else if(key == global::s){ // S: Save the current image (without the display of the bounding boxes)
        std::cout << "SAVE -> Name (with path): ";
        std::string path_save;
        std::cin >> path_save;
        cv::imwrite(path_save, this->img);
    }
    else if(key != -1){
        std::cout << "Key pressed: " <<key << std::endl;
    }
    return true;
}

bool api::input_click(int click_x, int click_y){
    if(this->new_bb){ // CASE NEW BB
        if(this->first_pt.x == -1){
            first_pt.x = click_x;
            first_pt.y = click_y;
            return true;
        }
        else if(first_pt.x != click_x && first_pt.y != click_y) {
            this->simple_bb.add_bb(std::min(first_pt.x, click_x), std::min(first_pt.y, click_y), std::abs(first_pt.x - click_x), std::abs(first_pt.y - click_y));
            this->first_pt.x = -1;
            this->first_pt.y = -1;
            this->new_bb = false;
            return true;
        }
    }
    else if(this->selected_bb.already_set()){ // CASE BB SELECTED
        if (this->selected_bb.selected_arrow()){
            this->selected_bb.move_arrow(click_x, click_y);
            return true;
        }
        else{
            this->simple_bb.add_bb(this->selected_bb.get_x(), this->selected_bb.get_y(), this->selected_bb.get_w(), this->selected_bb.get_h());
            this->selected_bb.clear();
            api::select_bb(click_x, click_y);
            return true;
        }
    }
    else{ // OTHER CASES
        return api::select_bb(click_x, click_y);
    }
}
