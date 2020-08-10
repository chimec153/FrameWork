#pragma once

#define PI	3.142592f

#include <math.h>

static float RadToDeg(float fRad)
{
	return fRad * 180.f / PI;
}

static float DegToRad(float fDeg)
{
	return fDeg * PI / 180.f;
}

typedef struct _tagResolution
{
	unsigned int iW;
	unsigned int iH;

	_tagResolution() :
		iW(0),
		iH(0)
	{

	}

	_tagResolution(int x, int y) :
		iW(x),
		iH(y)
	{

	}
}RESOLUTION, *PRESOLUTION;

typedef struct _tagPosition
{
	float	x, y;

	_tagPosition() :
		x(0.f),
		y(0.f)
	{
	}

	_tagPosition(float _x, float _y) :
		x(_x),
		y(_y)
	{
	}

	_tagPosition(const _tagPosition& pos)
	{
		x = pos.x;
		y = pos.y;
	}

	_tagPosition(const POINT& pt)
	{
		x = (float)pt.x;
		y = (float)pt.y;
	}

	void operator = (const _tagPosition& pos)
	{
		x = pos.x;
		y = pos.y;
	}

	void operator = (const POINT& pt)
	{
		x = (float)pt.x;
		y = (float)pt.y;
	}
	void operator = (float f[2])
	{
		x = f[0];
		y = f[1];
	}

	void operator = (float fValue)
	{
		x = fValue;
		y = fValue;
	}

	bool operator == (const _tagPosition& pos)
	{
		return x == pos.x && y == pos.y;
	}

	bool operator != (const _tagPosition& pos)
	{
		return x != pos.x || y != pos.y;
	}

	_tagPosition operator + (const _tagPosition& pos)	const
	{
		_tagPosition	tPos;
		tPos.x = x + pos.x;
		tPos.y = y + pos.y;
		return tPos;
	}
	_tagPosition operator + (const POINT& pos)	const
	{
		_tagPosition	tPos;
		tPos.x = x + pos.x;
		tPos.y = y + pos.y;
		return tPos;
	}
	_tagPosition operator + (float f[2])	const
	{
		_tagPosition	tPos;
		tPos.x = x + f[0];
		tPos.y = y + f[1];
		return tPos;
	}

	_tagPosition operator + (float f)	const
	{
		_tagPosition	tPos;
		tPos.x = x + f;
		tPos.y = y + f;
		return tPos;
	}
	void operator += (const _tagPosition& pos)
	{
		x +=pos.x;
		y += pos.y;
	}

	_tagPosition operator - (const _tagPosition& pos)	const
	{
		_tagPosition	tPos;
		tPos.x = x - pos.x;
		tPos.y = y - pos.y;
		return tPos;
	}
	_tagPosition operator - (const POINT& pos)	const
	{
		_tagPosition	tPos;
		tPos.x = x - pos.x;
		tPos.y = y - pos.y;
		return tPos;
	}
	_tagPosition operator - (float f[2])	const
	{
		_tagPosition	tPos;
		tPos.x = x - f[0];
		tPos.y = y - f[1];
		return tPos;
	}
	_tagPosition operator - (float f)	const
	{
		_tagPosition	tPos;
		tPos.x = x - f;
		tPos.y = y - f;
		return tPos;
	}

	friend _tagPosition operator - (const _tagPosition& tPos)
	{
		_tagPosition _tPos = tPos;

		_tPos.x = -tPos.x;
		_tPos.y = -tPos.y;

		return _tPos;
	}

	void operator -= (const _tagPosition& pos)
	{
		x -= pos.x;
		y -= pos.y;
	}

	_tagPosition operator * (const _tagPosition& pos)	const
	{
		_tagPosition	tPos;
		tPos.x = x * pos.x;
		tPos.y = y * pos.y;
		return tPos;
	}
	_tagPosition operator * (const POINT& pos)	const
	{
		_tagPosition	tPos;
		tPos.x = x * pos.x;
		tPos.y = y * pos.y;
		return tPos;
	}
	_tagPosition operator * (float f[2])	const
	{
		_tagPosition	tPos;
		tPos.x = x * f[0];
		tPos.y = y * f[1];
		return tPos;
	}
	_tagPosition operator * (float f)	const
	{
		_tagPosition	tPos;
		tPos.x = x * f;
		tPos.y = y * f;
		return tPos;
	}

	void operator *= (const _tagPosition& tPos)
	{
		x *= tPos.x;
		y *= tPos.y;
	}

	void operator *= (const POINT& tPos)
	{
		x *= tPos.x;
		y *= tPos.y;
	}

	void operator *= (float fValue)
	{
		x *= fValue;
		y *= fValue;
	}

	_tagPosition operator / (const _tagPosition& pos)	const
	{
		_tagPosition	tPos;
		tPos.x = x / pos.x;
		tPos.y = y / pos.y;
		return tPos;
	}
	_tagPosition operator / (const POINT& pos)	const
	{
		_tagPosition	tPos;
		tPos.x = x / pos.x;
		tPos.y = y / pos.y;
		return tPos;
	}
	_tagPosition operator / (float f[2])	const
	{
		_tagPosition	tPos;
		tPos.x = x / f[0];
		tPos.y = y / f[1];
		return tPos;
	}
	_tagPosition operator / (float f)	const
	{
		_tagPosition	tPos;
		tPos.x = x / f;
		tPos.y = y / f;
		return tPos;
	}

	void operator /= (const _tagPosition& tPos)
	{
		x /= tPos.x;
		y /= tPos.y;
	}

	void operator /= (const POINT& tPos)
	{
		x /= tPos.x;
		y /= tPos.y;
	}

	void operator /= (float fValue)
	{
		x /= fValue;
		y /= fValue;
	}

	float Length()
	{
		return sqrtf(x * x + y * y);
	}

	float Dot(const _tagPosition& tPos)
	{
		return x * tPos.x + y * tPos.y;
	}

	void Normalize()
	{
		float fLength = Length();
		x /= fLength;
		y /= fLength;
	}

	_tagPosition Normalize(const _tagPosition& tPos)
	{
		_tagPosition p1 = tPos;

		p1.Normalize();

		return p1;
	}

	float Angle(const _tagPosition& tPos)
	{
		_tagPosition p1 = *this;
		_tagPosition p2 = tPos;

		p1.Normalize();
		p2.Normalize();

		float fDot = p1.Dot(p2);

		float fAngle = acosf(fDot);

		return RadToDeg(fAngle);
	}

	float AngleAxis()
	{
		_tagPosition p1 = *this;
		_tagPosition p2(1.f, 0.f);

		return p1.Angle(p2);
	}

	static _tagPosition One;
	static _tagPosition Zero;

}POSITION, *PPOSITION, _SIZE, *_PSIZE;

typedef struct _tagRectangle
{
	float	l;
	float	t;
	float	r;
	float	b;

	_tagRectangle() :
		l(0.f),
		t(0.f),
		r(0.f),
		b(0.f)
	{
	}
}RECTANGLE, *PRECTANGLE;

typedef struct _tagSphere
{
	POSITION	tCenter;
	float		fRadius;

	_tagSphere() :
		tCenter(0.f, 0.f),
		fRadius(0.f)
	{
	}
}SPHERE, *PSPHERE;


typedef struct _tagPixel
{
	unsigned char	b;
	unsigned char	g;
	unsigned char	r;
}PIXEL, *PPIXEL;

enum ANIMATION_TYPE
{
	AT_ATLAS,
	AT_FRAME,
	AT_END
};

enum ANIMATION_OPTION
{
	AO_LOOP,
	AO_ONCE_RETURN,
	AO_ONCE_DESTROY,
	AO_TIME_RETURN,
	AO_TIME_DESTROY
};

typedef struct _tagFrameInfo
{
	POSITION tStart;
	POSITION tEnd;

	_tagFrameInfo() :
		tStart(),
		tEnd()
	{
	}
}FrameInfo, * PFrameInfo;

typedef struct _tagAnimationClip
{
	ANIMATION_TYPE				eType;
	ANIMATION_OPTION			eOption;
	vector<class Texture*>		vecTexture;
	vector<_tagFrameInfo>		vecFrame;
	float		fAnimationTime;
	float		fAnimationLimitTime;
	float		fAnimationFrameTime;
	int			iFrameX;
	int			iFrameY;
	int			iFrameMaxY;
	int			iFrameMaxX;
	int			iStartX;
	int			iStartY;
	int			iLengthX;
	int			iLengthY;
	float		fOptionTime;
	float		fOptionLimitTime;
	_SIZE		tFrameSize;
}ANIMATIONCLIP,*PANIMATIONCLIP;

// ������

static wchar_t* charToWChar(const char* text)
{
	const size_t size = strlen(text) + 1;
	wchar_t* wText = new wchar_t[size];
	mbstowcs(wText, text, size);
	return wText;
}