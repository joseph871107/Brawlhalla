#pragma   once
#ifndef GAMECOMFUNC_H
#define GAMECOMFUNC_H
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <map>

namespace game_framework
{
	struct ColArray {
		int width;
		int height;
		string fileName;
		vector<vector<bool>> pixel;
	};

	struct ColBmp {
		int width;
		int height;
		vector< vector<COLORREF>> pixel;
	};


	/////////////////////////
	// 碰撞矩陣全域變數
	/////////////////////////
	extern map<string, int> idbList;
	extern map<int, string> fileList;
	extern map<int, ColArray> cArray;

	/////////////////////////
	// 碰撞矩陣處理副程式
	/////////////////////////
	void InitializeAllBMP(bool = false, string = "Source\\resource.h", string = "Source\\game.rc");	// 初始化點陣圖運算資源
	void InitializeIDB(string = "Source\\resource.h");												// 初始化"resource.h"中點陣圖的資源編號
	void InitializeFile(string = "Source\\game.rc");												// 初始化"game.rc"中點陣圖的路徑
	void InitializeCollideArray(bool = false);														// 初始化點陣圖的布林碰撞矩陣

	/////////////////////////
	// 低層輔助副程式
	/////////////////////////
	string GetPathFromIDB(int);										// 以IDB取得資源路徑
	string GetNameFromIDB(int);										// 以IDB取得IDB名稱
	void TraceCollideArray(int, bool = true);				// 顯示點陣圖布林矩陣在Debugger中
	ColBmp readBMP(int file, int = 1000, int = 1000);				// file 為檔案路徑
	void GetCollideArray(int file, ColBmp*);						// file 為IDB名稱


	/////////////////////////
	// 通用輔助副程式
	/////////////////////////
	void OnShowText(string, int, int, int = 20, LPCTSTR = "Times New Roman", COLORREF = RGB(255, 255, 0));	// 簡化原本Game Framework 的字串顯示
}

#endif //define GAMECOMFUNC_H