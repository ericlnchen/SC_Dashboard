#include <globalData.h>

#define YP A1  // must be an analog pin, use "An" notation!
#define XM A8  // must be an analog pin, use "An" notation!
#define YM A3  // can be a digital pin
#define XP A2   // can be a digital pin
#define numTraces 4
#define u8g2 painter.u8g2

enum screens {menu,settings,gps,naughtTo60Timer};

class touchEvent{
public:
    int x = 0;
    int y = 0;
    uint8_t minimumDuration = 50;
    uint8_t staleDuration = 5; // rework this method of debouncing or de
    int duration = 0;
    int minPressure = 10;
    int maxPressure = 255;

    void detect(); // the detect touch function serves to identify if the screen has been touched, if it meets the minimum duration
    bool isPressed();
    bool isAreaPressed(int xCenter, int yCenter, int width, int height);
    bool isTapped();
    bool isAreaTapped(int xCenter, int yCenter, int width, int height);

private:
    elapsedMillis touchDuration;
    elapsedMillis lastEvent;
    bool isScreenPressed = false;
    int touchEventDuration = 0;
    struct trace // this stores a history of a "trace" - a touch event where pressed and moved
    {
        int x = 0;
        int y = 0;
        int z = 0;
        int t = 0;
    } traceData[numTraces];
    uint8_t traceIndex = 0;

};

class button{
public:
    int h0 = 0;
    int w0 = 0;
    const uint8_t* textFont = u8g2_font_logisoso16_tr; // note const uint8_t* is a pointer to a variable of const uint_8t and can still be modified
    char message[32] = "DEFAULT";
    bool useTopCordinate = false;
    touchEvent tap;

    button(const int x0, const int y0, const int height, const int width);
    void setText(const char* myMessage);
    void assignAction(void (*myFunction)());
    void draw();
    void fillButton();
    void read();
    void initialize(String text);

 private:
    int y_padding = 2;
    int x_padding = 5;
    int x0 = 0;
    int y0 = 0;
    int x1 = x0 + w0;
    int y1 = y0 + h0;
    bool isActive = false;
    bool actionAssigned = false; // use this to avoid performing actions that wernt asigned.
    int text_y0 = 0; // top of the text message
    void (*actionFunction)(); // use this to point to the function we want called when the button is pressed

};