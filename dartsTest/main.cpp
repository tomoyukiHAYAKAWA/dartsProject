//
//  main.cpp
//  dartsTest
//
//  Created by Tomoyuki Hayakawa on 2018/11/24.
//  Copyright © 2018 Tomoyuki Hayakawa. All rights reserved.
//

#include <iostream>
#include <stdio.h>
//OpenCV用のヘッダファイル
#include <opencv2/opencv.hpp>

#define FILE_NAME "/Users/tomoyukihayakawa/Life_is_Tech!/movie01.mp4"

int main(int argc, const char * argv[])
{
    // 入力映像
    cv::VideoCapture inputVideo(FILE_NAME);
    
    // 動画切り出し用のフレーム
    cv::Mat frame;
    cv::Mat hsv_img;
    cv::Mat dst_img;
    cv::Mat result_img;
    
    // 座標記録配列
    std::vector<cv::Point> flightPoint;
    
    
    while(1)
    {
        // 動画から1フレーム取り出す
        inputVideo >> frame;
        
        if (frame.empty()) {
            break;
        }
        // 取り出したフレームから緑の領域を抽出
        // 色変換 HSV
        
        dst_img = cv::Mat(frame.size(), CV_8UC1);
        
        cv::cvtColor(frame, hsv_img, cv::COLOR_BGR2HSV);
        for (int y = 0; y < hsv_img.rows; y++) {
            for(int x = 0; x < hsv_img.cols; x++) {
                unsigned char hue = hsv_img.at<cv::Vec3b>(y, x)[0];
                unsigned char sat = hsv_img.at<cv::Vec3b>(y, x)[1];
                unsigned char val = hsv_img.at<cv::Vec3b>(y, x)[2];
                // 緑の領域に白色画素を入力
                if ((45 < hue && hue < 80) && 120 < sat && 100 < val) {
                    dst_img.at<unsigned char>(y, x) = 255;
                // それ以外の領域に黒色画素を入力
                } else {
                    dst_img.at<unsigned char>(y, x) = 0;
                }
            }
        }

        // ラベリング
        cv::Mat LabelImg;
        cv::Mat stats;
        cv::Mat centroids;
        
        int nLab = cv::connectedComponentsWithStats(dst_img, LabelImg, stats, centroids);
        
        
        
        //重心の出力
        for (int i = 1; i < nLab; ++i) {
            double *centerParam = centroids.ptr<double>(i);
            int *areaParam = stats.ptr<int>(i);
            
            int are = areaParam[cv::ConnectedComponentsTypes::CC_STAT_AREA];
            
            if (100 < are) {
                int x = static_cast<int>(centerParam[0]);
                int y = static_cast<int>(centerParam[1]);
                cv::Point pos;
                pos.x = x;
                pos.y = y;
                // 中心座標を配列に加える
                flightPoint.push_back(pos);
                // 描画
                cv::circle(frame, cv::Point(x, y), 3, cv::Scalar(0, 0, 255), -1);
            }
        }
        
        // 座標配列の長さ分だけりループ
        for (int i = 0; i < flightPoint.size(); i++) {
            cv::circle(frame, flightPoint[i], 3, cv::Scalar(0, 0, 255), -1);
        }
        
        //cv::imshow("frame", frame);
        //cv::waitKey(0);
        //cv::imshow("result", dst_img);
        
    }
   
    // 最終フレームに描画
    
    // 画像保存
    
    return 0;
}
