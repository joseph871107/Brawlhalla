/* File description:
This is the header file 'GameEffect.h'. This is the API for the class 'GameEffect'. */

#include "camera.h"

namespace game_framework
{
#ifndef GAME_EFFECT_H
#define GAME_EFFECT_H
class GameEffect
{
    public:
        virtual ~GameEffect();
        //Destructor

        GameEffect();
        //Default constructor

        GameEffect(const GameEffect& objectValue);
        //Copy constructor

        GameEffect(const int& xValue, const int& yValue, const vector<CAnimation>& anisValue, const int& currentAniValue, Camera* const& cameraPtrValue);
        //Full constructor

        GameEffect& operator=(const GameEffect& rightObject);
        //Operator overloading function of the assignment operator

        // Required for Game Framework
        void Initialize(); /// DEBUG: Comment for future devs: unused function
        void LoadBitmap();
        void OnMove();
        void OnShow();

        // Others
        void SetXY(const int& newX, const int& newY);
        void SetCurrentAni(const int& newCurrentAni);
        void AddCAnimation(vector<int>*, double = 1.0, int = 10, bool = true, int = 1); // Push (bmps, (optional)size, (op)delay, (op)repeat, (op)repeat times) in vector of CAnimation
        bool IsCurrentAniFinalBitmap();
        double GetCurrentAnimationHeight();
        double GetCurrentAnimationWidth();
        const int& GetCurrentAni() const;

        // Camera
        void AddCamera(Camera* cameraPtrValue);	// Camera

        // Animations ID of '_anis'
        static const int ANI_ID_EXPLO_UP = 0;
        static const int ANI_ID_EXPLO_RIGHT = 1;
        static const int ANI_ID_EXPLO_DOWN = 2;
        static const int ANI_ID_EXPLO_LEFT = 3;


    private:
        int _x, _y;
        vector<CAnimation> _anis;
        int _currentAni;
        Camera* _cameraPtr;

};
#endif
}