/* File description:
This is the header file 'RespawnCourier.h'. This is the API for the class 'RespawnCourier'. */

namespace game_framework
{
#ifndef RESPAWN_COURIER_H
#define RESPAWN_COURIER_H
class Camera;

class RespawnCourier
{
    public:
        virtual ~RespawnCourier();
        //Destructor

        RespawnCourier();
        //Default constructor

        RespawnCourier(const RespawnCourier& objectValue);
        //Copy constructor

        RespawnCourier(const int& xValue, const int& yValue, const double& sizeValue, const CMovingBitmap& bmpLeftValue, const CMovingBitmap& bmpRightValue, Camera* const cameraPtrValue, const bool& dirValue);
        //Full constructor

        RespawnCourier& operator=(const RespawnCourier& rightObject);
        //Operator overloading function of the assignment operator

        // Required for Game Framework
        void LoadBitmap();
        void OnShow();

        //
        void Initialize(Camera* const& cameraPtrValue);
        void SetXY(const int& newX, const int& newY);
        double GetWidth();
        const int& GetX() const;
        const int& GetY() const;
        void SetDir(const bool& newDir);

    private:
        int _x, _y;
        double _size;
        CMovingBitmap _bmpLeft, _bmpRight;
        Camera* _cameraPtr;
        bool _dir; // false: left, true: right

};
#endif
}