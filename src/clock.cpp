#include "clock.h"

/**
 * Constructor.
 */
Clock::Clock(byte a, byte b, byte c, byte d, byte e, byte f, byte g, byte h, byte d1, byte d2, byte d3, byte d4, byte btn1, byte btn2) {
  _a = a;
  _b = b;
  _c = c;
  _d = d;
  _e = e;
  _f = f;
  _g = g;
  _h = h;
  _d1 = d1;
  _d2 = d2;
  _d3 = d3;
  _d4 = d4;
  _btn1 = btn1;
  _btn2 = btn2;

  segments[0] = _a;
  segments[1] = _b;
  segments[2] = _c;
  segments[3] = _d;
  segments[4] = _e;
  segments[5] = _f;
  segments[6] = _g;

  ranks[0] = _d1;
  ranks[1] = _d2;
  ranks[2] = _d3;
  ranks[3] = _d4;

  pinMode(_a, OUTPUT);
  pinMode(_b, OUTPUT);
  pinMode(_c, OUTPUT);
  pinMode(_d, OUTPUT);
  pinMode(_e, OUTPUT);
  pinMode(_f, OUTPUT);
  pinMode(_g, OUTPUT);
  pinMode(_h, OUTPUT);
  pinMode(_d1, OUTPUT);
  pinMode(_d2, OUTPUT);
  pinMode(_d3, OUTPUT);
  pinMode(_d4, OUTPUT);
}

/**
 * Display the data on the screen.
 */
void Clock::displayData() {
  byte rank = _currentDigit;
  char symbol = displayedSymbols[rank];
  
  if (++_currentDigit > 3) _currentDigit = 0;

  if (_mode == showSetup) {
    if (isFirstDigits && !isChangeDigits && !isShowDigits && rank < 2) rank = 255;
    if (!isFirstDigits && !isChangeDigits && !isShowDigits && rank > 1) rank = 255;
  }

  // neighbors light fix
  for (byte i = 0; i < 7; i++) {
    digitalWrite(segments[i], LOW);
  }
  
  byte rankPorts;

  switch(rank) {
    case 0: rankPorts = 0b00000001; break;
    case 1: rankPorts = 0b00000010; break;
    case 2: rankPorts = 0b00000100; break;
    case 3: rankPorts = 0b00001000; break;
    default: rankPorts = 0b00000000;
  }

  for (byte i = 0; i < 4; i++) {
    digitalWrite(ranks[i], (rankPorts >> i) & 1);
  }

  byte symbolPorts;

  switch (symbol) {
    case '0': symbolPorts = 0b00111111; break;
    case '1': symbolPorts = 0b00000110; break;
    case '2': symbolPorts = 0b01011011; break;
    case '3': symbolPorts = 0b01001111; break;
    case '4': symbolPorts = 0b01100110; break;
    case '5': symbolPorts = 0b01101101; break;
    case '6': symbolPorts = 0b01111101; break;
    case '7': symbolPorts = 0b00000111; break;
    case '8': symbolPorts = 0b01111111; break;
    case '9': symbolPorts = 0b01101111; break;
    case ' ': symbolPorts = 0b00000000; break;
    case '*': symbolPorts = 0b01100011; break;
    default: symbolPorts = 0b01000000;
  }

  for (byte i = 0; i < 7; i++) {
    digitalWrite(segments[i], (symbolPorts >> i) & 1);
  }
}

/**
 * Get current time from the Real Time Clock module.
 */
void Clock::getCurrentTime() {
  DateTime now = rtc.now();
  
  byte hour = now.hour();
  time[0] = String(hour / 10)[0];
  time[1] = String(hour % 10)[0];
  
  byte minute = now.minute();
  time[2] = String(minute / 10)[0];
  time[3] = String(minute % 10)[0];
}

/**
 * Get current temperature from the Real Time Clock module.
 */
void Clock::getCurrentTemperature() {
  int celsius = round(rtc.getTemperature());
  celsius -= 3;
  
  temperature[0] = ' ';
  temperature[1] = '*';
  temperature[2] = String(celsius / 10)[0];
  temperature[3] = String(celsius % 10)[0];
}

/**
 * The implementation to blink the separator.
 */
void Clock::delimiter(boolean on = true) {
  if (on) {
    if (blinkTimer.isReady()) {
      digitalWrite(_h, delimiterOn ? LOW : HIGH);
      delimiterOn = !delimiterOn;
    }
  } else {
    delimiterOn = false;
    digitalWrite(_h, LOW);
  }
}

/**
 * The initialize method.
 */
void Clock::init() {
  rtc.begin();

  rtcTimer = GTimer(MS, 300);
  blinkTimer = GTimer(MS, 500);
  showTemperatureTimer = GTimer(MS, 30 * 1000);
  hideTemperatureTimer = GTimer();
  
  btnMain = GButton(_btn1);
  btnMain.setTimeout(2000);
  
  btnAddition = GButton(_btn2);
  btnAddition.setStepTimeout(200);
}

/**
 * Time settings. Adding an hour.
 */
void Clock::displayedTimeHourAdd() {
  byte hours = (displayedSymbols[0] - '0') * 10 + (displayedSymbols[1] - '0') + 1;
  if (hours > 23) hours = 0;
  
  displayedSymbols[0] = String(hours / 10)[0];
  displayedSymbols[1] = String(hours % 10)[0];
}

/**
 * Time settings. Adding a minute.
 */
void Clock::displayedTimeMinuteAdd() {
  byte minutes = (displayedSymbols[2] - '0') * 10 + (displayedSymbols[3] - '0') + 1;
  if (minutes > 59) minutes = 0;
  
  displayedSymbols[2] = String(minutes / 10)[0];
  displayedSymbols[3] = String(minutes % 10)[0];
}

/**
 * Getting the time and temperature.
 */
void Clock::getRTCData() {
  if (!rtcTimer.isReady()) return;

  getCurrentTime();
  getCurrentTemperature();
}

/**
 * Main method of the program.
 */
void Clock::tick() {
  getRTCData();

  btnMain.tick();
  btnAddition.tick();
  
  // switch to setup time mode
  if (btnMain.isHold()) {
    hideTemperatureTimer.reset();
    _mode = showSetup;
    displayedSymbols = time;
  }
  
  // switch to temperature mode
  if (showTemperatureTimer.isReady() && _mode == showWatch) {
    _mode = showTemperature;
    hideTemperatureTimer.setTimeout(5000);
  }
  
  if (_mode == showWatch) {
    displayedSymbols = time;
    delimiter();
  } else if (_mode == showSetup) {
    // blink the digit before setup
    if (blinkTimer.isReady()) isShowDigits = !isShowDigits;
    
    // setup time
    if (btnAddition.isStep() || btnAddition.isClick()) {
      isChangeDigits = true;
      if (isFirstDigits) {
        displayedTimeHourAdd();
      } else {
        displayedTimeMinuteAdd();
      }
    }
    
    // switch to setting next digit or save time in the RTC
    if (btnMain.isClick()) {
      if (isFirstDigits) {
        isFirstDigits = false;
        isChangeDigits = false;
      } else {
        isFirstDigits = true;
        isChangeDigits = false;
        rtc.adjust(DateTime(2021, 4, 27, (displayedSymbols[0] - '0') * 10 + (displayedSymbols[1] - '0'), (displayedSymbols[2] - '0') * 10 + (displayedSymbols[3] - '0'), 0));
        _mode = showWatch;
      }
    }

    delimiter(false);
  } else if (_mode == showTemperature) {
    displayedSymbols = temperature;
    delimiter(false);
    if (hideTemperatureTimer.isReady()) _mode = showWatch;
  }
}