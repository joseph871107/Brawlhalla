#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "brawlhalla.h"
#include "ground.h"
#include "gameComFunc.h"

namespace game_framework
{

map<string, int> idbList;
map<string, int> idsList;
map<int, string> fileList;
map<int, string> soundList;
map<int, ColBmp> colBmp;
map<int, ColArray> cArray;

void InitializeAll(bool trace, string file1, string file2)
{
    InitializeNum();
    InitializeNum("IDS");
    InitializeFile();
    InitializeFile("SOUND");
    InitializeCollideArray(trace);
}
void OnShowText(string msg, int x, int y, int size, COLORREF color, COLORREF bgcolor, LPCTSTR font)
{
    CDC* pDC = CDDraw::GetBackCDC();			// 取得 Back Plain 的 CDC
    CFont f, *fp;
    f.CreatePointFont(size * 8, font);			// 產生 font f; 160表示16 point的字
    fp = pDC->SelectObject(&f);					// 選用 font f

    if (bgcolor == RGB(256, 256, 256))
        pDC->SetBkMode(TRANSPARENT);
    else
        pDC->SetBkColor(bgcolor);

    pDC->SetTextColor(color);
    pDC->TextOut(x, y, msg.c_str());
    pDC->SelectObject(fp);						// 放掉 font f (千萬不要漏了放掉)
    CDDraw::ReleaseBackCDC();					// 放掉 Back Plain 的 CDC
}

void DrawRectangle(int x, int y, int width, int height, COLORREF color)
{
    CDC* pDC = CDDraw::GetBackCDC();			// 取得 Back Plain 的 CDC
    CPen* pp, p(PS_NULL, 0, RGB(0, 0, 0));		// 清除pen
    pp = pDC->SelectObject(&p);
    CBrush* pb, b(color);
    pb = pDC->SelectObject(&b);
    pDC->Rectangle(x, y, x + width, y + height);
    pDC->SelectObject(pp);						// 釋放 pen
    pDC->SelectObject(pb);						// 釋放 brush
    CDDraw::ReleaseBackCDC();					// 放掉 Back Plain 的 CDC
}

void DrawRectangleBlock(int x, int y, int width, int height, COLORREF color)
{
    CDC* pDC = CDDraw::GetBackCDC();			// 取得 Back Plain 的 CDC
    CPen* pp, p(PS_SOLID, 0, color);
    pp = pDC->SelectObject(&p);
    pDC->MoveTo(CPoint(x, y));
    pDC->LineTo(CPoint(x + width, y));
    pDC->LineTo(CPoint(x + width, y + height));
    pDC->LineTo(CPoint(x, y + height));
    pDC->LineTo(CPoint(x, y));
    pDC->SelectObject(pp);						// 釋放 pen
    CDDraw::ReleaseBackCDC();					// 放掉 Back Plain 的 CDC
}

void DrawLine(int x1, int y1, int x2, int y2, COLORREF color)
{
    CDC* pDC = CDDraw::GetBackCDC();			// 取得 Back Plain 的 CDC
    CPen* pp, p(PS_SOLID, 0, color);
    pp = pDC->SelectObject(&p);
    pDC->MoveTo(CPoint(x1, y1));
    pDC->LineTo(CPoint(x2, y2));
    pDC->SelectObject(pp);						// 釋放 pen
    CDDraw::ReleaseBackCDC();					// 放掉 Back Plain 的 CDC
}

SIZE GetStringSize(string str, int textSIze)
{
    SIZE size;
    size.cx = 0;
    size.cy = 0;
    CDC* pDC = CDDraw::GetBackCDC();
    GetTextExtentPoint32(pDC->GetSafeHdc(), str.c_str(), str.length(), &size);
    CDDraw::ReleaseBackCDC();
    size.cx = size.cx * textSIze / 16;
    size.cy = size.cy * textSIze / 16;
    return size;
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
    Ctemp.fileName = GetNameFromResource(file);
    cArray.insert(pair<int, ColArray>(file, Ctemp));
}


void InitializeNum(string type, string file)
{
    TRACE(("Initializing map " + type + "...\n").c_str());
    ifstream myfile;
    myfile.open(file.c_str());
    string line;
    int count = 0;
    map<string, int>* list = (type == "IDB" ? &idbList : type == "IDS" ? &idsList : nullptr);

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

                if (s.find(type) != string::npos)
                {
                    s_n = s;
                    flag = true;
                }
            }
        }

        if (flag)
        {
            count++;
            list->insert(pair<string, int>(s_n, stoi(s)));
        }
    }

    TRACE("----Resource Number List size : %d\n", list->size());
    myfile.close();
}

void InitializeFile(string type, string file)
{
    TRACE("Initializing file...\n");
    ifstream myfile(file.c_str());
    string line;
    int count = 0;
    map<string, int>* list = (type == "BITMAP" ? &idbList : type == "SOUND" ? &idsList : nullptr);
    map<int, string>* flist = (type == "BITMAP" ? &fileList : type == "SOUND" ? &soundList : nullptr);

    while (getline(myfile, line))
    {
        bool flag = false;
        istringstream iss(line);
        string s, s_b;
        map<string, int>::iterator it;

        while (iss >> s && !flag)
        {
            it = list->find(s);

            if (it != list->end())
            {
                iss >> s;

                if (s == type)
                    flag = true;
            }

            s_b = s;
        }

        if (flag)
        {
            count++;
            s = s.substr(5, s.size() - 6);
            flist->insert(pair<int, string>(it->second, s));
        }
    }

    TRACE("----Resource File Path List size : %d\n", flist->size());
    myfile.close();
}

void InitializeCollideArray(bool trace)
{
    TRACE("Initializing collide array...\n");

    for (auto i = idbList.begin(); i != idbList.end(); i++)
    {
        TRACE("---Generating %s collide array...\n", i->first.c_str());
        ColBmp tt = readBMP(i->second);
        colBmp.insert(pair<int, ColBmp>(i->second, tt));
        GetCollideArray(i->second, &tt);
        TraceCollideArray(i->second, trace);
    }
}

string GetPathFromResource(int file, string type)
{
    map<int, string>* list = (type == "IDB" ? &fileList : type == "IDS" ? &soundList : nullptr);

    if (list != nullptr)
    {
        return list->find(file)->second;
    }

    return "";
}

string GetNameFromResource(int file, string type)
{
    map<string, int>* list = (type == "IDB" ? &idbList : type == "IDS" ? &idsList : nullptr);

    if (list != nullptr)
    {
        for (auto it = list->begin(); it != list->end(); ++it)
            if (it->second == file)
                return it->first;
    }

    return "";
}

ColBmp readBMP(int file, int x_max, int y_max)
{
    ColBmp tBmp;
    HBITMAP hBmp = (HBITMAP)LoadImage(NULL, (LPCSTR) GetPathFromResource(file).c_str(), IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE);

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


char* ToCharPtr(string str)
{
    static char ptr[128];
    strcpy(ptr, str.c_str());
    return ptr;
}
int random(int min, int max)
{
    return rand() % (max - min + 1) + min;
}
Ground* GetRandomGround(vector<Ground*>* ground)
{
    int total = 0, current = 0, last = 0;
    double selected = rand() / (double)RAND_MAX;
    vector<Ground*>::iterator finalSelect = ground->begin();

    for (auto g : *ground)
        total += g->GetWidth();

    for (auto i = ground->begin(); i != ground->end(); i++)  																// Randomly choose one of the ground object
    {
        current += (*i)->GetWidth();

        if (selected >= last / (double)total && selected < current / (double)total)
            finalSelect = i;

        last = current;
    }

    return *finalSelect;
}
bool IsCollide(int x1, int y1, int x2, int y2, int tx1, int ty1, int tx2, int ty2)
{
    return (tx2 >= x1 && tx1 <= x2 && ty2 >= y1 && ty1 <= y2);
}
vector<vector<CMovingBitmap>> CropSprite(int IDB, int row, int column, COLORREF color)
{
    vector<vector<CMovingBitmap>> sprite;
    CMovingBitmap tbmp;
    tbmp.LoadBitmap(IDB, color);
    int width = tbmp.Width() / column, height = tbmp.Height() / row;

    for (int i = 0; i < row; i++)
    {
        vector<CMovingBitmap> temp;

        for (int j = 0; j < column; j++)
        {
            RECT rect;
            rect.left = width * j;
            rect.right = rect.left + width;
            rect.top = height * i;
            rect.bottom = rect.top + height;
            CMovingBitmap bmp;
            char path[64];
            strcpy(path, GetPathFromResource(IDB).c_str());
            bmp.LoadBitmap(path, rect, color);
            temp.push_back(bmp);
        }

        sprite.push_back(temp);
    }

    return sprite;
}
void FlipSprite(vector<vector<CMovingBitmap>>* sprite)
{
    for (auto i = (*sprite).begin(); i != (*sprite).end(); i++)
        reverse(i->begin(), i->end());
}
void CRAnimation::NextPtr()
{
    bmp_iter++;

    if (bmp_iter == bmp.end())
        bmp_iter = bmp.begin();
}
void CRAnimation::PreviousPtr()
{
    bmp_iter--;

    if (bmp_iter == bmp.begin())
        bmp_iter = bmp.end();
}

int DoubleToInteger(double value)
{
    return ((int)(value + 0.5));
}
}