#include <opencv2\opencv.hpp>
#include <iostream>
#include <ctime>
using namespace cv;
using namespace std;

#define FILENAME "D:\\專題-影像處理\\飛機-影像\\Vertical_4k\\Vertical (2638).jpg"	//檔案路徑

int main(void) {
	clock_t start, stop;

	Mat img = imread(FILENAME, IMREAD_REDUCED_GRAYSCALE_4);//把圖檔以原圖的1/4讀進來並轉成灰階
	if (img.empty())
		return 1;

	start = clock();
	for (int x = 0; x < img.rows; x++) {
		for (int y = 0; y < img.cols - 1; y++) {
			int temp = (int)img.at<uchar>(x, y + 1) - img.at<uchar>(x, y);
			temp = (temp > 0) ? temp : -temp;
			if (temp > 24 && temp < 100) {
				vec.push_back(Point(x, y));		//將反白的pixel存到vector裡面
				img.at<uchar>(x, y) = 255;		//把pixel顏色相差過多的反白
			}
		}
	}

	stop = clock();

	cout << "\n\n執行時間 : " << double(stop - start) / CLOCKS_PER_SEC << " s" << endl;

	imshow("test", img);
	imwrite("test.jpg", img);
	waitKey(0);
	
	system("pause");
	return 0;
}