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
	// �I���x�}�����ܼ�
	/////////////////////////
	extern map<string, int> idbList;
	extern map<int, string> fileList;
	extern map<int, ColArray> cArray;

	/////////////////////////
	// �I���x�}�B�z�Ƶ{��
	/////////////////////////
	void InitializeAllBMP(bool = false, string = "Source\\resource.h", string = "Source\\game.rc");	// ��l���I�}�ϹB��귽
	void InitializeIDB(string = "Source\\resource.h");												// ��l��"resource.h"���I�}�Ϫ��귽�s��
	void InitializeFile(string = "Source\\game.rc");												// ��l��"game.rc"���I�}�Ϫ����|
	void InitializeCollideArray(bool = false);														// ��l���I�}�Ϫ����L�I���x�}

	/////////////////////////
	// �C�h���U�Ƶ{��
	/////////////////////////
	string GetPathFromIDB(int);										// �HIDB���o�귽���|
	string GetNameFromIDB(int);										// �HIDB���oIDB�W��
	void TraceCollideArray(int, bool = true);				// ����I�}�ϥ��L�x�}�bDebugger��
	ColBmp readBMP(int file, int = 1000, int = 1000);				// file ���ɮ׸��|
	void GetCollideArray(int file, ColBmp*);						// file ��IDB�W��


	/////////////////////////
	// �q�λ��U�Ƶ{��
	/////////////////////////
	void OnShowText(string, int, int, int = 20, LPCTSTR = "Times New Roman", COLORREF = RGB(255, 255, 0));	// ²�ƭ쥻Game Framework ���r�����
}

#endif //define GAMECOMFUNC_H