#include "stdafx.h"
#include <ddraw.h>
#include <math.h>
#include "gamelib.h"
#include "fImage.h"
#define PI 3.14159265

using namespace std;

namespace game_framework {
	int FImage::Width()
	{
		return imageWidth;
	}
	int FImage::Height()
	{
		return imageHeight;
	}
	void FImage::LoadBitmap(int resID)
	{
		this->useRGBMask = false;
		this->resID = resID;
		if (!image.IsNull())
			image.Destroy();
		image.LoadFromResource(AfxGetInstanceHandle(), MAKEINTRESOURCE(resID));
		this->imageWidth = image.GetWidth();
		this->imageHeight = image.GetHeight();
		GetArray();
	}

	void FImage::LoadBitmap(int resID, COLORREF RGBMask)
	{
		this->LoadBitmap(resID);
		this->RGBMask = RGBMask;
		this->useRGBMask = true;
	}

	void FImage::SetTopLeft(int xDst, int yDst, int wDst, int hDst)
	{
		this->xDst = xDst;
		this->yDst = yDst;
		this->wDst = wDst;
		this->hDst = hDst;
		useDestSize = !((wDst == -1) && (wDst == hDst));
	}

	void FImage::SetAlpha(int alpha)
	{
		this->alpha = (alpha > 255) ? 255 : (alpha < 0) ? 0 : alpha;
	}

	void FImage::GetArray()
	{
		vector<vector<COLORREF>> tarray;
		BYTE* oPtr = (BYTE*)image.GetBits();
		int oPitch = image.GetPitch();
		int oBPP = image.GetBPP() / 8;
		BYTE* pixel = (BYTE*)(oPtr);
		for (int i = 0; i < imageWidth; i++) {
			vector<COLORREF> temp;
			for (int j = 0; j < imageHeight; j++) {
				BYTE* imagePtr = &pixel[oPitch*j + oBPP * i];
				temp.push_back(RGB(imagePtr[2], imagePtr[1], imagePtr[0]));
			}
			tarray.push_back(temp);
		}array = arrayR = tarray;
	}

	void FImage::UseRgbMaskAlphaBlend(void* hdcDst, int wDst, int hDst, void* hdcImage)
	{
		CImage tempImage;
		tempImage.Create(imageWidth, imageHeight, 32, 1);
		HDC tmpImageHDC = tempImage.GetDC();
		::BitBlt(tmpImageHDC, 0, 0, imageWidth, imageHeight, (HDC)hdcImage, 0, 0, SRCCOPY);
		BYTE* tPtr = (BYTE*)tempImage.GetBits();
		int tPitch = tempImage.GetPitch();
		int tBPP = tempImage.GetBPP() / 8;
		for (int i = 0; i < imageWidth; i++)
			for (int j = 0; j < imageHeight; j++) {
				BYTE* tempImagePtr = (BYTE*)(tPtr + tPitch * j + tBPP * i);
				if (arrayR[i][j] == RGBMask)
					tempImagePtr[3] = 0;
				else
					tempImagePtr[3] = 255;
				tempImagePtr[2] = GetRValue(arrayR[i][j]);
				tempImagePtr[1] = GetGValue(arrayR[i][j]);
				tempImagePtr[0] = GetBValue(arrayR[i][j]);
			}
		::AlphaBlend((HDC)hdcDst, 0, 0, wDst, hDst,
			tmpImageHDC, 0, 0, imageWidth, imageHeight, _BLENDFUNCTION{ 0,0,BYTE(alpha),AC_SRC_ALPHA });
		tempImage.ReleaseDC();
		tempImage.Destroy();
	}

	void FImage::Rotate(int degree)
	{
		for (int i = 0; i < imageWidth; i++)
			for (int j = 0; j < imageHeight; j++) {
				int oi = i - imageWidth / 2, oj = j - imageHeight / 2;
				int ti = (int)(sin(degree*PI / 180) * oj + cos(degree*PI / 180) * oi) + imageWidth / 2, tj = (int)(cos(degree*PI / 180) * oj - sin(degree*PI / 180) * oi) + imageHeight / 2;
				if (ti >= 0 && ti < imageWidth && tj >= 0 && tj < imageHeight)
					arrayR[i][j] = array[ti][tj];
			}
	}

	void FImage::ShowBitmap()
	{
		if (this->alpha == 0) return;
		int wDst = imageWidth, hDst = imageHeight;
		if (useDestSize) {
			wDst = this->wDst;
			hDst = this->hDst;
		}
		// ���o HDC
		CDC *pDC = CDDraw::GetBackCDC();
		HDC hdcDst = pDC->GetSafeHdc();
		HDC imageHDC = image.GetDC();
		// �w��
		CDC cdcMem;
		cdcMem.CreateCompatibleDC(pDC);
		CBitmap bmpMem;
		bmpMem.CreateCompatibleBitmap(pDC, wDst, hDst);
		CBitmap* pOldBmp = cdcMem.SelectObject(&bmpMem);
		HDC hdcMem = cdcMem.GetSafeHdc();
		BitBlt(hdcMem, 0, 0, wDst, hDst, hdcDst, xDst, yDst, SRCCOPY);
		// �̳]�wø��
		if (this->useRGBMask)
			UseRgbMaskAlphaBlend(hdcMem, wDst, hDst, imageHDC);
		else if (this->alpha == 255)
			::StretchBlt(hdcMem, 0, 0, wDst, hDst, imageHDC, 0, 0, imageWidth, imageHeight, SRCCOPY);
		else
			::AlphaBlend(hdcMem, 0, 0, wDst, hDst, imageHDC, 0, 0, imageWidth, imageHeight, _BLENDFUNCTION{ 0,0,BYTE(alpha),0 });
		BitBlt(hdcDst, xDst, yDst, wDst, hDst, hdcMem, 0, 0, SRCCOPY);
		// ����
		image.ReleaseDC();
		cdcMem.SelectObject(pOldBmp);
		pOldBmp->DeleteObject();
		bmpMem.DeleteObject();
		cdcMem.DeleteDC();
		CDDraw::ReleaseBackCDC();
	}
}