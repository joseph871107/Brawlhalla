#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "brawlhalla.h"
#include "gameComFunc.h"

namespace game_framework
{

map<string, int> idbList;
map<int, string> fileList;
map<int, ColArray> cArray;

void InitializeAllBMP(bool trace, string file1, string file2)
{
    InitializeIDB();
    InitializeFile();
    InitializeCollideArray(trace);
}
void OnShowText(string msg, int x, int y, int size, LPCTSTR font, COLORREF color)
{
    CDC* pDC = CDDraw::GetBackCDC();			// 取得 Back Plain 的 CDC
    CFont f, *fp;
    f.CreatePointFont(size * 8, font);			// 產生 font f; 160表示16 point的字
    fp = pDC->SelectObject(&f);					// 選用 font f
    pDC->SetBkColor(RGB(0, 0, 0));
    pDC->SetTextColor(color);
    pDC->TextOut(x, y, msg.c_str());
    pDC->SelectObject(fp);						// 放掉 font f (千萬不要漏了放掉)
    CDDraw::ReleaseBackCDC();					// 放掉 Back Plain 的 CDC
}

int ti(int i, double j)
{
    return (int)(i * j);
}

void TraceCollideArray(int file, bool trace)
{
    ColArray Ctemp = cArray.find(file)->second;

    if (trace)
    {
        TRACE("%s\n", Ctemp.fileName.c_str());
        TRACE("width : %d height : %d\n", Ctemp.width, Ctemp.height);
    }

    vector<string> stri;

    for (auto i = Ctemp.pixel.begin(); i != Ctemp.pixel.end(); i++)
    {
        string st = "";

        for (auto j = i->begin(); j != i->end(); j++)
            st += (string)(*j ? "1" : "0") + " ";

        stri.push_back(st);
    }

    for (unsigned int i = 0; i < stri.size(); i++)
    {
        if (trace)
            TRACE("%s\n", stri[i].c_str());
    }

    if (trace)
        TRACE("\n");
}

void GetCollideArray(int file, ColBmp* bmp)
{
    ColArray Ctemp;
    vector <vector<bool>> trans;

    for (auto i = bmp->pixel.begin(); i != bmp->pixel.end(); i++)
    {
        vector<bool> temp;

        for (auto j = i->begin(); j != i->end(); j++)
            temp.push_back(*j == RGB(0, 0, 0) ? false : true);

        Ctemp.pixel.push_back(temp);
    }

    Ctemp.width = bmp->width;
    Ctemp.height = bmp->height;
    Ctemp.fileName = GetNameFromIDB(file);
    cArray.insert(pair<int, ColArray>(file, Ctemp));
}


void InitializeIDB(string file)
{
    TRACE("Initializing map IDB...\n");
    ifstream myfile;
    map<string, int>* idblist = (map<string, int>*)malloc(sizeof(map<string, int>));
    myfile.open(file.c_str());
    string line;

    while (getline(myfile, line))
    {
        bool flag = false;
        istringstream iss(line);
        string s, s_n;

        while (iss >> s && !flag)
        {
            if (s == "#define")
            {
                iss >> s;

                if (s.find("IDB") != string::npos)
                {
                    s_n = s;
                    flag = true;
                }
            }
        }

        if (flag)
            idbList.insert(pair<string, int>(s_n, stoi(s)));
    }

    free(idblist);
    myfile.close();
}
void InitializeFile(string file)
{
    TRACE("Initializing map file...\n");
    ifstream myfile(file.c_str());
    string line;

    while (getline(myfile, line))
    {
        bool flag = false;
        istringstream iss(line);
        string s, s_b;
        map<string, int>::iterator it;

        while (iss >> s && !flag)
        {
            it = idbList.find(s);

            if (it != idbList.end())
            {
                iss >> s;

                if (s == "BITMAP")
                    flag = true;
            }

            s_b = s;
        }

        if (flag)
        {
            s = s.substr(5, s.size() - 6);
            fileList.insert(pair<int, string>(it->second, s));
        }
    }

    myfile.close();
}

void InitializeCollideArray(bool trace)
{
    TRACE("Initializing collide array...\n");

    for (auto i = idbList.begin(); i != idbList.end(); i++)
    {
        TRACE("---Generating %s collide array...\n", i->first.c_str());
        ColBmp tt = readBMP(i->second);
        GetCollideArray(i->second, &tt);
        TraceCollideArray(i->second, trace);
    }
}

string GetPathFromIDB(int file)
{
    return fileList.find(file)->second;
}

string GetNameFromIDB(int file)
{
    for (auto it = idbList.begin(); it != idbList.end(); ++it)
        if (it->second == file)
            return it->first;

    return "";
}

ColBmp readBMP(int file, int x_max, int y_max)
{
    ColBmp tBmp;
    HBITMAP hBmp = (HBITMAP)LoadImage(NULL, (LPCSTR) GetPathFromIDB(file).c_str(), IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE);

    if (hBmp == NULL) return tBmp;

    BITMAP bmp;
    HDC hdc = CreateCompatibleDC(NULL);
    GetObject(hBmp, sizeof(bmp), &bmp);

    if (bmp.bmWidth > x_max || bmp.bmHeight > y_max) return tBmp;

    tBmp.width = bmp.bmWidth;
    tBmp.height = bmp.bmHeight;
    BitBlt(hdc, 0, 0, bmp.bmWidth, bmp.bmHeight, hdc, 0, 0, SRCCOPY);
    SelectObject(hdc, hBmp);

    for (int y = 0; y < bmp.bmHeight; y++)
    {
        vector<COLORREF> temp;

        for (int x = 0; x < bmp.bmWidth; x++) temp.push_back(GetPixel(hdc, x, y));

        tBmp.pixel.push_back(temp);
    }

    DeleteDC(hdc);
    DeleteObject(hBmp);
    return tBmp;
}
}