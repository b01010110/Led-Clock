#pragma once
#include <Arduino.h>
#include <RTClib.h>
#include <GyverTimer.h>
#include <GyverButton.h>

class Clock {
  public:
    Clock(byte a, byte b, byte c, byte d, byte e, byte f, byte g, byte h, byte d1, byte d2, byte d3, byte d4, byte btn1, byte btn2); // constructor
    String displayedSymbols = "0000";         // displayed symbols on the screen
    String time = "0000";                     // current time
    String temperature = " *00";              // current temperature
    void displayData();                       // display the data on the screen
    void tick();                              // main method of the program
    void init();                              // the initialize method
    enum mode {                               // the modes
      showWatch,                                // time display
      showTemperature,                          // temperature display
      showSetup                                 // time set up display
    };
  private:
    byte _a;                                  // port number for segment a
    byte _b;                                  // port number for segment b
    byte _c;                                  // port number for segment c
    byte _d;                                  // port number for segment d
    byte _e;                                  // port number for segment e
    byte _f;                                  // port number for segment f
    byte _g;                                  // port number for segment g
    byte _h;                                  // port number for segment h
    byte _d1;                                 // port number for rank 1
    byte _d2;                                 // port number for rank 2
    byte _d3;                                 // port number for rank 3
    byte _d4;                                 // port number for rank 4
    byte _btn1;                               // port number for main button
    byte _btn2;                               // port number for addition button

    byte segments[7];                         // port numbers for segments
    byte ranks[4];                            // port numbers for ranks
    
    GButton btnMain;                          // main button
    GButton btnAddition;                      // addition button
    
    RTC_DS3231 rtc;                           // RTC module
    
    GTimer rtcTimer;                          // RTC data timer
    GTimer blinkTimer;                        // blink timer
    GTimer showTemperatureTimer;              // timer for show temperature
    GTimer hideTemperatureTimer;              // timer for hide temperature
    
    volatile byte _currentDigit = 0;          // currently displayed digit
    
    void getRTCData();                        // getting the time and temperature
    void getCurrentTime();                    // getting current time from the Real Time Clock module
    void getCurrentTemperature();             // getting current temperature from the Real Time Clock module
    
    byte _mode = 0;                           // current mode
    boolean delimiterOn = false;              // flag of separator display for blink
    boolean isFirstDigits = true;             // flag of is the first digit?
    boolean isChangeDigits = false;           // flag of was digit changed?
    boolean isShowDigits = false;             // flag of blink digit
    
    void delimiter(boolean on);               // the implementation to blink the separator
    void displayedTimeHourAdd();              // time settings. adding an hour
    void displayedTimeMinuteAdd();            // time settings. adding a minute
};