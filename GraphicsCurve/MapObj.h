#pragma once
#include <afx.h>
#include "Afxtempl.h"
//想使用CArray必须包含此头文件
class MapObj :public CObject
{
public:
	int type;//所画图形的种类
	CArray<CPoint, CPoint> points;//所画图形的顶点
public:
	MapObj();
	~MapObj();
};

