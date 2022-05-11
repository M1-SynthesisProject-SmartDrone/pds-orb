
#include <loguru/loguru.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>

using namespace cv;
using namespace std;

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

//N : nombre de pixels regardés pour savoir si le pixel p est un point clefs

// void FAST(Point ** fewer_kps,Mat img,int N=9, float threshold=0.15, int nms_window=2)
// {
//     //Application du filtre gaussien 3*3
//     Mat img_filtered;
//     GaussianBlur(img,img_filtered,Size(3,3),0);

//     Mat cross_idx=(Mat_<int>(2,4) << 3,0,-3,0,0,3,0,-3);
//     Mat circle_idx=(Mat_<int>(2,16)<<3,3,2,1,0,-1,-2,-3,-3,-3,-2,-1,0,1,2,3,0,1,2,3,3,3,2,1,0,-1,-2,-3,-3,-3,-2,-1);

//     Mat corner_img=Mat::zeros(img.size(),CV_64FC1); //Attention si img pas mat 2d ne fonctionne pas donc à revoir
//     vector<Point> keypoints;

//     for(int x=3;i<img.size[0]-3;i++)
//     {
//         for(int y=3;j<img.size[1]-3;j++)
//         {
//             auto intensity=img[x][y];
//             if(threshold<1)
//             {
//                 threshold=threshold*pixel;
//             }
//             int darker_cpt=0;
//             int brighter_cpt=0;
//             //Vérification des pixels 1,5,9,13
//             for(int pos_circle=0;pos_circle<cross_idx.size[1];pos_circle++)
//             {
//                 if(img[x+cross_idx[0,pos_circle],y+cross_idx[1,pos_circle]] > (intensity+threshold)) 
//                 {
//                    darker_cpt++;
//                 }
//                 if(img[x+cross_idx[0,pos_circle],y+cross_idx[1,pos_circle]] < (intensity-threshold))
//                 {
//                     brighter_cpt++;
//                 }
//             }
//             if(darker_cpt>=3 || brighter_cpt>=3)
//             {
//                 darker_cpt=0;
//                 brighter_cpt=0;
//                 //Si c'est condition est respectée, alors on peut vérifier pour les autres points
//                 for(int pos_circle=0;pos_circle<circle_idx.size[1];pos_circle++)
//                 {
//                     if(img[x+circle_idx[0,pos_circle],y+circle_idx[1,pos_circle]] > (intensity+threshold)) 
//                     {
//                         darker_cpt++;
//                     } 
//                     else
//                     {
//                         darker_cpt=0;
//                     }
//                     if(img[x+circle_idx[0,pos_circle],y+circle_idx[1,pos_circle]] < (intensity-threshold)) 
//                     {
//                         brighter_cpt++;
//                     } 
//                     else
//                     {
//                         brighter_cpt=0;
//                     }
//                     if(brighter_cpt>=N || darker_cpt>=N)
//                     {
//                         break;
//                     }
//                 }
//                 //On ajoute le point clef si il y a plus de N pixels d'affilés qui sont soient plus foncés soient plus claires que p
//                 if(darker_cpt>=N || brighter_cpt>=N)
//                 {
//                     Point pt;
//                     pt.x=y;
//                     pt.y=x;
//                     keypoints.push_back(pt); //Keypoint [y,x]=[col,row]
//                     corner_img[x,y]=abs(intensity-img[x+circle_idx[0],y+circle_idx[1]]).sum();
//                 }
//             }

            
//         }
//     }
// }

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

    Mat cross_idx=(Mat_<int>(2,4) << 3,0,-3,0,0,3,0,-3);
    Mat circle_idx=(Mat_<int>(2,16)<<3,3,2,1,0,-1,-2,-3,-3,-3,-2,-1,0,1,2,3,0,1,2,3,3,3,2,1,0,-1,-2,-3,-3,-3,-2,-1);

    std::cout<<cross_idx.size()<<"\n";
    std::cout<<cross_idx.size[0]<<"\n";
    std::cout<<cross_idx.rows<<"\n";

    for(int i=0;i<cross_idx.rows;i++)
    {
        
        const int *pi=cross_idx.ptr<int>(i);
        for(int j=0;j<cross_idx.cols;j++)
        {
            cout<<pi[j]<<"\n";
        }
    }
    for(int j=0;j<cross_idx.cols;j++)
        {
        cout<<cross_idx.at<int>(0,j)<<","<<cross_idx.at<int>(1,j)<<"\n";
        }
    return 0;
}
