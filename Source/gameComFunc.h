#pragma   once
#ifndef GAMECOMFUNC_H
#define GAMECOMFUNC_H
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <map>

//using namespace std;
namespace game_framework
{
	/////////////////////////////////////////////////////////////////////////////
	// 這個class為遊戲的常用副程式
	/////////////////////////////////////////////////////////////////////////////

	struct CArray {
		int width;
		int height;
		string fileName;
		vector<vector<bool>> pixel;
	};

	struct Bmp {
		int width;
		int height;
		vector< vector<COLORREF>> pixel;
	};

	extern map<string, int> idbList;
	extern map<int, string> fileList;
	extern map<int, CArray> cArray;

	/*class IDBFile {
	public:
		IDBFile(string file1 = "Source\\resource.h", string file2 = "Source\\game.rc") {
			idbList = InitializeIDB(file1);
			fileList = InitializeFile(file2);
			cArray = InitializeCollideArray();
		};*/
		int ti(int i, double j);
		map<string, int> InitializeIDB(string);
		map<int, string> InitializeFile(string);
		map<int, CArray> InitializeCollideArray(bool = false);
		string BoolToString(bool b);
		string GetPathFromIDB(string);
		int GetNumFromIDB(string);
		void TraceCollideArray(CArray trans, bool = true);
		CArray GetCollideArray(string file);
		Bmp readBMP(string, int = 1000, int = 1000);
		void InitializeAllBMP(string = "Source\\resource.h", string = "Source\\game.rc");
		CArray GetCArray(int);
	//};

	/*class GameComFunc {
	public:
		GameComFunc();
		GameComFunc(int, string file1, string file2);*/
		void OnShowText(string, int, int, int = 20, LPCTSTR = "Times New Roman", COLORREF = RGB(255, 255, 0));	// 簡化原本Game Framework 的字串顯示
	//};
}

#endif //define GAMECOMFUNC_H