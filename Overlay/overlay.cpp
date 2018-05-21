#include <opencv2\opencv.hpp>
#include <iostream>
#include <string>
using namespace cv;
using namespace std;

int main(void) {
	clock_t start, stop;
	vector<Mat> vec;
	Mat img, img1;

	start = clock();
	for (int x = 1; x <= 2110; x++) {
		char str[100];

		sprintf_s(str, "D:\\專題-影像處理\\飛機-影像\\TiltForward_1080p\\TiltForward (%d).jpg", x);
		img = imread(str, IMREAD_GRAYSCALE);

		if (img.empty())
			return 1;
		
		vec.push_back(img.rowRange((img.rows / 2), (img.rows / 2) + 1).clone());
	}

	Mat img1(Size(img.cols, vec.size()), CV_8UC1);

	for (int x = vec.size() - 1; x >= 0; x--)
		img1.push_back(vec[x]);

	stop = clock();

	cout << "time : " << double(stop - start) / CLOCKS_PER_SEC << " s" << endl;

	imwrite("AirPlaneLine.jpg", img1);
	waitKey();
	cvDestroyAllWindows();

	system("pause");
	return 0;
}
