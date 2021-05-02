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
 * Display a digit on the screen.
 */
void Clock::setDigit(byte digit, char symbol) {
  switch (symbol) {
    case '0':
      digitalWrite(_a, HIGH);
      digitalWrite(_b, HIGH);
      digitalWrite(_c, HIGH);
      digitalWrite(_d, HIGH);
      digitalWrite(_e, HIGH);
      digitalWrite(_f, HIGH);
      digitalWrite(_g, LOW);
      break;
    case '1':
      digitalWrite(_a, LOW);
      digitalWrite(_b, HIGH);
      digitalWrite(_c, HIGH);
      digitalWrite(_d, LOW);
      digitalWrite(_e, LOW);
      digitalWrite(_f, LOW);
      digitalWrite(_g, LOW);
      break;
    case '2':
      digitalWrite(_a, HIGH);
      digitalWrite(_b, HIGH);
      digitalWrite(_c, LOW);
      digitalWrite(_d, HIGH);
      digitalWrite(_e, HIGH);
      digitalWrite(_f, LOW);
      digitalWrite(_g, HIGH);
      break;
    case '3':
      digitalWrite(_a, HIGH);
      digitalWrite(_b, HIGH);
      digitalWrite(_c, HIGH);
      digitalWrite(_d, HIGH);
      digitalWrite(_e, LOW);
      digitalWrite(_f, LOW);
      digitalWrite(_g, HIGH);
      break;
    case '4':
      digitalWrite(_a, LOW);
      digitalWrite(_b, HIGH);
      digitalWrite(_c, HIGH);
      digitalWrite(_d, LOW);
      digitalWrite(_e, LOW);
      digitalWrite(_f, HIGH);
      digitalWrite(_g, HIGH);
      break;
    case '5':
      digitalWrite(_a, HIGH);
      digitalWrite(_b, LOW);
      digitalWrite(_c, HIGH);
      digitalWrite(_d, HIGH);
      digitalWrite(_e, LOW);
      digitalWrite(_f, HIGH);
      digitalWrite(_g, HIGH);
      break;
    case '6':
      digitalWrite(_a, HIGH);
      digitalWrite(_b, LOW);
      digitalWrite(_c, HIGH);
      digitalWrite(_d, HIGH);
      digitalWrite(_e, HIGH);
      digitalWrite(_f, HIGH);
      digitalWrite(_g, HIGH);
      break;
    case '7':
      digitalWrite(_a, HIGH);
      digitalWrite(_b, HIGH);
      digitalWrite(_c, HIGH);
      digitalWrite(_d, LOW);
      digitalWrite(_e, LOW);
      digitalWrite(_f, LOW);
      digitalWrite(_g, LOW);
      break;
    case '8':
      digitalWrite(_a, HIGH);
      digitalWrite(_b, HIGH);
      digitalWrite(_c, HIGH);
      digitalWrite(_d, HIGH);
      digitalWrite(_e, HIGH);
      digitalWrite(_f, HIGH);
      digitalWrite(_g, HIGH);
      break;
    case '9':
      digitalWrite(_a, HIGH);
      digitalWrite(_b, HIGH);
      digitalWrite(_c, HIGH);
      digitalWrite(_d, HIGH);
      digitalWrite(_e, LOW);
      digitalWrite(_f, HIGH);
      digitalWrite(_g, HIGH);
      break;
    case ' ':
      digitalWrite(_a, LOW);
      digitalWrite(_b, LOW);
      digitalWrite(_c, LOW);
      digitalWrite(_d, LOW);
      digitalWrite(_e, LOW);
      digitalWrite(_f, LOW);
      digitalWrite(_g, LOW);
      break;
    case '*':
      digitalWrite(_a, HIGH);
      digitalWrite(_b, HIGH);
      digitalWrite(_c, LOW);
      digitalWrite(_d, LOW);
      digitalWrite(_e, LOW);
      digitalWrite(_f, HIGH);
      digitalWrite(_g, HIGH);
      break;
    default:
      digitalWrite(_a, LOW);
      digitalWrite(_b, LOW);
      digitalWrite(_c, LOW);
      digitalWrite(_d, LOW);
      digitalWrite(_e, LOW);
      digitalWrite(_f, LOW);
      digitalWrite(_g, HIGH);
  }
  
  switch(digit) {
    case 0:
      digitalWrite(_d1, HIGH);
      digitalWrite(_d2, LOW);
      digitalWrite(_d3, LOW);
      digitalWrite(_d4, LOW);
      break;
    case 1:
      digitalWrite(_d1, LOW);
      digitalWrite(_d2, HIGH);
      digitalWrite(_d3, LOW);
      digitalWrite(_d4, LOW);
      break;
    case 2:
      digitalWrite(_d1, LOW);
      digitalWrite(_d2, LOW);
      digitalWrite(_d3, HIGH);
      digitalWrite(_d4, LOW);
      break;
    case 3:
      digitalWrite(_d1, LOW);
      digitalWrite(_d2, LOW);
      digitalWrite(_d3, LOW);
      digitalWrite(_d4, HIGH);
      break;
    default:
      digitalWrite(_d1, LOW);
      digitalWrite(_d2, LOW);
      digitalWrite(_d3, LOW);
      digitalWrite(_d4, LOW);
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
 * Switch to next digit. For implementation of dynamic indication.
 */
byte Clock::nextDigit() {
  byte digit = _currentDigit;
  if (++_currentDigit > 3) _currentDigit = 0;
  return digit;
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
 * The implementation of clock mode.
 */
void Clock::watchAction() {
  displayedSymbols = time;
  byte digit = nextDigit();
  setDigit(digit, displayedSymbols[digit]);
  delimiter();
}

/**
 * The implementation of setup time mode.
 */
void Clock::setupAction() {
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
  
  byte digit = nextDigit();
  if (isFirstDigits && !isChangeDigits && !isShowDigits && digit < 2) digit = 255;
  if (!isFirstDigits && !isChangeDigits && !isShowDigits && digit > 1) digit = 255;
  setDigit(digit, displayedSymbols[digit > 3 ? 0 : digit]);
  delimiter(false);
}

/**
 * The implementation of temperature mode.
 */
void Clock::temperatureAction() {
  displayedSymbols = temperature;
  byte digit = nextDigit();
  setDigit(digit, displayedSymbols[digit]);
  delimiter(false);
}

/**
 * Main method of the program.
 */
void Clock::tick() {
  btnMain.tick();
  btnAddition.tick();
  
  // get time and temperature
  if (_currentDigit == 0) {
    getCurrentTime();
    getCurrentTemperature();
  }
  
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
    watchAction();
  } else if (_mode == showSetup) {
    setupAction();
  } else if (_mode == showTemperature) {
    temperatureAction();
    if (hideTemperatureTimer.isReady()) _mode = showWatch;
  }
  
  delay(4);
}