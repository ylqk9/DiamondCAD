#include "stdafx.h"

class CVector2D
{
public:
	double x;
	double y;
	vector<int>	connection;
	CVector2D() : x(0.0), y(0.0) {};
	CVector2D(const double &x_in, const double &y_in) : x(x_in), y(y_in) {};
	virtual ~CVector2D() {};
};

class CVector3D
{
public:
	double	x;
	double	y;
	double	z;
	vector<int>	connection;
	CVector3D(const double &x_in, const double &y_in, const double &z_in) : x(x_in), y(y_in), z(z_in) {};
	virtual ~CVector3D() {};
};

CVector3D operator+(const CVector3D &v1, const CVector3D &v2) {
	return CVector3D(v1.x+v2.x, v1.y+v2.y, v1.z+v2.z); 
}

CVector3D operator*(const CVector3D &Vec, const double &Fac) {
	return CVector3D(Vec.x*Fac, Vec.y*Fac, Vec.z*Fac); 
}

CVector3D operator*(const double &Fac, const CVector3D &Vec) {
	return CVector3D(Vec.x*Fac, Vec.y*Fac, Vec.z*Fac); 
}

double Distance(const CVector3D &a, const CVector3D &b) {
	return sqrt((a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y) + (a.z - b.z)*(a.z - b.z));
}

CVector3D Cross_product(const CVector3D &a, const CVector3D &b) {
	return CVector3D(a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x);
}

void Rotate(const double &A, const double &B, const double &C, CVector3D &Vec) {
	CVector3D result(0.0, 0.0, 0.0);
	result.x = Vec.x*(cos(B)*cos(C) - cos(A)*sin(B)*sin(C)) + Vec.y*(cos(C)*sin(B) + cos(A)*cos(B)*sin(C)) + Vec.z*sin(C)*sin(A);
	result.y = Vec.x*(-sin(C)*cos(B) - cos(A)*sin(B)*cos(C)) + Vec.y*(-sin(C)*sin(B) + cos(A)*cos(B)*cos(C)) + Vec.z*cos(C)*sin(A);
	result.z = Vec.x*sin(A)*sin(B) - Vec.y*sin(A)*cos(B) + Vec.z*cos(A);
	Vec.x = result.x;
	Vec.y = result.y;
	Vec.z = result.z;
	return;
}

void Rotate(const double &A, const double &B, const double &C, vector<CVector3D> &Vec) {
	for(auto &i : Vec) Rotate(A, B, C, i);
}

void Project2xy(const vector<CVector3D> &Vec3D, vector<CVector2D> &Vec2D) {
	Vec2D.reserve(Vec3D.size());
	for(auto &i : Vec3D) {
		Vec2D.push_back(CVector2D(i.x, i.y));
		Vec2D.back().connection = i.connection;
	}
}

void GetPointsOnCircle(const int &NPoints, const CVector3D &Normal, const CVector3D &Center, const double &Radius, const CVector3D &InitVec, vector<CVector3D> &result) {
	CVector3D v1 = InitVec;
	CVector3D v2 = Cross_product(Normal, InitVec);
	for(int i = 0; i < NPoints; ++i) {
		double angle = i*2*3.14159265358979/NPoints;
		CVector3D NewPoint(v1*Radius*cos(angle) + v2*Radius*sin(angle) + Center);
		result.push_back(NewPoint);
	}
}


void Have_Some_Fun(std::vector<CVector2D> &shape)
{
	const double TAngle(1.71915);
	const double WAngle(0.6);
	const int NSides(9);
	vector<int> OffSetDiamond3D;
	int OffSetSecond2Last(0);
	ofstream f("diamond.txt");
	vector<CVector3D> Diamond;
	Diamond.reserve(1000);
	OffSetDiamond3D.push_back(1);
	Diamond.push_back(CVector3D(0.0, 0.0, 0.0));
	
	double intersection1 = cos(TAngle/2.0);
	double height1 = sin(TAngle/2.0);
	GetPointsOnCircle(NSides, CVector3D(0.0, 0.0, 1.0), CVector3D(0.0, 0.0, height1), intersection1, CVector3D(1.0, 0.0, 0.0), Diamond);
	for(int i = 0; i < NSides; ++i) Diamond[i + OffSetDiamond3D.back()].connection.push_back(0);
	OffSetSecond2Last = OffSetDiamond3D.back();
	OffSetDiamond3D.push_back(OffSetDiamond3D.back() + NSides);

	double intersection2 = intersection1*4.0;
	double height2 = height1*4.0;
	GetPointsOnCircle(2*NSides, CVector3D(0.0, 0.0, 1.0), CVector3D(0.0, 0.0, height2), intersection2, CVector3D(1.0, 0.0, 0.0), Diamond);
	for(int i = 0; i < 2*NSides; ++i) {
		Diamond[i + OffSetDiamond3D.back()].connection.push_back(OffSetSecond2Last + i/2);
		if(i%2 == 0) continue;
		if(OffSetSecond2Last + i/2 + 1 < OffSetDiamond3D.back()) {
			Diamond[i + OffSetDiamond3D.back()].connection.push_back(OffSetSecond2Last + i/2 + 1);
		} else {
			Diamond[i + OffSetDiamond3D.back()].connection.push_back(OffSetSecond2Last + i/2 + 1 - (OffSetDiamond3D.back() - OffSetSecond2Last));
		}
	}
	for(int i = 0; i < 2*NSides - 1; ++i) Diamond[i + OffSetDiamond3D.back()].connection.push_back(i + OffSetDiamond3D.back() + 1);
	Diamond[2*NSides - 1 + OffSetDiamond3D.back()].connection.push_back(0 + OffSetDiamond3D.back());
	OffSetSecond2Last = OffSetDiamond3D.back();
	OffSetDiamond3D.push_back(OffSetDiamond3D.back() + 2*NSides);

	double intersection3 = intersection2 - cos(WAngle) + 0.1;
	double height3 = height2 + sin(WAngle);
	GetPointsOnCircle(NSides, CVector3D(0.0, 0.0, 1.0), CVector3D(0.0, 0.0, height3), intersection3, CVector3D(1.0, 0.0, 0.0), Diamond);
	for(int i = 0; i < NSides; ++i) {
		Diamond[i + OffSetDiamond3D.back()].connection.push_back(OffSetSecond2Last + i*2);
		if(OffSetSecond2Last + i*2 - 1 >= OffSetSecond2Last) {
			Diamond[i + OffSetDiamond3D.back()].connection.push_back(OffSetSecond2Last + i*2 - 1);
		} else {
			Diamond[i + OffSetDiamond3D.back()].connection.push_back(OffSetSecond2Last + i*2 - 1 + (OffSetDiamond3D.back() - OffSetSecond2Last));
		}
		if(OffSetSecond2Last + i*2 + 1 < OffSetDiamond3D.back()) {
			Diamond[i + OffSetDiamond3D.back()].connection.push_back(OffSetSecond2Last + i*2 + 1);
		} else {
			Diamond[i + OffSetDiamond3D.back()].connection.push_back(OffSetSecond2Last + i*2 + 1 - (OffSetDiamond3D.back() - OffSetSecond2Last));
		}
	}	
	OffSetSecond2Last = OffSetDiamond3D.back();
	OffSetDiamond3D.push_back(OffSetDiamond3D.back() + NSides);

	double intersection4 = intersection2 - 1.5*cos(0.6);
	double height4 = height2 + 1.5*sin(0.6);
	GetPointsOnCircle(NSides, CVector3D(0.0, 0.0, 1.0), CVector3D(0.0, 0.0, height4), intersection4, CVector3D(1.0*cos(3.14159265358979/NSides), 1.0*sin(3.14159265358979/NSides), 0.0), Diamond);
	for(int i = 0; i < NSides; ++i) {
		Diamond[i + OffSetDiamond3D.back()].connection.push_back(OffSetSecond2Last + i);
		if(OffSetSecond2Last + i + 1 < OffSetDiamond3D.back()) {
			Diamond[i + OffSetDiamond3D.back()].connection.push_back(OffSetSecond2Last + i + 1);
		} else {
			Diamond[i + OffSetDiamond3D.back()].connection.push_back(OffSetSecond2Last + i + 1 - (OffSetDiamond3D.back() - OffSetSecond2Last));
		}
	}
	for(int i = 0; i < NSides - 1; ++i) Diamond[i + OffSetDiamond3D.back()].connection.push_back(i + OffSetDiamond3D.back() + 1);
	Diamond[NSides - 1 + OffSetDiamond3D.back()].connection.push_back(0 + OffSetDiamond3D.back());

	Rotate(3.14159/2.0 - 1, 1.0, 0.0, Diamond);
	
	Project2xy(Diamond, shape);
	cout.precision(10);
	std::setw(10);
	//for(auto &i : Diamond) {
	//	if(i.connection.size() > 0) {
	//		for(auto &c : (i.connection)) {
	//			f << i.x << " " << i.y << " " << i.z << endl;
	//			f << Diamond[c].x << " " << Diamond[c].y << " " << Diamond[c].z << endl;
	//			f << endl;
	//			f << endl;
	//		}
	//	}
	//}
	for(auto &i : shape) {
		if(i.connection.size() > 0) {
			for(auto &c : (i.connection)) {
				f << i.x << " " << i.y << endl;
				f << shape[c].x << " " << shape[c].y << endl;
				f << endl;
				f << endl;
			}
		}
	}
	return;
}
