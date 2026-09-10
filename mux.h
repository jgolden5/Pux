#pragma once
#ifndef MUX_H
#define MUX_H

class Mux
{
private:
    bool states[4];

public:
    Mux();

    void on(int pin);
    void off(int pin);

    int getValue();
};

#endif
