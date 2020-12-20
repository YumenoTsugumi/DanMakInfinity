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
	static const int MAXSTRING;

	//---------------------------------
	//	インラインしたい
	//---------------------------------
	//2乗
	static double Square(double X);
	static int Square(int X);

	//deg->rad
	static double ToRad(double ANGLE);
	//rad->deg
	static double ToDeg(double ANGLE);

	//絶対値
	static int ToAbs(int VALUE);
	static double ToAbs(double VALUE);

	//乱数取得
	static int RandI(int MIN,int MAX);
	static double RandF(int MIN,int MAX);

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
	static int GetDigit(int NUMBER);

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
			CPos *p0, CPos *v0, CPos *p1, CPos *v1,
			int DIVIDE=16);
	//ベジェ曲線の座標リストを取得
	//@param p0 始点	//@param p1 制御点１
	//@param p2 制御点２	//@param p3 終点
	//@param DIVIDE 分解度
	//@return 座標リスト
	static std::vector<CPos> GetBezierCurvePointList(
			CPos *p0, CPos *p1, CPos *p2, CPos *p3,
			int DIVIDE=16);
	//Bスプライン曲線の座標リストを取得
	//@param p0 始点	//@param p1 制御点１
	//@param p2 制御点２	//@param p3 終点
	//@param DIVIDE 分解度
	//@return 座標リスト
	static std::vector<CPos> GetBSplineCurvePointList(
			CPos *p0, CPos *p1, CPos *p2, CPos *p3,
			int DIVIDE=16);

	//P2に対して...不明
	//P1は結果
	static void RotatingMatrix(CPos *P1, CPos P2, CPos P3, double ANGLE);

	//v1とv2の2次元内積
	static double InsideProduct(CPos v1, CPos v2);
	//v1とv2の2次元外積
	static double OutsideProduct(CPos v1, CPos v2);

	//1~2を結ぶ線分と3までの最近線の距離
	static double GetDistance(double x1, double y1, double x2, double y2, double x3, double y3);
	static double GetDistance(CPos p1, CPos p2, CPos p3);

	//2点の角度
	static double GetTwoPointAngle(CPos, CPos);
	static double GetTwoPointAngle(double x1, double y1, double x2, double y2);

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

};

//矩形領域
class CRect{
public:
	CRect();
	CRect(const double &x1, const double &y1, const double &x2, const double &y2);
	void Set(const double &x1, const double &y1, const double &x2, const double &y2);

	CPos leftUp;	//左上座標
	CPos rightDown;	//右下座標

};
