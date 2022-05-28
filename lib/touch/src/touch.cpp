#include "touch.hpp"

void modifyPointToScreen(){
    p.x =  map(p.x,100,944,0,240);
    p.y =  map(p.y,190,860,0,128);
    if(p.z < 5){
        return; // ignore values below 5: they show up when not pressed
    }
    p.z = abs(map(p.z,900,250,0,255));
}

TouchScreen ts = TouchScreen(XP, YP, XM, YM , 730);
screens currentScreen = menu;
bool updateRequest = true; // use this as a flag to see if the screen needs to be updated

void touchEvent::detect(){ // the detect touch function serves to identify if the screen has been touched, if it meets the minimum duration
    p = ts.getPoint();
    modifyPointToScreen(); // maybe avoid use of MAP function for this process intensive
    // Serial.print(p.x);
    // Serial.print(",");
    // Serial.print(p.y);
    // Serial.print(",");
    // Serial.println(p.z);
    if (p.z > minPressure && p.z < maxPressure) // if the pressure meets our criteria
    {
        if(isScreenPressed == false) // if the screen was previously not pressed
        {
            touchDuration = 0; // reset the touch duration counter
        }
        isScreenPressed = true; // note that the screen is now being pressed
        x = p.x;
        y = p.y;
        // record the trace data
        traceData[traceIndex].x = x;
        traceData[traceIndex].y = y;
        traceData[traceIndex].z = p.z;
        traceData[traceIndex].t = touchDuration;
        traceIndex++;
        if(traceIndex >= numTraces) { traceIndex = 0;} // if we exceed the maximum number of traces reset it
        
        // Serial.print(x);
        // Serial.print(",");
        // Serial.print(y);
        // Serial.print(",");
        // Serial.println(touchDuration);
    }
    else
    {
        if(isScreenPressed == true) // if the screen was previously being pressed
        {
            touchEventDuration = touchDuration; // record the duration of the touch event
            duration = touchEventDuration;
            for(int i = traceIndex + 1; i < numTraces; i++)
            // go through the trace loop and fill in any remaining spots with the last available data
            {
                traceData[i].x = traceData[traceIndex].x;
                traceData[i].y = traceData[traceIndex].y;
                traceData[i].z = traceData[traceIndex].z;
                //traceData[i].t = traceData[traceIndex].t;
                traceData[i].t = duration; // use the current duration to reflect the total time of press

            }
            //traceData[traceIndex].t = duration;
            lastEvent = 0; // record the time since the last press
            traceIndex = 0; // reset the trace index
        }
        isScreenPressed = false; // note that the screen is no longer being pressed

    }
}

bool touchEvent::isPressed(){
    return(isScreenPressed);
}
    
bool touchEvent::isAreaPressed(int xCenter, int yCenter, int width, int height){
    if(isScreenPressed == false)
    {
        return(false);
    }
    // test for a faulty case and return false if found
    if(abs(x - xCenter) > width)
    {
        return(false);
    }

    if(abs(y - yCenter) > height)
    {
        return(false);
    }
    // if we pass all tests then our area must have been tapped
    return(true);
}

bool touchEvent::isTapped(){
    
    return( (touchEventDuration > minimumDuration) && (lastEvent < staleDuration) && (isScreenPressed == false)  );
    // all of these conditions must be true for a valid touch press
}

bool touchEvent::isAreaTapped(int xCenter, int yCenter, int width, int height){
    // a better way to do this might be to go from the last trace value back to the first
    // basically checking that the button has been pressed
    // might be able just to run the loop backwards from the trace index
    if((lastEvent > staleDuration)){ // check to make sure the event did not occur too long ago
        return(false);
    }
    int i = 0;
    #define minHoldDuration 80
    while( (traceData[i].t < minHoldDuration) && (i < numTraces)) //go though the trace data until the hold duration is exceeded or the index expries
    {
        // test for a faulty case and return false if found
        // see if the diffrence between the location of known touch points and the center is greater than the allowable tolerance
        if(abs(traceData[i].x - xCenter) > width)
        {
            Serial.print("xFailed @");
            Serial.print(traceData[i].x);
            Serial.print(" ! ");
            Serial.print(xCenter);
            Serial.print(" @index: ");
            Serial.print(i);
            Serial.print(" @dur: ");
            Serial.println(traceData[i].t);

            Serial.print("xFailed @");
            Serial.print(traceData[i-1].x);
            Serial.print(" ! ");
            Serial.print(xCenter);
            Serial.print(" @index: ");
            Serial.print(i-1);
            Serial.print(" @dur: ");
            Serial.println(traceData[i-1].t);

            return(false); // return the failed result
        }
        if(abs(traceData[i].y - yCenter) > height) 
        {
            Serial.print("yFailed @");
            Serial.print(traceData[i].y);
            Serial.print(" ! ");
            Serial.print(yCenter);
            Serial.print(" @index: ");
            Serial.print(i);
            Serial.print(" @dur: ");
            Serial.println(traceData[i].t);

            Serial.print("yFailed @");
            Serial.print(traceData[i-1].y);
            Serial.print(" ! ");
            Serial.print(yCenter);
            Serial.print(" @index: ");
            Serial.print(i-1);
            Serial.print(" @dur: ");
            Serial.println(traceData[i-1].t);
            return(false);
        }
        
        i++; //increment the index
    }
    // if we pass all tests then our area must have been tapped
    // clear the area
    
    return(true);
}

button::button(const int x, const int y, const int height, const int width){

    this->x0 = x;
    this->y0 = y;
    this->h0 = height;
    this->w0 = width;
}

void button::initialize(String text){
    u8g2.setFont(u8g2_font_logisoso16_tr);
    u8g2.drawFrame(x0, y0, x0+w0, y0+h0);
    u8g2.drawStr(x0 + x_padding,y0 + h0 + y_padding,"MENU");
}

void button::assignAction(void (*myFunction)())
{
    actionFunction = myFunction;
    actionAssigned = true;
}

void button::fillButton(){
    u8g2.setFontMode(1);
    u8g2.setDrawColor(1);
    u8g2.drawBox(x0,y0,w0,h0);
    u8g2.setDrawColor(2); // make sure the message will stay visable
    u8g2.setFont(textFont);
    u8g2.drawStr(x0,y0,message);
    u8g2.setDrawColor(1);
    u8g2.setFontMode(0);
}

void button::read()
{
    if((tap.x >= x0) && (tap.x <= x1) && (tap.y >= y0) && (tap.y <= y1)) {
    // check to see if the tap is inside the button box
        if(tap.isPressed()){ // if the user is pressing the button
        //if(tap.isAreaPressed(((box_x0 + box_x1)/2), ((box_y0 + box_y1)/2),40,20)){
                fillButton();
            //Serial.println("pressed");
            updateRequest = true;
        }
        if(tap.isTapped()){ // if the user tapped the button
            //if(tap.isAreaTapped(((box_x0 + box_x1)/2), ((box_y0 + box_y1)/2),40,20)){ // checking area taped should be more accurate but it is in gamma stage
            Serial.println("we got it!");
            draw();
            updateRequest = true;
            if(actionAssigned){ // make sure there is an action assigned before runing anything
                actionFunction(); // Run the function that we previously associated with the button
            }
        }
    }
    else{
        if(tap.isPressed()){ // if we are outside the button box but still pressing the screen
        draw();
        }
    }

}