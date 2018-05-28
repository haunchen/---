#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
using namespace std;
using namespace cv;

#define FILENAME "C:\\Users\\haunchen\\Google 雲端硬碟\\專題-影像處理\\飛機-影像\\Vertical_1080p\\Vertical (833).jpg"

void drawHistImg(const Mat &src, Mat &dst) {
	int histSize = 256;
	float histMaxValue = 0;
	for (int i = 0; i < histSize; i++) {
		float tempValue = src.at<float>(i);
		if (histMaxValue < tempValue)
			histMaxValue = tempValue;
	}

	float scale = (0.9 * 256) / histMaxValue;
	for (int i = 0; i < histSize; i++) {
		int intensity = static_cast<int>(src.at<float>(i)*scale);
		line(dst, Point(i, 255), Point(i, 255 - intensity), Scalar(0));
	}
}
int main() {
	clock_t start, stop;
	int hist[12][16][256] = { 0 };
	uchar max[12][16] = { 0 }, min[12][16] = { 255 };		//save per block max and min
	double avg[12][16][7] = { 0.0 };		//avg[0] min&total avg/2, avg[1] total avg, avg[2] max&total avg/2
	int total[12][16][4] = { 0 };			//total of the four areas
	int count[12][16][4] = { 0 };			//count of the four areas

	start = clock();
	Mat img = imread(FILENAME, IMREAD_GRAYSCALE);
	resize(img, img, Size(640, 480));

	for (int x = 0; x < img.rows; x++) {
		uchar* ptr = img.ptr<uchar>(x);
		for (int y = 0; y < img.cols; y++) {
			int i = x / 40, j = y / 40;
			if ((x % 40) == 0 && (y % 40) == 0)
				max[i][j] = min[i][j] = *ptr;
			hist[i][j][*ptr]++;

			if (*ptr > max[i][j])
				max[i][j] = *ptr;
			if (*ptr < min[i][j])
				min[i][j] = *ptr;

			avg[i][j][0] = ((min[i][j] + avg[i][j][1]) / 2.0);
			avg[i][j][1] += (*ptr++ / 1600.0);
			avg[i][j][2] = ((max[i][j] + avg[i][j][1]) / 2.0);
		}
	}

	for (int x = 0; x < 12; x++) {
		for (int y = 0; y < 16; y++) {
			for (int z = min[x][y]; z <= max[x][y]; z++) {
				if (min[x][y] <= z && z <= avg[x][y][0]) {
					total[x][y][0] += (z * hist[x][y][z]);
					count[x][y][0] += hist[x][y][z];
				}
				if (avg[x][y][0] <= z && z <= avg[x][y][1]) {
					total[x][y][1] += (z * hist[x][y][z]);
					count[x][y][1] += hist[x][y][z];
				}
				if (avg[x][y][1] <= z && z <= avg[x][y][2]) {
					total[x][y][2] += (z * hist[x][y][z]);
					count[x][y][2] += hist[x][y][z];
				}
				if (avg[x][y][2] <= z && z <= max[x][y]) {
					total[x][y][3] += (z * hist[x][y][z]);
					count[x][y][3] += hist[x][y][z];
				}
			}

			for (int z = 0; z < 4; z++)
				avg[x][y][z + 3] = total[x][y][z] / (double)count[x][y][z];
		}
	}
	for (int x = 0; x < img.rows; x++) {
		uchar* ptr = img.ptr<uchar>(x);
		for (int y = 0; y < img.cols; y++) {
			int i = x / 40, j = y / 40;
			if ((avg[i][j][6] - avg[i][j][3]) <= 48) {
				*ptr++ = 0;
			}
			else {
				if (*ptr <= (avg[i][j][3] * 1.1))
					*ptr++ = 255;
				else
					*ptr++ = 0;
			}
		}
	}
	Mat labelImg;
	int labels = connectedComponents(img, labelImg);
	cout << "labels : " << labels << endl;

	Mat mat(img.size(), CV_8UC1, Scalar(255));
	for (int x = 0; x < mat.rows; x++) {
		uchar* ptr = mat.ptr<uchar>(x);
		int* ptr1 = labelImg.ptr<int>(x);
		for (int y = 0; y < mat.cols; y++) {
			if (*ptr1++ == 1)
				*ptr = 0;
			*ptr++;
		}
	}

	stop = clock();
	cout << double(stop - start) / CLOCKS_PER_SEC << endl;

	imshow("test", mat);
	waitKey();
	cvDestroyAllWindows();

	system("pause");
	return 0;
}
