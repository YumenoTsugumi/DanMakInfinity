#pragma once

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <windows.h>
#include <vector>
#include <assert.h>

#include <iostream>

//座標を管理する構造体
class CPos{
public:
	double x,y;
	CPos(double X,double Y){
		this->x = X;
		this->y = Y;
	}
	CPos(const CPos& p){
		this->x = p.x;
		this->y = p.y;
	}
	CPos(){
		this->x = 0;
		this->y = 0;
	}
	~CPos(){
	}
	void operator=(const CPos& P){
		x = P.x;
		y = P.y;
	}
	void operator+=(const CPos& P){
		x += P.x;
		y += P.y;
	}
	void operator-=(const CPos& P){
		x -= P.x;
		y -= P.y;
	}
	void operator*=(const CPos& P){
		x *= P.x;
		y *= P.y;
	}
	void operator/=(const CPos& P){
		x /= P.x;
		y /= P.y;
	}

	// https://qiita.com/Reputeless/items/96226cfe1282a014b147 参考
	CPos operator +() const
	{
		return *this;
	}

	CPos operator -() const
	{
		return{ -x, -y };
	}

	CPos operator +(const CPos& other) const // 2項 +
	{
		return{ x + other.x, y + other.y };
	}

	CPos operator -(const CPos& other) const // 2項 -
	{
		return{ x - other.x, y - other.y };
	}

	CPos operator *(double s) const // 2項 *
	{
		return{ x * s, y * s };
	}

	CPos operator /(double s) const // 2項 /
	{
		return{ x / s, y / s };
	}


	void Unit(){
		double len;
		len = sqrt(x*x + y*y);

		if(len < (1e-6)){
			return ;
		}
		len = 1.0 / len;
		x *= len;
		y *= len;
	}
	double Length(){
		return sqrt(x*x + y*y);
	}
};

class CFunc{
public:
	//定数
	static const double PI;
	static const double RAD;
	static const double TOL1;
	static const double TOL2;
	static const double TOLZERO;
	static const int MAXSTRING;

	//---------------------------------
	//	インラインしたい
	//---------------------------------
	//2乗
	static double Square(double X);
	static int Square(int X);

	//deg->rad
	static double ToRad(double angle);
	//rad->deg
	static double ToDeg(double angle);

	//絶対値
	static int ToAbs(int VALUE);
	static double ToAbs(double VALUE);

	//乱数取得
	static int RandI(int MIN,int MAX);
	static double RandF(int MIN,int MAX);
	static double RandD(double MIN, double MAX);
	static bool RandB();
	//MINMAXの取得
	static double GetMax(double value1, double value2);
	static double GetMin(double value1, double value2);
	static int GetMax(int value1, int value2);
	static int GetMin(int value1, int value2);

	//log5(125) = 3.00
	//log9(81) = 2.00
	static double Logn(int base, double antilog);

	//桁を取得する
	//ex)23456->5
	static int GetDigit(int number);
	static int GetDigit(long long number);
	//	m_resultUI_Np,m_resultUI_Nd,m_resultUI_Nc
	//0123456789
	// 10[%]  11[.] 12[,]
	// 1234567
	// [0] : 7
	// [1] : 6
	// [2] : 5
	// [3] : ,
	// [4] : 4
	// [5] : 3
	static void GetDigitArray(int zeroumeDigit, long long number, std::vector<int>& indexAry);

	//数値だけの文字列に,をつけていく
	//input:1234567
	//output:1,234,567
	//output:2（,の数）
	// -1：エラー
	static int AddComma(char *str);

	//ダンプファイル
	//直下に入力文字列を吐く　ファイル名は dmp_"yyyymmdd"_+m_count(4ケタ)
	static void DumpFile(char *txt);
	//直下に入力名のファイルを吐く　中身は何も書かない
	static void DumpFile2(char *filename);

	//ダンプファイル	デバックだと吐く	あまり用途はないかもｗ
	//直下に入力文字列を吐く　ファイル名は dmp_"yyyymmdd"_+m_count(4ケタ)
	static void DumpFileDebug(char *txt);
	//直下に入力名のファイルを吐く　中身は何も書かない
	static void DumpFileDebug2(char *filename);


	//エルミート曲線の座標リストを取得
	//@param p0 始点	//@param v0 始点ベクトル
	//@param p1 終点	//@param v1 終点ベクトル
	//@param DIVIDE 分解度
	//@return 座標リスト
	static std::vector<CPos> GetHermiteCurvePointList(
			const CPos& p0, const CPos& v0, const CPos& p1, const CPos& v1,
			int DIVIDE=16);
	//ベジェ曲線の座標リストを取得
	//@param p0 始点	//@param p1 制御点１
	//@param p2 制御点２	//@param p3 終点
	//@param DIVIDE 分解度
	//@return 座標リスト
	static std::vector<CPos> GetBezierCurvePointList(
			const CPos& p0, const CPos& p1, const CPos& p2, const CPos& p3,
			int DIVIDE=16);
	//Bスプライン曲線の座標リストを取得
	//@param p0 始点	//@param p1 制御点１
	//@param p2 制御点２	//@param p3 終点
	//@param DIVIDE 分解度
	//@return 座標リスト
	static std::vector<CPos> GetBSplineCurvePointList(
			const CPos& p0, const CPos& p1, const CPos& p2, const CPos& p3,
			int DIVIDE=16);

	// 連続するCPos列の距離を計測する
	static double GetCurvePointListDistance(const std::vector<CPos>& posArray);

	// 回転
	static void RotatingMatrix(CPos *resultPos, const CPos& RelationalPos, const CPos& RotationCenter, double angleRad);

	//v1とv2の2次元内積
	static double InsideProduct(CPos v1, CPos v2);
	//v1とv2の2次元外積
	static double OutsideProduct(CPos v1, CPos v2);

	//1~2を結ぶ線分と3までの最近線の距離
	static double GetDistance(double x1, double y1, double x2, double y2, double x3, double y3);
	static double GetDistance(CPos p1, CPos p2, CPos p3);

	//1ベクトルの角度
	static double GetOneVectorAngle(CPos);
	static double GetOneVectorAngle(double x1, double y1);
	static double GetOneVectorAngleDeg(CPos);
	static double GetOneVectorAngleDeg(double x1, double y1);

	//2点の角度
	static double GetTwoPointAngle(CPos, CPos);
	static double GetTwoPointAngle(double x1, double y1, double x2, double y2);
	static double GetTwoPointAngle_180Deg(CPos, CPos); // +180度してかつDEGに戻してあるのであるので、そのままAddBulletに入力できる版

	//近似角度
	//targetAngle:32
	//nearAngle:10
	//targetAngleでnearAngleに近い角度を探す
	//output:30
	static double GetNearAngle(double targetAngle, double nearAngle);

	//線分同士の当たり判定
	//true	当たってる
	//false	当たっていない
	//(0,0)-(0,50)の線分 と (0,0)-(0,0)は当たっている判定
	static bool CheckLineCross(CPos p1, CPos p2, CPos p3, CPos p4);

	//2ベクトルの角度
	static double GetTwoVectorAngle(CPos &p1, CPos &p2);

	// 2点間の距離
	static double GetDistanceSqrt(const CPos& p1, const CPos& p2);
	static double GetDistance(const CPos& p1, const CPos& p2);

	// ２つの円が重なっているか
	static bool CollisionCircleToCircle(const CPos& p1, double p1Radius, const CPos& p2, double p2Radius);
};

//矩形領域
class CRect{
public:
	CRect();
	CRect(CPos lt, CPos rb);
	CRect(const double &x1, const double &y1, const double &x2, const double &y2);
	void Set(const double &x1, const double &y1, const double &x2, const double &y2);

	CPos leftUp;	//左上座標
	CPos rightDown;	//右下座標

};

// http://pyopyopyo.hatenablog.com/entry/2019/02/08/102456　参考
template <typename ... Args> std::string MyFormat(const std::string& fmt, Args ... args)
{
	size_t len = std::snprintf(nullptr, 0, fmt.c_str(), args ...);
	std::vector<char> buf(len + 1);
	std::snprintf(&buf[0], len + 1, fmt.c_str(), args ...);
	return std::string(&buf[0], &buf[0] + len);
}