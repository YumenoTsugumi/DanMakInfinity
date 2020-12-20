#include "DXFunc.h"
#include "DxLib.h"

int CDxFunc::DrawRotaGraph(double x, double y, double ExtRate, double Angle, int GrHandle) {
	return DrawRotaGraphF((float)x, (float)y, ExtRate, Angle, GrHandle, TRUE);
}
int CDxFunc::DrawRotaGraph(CPos p, double ExtRate, double Angle, int GrHandle) {
	//↑のをコール
	return DrawRotaGraph(p.x, p.y, ExtRate, Angle, GrHandle);
}


int CDxFunc::DrawRotaGraph2(double x1, double y1, double x2, double y2, double ExtRate, double Angle, int GrHandle) {
	return DrawRotaGraph2F((float)x1, (float)y1, (float)x2, (float)y2,
		ExtRate, Angle, GrHandle, TRUE);
}
int CDxFunc::DrawRotaGraph2(CPos p1, CPos p2, double ExtRate, double Angle, int GrHandle) {
	//↑のをコール
	return DrawRotaGraph2(p1.x, p1.y, p2.x, p2.y, ExtRate, Angle, GrHandle);
}


int CDxFunc::DrawBox(double x1, double y1, double x2, double y2, int cr, int FillFlag) {
	return DrawBox((int)x1, (int)y1, (int)x2, (int)y2, cr, TRUE);
}
int CDxFunc::DrawBox(CPos p1, CPos p2, int cr, int FillFlag) {
	return DrawBox(p1.x, p1.y, p2.x, p2.y, cr, FillFlag);
}


int CDxFunc::DrawString(double x1, double y1, const char* string, int fontCr, int font, int edgeCr) {
	return DrawStringToHandle((int)x1, (int)y1, string, fontCr, font, edgeCr);
}
int CDxFunc::DrawString(CPos p1, const char* string, int fontCr, int font, int edgeCr) {
	return CDxFunc::DrawString(p1.x, p1.y, string, fontCr, font, edgeCr);
}


int CDxFunc::DrawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, int cr, int FillFlag) {
	return DrawTriangle(x1, y1, x2, y2, x3, y3, cr, FillFlag);
}
int CDxFunc::DrawTriangle(double x1, double y1, double x2, double y2, double x3, double y3, int cr, int FillFlag) {
	return DrawTriangle((int)x1, (int)y1, (int)x2, (int)y2, (int)x3, (int)y3, cr, FillFlag);
}
int CDxFunc::DrawTriangle(CPos p1, CPos p2, CPos p3, int cr, int FillFlag) {
	return DrawTriangle((int)(p1.x), (int)(p1.y), (int)(p2.x), (int)(p2.y), (int)(p3.x), (int)(p3.y), cr, FillFlag);
}


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



