#ifndef SC1602_H_
#define SC1602_H_

#include <stdint.h>

#ifdef RPI
# include <wiringPi.h>
#else
# include <Arduino.h>
#endif

#if HIGH != 1 || LOW != 0
#error SC1602 Library assumes (HIGH, LOW) is (1, 0)
#endif

/**
 * \param Neg true if signal lines are negative logic.
 */
template <bool Neg>
class SC1602 {
    const int db4_, db5_, db6_, db7_, rs_, e_;

    void digitalWrite(uint8_t pin, uint8_t b);
    void out_4(unsigned char rs, unsigned char value);
    void out_8(unsigned char rs, unsigned char value);

public:
    /** SC1602 creates an instance for a SC1602 display
     * with the given port definitions.
     *
     * \param db4 Port number for DB4 signal.
     * \param db5 Port number for DB5 signal.
     * \param db6 Port number for DB6 signal.
     * \param db7 Port number for DB7 signal.
     * \param rs Port number for RS signal.
     * \param e Port number for E signal.
     */
    SC1602(int db4, int db5, int db6, int db7, int rs, int e);

    /** initialize initializes this display. */
    void initialize();

    /** exec executes the given command (RS=0). */
    void exec(unsigned char cmd);

    /** put puts the given character (RS=1). */
    void put(char c);

    /** put puts the given string. */
    void put(const char* s);

    /** cursor_at moves a cursor at the given position. */
    void cursor_at(unsigned char x, unsigned char y);

    /** clear clears this display and moves cursor at home position. */
    void clear();
};

template <bool Neg>
SC1602<Neg>::SC1602(int db4, int db5, int db6, int db7, int rs, int e)
: db4_(db4), db5_(db5), db6_(db6), db7_(db7),
  rs_(rs), e_(e)
{}

template <bool Neg>
void SC1602<Neg>::digitalWrite(uint8_t pin, uint8_t b)
{
    ::digitalWrite(pin, 1 - b);
}

template <>
void SC1602<false>::digitalWrite(uint8_t pin, uint8_t b)
{
    ::digitalWrite(pin, b);
}

template <bool Neg>
void SC1602<Neg>::out_4(unsigned char rs, unsigned char value)
{
    digitalWrite(rs_, rs);
    digitalWrite(e_, HIGH);
    digitalWrite(db4_, (value & 0x1u) != 0);
    digitalWrite(db5_, (value & 0x2u) != 0);
    digitalWrite(db6_, (value & 0x4u) != 0);
    digitalWrite(db7_, (value & 0x8u) != 0);
    delayMicroseconds(1);
    digitalWrite(e_, LOW);
}

template <bool Neg>
void SC1602<Neg>::out_8(unsigned char rs, unsigned char value)
{
    out_4(rs, value >> 4u);
    delayMicroseconds(1);
    out_4(rs, value & 0x0fu);
}

template <bool Neg>
void SC1602<Neg>::initialize()
{
    pinMode(db4_, OUTPUT);
    pinMode(db5_, OUTPUT);
    pinMode(db6_, OUTPUT);
    pinMode(db7_, OUTPUT);
    pinMode(rs_, OUTPUT);
    pinMode(e_, OUTPUT);

    out_4(0, 0x3u);
    delayMicroseconds(4100);
    out_4(0, 0x3u);
    delayMicroseconds(40);
    out_4(0, 0x3u);
    delayMicroseconds(40);
    out_4(0, 0x2u);  // 4bit mode after this line
    delayMicroseconds(40);

    exec(0x28u); // function set: 4bit, 2lines, 5x7dots
    exec(0x08u); // display on/off: display off, cursor off, no blink
    exec(0x01u); // clear display
    exec(0x06u); // entry mode set: increment, with display shift
    exec(0x0cu); // display on/off: display on, cursor off, no blink
}

template <bool Neg>
void SC1602<Neg>::exec(unsigned char cmd)
{
    out_8(0, cmd);
    if (cmd == 0x01u || cmd == 0x02u) {
        delayMicroseconds(1640);
    } else {
        delayMicroseconds(40);
    }
}

template <bool Neg>
void SC1602<Neg>::put(char c)
{
    out_8(1, c);
    delayMicroseconds(40);
}

template <bool Neg>
void SC1602<Neg>::put(const char* s)
{
    while (*s) {
        put(*s);
        ++s;
    }
}

template <bool Neg>
void SC1602<Neg>::cursor_at(unsigned char x, unsigned char y)
{
    if (y == 0) {
        exec(0x80u | x);
    } else if (y == 1) {
        exec(0x80u | 0x40u | x);
    }
}

template <bool Neg>
void SC1602<Neg>::clear()
{
    exec(0x01u);
}

#endif // SC1602_H_
