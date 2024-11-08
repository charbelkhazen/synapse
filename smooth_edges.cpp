#include <opencv2/opencv.hpp>
#include <algorithm>
#include <iostream>

using namespace cv;
using namespace std;

int main() {
    // Load image
    string path = "input.jpg";
    Mat img = imread(path);
    if(img.empty()) {
        cerr << "Error: Cannot load image at " << path << endl;
        return -1;
    }

    // Convert to LAB and apply CLAHE to L channel
    Mat lab;
    cvtColor(img, lab, COLOR_BGR2Lab);
    vector<Mat> channels;
    split(lab, channels);
    Ptr<CLAHE> clahe = createCLAHE();
    clahe->setClipLimit(2.0);
    clahe->apply(channels[0], channels[0]);
    merge(channels, lab);
    cvtColor(lab, img, COLOR_Lab2BGR);

    // Convert to HSV and create mask for paper color
    Mat hsv;
    cvtColor(img, hsv, COLOR_BGR2HSV);
    // Define paper color range (adjust values as needed)
    Scalar lower(0, 0, 200); // Light colors
    Scalar upper(180, 30, 255);
    Mat mask;
    inRange(hsv, lower, upper, mask); // Corrected: added 'mask' as the output

    // Morphological operations
    Mat kernel = getStructuringElement(MORPH_RECT, Size(5,5));
    morphologyEx(mask, mask, MORPH_CLOSE, kernel);

    // Find contours
    vector<vector<Point>> contours;
    findContours(mask, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    if(contours.empty()) {
        cerr << "No contours found." << endl;
        return -1;
    }

    // Largest contour
    auto largest = max_element(contours.begin(), contours.end(),
        [](const vector<Point>& a, const vector<Point>& b) {
            return contourArea(a) < contourArea(b);
        });

    // Crop and save
    Rect bbox = boundingRect(*largest);
    Mat cropped = img(bbox);
    if(!imwrite("cropped.jpg", cropped)) {
        cerr << "Failed to save cropped image." << endl;
        return -1;
    }

    cout << "Cropped image saved successfully." << endl;
    return 0;
}
