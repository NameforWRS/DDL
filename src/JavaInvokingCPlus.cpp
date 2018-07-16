// CallInterpreter.cpp : 定义控制台应用程序的入口点。
//

#include "JavaInvokingCPlus.h"

void DeleteTree(ExpressionMgr * &tree) {
	if (tree == NULL)
		return;
	for (int i = 0; i < tree->Children.size(); i++) {
		DeleteTree(tree->Children[i]);
	}
	delete tree;
	tree = NULL;
	return;
}

JNIEXPORT jdouble JNICALL Java_ec_app_batch_JavaCallCPlus_getsimpleobj(
		JNIEnv *env, jobject, jstring data) {

	/*
	 * get the source from java
	 */
	const char* heuristic = env->GetStringUTFChars(data, NULL);
	//string code1;
	//code1 = heuristic;
	env->ReleaseStringUTFChars(data, heuristic);

	vector<int> n;
	vector<vector<int>> p1;
	vector<vector<int>> p2;
	vector<vector<int>> s1;
	vector<vector<int>> r;
	vector<int> W;
	vector<int> B;
	int ram = uniform(1, 10);
	int seed1 = 0;
	if (ram == 1) {
		seed1 = 1234567889;
	} else if (ram == 2) {
		seed1 = 19960809;
	} else if (ram == 3) {
		seed1 = 19960810;
	} else if (ram == 4) {
		seed1 = 19960812;
	} else if (ram == 5) {
		seed1 = 19960815;
	} else if (ram == 6) {
		seed1 = 19960818;
	} else if (ram == 7) {
		seed1 = 19960823;
	} else if (ram == 8) {
		seed1 = 19960834;
	} else if (ram == 9) {
		seed1 = 19960856;
	} else if (ram == 10) {
		seed1 = 19960813;
	}

	//导入派单规则
	string code1 = "";
	string code2 = "";
	string code3 = "";
	string code_batchformation = "";
	string code_batchsequence = "";
	string code_jobsequence = "";
	code1 =
			"(max (- (If (+ (* (+ (* FMINPT FMAXPT) (+ RSMINPT RFMINPT)) (+ (- RSMINPT SMINPT) (+ SMINPT SMAXPT))) (If (div (div RSMINPT SMAXPT) (- FMAXPT SMAXPT)) (+ RSMINPT RFMINPT) (max (* FPT RFMAXPT) FMAXPT))) (If FPT RFMINPT RSMINPT) (If (* (- (- RSMINPT SMINPT) FMINPT) FSUMPT) (* (+ (max (- (If (div SPT SSUMPT) (div FSUMPT RFMAXPT) (max SMAXPT FPT)) (* (- (+ SPT SMAXPT) (+ SPT SMAXPT)) (- SPT SSUMPT))) (+ (- (max SMAXPT FPT) SSUMPT) (+ SPT SMAXPT))) (- RSMINPT (max (- (If (div SPT SSUMPT) (div (max SMAXPT FMAXPT) (* SPT FMAXPT)) (max SMAXPT (* SMAXPT FMINPT))) (+ RFMAXPT FPT)) (+ (max (- RSSUMPT (+ RFMAXPT FPT)) (+ (- SPT SSUMPT) (- RSMINPT SMINPT))) (- RSMINPT (div (+ RFMAXPT SMAXPT) (+ SMAXPT FSUMPT))))))) (div (max SMAXPT FMAXPT) (* SPT FMAXPT))) (div FSUMPT RFMAXPT))) (max (- (If (div SPT SSUMPT) (div FSUMPT RFMAXPT) (max SMAXPT (* SPT FMAXPT))) (+ RFMAXPT FPT)) (+ (max (- (+ SPT SMAXPT) (+ SPT SMAXPT)) (+ (max (- (If (div SPT SSUMPT) (div FSUMPT RFMAXPT) (max SMAXPT (* SPT FMAXPT))) (+ RFMAXPT FPT)) (+ (max SSUMPT (+ (- SPT SSUMPT) (- RSMINPT SMINPT))) (- RSMINPT (div SMAXPT FMAXPT)))) (- RSMINPT SMINPT))) (- RSMINPT (div SMAXPT FMAXPT))))) (* (div (+ SMINPT SMAXPT) (If (* (max SPT FMINPT) RSMINPT) (If SMINPT RSSUMPT SSUMPT) (div SMAXPT FMAXPT))) FSUMPT))";
	code2 = heuristic;
	code3 = "PTS";

	ExpressionMgr mgr1;
	ExpressionMgr mgr2;
	ExpressionMgr mgr3;

	///一头一尾加上括号，方便处理
	code_batchformation = code1;
	if (code_batchformation[0] != '(') {
		if (code_batchformation.size() > 4) {
			code_batchformation = "(" + code_batchformation + ")";
		} else {
			code_batchformation = "(* " + code_batchformation + ")";
		}
	}
	code_batchsequence = code2;
	if (code_batchsequence[0] != '(') {
		if (code_batchsequence.size() > 8) {
			code_batchsequence = "(" + code_batchsequence + ")";
		} else {
			code_batchsequence = "(* " + code_batchsequence + ")";
		}
	}
	code_jobsequence = code3;
	if (code_jobsequence[0] != '(') {
		if (code_jobsequence.size() > 4) {
			code_jobsequence = "(" + code_jobsequence + ")";
		} else {
			code_jobsequence = "(* " + code_jobsequence + ")";
		}
	}

	///将表达式转换为树
	ExpressionMgr * scheme1 = mgr1.ParseAsIScheme(code_batchformation);
	ExpressionMgr * scheme2 = mgr2.ParseAsIScheme(code_batchsequence);
	ExpressionMgr * scheme3 = mgr3.ParseAsIScheme(code_jobsequence);
	IDG_simple(n, p1, p2, s1, r, W, B, seed1);
	vector<double> Obj;

	for (int i = 0; i < n.size(); i++) {
		for (int batch = 0; batch < B.size(); batch++) {
			for (int w = 0; w < W.size(); w++) {
				double Obj1 = GBLWT_GP(n[i], p1[i], p2[i], s1[i], r[i], W[w],
						B[batch], scheme1, scheme2, scheme3);
				double obj_GDSPT = GBLWT(n[i], p1[i], p2[i], s1[i], r[i], W[w],
						B[batch]);
				//cout << Obj1 << "	" << obj_GDSPT << endl;
				Obj.push_back(100.0 * (Obj1 - obj_GDSPT) / obj_GDSPT);
			}
		}
	}
	DeleteTree(scheme1);
	DeleteTree(scheme2);
	DeleteTree(scheme3);

	//cout<<"the simple ratio of OBJ_GP/ OBJ_O is "<<std::accumulate(std::begin(Obj), std::end(Obj), 0.0) / Obj.size()<<endl;
	return std::accumulate(std::begin(Obj), std::end(Obj), 0.0) / Obj.size();
}

JNIEXPORT jdouble JNICALL Java_ec_app_batch_JavaCallCPlus_getsimpleobjwithseed(
		JNIEnv *env, jobject, jstring data, jlong seed) {

	/*
	 * get the source from java
	 */
	const char* heuristic = env->GetStringUTFChars(data, NULL);
	//string code1;
	//code1 = heuristic;
	env->ReleaseStringUTFChars(data, heuristic);

	vector<int> n;
	vector<vector<int>> p1;
	vector<vector<int>> p2;
	vector<vector<int>> s1;
	vector<vector<int>> r;
	vector<int> W;
	vector<int> B;
	//long seed1 = seed;
	//导入派单规则
	string code1 = "";
	string code2 = "";
	string code3 = "";
	string code_batchformation = "";
	string code_batchsequence = "";
	string code_jobsequence = "";
	code1 =
			"(max (- (If (+ (* (+ (* FMINPT FMAXPT) (+ RSMINPT RFMINPT)) (+ (- RSMINPT SMINPT) (+ SMINPT SMAXPT))) (If (div (div RSMINPT SMAXPT) (- FMAXPT SMAXPT)) (+ RSMINPT RFMINPT) (max (* FPT RFMAXPT) FMAXPT))) (If FPT RFMINPT RSMINPT) (If (* (- (- RSMINPT SMINPT) FMINPT) FSUMPT) (* (+ (max (- (If (div SPT SSUMPT) (div FSUMPT RFMAXPT) (max SMAXPT FPT)) (* (- (+ SPT SMAXPT) (+ SPT SMAXPT)) (- SPT SSUMPT))) (+ (- (max SMAXPT FPT) SSUMPT) (+ SPT SMAXPT))) (- RSMINPT (max (- (If (div SPT SSUMPT) (div (max SMAXPT FMAXPT) (* SPT FMAXPT)) (max SMAXPT (* SMAXPT FMINPT))) (+ RFMAXPT FPT)) (+ (max (- RSSUMPT (+ RFMAXPT FPT)) (+ (- SPT SSUMPT) (- RSMINPT SMINPT))) (- RSMINPT (div (+ RFMAXPT SMAXPT) (+ SMAXPT FSUMPT))))))) (div (max SMAXPT FMAXPT) (* SPT FMAXPT))) (div FSUMPT RFMAXPT))) (max (- (If (div SPT SSUMPT) (div FSUMPT RFMAXPT) (max SMAXPT (* SPT FMAXPT))) (+ RFMAXPT FPT)) (+ (max (- (+ SPT SMAXPT) (+ SPT SMAXPT)) (+ (max (- (If (div SPT SSUMPT) (div FSUMPT RFMAXPT) (max SMAXPT (* SPT FMAXPT))) (+ RFMAXPT FPT)) (+ (max SSUMPT (+ (- SPT SSUMPT) (- RSMINPT SMINPT))) (- RSMINPT (div SMAXPT FMAXPT)))) (- RSMINPT SMINPT))) (- RSMINPT (div SMAXPT FMAXPT))))) (* (div (+ SMINPT SMAXPT) (If (* (max SPT FMINPT) RSMINPT) (If SMINPT RSSUMPT SSUMPT) (div SMAXPT FMAXPT))) FSUMPT))";
	code2 = heuristic;
	code3 = "PTS";

	ExpressionMgr mgr1;
	ExpressionMgr mgr2;
	ExpressionMgr mgr3;

	///一头一尾加上括号，方便处理
	code_batchformation = code1;
	if (code_batchformation[0] != '(') {
		if (code_batchformation.size() > 4) {
			code_batchformation = "(" + code_batchformation + ")";
		} else {
			code_batchformation = "(* " + code_batchformation + ")";
		}
	}
	code_batchsequence = code2;
	if (code_batchsequence[0] != '(') {
		if (code_batchsequence.size() > 8) {
			code_batchsequence = "(" + code_batchsequence + ")";
		} else {
			code_batchsequence = "(* " + code_batchsequence + ")";
		}
	}
	code_jobsequence = code3;
	if (code_jobsequence[0] != '(') {
		if (code_jobsequence.size() > 4) {
			code_jobsequence = "(" + code_jobsequence + ")";
		} else {
			code_jobsequence = "(* " + code_jobsequence + ")";
		}
	}

	///将表达式转换为树
	ExpressionMgr * scheme1 = mgr1.ParseAsIScheme(code_batchformation);
	ExpressionMgr * scheme2 = mgr2.ParseAsIScheme(code_batchsequence);
	ExpressionMgr * scheme3 = mgr3.ParseAsIScheme(code_jobsequence);
	IDG_simple(n, p1, p2, s1, r, W, B, seed);
	vector<double> Obj;

	for (int i = 0; i < n.size(); i++) {
		for (int batch = 0; batch < B.size(); batch++) {
			for (int w = 0; w < W.size(); w++) {
				double Obj1 = GBLWT_GP(n[i], p1[i], p2[i], s1[i], r[i], W[w],
						B[batch], scheme1, scheme2, scheme3);
				double obj_GDSPT = GBLWT(n[i], p1[i], p2[i], s1[i], r[i], W[w],
						B[batch]);
				//cout << Obj1 << "	" << obj_GDSPT << endl;
				Obj.push_back(100.0 * (Obj1 - obj_GDSPT) / obj_GDSPT);
			}
		}
	}
	DeleteTree(scheme1);
	DeleteTree(scheme2);
	DeleteTree(scheme3);
	//cout<<"the simple ratio of OBJ_GP/ OBJ_O is "<<std::accumulate(std::begin(Obj), std::end(Obj), 0.0) / Obj.size()<<endl;
	return std::accumulate(std::begin(Obj), std::end(Obj), 0.0) / Obj.size();
}

/*
 * This part mainly return the obj of main invoking training set
 *
 *
 *
 *
 * */

JNIEXPORT jdouble JNICALL Java_ec_app_batch_JavaCallCPlus_getobj(JNIEnv *env,
		jobject, jstring data, jlong seed) {
	//引入字符串，并将其转化为string型
	const char* heuristic = env->GetStringUTFChars(data, NULL);
	//string code1;
	//code1 = heuristic;
	env->ReleaseStringUTFChars(data, heuristic);

	vector<int> n;
	vector<vector<int>> p1;
	vector<vector<int>> p2;
	vector<vector<int>> s1;
	vector<vector<int>> r;
	vector<int> W;
	vector<int> B;
	int A = 10;
	int N = 4;
	int R = 10;
	int F = 10;
	int G = 50;

	int timelimit = 10;

	//导入派单规则
	string code1 = "";
	string code2 = "";
	string code3 = "";
	string code_batchformation = "";
	string code_batchsequence = "";
	string code_jobsequence = "";
	code1 =
			"(max (- (If (+ (* (+ (* FMINPT FMAXPT) (+ RSMINPT RFMINPT)) (+ (- RSMINPT SMINPT) (+ SMINPT SMAXPT))) (If (div (div RSMINPT SMAXPT) (- FMAXPT SMAXPT)) (+ RSMINPT RFMINPT) (max (* FPT RFMAXPT) FMAXPT))) (If FPT RFMINPT RSMINPT) (If (* (- (- RSMINPT SMINPT) FMINPT) FSUMPT) (* (+ (max (- (If (div SPT SSUMPT) (div FSUMPT RFMAXPT) (max SMAXPT FPT)) (* (- (+ SPT SMAXPT) (+ SPT SMAXPT)) (- SPT SSUMPT))) (+ (- (max SMAXPT FPT) SSUMPT) (+ SPT SMAXPT))) (- RSMINPT (max (- (If (div SPT SSUMPT) (div (max SMAXPT FMAXPT) (* SPT FMAXPT)) (max SMAXPT (* SMAXPT FMINPT))) (+ RFMAXPT FPT)) (+ (max (- RSSUMPT (+ RFMAXPT FPT)) (+ (- SPT SSUMPT) (- RSMINPT SMINPT))) (- RSMINPT (div (+ RFMAXPT SMAXPT) (+ SMAXPT FSUMPT))))))) (div (max SMAXPT FMAXPT) (* SPT FMAXPT))) (div FSUMPT RFMAXPT))) (max (- (If (div SPT SSUMPT) (div FSUMPT RFMAXPT) (max SMAXPT (* SPT FMAXPT))) (+ RFMAXPT FPT)) (+ (max (- (+ SPT SMAXPT) (+ SPT SMAXPT)) (+ (max (- (If (div SPT SSUMPT) (div FSUMPT RFMAXPT) (max SMAXPT (* SPT FMAXPT))) (+ RFMAXPT FPT)) (+ (max SSUMPT (+ (- SPT SSUMPT) (- RSMINPT SMINPT))) (- RSMINPT (div SMAXPT FMAXPT)))) (- RSMINPT SMINPT))) (- RSMINPT (div SMAXPT FMAXPT))))) (* (div (+ SMINPT SMAXPT) (If (* (max SPT FMINPT) RSMINPT) (If SMINPT RSSUMPT SSUMPT) (div SMAXPT FMAXPT))) FSUMPT))";
	code2 = heuristic;
	code3 = "PTS";

	ExpressionMgr mgr1;
	ExpressionMgr mgr2;
	ExpressionMgr mgr3;

	///一头一尾加上括号，方便处理
	code_batchformation = code1;
	if (code_batchformation[0] != '(') {
		if (code_batchformation.size() > 4) {
			code_batchformation = "(" + code_batchformation + ")";
		} else {
			code_batchformation = "(* " + code_batchformation + ")";
		}
	}
	code_batchsequence = code2;
	if (code_batchsequence[0] != '(') {
		if (code_batchsequence.size() > 8) {
			code_batchsequence = "(" + code_batchsequence + ")";
		} else {
			code_batchsequence = "(* " + code_batchsequence + ")";
		}
	}
	code_jobsequence = code3;
	if (code_jobsequence[0] != '(') {
		if (code_jobsequence.size() > 4) {
			code_jobsequence = "(" + code_jobsequence + ")";
		} else {
			code_jobsequence = "(* " + code_jobsequence + ")";
		}
	}

	///将表达式转换为树
	ExpressionMgr * scheme1 = mgr1.ParseAsIScheme(code_batchformation);
	ExpressionMgr * scheme2 = mgr2.ParseAsIScheme(code_batchsequence);
	ExpressionMgr * scheme3 = mgr3.ParseAsIScheme(code_jobsequence);

	IDG_1(n, p1, p2, s1, r, W, B, seed);
	//cout<<"seed is"<<seed<<endl;
	vector<double> Obj;

	for (int i = 0; i < n.size(); i++) {
		for (int batch = 0; batch < B.size(); batch++) {
			for (int w = 0; w < W.size(); w++) {
				double Obj1 = GBLWT_GP(n[i], p1[i], p2[i], s1[i], r[i], W[w],
						B[batch], scheme1, scheme2, scheme3);
				double obj_GDSPT = GBLWT(n[i], p1[i], p2[i], s1[i], r[i], W[w],
						B[batch]);
				//cout << Obj1 << "	" << obj_GDSPT << endl;
				Obj.push_back(100.0 * (Obj1 - obj_GDSPT) / obj_GDSPT);
			}
		}
	}
	DeleteTree(scheme1);
	DeleteTree(scheme2);
	DeleteTree(scheme3);
	return std::accumulate(std::begin(Obj), std::end(Obj), 0.0) / Obj.size();

}

