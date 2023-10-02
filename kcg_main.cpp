#include "KcgMatch.h"

using namespace kcg;

int kcg_main(int argc, char** argv) {

	// ʵ����KcgMatch 
	// "demo/k"Ϊ�洢ģ��ĸ�Ŀ¼ 
	// "k"Ϊģ�������
	KcgMatch kcg("G:/ģ��/template3", "template3");

	// ��ȡģ��ͼ��
	Mat model = imread("G:ģ��/template3/template.png");

	// ת�Ҷ�
	if (model.channels() == 3) {
		cvtColor(model, model, COLOR_BGR2GRAY);
	}

	// ָ��Ҫ������ģ��Ƕȣ��߶ȷ�Χ
	AngleRange ar(-180.f, 180.f, 10.f);
	ScaleRange sr(0.70f, 1.3f, 0.05f);
	// ��ʼ����ģ�壨����G:/ģ��/template3��·��������һ��yaml�ļ������汣�������ɵ�ģ��������Ϣ��
	kcg.MakingTemplates(model, ar, sr, 0, 30.f, 60.f);

	// ����ģ��
	cout << "Loading model ......" << endl;
	kcg.LoadModel();
	cout << "Load succeed." << endl;

	// ��ȡ����ͼ��
	Mat source = imread("G:/ģ��/template3/search.png");

	Mat draw_source;
	source.copyTo(draw_source);
	if (source.channels() == 3) {
		cvtColor(source, source, COLOR_BGR2GRAY);
	}

	//����ƥ��ʱ�䣨����Ҫwindows.h�ļ������ʱ��ķ�����
	double dur;
	clock_t start, end;
	start = clock();
	//����ģ��ƥ��
	auto matches = kcg.Matching(source, 0.80f, 0.1f, 30.f, 0.9f, PyramidLevel_2, 2, 12, Strategy_Accurate);
	end = clock();
	dur = (double)(end - start);
	printf("Use Time:%f\n", (dur / CLOCKS_PER_SEC));
	cout << "Final match size: " << matches.size() << endl << endl;

	// ����ƥ����
	kcg.DrawMatches(draw_source, matches, Scalar(0, 0, 255));

	// ����ƥ��ʱ��
	rectangle(draw_source, Rect(Point(0, 0), Point(136, 20)), Scalar(255, 255, 255), -1);
	cv::putText(draw_source,
		"time: " + to_string(dur / CLOCKS_PER_SEC) + "s",
		Point(0, 16), FONT_HERSHEY_PLAIN, 1.f, Scalar(0, 0, 0), 1);

	// ��ʾ���ͼ��
	namedWindow("draw_source", 0);
	imshow("draw_source", draw_source);
	imwrite("draw_source.jpg", draw_source);
	waitKey(0);
	//system("pause");
	return 0;
}
