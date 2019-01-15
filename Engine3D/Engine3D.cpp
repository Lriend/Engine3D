#include "olcConsoleGameEngine.h"
using namespace std;


struct vec3d
{
	float x, y, z;
};

struct triangle
{
	vec3d p[3];
};

struct mesh
{
	vector<triangle> tris;
};

struct mat4x4
{
	float m[4][4] = { 0 };
};

class olcEngine3D : public olcConsoleGameEngine
{
public:
	olcEngine3D()
	{
		m_sAppName = L"3D Demo";
	}


private:
	mesh meshCube;
	mat4x4 matProj;

	vec3d vCamera;

	float fKapitanWarszawa;

	void MnozonkoMacierzyyyyyyVec(vec3d &i, vec3d &o, mat4x4 &m)
	{
		o.x = i.x * m.m[0][0] + i.y * m.m[1][0] + i.z * m.m[2][0] + m.m[3][0];
		o.y = i.x * m.m[0][1] + i.y * m.m[1][1] + i.z * m.m[2][1] + m.m[3][1];
		o.z = i.x * m.m[0][2] + i.y * m.m[1][2] + i.z * m.m[2][2] + m.m[3][2];
		float w = i.x * m.m[0][3] + i.y * m.m[1][3] + i.z * m.m[2][3] + m.m[3][3];

		if (w != 0.0f)
		{
			o.x /= w; o.y /= w; o.z /= w;
		}
	}

public:
	bool OnUserCreate() override
	{
		meshCube.tris = {

			// S
			{ 0.0f, 0.0f, 0.0f,    0.0f, 1.0f, 0.0f,    1.0f, 1.0f, 0.0f },
			{ 0.0f, 0.0f, 0.0f,    1.0f, 1.0f, 0.0f,    1.0f, 0.0f, 0.0f },

			// E                                                     
			{ 1.0f, 0.0f, 0.0f,    1.0f, 1.0f, 0.0f,    1.0f, 1.0f, 1.0f },
			{ 1.0f, 0.0f, 0.0f,    1.0f, 1.0f, 1.0f,    1.0f, 0.0f, 1.0f },

			// N                                                   
			{ 1.0f, 0.0f, 1.0f,    1.0f, 1.0f, 1.0f,    0.0f, 1.0f, 1.0f },
			{ 1.0f, 0.0f, 1.0f,    0.0f, 1.0f, 1.0f,    0.0f, 0.0f, 1.0f },

			// W                                                     
			{ 0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 1.0f,    0.0f, 1.0f, 0.0f },
			{ 0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 0.0f,    0.0f, 0.0f, 0.0f },

			// T                                                     
			{ 0.0f, 1.0f, 0.0f,    0.0f, 1.0f, 1.0f,    1.0f, 1.0f, 1.0f },
			{ 0.0f, 1.0f, 0.0f,    1.0f, 1.0f, 1.0f,    1.0f, 1.0f, 0.0f },

			// B                                                 
			{ 1.0f, 0.0f, 1.0f,    0.0f, 0.0f, 1.0f,    0.0f, 0.0f, 0.0f },
			{ 1.0f, 0.0f, 1.0f,    0.0f, 0.0f, 0.0f,    1.0f, 0.0f, 0.0f },

		};

		// Macierz Rzutowania
		float fNear = 0.1f;
		float fFar = 1000.0f;
		float fFov = 90.0f;
		float fAspectRatio = (float)ScreenHeight() / (float)ScreenWidth();
		float fFovRad = 1.0f / tanf(fFov * 0.5f / 180.0f * 3.14159f);

		matProj.m[0][0] = fAspectRatio * fFovRad;
		matProj.m[1][1] = fFovRad;
		matProj.m[2][2] = fFar / (fFar - fNear);
		matProj.m[3][2] = (-fFar * fNear) / (fFar - fNear);
		matProj.m[2][3] = 1.0f;
		matProj.m[3][3] = 0.0f;

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		// Czyszczenie ekranu
		Fill(0, 0, ScreenWidth(), ScreenHeight(), PIXEL_SOLID, FG_BLACK);

		// Ustawienia macierzy dot. rottttttowaniaaaaaa!!!!!
		mat4x4 matRotZ, matRotX;
		fKapitanWarszawa += 1.0f * fElapsedTime;

		// Rotacja po Z
		matRotZ.m[0][0] = cosf(fKapitanWarszawa);
		matRotZ.m[0][1] = sinf(fKapitanWarszawa);
		matRotZ.m[1][0] = -sinf(fKapitanWarszawa);
		matRotZ.m[1][1] = cosf(fKapitanWarszawa);
		matRotZ.m[2][2] = 1;
		matRotZ.m[3][3] = 1;

		// Rotacja po X
		matRotX.m[0][0] = 1;
		matRotX.m[1][1] = cosf(fKapitanWarszawa * 0.5f);
		matRotX.m[1][2] = sinf(fKapitanWarszawa * 0.5f);
		matRotX.m[2][1] = -sinf(fKapitanWarszawa * 0.5f);
		matRotX.m[2][2] = cosf(fKapitanWarszawa * 0.5f);
		matRotX.m[3][3] = 1;

		// Rysowanie trójkontóf
		for (auto tri : meshCube.tris)
		{
			triangle triProjected, triTranslated, triRotatedZ, triRotatedZX;

			// Rotowanie po Z
			MnozonkoMacierzyyyyyyVec(tri.p[0], triRotatedZ.p[0], matRotZ);
			MnozonkoMacierzyyyyyyVec(tri.p[1], triRotatedZ.p[1], matRotZ);
			MnozonkoMacierzyyyyyyVec(tri.p[2], triRotatedZ.p[2], matRotZ);

			// Rotowanie po X
			MnozonkoMacierzyyyyyyVec(triRotatedZ.p[0], triRotatedZX.p[0], matRotX);
			MnozonkoMacierzyyyyyyVec(triRotatedZ.p[1], triRotatedZX.p[1], matRotX);
			MnozonkoMacierzyyyyyyVec(triRotatedZ.p[2], triRotatedZX.p[2], matRotX);

			// Offset
			triTranslated = triRotatedZX;
			triTranslated.p[0].z = triRotatedZX.p[0].z + 3.0f;
			triTranslated.p[1].z = triRotatedZX.p[1].z + 3.0f;
			triTranslated.p[2].z = triRotatedZX.p[2].z + 3.0f;

			//Vector-product tzw. cross-product łamane na uzyskiwanie normala
			vec3d normal, line1, line2;
			line1.x = triTranslated.p[1].x - triTranslated.p[0].x;
			line1.y = triTranslated.p[1].y - triTranslated.p[0].y;
			line1.z = triTranslated.p[1].z - triTranslated.p[0].z;

			line2.x = triTranslated.p[2].x - triTranslated.p[0].x;
			line2.y = triTranslated.p[2].y - triTranslated.p[0].y;
			line2.z = triTranslated.p[2].z - triTranslated.p[0].z;

			normal.x = line1.y * line2.z - line1.z * line2.y;
			normal.y = line1.z * line2.x - line1.x * line2.z;
			normal.z = line1.x * line2.y - line1.y * line2.x;

			//Normalnie normalnym jest normalizowanie normala
			float l = sqrtf(normal.x*normal.x + normal.y*normal.y + normal.z*normal.z);
			normal.x /= l; normal.y /= l; normal.z /= l;

			//if (normal.z < 0) {
			if (normal.x*(triTranslated.p[0].x - vCamera.x) +
			   normal.y*(triTranslated.p[0].y - vCamera.y) +
			   normal.z*(triTranslated.p[0].z - vCamera.z) < 0.0f){

				// 3D na 2D
				MnozonkoMacierzyyyyyyVec(triTranslated.p[0], triProjected.p[0], matProj);
				MnozonkoMacierzyyyyyyVec(triTranslated.p[1], triProjected.p[1], matProj);
				MnozonkoMacierzyyyyyyVec(triTranslated.p[2], triProjected.p[2], matProj);

				// Skalowanie widoku
				triProjected.p[0].x += 1.0f; triProjected.p[0].y += 1.0f;
				triProjected.p[1].x += 1.0f; triProjected.p[1].y += 1.0f;
				triProjected.p[2].x += 1.0f; triProjected.p[2].y += 1.0f;
				triProjected.p[0].x *= 0.5f * (float)ScreenWidth();
				triProjected.p[0].y *= 0.5f * (float)ScreenHeight();
				triProjected.p[1].x *= 0.5f * (float)ScreenWidth();
				triProjected.p[1].y *= 0.5f * (float)ScreenHeight();
				triProjected.p[2].x *= 0.5f * (float)ScreenWidth();
				triProjected.p[2].y *= 0.5f * (float)ScreenHeight();

				// Rastrowanie trójkontóf
				DrawTriangle(triProjected.p[0].x, triProjected.p[0].y,
					triProjected.p[1].x, triProjected.p[1].y,
					triProjected.p[2].x, triProjected.p[2].y,
					PIXEL_SOLID, FG_WHITE);
			}
		}


		return true;
	}

};

int main()
{
	olcEngine3D demo;
	if (demo.ConstructConsole(256, 240, 4, 4))
		demo.Start();
	return 0;
}