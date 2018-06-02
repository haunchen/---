#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
using namespace std;
using namespace cv;
//#define RUN
//
//C:\\Users\\haunchen\\Google 雲端硬碟\\專題-影像處理\\飛機-影像\\TiltForward_1080p\\TiltForward (466).jpg

#define FILENAME "C:\\Users\\haunchen\\Google 雲端硬碟\\專題-影像處理\\飛機-影像\\Vertical_1080p\\Vertical (833).jpg"

int main() {
	clock_t start, stop;
	Mat img, imgroi, imgcopy(Size(640, 480), CV_8UC3), imgcopyroi;

	double avg[7] = { 0.0 };
	int total[4] = { 0 }, count[4] = { 0 }, hist[256] = { 0 };
	int countpixel = 0;
	uchar max = 0, min = 0;

	start = clock();

#ifdef RUN
	char str[100] = "";
	for (int qqq = 337; qqq <= 1823; qqq++) {
		sprintf_s(str, "C:\\Users\\haunchen\\Google 雲端硬碟\\專題-影像處理\\飛機-影像\\Vertical_1080p\\Vertical (%d).jpg", qqq);
		cout << str << endl;
#endif
		imgcopy.setTo(Scalar(0, 0, 0));

#ifdef RUN
		resize(imread(str, IMREAD_GRAYSCALE), img, Size(640, 480));
#else
		resize(imread(FILENAME, IMREAD_GRAYSCALE), img, Size(640, 480));
#endif

		for (int x = 0; x < img.rows; x += 40) {
			for (int y = 0; y < img.cols; y += 40) {
				imgroi = img(Rect(y, x, 40, 40));	//ROI
				countpixel = max = min = 0;
				for (int zero = 0; zero < 256; zero++) {
					if (zero < 7) {
						if (zero < 4)
							total[zero] = count[zero] = 0;
						avg[zero] = 0.0;
					}
					hist[zero] = 0;
				}

				for (int i = 0; i < imgroi.rows; i++) {
					uchar* ptr = imgroi.ptr<uchar>(i);
					for (int j = 0; j < imgroi.cols; j++) {
						if (i == 0 && j == 0)
							max = min = *ptr;

						if (*ptr < min)
							min = *ptr;
						else if (*ptr > max)
							max = *ptr;

						hist[*ptr]++;

						countpixel += *ptr;
						*ptr++;
					}
				}

				avg[1] = (countpixel / 1600.0);
				avg[0] = ((min + avg[1]) / 2.0);
				avg[2] = ((max + avg[1]) / 2.0);

				for (int z = min; z <= max; z++) {
					if (min <= z && z <= avg[0]) {
						total[0] += (z * hist[z]);
						count[0] += hist[z];
					}
					if (avg[0] <= z && z <= avg[1]) {
						total[1] += (z * hist[z]);
						count[1] += hist[z];
					}
					if (avg[1] <= z && z <= avg[2]) {
						total[2] += (z * hist[z]);
						count[2] += hist[z];
					}
					if (avg[2] <= z && z <= max) {
						total[3] += (z * hist[z]);
						count[3] += hist[z];
					}
				}

				for (int z = 0; z < 4; z++)
					avg[z + 3] = total[z] / (double)count[z];

				//判斷histogram是否集中，集中的是巧拼；不集中的"可能"是黑線，並找出邊緣來
				if ((avg[6] - avg[3]) <= 48) {
					imgroi = 0;
				}
				else {
					threshold(imgroi, imgroi, (avg[3] * 1.1), 255, THRESH_BINARY_INV);	//二值化
					dilate(imgroi.clone(), imgroi, Mat());								//膨脹
					Canny(imgroi, imgroi, 50, 150, 3);									//輪廓

					Mat tempimg(imgcopy, Rect(y, x, 40, 40));
					Mat_<Vec3b>::iterator it = tempimg.begin<Vec3b>();
					for (int q = 0; q < imgroi.rows; q++)
						for (int qq = 0; qq < imgroi.cols; qq++)
							(*it++) = Vec3d(imgroi.at<uchar>(q, qq), imgroi.at<uchar>(q, qq), imgroi.at<uchar>(q, qq));

					/*鏈碼*/
					vector<vector<Point>> contours;
					vector<Vec4i> hierarchy;

					findContours(imgroi, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

					vector<int[8]> vec(contours.size());
					int countPixelAll = 0, countPixel26 = 0;
					int temp2 = 0, temp6 = 0;
					bool choice2 = true, choice6 = true;

					for (int i = 0; i < contours.size(); i++) {
						for (int j = 0; j < contours[i].size() - 1; j++) {
							int x_temp = contours[i][j + 1].x - contours[i][j].x;
							int y_temp = contours[i][j + 1].y - contours[i][j].y;

							if (x_temp == 0 && y_temp == 1)
								vec[i][0]++;
							else if (x_temp == -1 && y_temp == 1)
								vec[i][1]++;
							else if (x_temp == -1 && y_temp == 0) {
								vec[i][2]++;
								countPixel26++;
							}
							else if (x_temp == -1 && y_temp == -1)
								vec[i][3]++;
							else if (x_temp == 0 && y_temp == -1)
								vec[i][4]++;
							else if (x_temp == 1 && y_temp == -1)
								vec[i][5]++;
							else if (x_temp == 1 && y_temp == 0) {
								vec[i][6]++;
								countPixel26++;
							}
							else if (x_temp == 1 && y_temp == 1)
								vec[i][7]++;

							countPixelAll++;

							if (x_temp == -1 && y_temp == 0)
								temp2++;
							else {
								if (temp2 > 7) {
									choice2 = false;
									break;
								}
								else
									temp2 = 0;
							}
							if (x_temp == 1 && y_temp == 0)
								temp6++;
							else {
								if (temp6 > 7) {
									choice6 = false;
									break;
								}
								else
									temp6 = 0;
							}
						}
						if (!choice2 || !choice6)
							break;
					}

					if (choice2 && choice6 && countPixelAll > 20 && countPixel26 < (countPixelAll * 0.10)) {
						int aa = 5;
						for (int i = 0; i < vec.size(); i++) {
						if (vec[i][0] > aa && vec[i][3] > aa && vec[i][4] > aa && vec[i][7] > aa)
						rectangle(imgcopy, Rect(y, x, 40, 40), Scalar(0, 0, 255));
						else if (vec[i][3] > aa && vec[i][7] > aa)
						rectangle(imgcopy, Rect(y, x, 40, 40), Scalar(0, 255, 0));
						else if (vec[i][0] > aa && vec[i][4] > aa)
						rectangle(imgcopy, Rect(y, x, 40, 40), Scalar(255, 0, 0));
						else if (vec[i][3] > aa && vec[i][4] > aa && vec[i][7] > aa)
						rectangle(imgcopy, Rect(y, x, 40, 40), Scalar(128, 128, 128));
						}
					}
				}
			}
		}
#ifdef RUN
		sprintf_s(str, "test\\%d.jpg", qqq);
		imwrite(str, imgcopy);
	}
#endif

	stop = clock();

	cout << double(stop - start) / CLOCKS_PER_SEC << endl;

#ifndef RUN
	imshow("test", imgcopy);
	imwrite("test.jpg", imgcopy);
	waitKey(0);
	cvDestroyAllWindows();
#endif

	system("pause");
	return 0;
}
