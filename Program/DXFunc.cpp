#include "DXFunc.h"
#include "DxLib.h"

int CDxFunc::MyDrawRotaGraph(double x, double y, double ExtRate, double Angle, int GrHandle, int TransFlag, int TurnFlag) {
	return DrawRotaGraphF((float)x, (float)y, ExtRate, Angle, GrHandle, TransFlag, TurnFlag);
}
int CDxFunc::MyDrawRotaGraph(CPos p, double ExtRate, double Angle, int GrHandle, int TransFlag, int TurnFlag) {
	//↑のをコール
	return DrawRotaGraphF((float)p.x, (float)p.y, ExtRate, Angle, GrHandle, TransFlag, TurnFlag);
}


int CDxFunc::MyDrawRotaGraph2(double x1, double y1, double x2, double y2, double ExtRate, double Angle, int GrHandle, int TransFlag, int TurnFlag) {
	return DrawRotaGraph2F((float)x1, (float)y1, (float)x2, (float)y2,
		ExtRate, Angle, GrHandle, TransFlag, TurnFlag);
}
int CDxFunc::MyDrawRotaGraph2(CPos p1, CPos p2, double ExtRate, double Angle, int GrHandle, int TransFlag, int TurnFlag) {
	//↑のをコール
	return MyDrawRotaGraph2(p1.x, p1.y, p2.x, p2.y, ExtRate, Angle, GrHandle, TransFlag, TurnFlag);
}


//回転中心指示3
int CDxFunc::MyDrawRotaGraph3(double x1, double y1, double x2, double y2, double ExtRateX, double ExtRateY, double Angle, int GrHandle, int TransFlag, int TurnFlag)
{
	return DrawRotaGraph3F((float)x1, (float)y1, (float)x2, (float)y2, ExtRateX, ExtRateY, Angle, GrHandle, TransFlag, TurnFlag);
}
int CDxFunc::MyDrawRotaGraph3(CPos p1, CPos p2, double ExtRateX, double ExtRateY, double Angle, int GrHandle, int TransFlag, int TurnFlag)
{
	return MyDrawRotaGraph3(p1.x, p1.y, p2.x, p2.y, ExtRateX, ExtRateY, Angle, GrHandle, TransFlag, TurnFlag);
}


int CDxFunc::DrawBox2(double x1, double y1, double x2, double y2, int cr, int FillFlag) {
	return DrawBox((int)x1, (int)y1, (int)x2, (int)y2, cr, TRUE);
}
int CDxFunc::DrawBox2(CPos p1, CPos p2, int cr, int FillFlag) {
	return DrawBox2(p1.x, p1.y, p2.x, p2.y, cr, FillFlag);
}


int CDxFunc::DrawString(double x1, double y1, const char* string, int fontCr, int m_font, int edgeCr) {
	return DrawStringToHandle((int)x1, (int)y1, string, fontCr, m_font, edgeCr);
}
int CDxFunc::DrawString(CPos p1, const char* string, int fontCr, int m_font, int edgeCr) {
	return CDxFunc::DrawString(p1.x, p1.y, string, fontCr, m_font, edgeCr);
}

int CDxFunc::DrawCircle(CPos p1, double r, int posnum, unsigned int color, int fillFlag)
{
	return DrawCircleAA((float)p1.x, (float)p1.y, (float)r, posnum, color, fillFlag);
}


//int CDxFunc::DrawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, int cr, int FillFlag) {
//	return DrawTriangle(x1, y1, x2, y2, x3, y3, cr, FillFlag);
//}
//int CDxFunc::DrawTriangle(double x1, double y1, double x2, double y2, double x3, double y3, int cr, int FillFlag) {
//	return DrawTriangle((int)x1, (int)y1, (int)x2, (int)y2, (int)x3, (int)y3, cr, FillFlag);
//}
//int CDxFunc::DrawTriangle(CPos p1, CPos p2, CPos p3, int cr, int FillFlag) {
//	return DrawTriangle((int)(p1.x), (int)(p1.y), (int)(p2.x), (int)(p2.y), (int)(p3.x), (int)(p3.y), cr, FillFlag);
//}


//------------------------------------------------------
//	CFPSManager
//------------------------------------------------------
CFPSManager::CFPSManager() {
	m_startTime = GetNowCount();

	memset(m_timeAry, 0, sizeof(m_timeAry));
	m_frameCount = 0;

	m_skipFlg = false;
}
CFPSManager::~CFPSManager() {

}
void CFPSManager::Start() {
	m_startTime = GetNowCount();
}
void CFPSManager::End() {
	int waitTime = GetNowCount() - m_startTime;

	m_timeAry[m_frameCount] = waitTime;
	m_frameCount++;
	if (m_frameCount >= 60) {
		m_frameCount = 0;
	}

	if (16 - waitTime > 0) {
		m_skipFlg = false;
		WaitTimer(16 - waitTime);
	}
	else {
		m_skipFlg = true;
	}
}


double CFPSManager::GetFPS() {
	double avr = 0.0;
	for (int i = 0; i < 60; i++) {
		avr += (double)(m_timeAry[i]);
	}
	return (avr / 60.0);
}

bool CFPSManager::GetSkipFlg() {
	return m_skipFlg;
}



