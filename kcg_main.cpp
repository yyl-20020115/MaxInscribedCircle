#include "KcgMatch.h"

using namespace kcg;

int kcg_main(int argc, char** argv) {

	// 实例化KcgMatch 
	// "demo/k"为存储模板的根目录 
	// "k"为模板的名字
	KcgMatch kcg("G:/模板/template3", "template3");

	// 读取模板图像
	Mat model = imread("G:模板/template3/template.png");

	// 转灰度
	if (model.channels() == 3) {
		cvtColor(model, model, COLOR_BGR2GRAY);
	}

	// 指定要制作的模板角度，尺度范围
	AngleRange ar(-180.f, 180.f, 10.f);
	ScaleRange sr(0.70f, 1.3f, 0.05f);
	// 开始制作模板（会在G:/模板/template3的路径下生成一个yaml文件，里面保存着生成的模板特征信息）
	kcg.MakingTemplates(model, ar, sr, 0, 30.f, 60.f);

	// 加载模板
	cout << "Loading model ......" << endl;
	kcg.LoadModel();
	cout << "Load succeed." << endl;

	// 读取搜索图像
	Mat source = imread("G:/模板/template3/search.png");

	Mat draw_source;
	source.copyTo(draw_source);
	if (source.channels() == 3) {
		cvtColor(source, source, COLOR_BGR2GRAY);
	}

	//计算匹配时间（不需要windows.h的计算毫秒时间的方法）
	double dur;
	clock_t start, end;
	start = clock();
	//进行模板匹配
	auto matches = kcg.Matching(source, 0.80f, 0.1f, 30.f, 0.9f, PyramidLevel_2, 2, 12, Strategy_Accurate);
	end = clock();
	dur = (double)(end - start);
	printf("Use Time:%f\n", (dur / CLOCKS_PER_SEC));
	cout << "Final match size: " << matches.size() << endl << endl;

	// 画出匹配结果
	kcg.DrawMatches(draw_source, matches, Scalar(0, 0, 255));

	// 画出匹配时间
	rectangle(draw_source, Rect(Point(0, 0), Point(136, 20)), Scalar(255, 255, 255), -1);
	cv::putText(draw_source,
		"time: " + to_string(dur / CLOCKS_PER_SEC) + "s",
		Point(0, 16), FONT_HERSHEY_PLAIN, 1.f, Scalar(0, 0, 0), 1);

	// 显示结果图像
	namedWindow("draw_source", 0);
	imshow("draw_source", draw_source);
	imwrite("draw_source.jpg", draw_source);
	waitKey(0);
	//system("pause");
	return 0;
}
