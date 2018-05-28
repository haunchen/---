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
	vector<vector<uchar>> vec;
	for (int x = 0, count = 1; x < img.rows; x += 40) {
		for (int y = 0; y < img.cols; y += 40, count++) {
			vector<uchar> temp(256);
			for (int i = 0; i < 40; i++)
				for (int j = 0; j < 40; j++)
					temp[img.at<uchar>(x + i, y + j)]++;
			vec.push_back(temp);
		}
	}

	stop = clock();
	cout << double(stop - start) / CLOCKS_PER_SEC << endl;

	system("pause");
	return 0;
}
