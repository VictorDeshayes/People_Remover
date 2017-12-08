# People_Remover

Simple C++ application for removing people in an image.

## Getting Started

### Prerequisites

OpenCV 3.3.1

### Setup

Download yolo.cfg, yolo.weights and coco.names: https://pjreddie.com/darknet/yolo/.
It's easier to have those 3 files next to the executable. If it is not the case, you will have to give the path to them each time.

## Running

### Input

The only input is an image, it can be the first argument:
```
./People_Remover ../data/img_test1.jpg
```

Or directly when the application is running:
```
./People_Remover
Enter the path to the image: 
../data/img_test1.jpg
```
### People Detection

If the 3 Yolo files are found, the people detection is done automatically.

### Bounding Box Manipulation

At that point the image is displayed with some bounding boxes around the people in the image.
You can select a bounding box by clicking on it.
You can delete the selected bounding box by pressing **suppr**.
You can modify a bounding box side by first pressing the corresponding **arrow** on the keyboard and then clicking on the image where that side should be.
You can add a new bounding box by pressing **n** and then clicking two of its opposite corner.

### Segmentation

Once a bounding box is selected and ready to process the person removal press **enter** and a new image will appear.
Blue zones are the selected ones that will be removed.
By clicking inside a zone you add/remove it from the selected zone.
Once it is done, press **enter** again.

### Other options

At any time you can save the current image (without the bounding boxes) by pressing **s** and giving the full name for your image.
Key configuration can be changed in the "global.cpp" file


