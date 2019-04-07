#pragma once
#ifndef FIMAGE_H
#define FIMAGE_H
#include <atlimage.h>
#include <vector>

namespace game_framework {

	class FImage
	{
	public:
		int Width();
		int Height();
		void LoadBitmap(int resID);
		void LoadBitmap(int resID, COLORREF RGBMask);
		void SetTopLeft(int xDst, int yDst, int wDst = -1, int hDst = -1);
		void SetAlpha(int alpha = 255);
		void ShowBitmap();
		void GetArray();
		void UseRgbMaskAlphaBlend(void* hdcDst, int wDst, int hDst, void* hdcImage);
		void Rotate(int degree);
	private:
		int resID = 0, alpha = 255, xDst = 0, yDst = 0, wDst = 0, hDst = 0;
		int imageWidth = 0, imageHeight = 0;
		bool useRGBMask = false, useDestSize = false;
		COLORREF RGBMask = 0;
		vector<vector<COLORREF>> array;
		vector<vector<COLORREF>> arrayR;
		CImage image;
	};

}

#endif // !FIMAGE_H
