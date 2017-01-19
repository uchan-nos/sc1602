# SC1602 character LCD controller for Arduino

## Import this library to Arduino IDE

see [Installing Additional Arduino Libraries](https://www.arduino.cc/en/Guide/Libraries).

## Basic usage

    #include <sc1602.h>

    SC1602<true> lcd(
        4, 5, 6, 7, // DB4-DB7
        8, 9);      // RS, E

    void setup() {
        delay(40);
        lcd.initialize();
    }

    void loop() {
        lcd.clear();
        lcd.put("hello");
        delay(1000);
        lcd.cursor_at(0, 1);
        lcd.put("world");
        delay(1000);
    }

1. Instanciate SC1602 class with port definitions.
    - A template argument `Neg` is used to declare that your Arduino
    connects to SC1602 via any kinds of circuit which inverts signal logic.
    In that case `Neg` argument should be true, otherwise false.
2. Call `initialize` method within `setup`.
    - You must wait some duration before calling `initialize`.
3. 