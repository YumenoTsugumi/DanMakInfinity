﻿#pragma once

#include "Func.h"



class CDxFunc {
public:
	static double m_windowRatio;
	static void SetWindowRatio(double windowRatio) {
		m_windowRatio = windowRatio;
	}

	//普通描画
	static int MyDrawGraph(double x, double y,  int GrHandle, int TransFlag = TRUE);
	static int MyDrawGraph(CPos p, int GrHandle, int TransFlag = TRUE);

	//普通描画
	static int MyDrawRotaGraph(double x, double y, double ExtRate, double Angle, int GrHandle, int TransFlag = TRUE, int TurnFlag = FALSE);
	static int MyDrawRotaGraph(CPos p, double ExtRate, double Angle, int GrHandle, int TransFlag = TRUE, int TurnFlag = FALSE);

	////回転中心指示
	//static int MyDrawRotaGraph2(double x1, double y1, double x2, double y2, double ExtRate, double Angle, int GrHandle, int TransFlag = TRUE, int TurnFlag = FALSE);
	//static int MyDrawRotaGraph2(CPos p1, CPos p2, double ExtRate, double Angle, int GrHandle, int TransFlag = TRUE, int TurnFlag = FALSE);

	////回転中心指示3
	//static int MyDrawRotaGraph3(double x1, double y1, double x2, double y2, double ExtRateX, double ExtRateY, double Angle, int GrHandle, int TransFlag = TRUE, int TurnFlag = FALSE);
	//static int MyDrawRotaGraph3(CPos p1, CPos p2, double ExtRateX, double ExtRateY, double Angle, int GrHandle, int TransFlag = TRUE, int TurnFlag = FALSE);

	//四角
	static int DrawBox2(double x1, double y1, double x2, double y2, int cr, int FillFlag = TRUE);
	static int DrawBox2(CPos p1, CPos p2, int cr, int FillFlag = TRUE);

	//文字
	static int DrawString(double x1, double y1, const char* string, int fontCr, int m_font, int edgeCr);
	static int DrawString(CPos p1, const char* string, int fontCr, int m_font, int edgeCr);

	//丸
	static int DrawCircle(CPos p1, double r, int posnum, unsigned int Color, int FillFlag);

	//三角
	//static int DrawTriangle(double x1, double y1, double x2, double y2, double x3, double y3, int cr, int FillFlag = TRUE);
	//static int DrawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, int cr, int FillFlag = TRUE);
	//static int DrawTriangle(CPos p1, CPos p2, CPos p3, int cr, int FillFlag = TRUE);

};

class CFPSManager {
	int m_timeAry[60];	//1sの平均を出すために値を覚えておく
	int m_frameCount;	//frをインクリメントする　0～59をループする

	int m_startTime;	//start時の時間

	bool m_skipFlg;	//1frが16超えてたら
					//次を描画を飛ばす？

public:
	CFPSManager();
	~CFPSManager();

	void Start();
	void End();

	double GetFPS();
	bool GetSkipFlg();
};

