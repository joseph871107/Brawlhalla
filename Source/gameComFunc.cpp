#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "brawlhalla.h"
#include "gameComFunc.h"

//using namespace std;
namespace game_framework {
	void InitializeAllBMP(string file1, string file2)
	{
		idbList = InitializeIDB(file1);
		fileList = InitializeFile(file2);
		cArray = InitializeCollideArray(true);
	}

	CArray GetCArray(int resource) {
		return cArray.find(resource)->second;
	}

	map<string, int> idbList;
	map<int, string> fileList;
	map<int, CArray> cArray;

	void OnShowText(string msg, int x, int y, int size, LPCTSTR font, COLORREF color)
	{
		CDC *pDC = CDDraw::GetBackCDC();			// 取得 Back Plain 的 CDC 
		CFont f, *fp;
		f.CreatePointFont(size * 8, font);			// 產生 font f; 160表示16 point的字
		fp = pDC->SelectObject(&f);					// 選用 font f
		pDC->SetBkColor(RGB(0, 0, 0));
		pDC->SetTextColor(color);
		pDC->TextOut(x, y, msg.c_str());
		pDC->SelectObject(fp);						// 放掉 font f (千萬不要漏了放掉)
		CDDraw::ReleaseBackCDC();					// 放掉 Back Plain 的 CDC
	}

	string BoolToString(bool b)
	{
		return b ? "1" : "0";
	}

	int ti(int i, double j){
		return (int)(i*j);
	}

	void TraceCollideArray(CArray cArray, bool trace) {
		if (trace)
			TRACE("%s\n", cArray.fileName.c_str());
		vector<string> stri;
		for (auto i = cArray.pixel.begin(); i != cArray.pixel.end(); i++) {
			string st = "";
			for (auto j = i->begin(); j != i->end(); j++)
				st += BoolToString(*j) + " ";
			stri.push_back(st);
		}
		for (unsigned int i = 0; i < stri.size(); i++) {
			if (trace)
				TRACE("%s\n", stri[i].c_str());
		}
		if (trace)
			TRACE("\n");
	}

	CArray GetCollideArray(string file) {
		Bmp bmp = readBMP(GetPathFromIDB(file));
		CArray carray;
		vector <vector<bool>> trans;
		for (auto i = bmp.pixel.begin(); i != bmp.pixel.end(); i++) {
			vector<bool> temp;
			for (auto j = i->begin(); j != i->end(); j++)
				temp.push_back(*j == RGB(0, 0, 0) ? false : true);
			trans.push_back(temp);
		}
		carray.pixel = trans; carray.width = bmp.width; carray.height = bmp.height; carray.fileName = file;
		return carray;
	}


	map<string, int> InitializeIDB(string file)
	{
		TRACE("Initializing map IDB...\n");
		ifstream myfile;
		map<string, int> idblist;
		myfile.open(file.c_str());
		string line;
		while (getline(myfile, line))
		{
			bool flag = false;
			istringstream iss(line);
			string s, s_n;
			while (iss >> s && !flag) {
				if (s == "#define") {
					iss >> s;
					if (s.find("IDB") != string::npos) {
						s_n = s;
						flag = true;
					}
				}
			}
			if (flag)
				idblist.insert(pair<string, int>(s_n, stoi(s)));
		}
		myfile.close();
		return idblist;
	}
	map<int, string> InitializeFile(string file)
	{
		TRACE("Initializing map file...\n");
		ifstream myfile;
		map<int, string> filelist;
		myfile.open(file.c_str());
		string line;
		while (getline(myfile, line))
		{
			bool flag = false;
			istringstream iss(line);
			string s,s_b;
			map<string, int>::iterator it;
			while (iss >> s && !flag) {
				it = idbList.find(s);
				if (it != idbList.end()) {
					iss >> s;
					if (s == "BITMAP")
						flag = true;
				}s_b = s;
			}
			if (flag) {
				s = s.substr(5, s.size() - 6);
				filelist.insert(pair<int,string>(it->second,s));
			}
		}
		myfile.close();
		return filelist;
	}

	map<int, CArray> InitializeCollideArray(bool trace)
	{
		TRACE("Initializing collide array...\n");
		map<int, CArray> Mtemp;
		for (auto i = idbList.begin(); i != idbList.end(); i++) {
			TRACE("---Generating %s collide array...\n",i->first.c_str());
			CArray temp = GetCollideArray(i->first);
			TraceCollideArray(temp,trace);
			Mtemp.insert(pair<int, CArray>(i->second, temp));
		}
		return Mtemp;
	}

	string GetPathFromIDB(string file)
	{
		return fileList.find(GetNumFromIDB(file))->second;
	}

	int GetNumFromIDB(string file)
	{
		return idbList.find(file)->second;
	}

	Bmp readBMP(string file, int x_max, int y_max)
	{
		Bmp tBmp;
		vector<vector<COLORREF>> pixels;
		HBITMAP hBmp = (HBITMAP)LoadImage(NULL, (LPCSTR)file.c_str(), IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE);
		if (hBmp == NULL)
			return tBmp;
		BITMAP bmp;
		HDC hdc = CreateCompatibleDC(NULL);
		GetObject(hBmp, sizeof(bmp), &bmp);
		tBmp.width = bmp.bmWidth;
		tBmp.height = bmp.bmHeight;
		if (tBmp.width > x_max || tBmp.height > y_max)
			return tBmp;
		BitBlt(hdc, 0, 0, bmp.bmWidth, bmp.bmHeight, hdc, 0, 0, SRCCOPY);
		SelectObject(hdc, hBmp);

		for (int y = 0; y < bmp.bmHeight; y++)
		{
			vector<COLORREF> temp;
			for (int x = 0; x < bmp.bmWidth; x++)
			{
				COLORREF clr;
				clr = GetPixel(hdc, x, y);

				if (clr != CLR_INVALID)
					temp.push_back(clr);
			}
			pixels.push_back(temp);
		}

		DeleteDC(hdc);
		DeleteObject(hBmp);
		tBmp.pixel = pixels;
		return tBmp;
	}
}