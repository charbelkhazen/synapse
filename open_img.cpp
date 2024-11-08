#include <functions.h>
#include <opencv2/opencv.hpp>
#include <string>

void displayImage(const std::string& imagePath) {
    // Load the image
    cv::Mat img = cv::imread(imagePath);
    if (img.empty()) {
        // Handle error
        return;
    }

    // Create a window
    cv::namedWindow("Image", cv::WINDOW_AUTOSIZE);
    cv::imshow("Image", img);

    // Wait for a mouse click
    cv::waitKey(0);

    // Destroy the window
    cv::destroyWindow("Image");
}


int main(){
    const char * a ; 
    std::cout <<"yalla" ;
    std::cin << a ;
    displayImage()
}