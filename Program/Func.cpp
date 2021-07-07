


#include "DxLib.h"
#include "Func.h"

#include <random>
const double CFunc::PI = 3.1415926535897932384626433832795;
const double CFunc::RAD = 57.295779513082320876798154814105;
const double CFunc::TOL1 = 0.01;
const double CFunc::TOL2 = 0.1 / CFunc::RAD;
const double CFunc::TOLZERO = 0.0000001;
const int CFunc::MAXSTRING = 256;

double CFunc::Square(double X) {
	return X * X;
}
int CFunc::Square(int X) {
	return X * X;
}

int CFunc::ToAbs(int VALUE) {
	return ((VALUE >= 0) ? VALUE : -VALUE);
}
double CFunc::ToAbs(double VALUE) {
	return ((VALUE >= 0) ? VALUE : -VALUE);
}

double CFunc::ToRad(double angle) {
	return angle / RAD;
}
double CFunc::ToDeg(double angle) {
	return angle * RAD;
}

int CFunc::RandI(int MIN, int MAX) {
	if (MIN > MAX) {
		using std::swap;
		swap(MIN, MAX);
	}
	return MIN + (int)(rand() * (MAX - MIN + 1) / (1 + RAND_MAX));
}
double CFunc::RandF(int MIN, int MAX) {
	if (MIN > MAX) {
		using std::swap;
		swap(MIN, MAX);
	}
	return MIN + (double)(rand() * (MAX - MIN + 1) / (1 + RAND_MAX));
}
double CFunc::RandD(double MIN, double MAX) {
	if (MIN > MAX) {
		using std::swap;
		swap(MIN, MAX);
	}


	int min = MIN * 1000000;
	int max = MAX * 1000000;
	bool minus = false;
	int sub = 0;
	if (min < 0) {
		minus = true;
		sub = - min;

		min += sub;
		max += sub;
	}

	std::random_device rnd;     // 非決定的な乱数生成器を生成
	std::mt19937 mt(rnd());     //  メルセンヌ・ツイスタの32ビット版、引数は初期シード値
	std::uniform_int_distribution<> rand100(min, max); 

	int result = rand100(mt);

	return (rand100(mt) - sub) / 1000000.0;

	//int min = MIN * 1000;
	//int max = MAX * 1000;
	//int result = min + (double)(rand() * (max - min + 1) / (1 + RAND_MAX));
	//if (result < min)result = min;
	//if (result > max)result = max;
	//return (double)result / 1000.0;
}
bool CFunc::RandB() {
	return (bool)RandI(0, 1);
}

double CFunc::GetMax(double value1, double value2) {
	return value1 > value2 ? value1 : value2;
}
int CFunc::GetMax(int value1, int value2) {
	return value1 > value2 ? value1 : value2;
}
double CFunc::GetMin(double value1, double value2) {
	return value1 > value2 ? value2 : value1;
}
int CFunc::GetMin(int value1, int value2) {
	return value1 > value2 ? value2 : value1;
}

double CFunc::Logn(int base, double antilog) {
	return log(antilog) / log((double)base);
}

int CFunc::AddComma(char* str) {
	int len = (int)strlen(str);
	if (len >= MAXSTRING) {
		return -1;
	}
	char wrkStr[MAXSTRING];
	int co = 0;
	int strCo = 0;

	strcpy_s(wrkStr, MAXSTRING, str);

	for (int i = len - 1; i >= 0; i--) {
		wrkStr[co] = str[i];
		co++;
		strCo++;
		if (strCo % 3 == 0 && i != 0) {
			wrkStr[co] = 44;	//,
			co++;
		}
	}
	for (int i = 0; i < co; i++) {
		str[i] = wrkStr[co - i - 1];
	}
	str[co] = '\0';

	return co;
}




void CFunc::DumpFile(char* txt) {
	static int m_count = 0;

	// dmp_"yyyymmdd"_+m_count(4ケタ)
	FILE* fp;
	char filename[80];
	time_t now = time(nullptr);
	struct tm pnow;
	localtime_s(&pnow, &now);
	sprintf_s(filename, "dmp_%d%02d%d_%d", pnow.tm_year + 1900, pnow.tm_mon + 1, pnow.tm_mday, m_count);
	m_count++;
	fopen_s(&fp, filename, "wb");
	if (fp) {
		fclose(fp);
	}
}
void CFunc::DumpFile2(char* filename) {
	FILE* fp;
	fopen_s(&fp, filename, "wb");
	if (fp) {
		fclose(fp);
	}
}

void CFunc::DumpFileDebug(char* txt) {
#if _DEBUG
	DumpFile(txt);
#endif
}
void CFunc::DumpFileDebug2(char* filename) {
#if _DEBUG
	DumpFile2(filename);
#endif
}



bool CFunc::CheckLineCross(CPos p1, CPos p2, CPos p3, CPos p4) {
	if (((p1.x - p2.x) * (p3.y - p1.y) + (p1.y - p2.y) * (p1.x - p3.x)) *
		((p1.x - p2.x) * (p4.y - p1.y) + (p1.y - p2.y) * (p1.x - p4.x)) <= 0) {
		if (((p3.x - p4.x) * (p1.y - p3.y) + (p3.y - p4.y) * (p3.x - p1.x)) *
			((p3.x - p4.x) * (p2.y - p3.y) + (p3.y - p4.y) * (p3.x - p2.x)) <= 0) {
			return true;
		}
	}
	return false;
}

// 線分同士の交点
// https://www.hiramine.com/programming/graphics/2d_segmentintersection.html
bool CFunc::CalcIntersectionPoint(const CPos& pointA, // 線分A始点
	const CPos& pointB, // 線分A終点
	const CPos& pointC, // 線分B始点
	const CPos& pointD,	// 線分B終点
	CPos& pointIntersection, // 交点
	double& dR, // r < 0 ：点Ｐは、点Ａより手前	r > 1 ：点Ｐは、点Ｂより奥
	double& dS // s < 0 ：点Ｑは、点Ｃより手前	s > 1 ：点Ｑは、点Ｄより奥
) {
	double dBunbo = (pointB.x - pointA.x)
					* (pointD.y - pointC.y)
					- (pointB.y - pointA.y)
					* (pointD.x - pointC.x);
	if (0 == dBunbo){	
		return false;// 平行
	}
	CPos vectorAC = pointC - pointA;
	dR = ((pointD.y - pointC.y) * vectorAC.x
		- (pointD.x - pointC.x) * vectorAC.y) / dBunbo;
	dS = ((pointB.y - pointA.y) * vectorAC.x
		- (pointB.x - pointA.x) * vectorAC.y) / dBunbo;

	CPos p;
	p.x = (pointB.x - pointA.x) * dR;
	p.y = (pointB.y - pointA.y) * dR;
	pointIntersection = pointA + p;

	if (dR > 0 && dR < 1 && dS > 0 && dS < 1) {
		return true;
	}
	return false;
}


double CFunc::GetTwoVectorAngle(CPos& p1, CPos& p2) {
	double p1Len = p1.Length();
	double p2Len = p2.Length();
	if (p1Len * p2Len == 0) {
		return 0.0;
	}
	double cos_sita = CFunc::InsideProduct(p1, p2) / (p1Len * p2Len);
	if (cos_sita < -1.0)cos_sita = -1.0;
	if (cos_sita > 1.0)cos_sita = 1.0;
	double sita = acos(cos_sita);

	return sita;
}

double CFunc::InsideProduct(CPos v1, CPos v2) {
	return v1.x * v2.x + v1.y * v2.y;
}
double CFunc::OutsideProduct(CPos v1, CPos v2) {
	return v1.x * v2.y - v1.y * v2.x;
}
double CFunc::GetDistance(double x1, double y1, double x2, double y2, double x3, double y3) {
	CPos A, B;
	double a, b, t, D;
	A.x = x2 - x1;
	A.y = y2 - y1;
	a = pow(A.x, 2) + pow(A.y, 2);
	b = A.x * (x1 - x3) + A.y * (y1 - y3);
	t = -b / a;
	if (t < 0) t = 0;
	if (t > 1) t = 1;
	B.x = x1 + A.x * t;
	B.y = y1 + A.y * t;
	D = CFunc::Square(x3 - B.x) + CFunc::Square(y3 - B.y);
	return D;
}

double CFunc::GetDistance(CPos p1, CPos p2, CPos p3) {
	return (GetDistance(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y));
}

double CFunc::GetNearAngle(double targetAngle, double nearAngle) {
	double ang = 0;
	double subAng = 360.0;
	double returnAng = 0.0;

	while (targetAngle < 0) {
		targetAngle += 360;
	}
	while (targetAngle > 360) {
		targetAngle -= 360;
	}

	while (ang < 360.0) {
		if (CFunc::ToAbs(ang - targetAngle) < subAng) {
			subAng = CFunc::ToAbs(ang - targetAngle);
			returnAng = ang;
		}
		ang += nearAngle;
	}

	return returnAng;
}

//桁を取得する
//ex)23456->5
int CFunc::GetDigit(int number)
{
	unsigned digit = 0;
	while (number != 0) {
		number /= 10;
		digit++;
	}
	return digit;
	//return log10(number) + 1;
}
int CFunc::GetDigit(long long number)
{
	unsigned digit = 0;
	while (number != 0) {
		number /= 10;
		digit++;
	}
	return digit;
}

//	m_resultUI_Np,m_resultUI_Nd,m_resultUI_Nc
//0123456789
// 10[%]  11[.] 12[,]
// 7 1234567
// [0] : 7
// [1] : 6
// [2] : 5
// [3] : ,
// [4] : 4
// [6] : 3
// [7] : 2
// [8] : ,
// [9] : 1

// 5 123
// [0] : 3
// [1] : 2
// [2] : 1
// [3] : ,
// [4] : 0
// [5] : 0
// zeroumeDigitが6でnumberが12345　なら　012,345になる
void CFunc::GetDigitArray(int zeroDigit, long long number, std::vector<int>& indexAry)
{
	int count = 0;
	if(number == 0){
		for (int ii = zeroDigit; ii > 0; ii--) {
			count++;
			indexAry.push_back(0);
			if (count % 3 == 0) {
				if (ii == 1) {
					continue;
				}
				indexAry.push_back(12); // 12はカンマ
			}
		}
		return;
	}

	int digit = CFunc::GetDigit(number);
	for (int ii = digit; ii > 0; ii--) {
		count++;
		int index = (number / (int)pow(10, digit - ii)) % 10;

		indexAry.push_back(index);
		if (count % 3 == 0) {
			if (ii == 1) {
				continue;
			}
			indexAry.push_back(12); // 12はカンマ
		}
	}
	// 0が続く場合
	if (zeroDigit - digit > 0) {
		if (count % 3 == 0) {
			indexAry.push_back(12); // 12はカンマ
		}
	}
	for (int ii = 0; ii < zeroDigit - digit; ii++) {
		count++;
		indexAry.push_back(0);
		if (count % 3 == 0) {
			if (ii + digit == zeroDigit-1) {
				continue;
			}
			indexAry.push_back(12); // 12はカンマ
		}
	}
	return;
}

void CFunc::GetDigitArray2(long long number, std::vector<int>& indexAry, bool front /*= true*/)
{
	int digit = CFunc::GetDigit(number);
	for (int ii = digit; ii > 0; ii--) {
		int index = (number / (long long)pow(10, digit - ii)) % 10;
		if (front) {
			indexAry.insert(indexAry.begin(), index);
		}
		else {
			indexAry.push_back(index);
		}
	}
}
// 4 123 -> 0123
void CFunc::GetDigitArray3(int zeroDigit, long long number, std::vector<int>& indexAry, bool front /*= true*/)
{
	int count = 0;
	if (number == 0) {
		for (int ii = zeroDigit; ii > 0; ii--) {
			count++;
			if (front) {
				indexAry.insert(indexAry.begin(), 0);
			}
			else {
				indexAry.push_back(0);
			}
			if (count % 3 == 0) {
				if (ii == 1) {
					continue;
				}
			}
		}
		return;
	}

	int digit = CFunc::GetDigit(number);
	for (int ii = digit; ii > 0; ii--) {
		count++;
		int index = (number / (int)pow(10, digit - ii)) % 10;
		if (front) {
			indexAry.insert(indexAry.begin(), index);
		} else {
			indexAry.push_back(index);
		}
		if (count % 3 == 0) {
			if (ii == 1) {
				continue;
			}
		}
	}
	// 0が続く場合
	if (zeroDigit - digit > 0) {
		if (count % 3 == 0) {

		}
	}
	for (int ii = 0; ii < zeroDigit - digit; ii++) {
		count++;
		if (front) {
			indexAry.insert(indexAry.begin(), 0);
		}
		else {
			indexAry.push_back(0);
		}
		if (count % 3 == 0) {
			if (ii + digit == zeroDigit - 1) {
				continue;
			}
		}
	}
	return;
}

//1ベクトルの角度
double CFunc::GetOneVectorAngle(CPos p)
{
	return GetOneVectorAngle(p.x, p.y);
}
double CFunc::GetOneVectorAngle(double x1, double y1)
{
	return atan2(y1, x1);
}

double CFunc::GetOneVectorAngleDeg(CPos p)
{
	return GetOneVectorAngle(p.x, p.y) * CFunc::RAD;
}
double CFunc::GetOneVectorAngleDeg(double x1, double y1)
{
	return GetOneVectorAngle(x1, y1) * CFunc::RAD;
}


double CFunc::GetTwoPointAngle(CPos p1, CPos p2) {
	return GetTwoPointAngle(p1.x, p1.y, p2.x, p2.y);
}
double CFunc::GetTwoPointAngle(double x1, double y1, double x2, double y2) {
	return atan2((y1 - y2), (x1 - x2));
}
// +180度してあるので、そのままAddBulletに入力できる版
double CFunc::GetTwoPointAngle_180Deg(CPos p1, CPos p2)
{
	return  CFunc::ToDeg(GetTwoPointAngle(p1.x, p1.y, p2.x, p2.y)) +(180.0);
}

// ２つの円が重なっているか
bool CFunc::CollisionCircleToCircle(const CPos& p1, double p1Radius, const CPos& p2, double p2Radius)
{
	double x = p1.x - p2.x;
	double y = p1.y - p2.y;
	double r = x * x + y * y;
	double s = p1Radius * p1Radius + p2Radius * p2Radius;
	if (r < s) {
		return true;
	}
	return false;
}

// C++でフォルダ以下のファイル一覧を取得する
// https://qiita.com/tes2840/items/8d295b1caaf10eaf33ad
bool CFunc::GetFileNames(std::string folderPath, std::string extName, std::vector<std::string>& file_names)
{
	HANDLE hFind;
	WIN32_FIND_DATA win32fd;
	// extName : *.*
	std::string search_name = folderPath + "\\" + extName;

	hFind = FindFirstFile(search_name.c_str(), &win32fd);

	if (hFind == INVALID_HANDLE_VALUE) {
		throw std::runtime_error("file not found");
		return false;
	}

	/* 指定のディレクトリ以下のファイル名をファイルがなくなるまで取得する */
	do {
		if (win32fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
			/* ディレクトリの場合は何もしない */
			//printf("directory\n");
		}
		else {
			/* ファイルが見つかったらVector配列に保存する */
			file_names.push_back(win32fd.cFileName);
			//printf("%s\n", file_names.back().c_str());
		}
	} while (FindNextFile(hFind, &win32fd));

	FindClose(hFind);

	return true;
}



/**
 * エルミート曲線の座標リストを取得
 * @param p0 始点
 * @param v0 始点ベクトル
 * @param p1 終点
 * @param v1 終点ベクトル
 * @param DIVIDE 分解度
 * @return 座標リスト
 */
std::vector<CPos> CFunc::GetHermiteCurvePointList(
	const CPos&  p0, const CPos&  v0, const CPos&  p1, const CPos&  v1,
	int DIVIDE/*=16*/) {
	std::vector<CPos> pList;
	for (int i = 0; i < DIVIDE; i++) {
		int div = DIVIDE - 1;
		double u1 = i * 1.0 / (div);
		double u2 = u1 * u1;
		double u3 = u1 * u1 * u1;
		double mP0 = 2 * u3 - 3 * u2 + 0 + 1;
		double mV0 = u3 - 2 * u2 + u1;
		double mP1 = -2 * u3 + 3 * u2;
		double mV1 = u3 - u2;
		pList.push_back(CPos(p0.x * mP0 + v0.x * mV0 + p1.x * mP1 + v1.x * mV1,
			p0.y * mP0 + v0.y * mV0 + p1.y * mP1 + v1.y * mV1));
	}
	return pList;
}




/**
 * ベジェ曲線の座標リストを取得
 * @param p0 始点
 * @param p1 制御点１
 * @param p2 制御点２
 * @param p3 終点
 * @param DIVIDE 分解度
 * @return 座標リスト
 */
std::vector<CPos> CFunc::GetBezierCurvePointList(
	const CPos&  p0, const CPos&  p1, const CPos&  p2, const CPos&  p3,
	int DIVIDE/*=16*/) {
	std::vector<CPos> pList;
	for (int i = 0; i < DIVIDE; i++) {
		int div = DIVIDE - 1;
		double u = i * 1.0 / div;
		double mP0 = (1 - u) * (1 - u) * (1 - u);
		double mP1 = 3 * u * (1 - u) * (1 - u);
		double mP2 = 3 * u * u * (1 - u);
		double mP3 = u * u * u;
		pList.push_back(CPos(p0.x * mP0 + p1.x * mP1 + p2.x * mP2 + p3.x * mP3,
			p0.y * mP0 + p1.y * mP1 + p2.y * mP2 + p3.y * mP3));
	}
	return pList;
}


/**
 * Bスプライン曲線の座標リストを取得
 * @param p0 始点
 * @param p1 制御点１
 * @param p2 制御点２
 * @param p3 終点
 * @param DIVIDE 分解度
 * @return 座標リスト
 */
std::vector<CPos> CFunc::GetBSplineCurvePointList(
	const CPos&  p0, const CPos&  p1, const CPos&  p2, const CPos&  p3,
	int DIVIDE/*=16*/) {
	std::vector<CPos> pList;
	for (int i = 0; i < DIVIDE; i++) {
		int div = DIVIDE - 1;
		double u1 = i * 1.0 / div;
		double u2 = u1 * u1;
		double u3 = u1 * u1 * u1;
		double mP0 = (-1 * u3 + 3 * u2 - 3 * u1 + 1) * 1.0 / 6;
		double mP1 = (3 * u3 - 6 * u2 + 4) * 1.0 / 6;
		double mP2 = (-3 * u3 + 3 * u2 + 3 * u1 + 1) * 1.0 / 6;
		double mP3 = (1 * u3) * 1.0 / 6;
		pList.push_back(CPos(p0.x * mP0 + p1.x * mP1 + p2.x * mP2 + p3.x * mP3,
			p0.y * mP0 + p1.y * mP1 + p2.y * mP2 + p3.y * mP3));
	}
	return pList;
}

// 連続するCPos列の距離を計測する
double CFunc::GetCurvePointListDistance(const std::vector<CPos>& posArray)
{
	double dist = 0;
	for (int ii = 0; ii < posArray.size() -1; ii++) {
		CPos p1 = posArray[ii];
		CPos p2 = posArray[ii+1];
		dist += GetDistanceSqrt(p1, p2);
	}
	return dist;
}

// 回転させる
void CFunc::RotatingMatrix(CPos* resultPos, const CPos& RelationalPos, const CPos& RotationCenter, double angleRad) {
	double ox = RelationalPos.x - RotationCenter.x;
	double oy = RelationalPos.y - RotationCenter.y;
	resultPos->x = ox * cos(angleRad) + oy * sin(angleRad);
	resultPos->y = -ox * sin(angleRad) + oy * cos(angleRad);
	resultPos->x += RotationCenter.x;
	resultPos->y += RotationCenter.y;
}

double CFunc::GetDistanceSqrt(const CPos& p1, const CPos& p2)
{
	return sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y));
}
double CFunc::GetDistance(const CPos& p1, const CPos& p2)
{
	return ((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y));
}


CRect::CRect() {
	Set(0, 0, 0, 0);
}
CRect::CRect(CPos lt, CPos rb) {
	Set(lt.x, lt.y, rb.x, rb.y);
}
CRect::CRect(const double& x1, const double& y1, const double& x2, const double& y2) {
	Set(x1, y1, x2, y2);
}
void CRect::Set(const double& x1, const double& y1, const double& x2, const double& y2) {
	CPos p1 = CPos(x1, y1);
	CPos p2 = CPos(x2, y2);

	leftUp = p1;	//左上座標
	rightDown = p2;	//右下座標
}


