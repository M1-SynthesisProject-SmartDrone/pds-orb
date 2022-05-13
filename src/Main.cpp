
#include <loguru/loguru.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>

using namespace cv;
using namespace std;

const int WIDTH = 640;
const int HEIGHT = 480;

// Convert RGB image to grayscale using the luminosity method
void RGBtoGrayScale(Mat rgb, Mat *grayscale)
{
    std::vector<Mat> channels(3);
    split(rgb, channels);
    *grayscale = (0.07 * channels[0] + 0.72 * channels[1] + 0.21 * channels[2]);
}

Mat createImgStraightLines()
{
    Mat image(HEIGHT, WIDTH, CV_8UC3, Scalar(255, 255, 255));
    line(image, Point(WIDTH / 2, 0), Point(WIDTH / 2, HEIGHT - 1), Scalar(0, 0, 0), 5);
    line(image, Point(WIDTH / 4, HEIGHT / 2), Point(WIDTH / 4, HEIGHT - 1), Scalar(0, 0, 255), 10);
    imwrite("./imgs_test/test_line_1.jpg", image);
    return image;
}

// N : nombre de pixels regardés pour savoir si le pixel p est un point clefs vector<Point> * fewer_kps

vector<Point> myFast(Mat &img, int N = 9, float threshold = 0.15, int nms_window = 2)
{
    // Application du filtre gaussien 3*3
    Mat img_filtered;
    GaussianBlur(img, img_filtered, Size(3, 3), 0);

    Mat cross_idx = (Mat_<int>(2, 4) << 3, 0, -3, 0, 0, 3, 0, -3);
    // On double les coordonnées des pixels du cercle afin de pouvoir faire un peu plus d'un tour dans le cas ou les pixels qui se suivent
    // commencent à la fin du cercle et se poursuivent au début
    Mat circle_idx = (Mat_<int>(2, 32) << 3, 3, 2, 1, 0, -1, -2, -3, -3, -3, -2, -1, 0, 1, 2, 3, 3, 3, 2, 1, 0, -1, -2, -3, -3, -3, -2, -1, 0, 1, 2, 3,
                      0, 1, 2, 3, 3, 3, 2, 1, 0, -1, -2, -3, -3, -3, -2, -1, 0, 1, 2, 3, 3, 3, 2, 1, 0, -1, -2, -3, -3, -3, -2, -1);

    Mat corner_img = Mat::zeros(img.size(), CV_8UC1); // Attention si img pas mat 2d ne fonctionne pas donc à revoir
    vector<Point> keypoints;

    for (int x = 3; x < img.rows - 3; x++)
    {
        const uchar *ptr_img = img.ptr<uchar>(x);

        for (int y = 3; y < img.cols - 3; y++)
        {
            auto intensity_pixel = ptr_img[y];
            if (threshold < 1)
            {
                threshold = threshold * intensity_pixel;
            }
            
            int darker_cpt = 0;
            int brighter_cpt = 0;
            // Vérification des pixels 1,5,9,13
            // ATTENTION plus fonce -> valeur pixel plus grande donc en vrai plus clair
            for (int pos_circle = 0; pos_circle < cross_idx.cols; pos_circle++)
            {
                const uchar *ptr_img_cross = img.ptr<uchar>(x + cross_idx.at<int>(0, pos_circle));
                auto pixel = ptr_img_cross[y + cross_idx.at<int>(1, pos_circle)];
                if (ptr_img_cross[y + cross_idx.at<int>(1, pos_circle)] > (intensity_pixel + threshold))
                {
                    darker_cpt++;
                }
                if (ptr_img_cross[y + cross_idx.at<int>(1, pos_circle)] < (intensity_pixel - threshold))
                {
                    brighter_cpt++;
                }
            }
            if (darker_cpt >= 3 || brighter_cpt >= 3)
            {
                darker_cpt = 0;
                brighter_cpt = 0;
                // Si c'est condition est respectée, alors on peut vérifier pour les autres points
                int nbIterCircle = circle_idx.cols / 2 + N;
                for (int pos_circle = 0; pos_circle < nbIterCircle; pos_circle++)
                {
                    const uchar *ptr_img_circle = img.ptr<uchar>(x + circle_idx.at<int>(0, pos_circle));

                    if (ptr_img_circle[y + circle_idx.at<int>(1, pos_circle)] >= (intensity_pixel + threshold))
                    {
                        darker_cpt++;
                    }
                    else
                    {
                        darker_cpt = 0;
                    }
                    if (ptr_img_circle[y + circle_idx.at<int>(1, pos_circle)] <= (intensity_pixel - threshold))
                    {
                        brighter_cpt++;
                    }
                    else
                    {
                        brighter_cpt = 0;
                    }
                    if (brighter_cpt >= N || darker_cpt >= N)
                    {
                        break;
                    }
                }
                // On ajoute le point clef si il y a plus de N pixels d'affilés qui sont soient plus foncés soient plus claires que p
                if (darker_cpt >= N || brighter_cpt >= N)
                {
                    Point pt;
                    pt.x = y;
                    pt.y = x;
                    keypoints.push_back(pt); // Keypoint [y,x]=[col,row]
                    int nbIterCircle = circle_idx.cols / 2;
                    for (int j = 0; j < nbIterCircle; j++)
                    {
                        corner_img.at<uchar>(x, y) += abs(intensity_pixel - img.at<uchar>(x + circle_idx.at<int>(0, j), y + circle_idx.at<int>(1, j)));
                    }
                }
            }
        }
    }

    // Phase NMS - Non Maximal Suppresion -- A DEBUGGER 
    vector<Point> fewer_kps;
    if (nms_window != 0)
    {
        for (Point &point : keypoints)
        {
            int initValueX = point.x - nms_window;
            int finalValueX = point.x + nms_window + 1;
            int initValueY = point.y - nms_window;
            int finalValueY = point.y + nms_window + 1;
            // Dans le pire des cas, tous les pixels sont des points d'interets
            Mat window = Mat::zeros(Size(2 * nms_window, 2 * nms_window), CV_8UC1);
            for (int i = initValueX; i < finalValueX; i++)
            {
                const uchar *ptr_corner = corner_img.ptr<uchar>(i);
                for (int j = initValueY; j < finalValueY; j++)
                {
                    window.at<uchar>(i, j) = ptr_corner[j];
                }
            }
            double min, max;
            Point minLoc, maxLoc;
            minMaxLoc(window, &min, &max, &minLoc, &maxLoc);
            // Attention les index renvoyés dans minLoc et maxLoc sont inversés par rapport aux index de la matrice de base
            auto x_new = point.x + maxLoc.y - nms_window;
            auto y_new = point.y + maxLoc.x - nms_window;
            Point new_kp = Point(x_new, y_new);
            auto it = find(fewer_kps.begin(), fewer_kps.end(), new_kp);
            if (it == fewer_kps.end())
            {
                fewer_kps.push_back(new_kp);
            }
        }
    }
    else
    {
        fewer_kps = keypoints;
    }
    return fewer_kps;
}

int main(int argc, char *argv[])
{
    loguru::init(argc, argv);
    LOG_F(INFO, "Start orb test application");

    LOG_F(INFO, "Create image");
    //auto img = createImgStraightLines();
    Mat img = imread("ref1.png", IMREAD_COLOR);

    LOG_F(WARNING, "Display image on screen");
    imshow("The best window ever", img);
    waitKey(0);
    
    LOG_F(INFO, "End of the orb test application");
    std::cout << "Bonjour tout le monde !.\n";

    Vec3b color{255, 0, 0};
    img.at<Vec3b>(Point(3, 3)) = color;

    Mat grayscale = Mat::zeros(Size(img.rows, img.cols), CV_8UC1);
    cvtColor(img, grayscale, COLOR_BGR2GRAY);
    imshow("The best window ever", grayscale);
    waitKey(0);

    // TEST FCT MINMAXLOC
    /*
    Mat cross_idx = (Mat_<int>(2, 4) << 5, 0, -3, 0, 6, 3, 0, -3);
    cout << cross_idx << "\n";
    cout << cross_idx.at<int>(0,1) << "\n";
    double min, max;
    Point minLoc, maxLoc;
    minMaxLoc(cross_idx, &min, &max, &minLoc, &maxLoc);
    cout << "min" << min << "max : " << max << "minloc :" << minLoc << " maxloc :" << maxLoc;
    */

    vector<Point> KeyPoint = myFast(grayscale, 12);

    LOG_F(INFO, "fini myfast \n");
    cout << KeyPoint;

    for (Point &point : KeyPoint)
    {
        circle(img,point,3,color);
        //img.at<Vec3b>(point) = color;
    }
    imshow("The best window ever", img);
    waitKey(0);

    return 0;
}
