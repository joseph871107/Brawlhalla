/* File description:
This is the header file 'UIMessage.h'. This is the API for the class 'UIMessage'. */

namespace game_framework
{
#ifndef UI_MESSAGE_H
#define UI_MESSAGE_H
class UIMessage
{
    public:
        virtual ~UIMessage();
        //Destructor

        UIMessage();
        //Default constructor

        UIMessage(const UIMessage& objectValue);
        //Copy constructor

        UIMessage(const string& messageValue, const int& xValue, const int& yValue, const int& currentFrameValue, const int& maxFrameValue);
        //Full constructor

        UIMessage& operator=(const UIMessage& rightObject);
        //Operator overloading function of the assignment operator

		const string& GetMessage() const;
		const int& GetX() const;
		const int& GetY() const;
		const int& GetCurrentFrame() const;
		const int& GetMaxFrame() const;
		void Initialize(const string& messageValue, const int& xValue, const int& yValue, const int& maxFrameValue);

		void IncrementCurrentFrame();

    private:
		string _message;
		int _x, _y;
		int _currentFrame;
		int _maxFrame;
};
#endif
}