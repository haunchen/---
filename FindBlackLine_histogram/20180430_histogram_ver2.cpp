#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
using namespace std;
using namespace cv;

#define FILENAME "C:\\Users\\haunchen\\Google 雲端硬碟\\專題-影像處理\\飛機-影像\\Vertical_1080p\\Vertical (833).jpg"

int main() {
	clock_t start, stop;
	
	start = clock();
	
	Mat img = imread(FILENAME, IMREAD_GRAYSCALE);
	resize(img, img, Size(640, 480));
	uchar hist[12][16][256] = { 0 };

	for (int x = 0; x < img.rows; x++)
		for (int y = 0; y < img.cols; y++)
			hist[x / 40][y / 40][img.at<uchar>(x, y)]++;

	stop = clock();
	cout << double(stop - start) / CLOCKS_PER_SEC << endl;

	system("pause");
	return 0;
}
