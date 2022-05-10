
#include <loguru/loguru.hpp>
#include <opencv2/opencv.hpp>

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

int main(int argc, char const *argv[])
{
    LOG_F(INFO, "Start orb test application");

    LOG_F(INFO, "Create image");
    auto img = createImgStraightLines();

    LOG_F(WARNING, "Display image on screen");
    imshow("The best window ever", img);
    waitKey(0);

    LOG_F(INFO, "End of the orb test application");
    return 0;
}
