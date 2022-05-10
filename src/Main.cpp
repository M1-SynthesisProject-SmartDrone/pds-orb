
#include <loguru/loguru.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
using namespace cv;

const int WIDTH = 640;
const int HEIGHT = 480;

Mat createImgStraightLines()
{
    Mat image(HEIGHT, WIDTH, CV_8UC3, Scalar(255, 255, 255));
    line(image, Point(WIDTH / 2, 0), Point(WIDTH / 2, HEIGHT - 1), Scalar(0, 0, 0), 5);
    line(image, Point(WIDTH / 4, HEIGHT / 2), Point(WIDTH / 4, HEIGHT - 1), Scalar(0, 0, 255), 10);
    imwrite("./imgs_test/test_line_1.jpg", image);
    return image;
}


void FAST(Point ** fewer_kps,Mat img, float threshold=0.15, int nms_window=2)
{
    //Application du filtre gaussien 3*3
    Mat img_filtered;
    GaussianBlur(img,img_filtered,Size(3,3),0);

    Mat cross_idx=(Mat_<int>(2,4) << 3,0,-3,0,0,3,0,-3);
    Mat circle_idx=(Mat <int>(2,16)<<3,3,2,1,0,-1,-2,-3,-3,-3,-2,-1,0,1,2,3,0,1,2,3,3,3,2,1,0,-1,-2,-3,-3,-3,-2,-1);

    Mat corner_img=Mat::zeros(img.size()) //Attention si img pas mat 2d ne fonctionne pas donc à revoir
    vector<Point> keypoints;
    
    double pixel;
    for(int i=3;i<img.size(0)-3;i++)
    {
        for(int j=3;j<img.size(1)-3;j++)
        {
            pixel=img[i][j];
            if(threshold<1)
            {
                threshold=threshold*pixel;
            }

        }
    }
    std::cout<<kernel;
}

int main(int argc, char const *argv[])
{
    LOG_F(INFO, "Start orb test application");

    LOG_F(INFO, "Create image");
    auto img = createImgStraightLines();

    LOG_F(WARNING, "Display image on screen");
    imshow("The best window ever", img);
    waitKey(0);

    LOG_F(INFO, "End of the orb test application");
    std::cout<<"Bonjour tout le monde !.\n"; 
    return 0;
}
