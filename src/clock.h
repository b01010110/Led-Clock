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
    void tick();                              // main method of the program
    void init();                              // the initialize method
    enum mode {                               // the modes
      showWatch,                                // time display
      showTemperature,                          // temperature display
      showSetup                                 // time set up display
    };
  private:
    byte _a;                                  // the number of port for rank a
    byte _b;                                  // the number of port for rank b
    byte _c;                                  // the number of port for rank c
    byte _d;                                  // the number of port for rank d
    byte _e;                                  // the number of port for rank e
    byte _f;                                  // the number of port for rank f
    byte _g;                                  // the number of port for rank g
    byte _h;                                  // the number of port for rank h
    byte _d1;                                 // the number of port for digit 1
    byte _d2;                                 // the number of port for digit 2
    byte _d3;                                 // the number of port for digit 3
    byte _d4;                                 // the number of port for digit 4
    byte _btn1;                               // the number of port for main button
    byte _btn2;                               // the number of port for addition button
    
    GButton btnMain;                          // main button
    GButton btnAddition;                      // addition button
    
    RTC_DS3231 rtc;                           // RTC module
    
    GTimer blinkTimer;                        // blink timer
    GTimer showTemperatureTimer;              // timer for show temperature
    GTimer hideTemperatureTimer;              // timer for hide temperature
    
    byte _currentDigit = 0;                   // currently displayed digit
    byte nextDigit();                         // switch to next digit. for implementation of dynamic indication
    void setDigit(byte digit, char symbol);   // display a digit on the screen
    
    void getCurrentTime();                    // get current time from the Real Time Clock module
    void getCurrentTemperature();             // get current temperature from the Real Time Clock module
    
    void watchAction();                       // the implementation of clock mode
    void setupAction();                       // the implementation of setup time mode
    void temperatureAction();                 // the implementation of temperature mode
    
    byte _mode = 0;                           // current mode
    boolean delimiterOn = false;              // flag of separator display for blink
    boolean isFirstDigits = true;             // flag of is the first digit?
    boolean isChangeDigits = false;           // flag of was digit changed?
    boolean isShowDigits = false;             // flag of blink digit
    
    void delimiter(boolean on);               // the implementation to blink the separator
    void displayedTimeHourAdd();              // time settings. adding an hour
    void displayedTimeMinuteAdd();            // time settings. adding a minute
};