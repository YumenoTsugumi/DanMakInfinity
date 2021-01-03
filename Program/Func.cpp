


#include "DxLib.h"
#include "Func.h"

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
	return MIN + (int)(rand() * (MAX - MIN + 1) / (1 + RAND_MAX));
}
double CFunc::RandF(int MIN, int MAX) {
	return MIN + (double)(rand() * (MAX - MIN + 1) / (1 + RAND_MAX));
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


void CFunc::RotatingMatrix(CPos* P1, CPos P2, CPos P3, double angle) {
	double ox = P2.x - P3.x;
	double oy = P2.y - P3.y;
	P1->x = ox * cos(angle) + oy * sin(angle);
	P1->y = -ox * sin(angle) + oy * cos(angle);
	P1->x += P3.x;
	P1->y += P3.y;
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
CRect::CRect(const double& x1, const double& y1, const double& x2, const double& y2) {
	Set(x1, y1, x2, y2);
}
void CRect::Set(const double& x1, const double& y1, const double& x2, const double& y2) {
	CPos p1 = CPos(x1, y1);
	CPos p2 = CPos(x2, y2);

	leftUp = p1;	//左上座標
	rightDown = p2;	//右下座標
}


