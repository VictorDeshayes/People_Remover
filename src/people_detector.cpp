#include "people_detector.h"

people_detector::people_detector(){

}

simple_bounding_box_list people_detector::process(cv::Mat img){
    FILE *tmp;
    std::string path, path_cfg, path_weights, path_names;
    path = "yolo.cfg";
    if (tmp = fopen(path.c_str(), "r")){
        fclose(tmp);
    }
    else{
        while(!(tmp = fopen(path.c_str(), "r"))){
            std::cout << path.c_str() << " not found, please enter the complete path: " << std::endl;
            std::cin >> path;
        }
    }
    path_cfg = path;
    path = "yolo.weights";
    if (tmp = fopen(path.c_str(), "r")){
        fclose(tmp);
    }
    else{
        while(!(tmp = fopen(path.c_str(), "r"))){
            std::cout << path.c_str() << " not found, please enter the complete path: " << std::endl;
            std::cin >> path;
        }
    }
    path_weights = path;
    path = "coco.names";
    if (tmp = fopen(path.c_str(), "r")){
        fclose(tmp);
    }
    else{
        while(!(tmp = fopen(path.c_str(), "r"))){
            std::cout << path.c_str() << " not found, please enter the complete path: " << std::endl;
            std::cin >> path;
        }
    }
    path_names = path;
    std::cout << "People detection initialization" << std::endl;

    const size_t network_width = 416;
    const size_t network_height = 416;
    simple_bounding_box_list output;

    //! [Initialize network]
    cv::dnn::Net net = cv::dnn::readNetFromDarknet(path_cfg, path_weights);

    if (net.empty()){
        std::cout << "Can't load network by using the following files: " << std::endl;
        std::cout << "cfg-file:     " << path_cfg << std::endl;
        std::cout << "weights-file: " << path_weights << std::endl;
        return output;
    }

    std::vector<std::string> classNamesVec;
    std::ifstream classNamesFile(path_names.c_str());
    if (classNamesFile.is_open()){
        std::string className = "";
        while (classNamesFile >> className)
            classNamesVec.push_back(className);
    }

    //! [Resizing without keeping aspect ratio]
    cv::Mat resized;
    cv::resize(img, resized, cv::Size(network_width, network_height));

    //! [Prepare blob]
    cv::Mat inputBlob = cv::dnn::blobFromImage(resized, 1 / 255.F); //Convert Mat to batch of images

    //! [Set input blob]
    net.setInput(inputBlob, "data");                   //set the network input

    //! [Make forward pass]
    cv::Mat detectionMat = net.forward("detection_out");   //compute output
    float confidenceThreshold = 0.24;
    for (int i = 0; i < detectionMat.rows; i++){
        const int probability_index = 5;
        const int probability_size = detectionMat.cols - probability_index;
        float *prob_array_ptr = &detectionMat.at<float>(i, probability_index);
        size_t objectClass = std::max_element(prob_array_ptr, prob_array_ptr + probability_size) - prob_array_ptr;
        float confidence = detectionMat.at<float>(i, (int)objectClass + probability_index);
        if (confidence > confidenceThreshold && objectClass == 0){ // Only keeps people detection
            float x = detectionMat.at<float>(i, 0);
            float y = detectionMat.at<float>(i, 1);
            float width = detectionMat.at<float>(i, 2);
            float height = detectionMat.at<float>(i, 3);
            int xLeftBottom = std::max(static_cast<int>((x - width / 2) * img.cols) - 10, 0);
            int yLeftBottom = std::max(static_cast<int>((y - height / 2) * img.rows) - 10, 0);
            int xRightTop = std::min(static_cast<int>((x + width / 2) * img.cols) + 10, global::width);
            int yRightTop = std::min(static_cast<int>((y + height / 2) * img.rows) + 10, global::height);
            output.add_bb(xLeftBottom, yLeftBottom, xRightTop - xLeftBottom, yRightTop - yLeftBottom);
        }
    }
    std::cout << "People detection done" << std::endl;
    return output;
}
