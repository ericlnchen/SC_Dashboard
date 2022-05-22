#include <globalData.h>

#define YP A1  // must be an analog pin, use "An" notation!
#define XM A8  // must be an analog pin, use "An" notation!
#define YM A3  // can be a digital pin
#define XP A2   // can be a digital pin
#define numTraces 4

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
    int height = 0;
    int width = 0;
    const uint8_t* textFont = u8g2_font_VCR_OSD_mf; // note const uint8_t* is a pointer to a variable of const uint_8t and can still be modified
    char message[32] = "DEFAULT";
    bool useTopCordinate = false;
    touchEvent tap;

    button(const char* text, const int x0, const int y0);
    void setText(const char* myMessage);
    void assignAction(void (*myFunction)());
    void draw();
    void fillButton();
    void read();

 private:
    int x0 = 0;
    int y0 = 0;
    bool isActive = false;
    bool actionAssigned = false; // use this to avoid performing actions that wernt asigned.
    int text_y0 = 0; // top of the text message
    int box_y0 = 0;
    int box_y1 = 0;
    int box_x0 = 0;
    int box_x1 = 0;
    void (*actionFunction)(); // use this to point to the function we want called when the button is pressed

};