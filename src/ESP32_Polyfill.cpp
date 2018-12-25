/*
 * MIT License
 * 
 * Copyright (c) 2018 Nuntipat Narkthong (nuntipat@gmail.com)
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "ESP32_Polyfill.h"

const uint8_t pinList[] = {13, 14, 16, 17, 18, 19, 21, 22, 23, 25, 26, 27};
constexpr uint8_t channelCount = sizeof(pinList) / sizeof(pinList[0]);

bool pinUsed[channelCount] = {false};

// find ledc channel for a pin
uint8_t getChannel(uint8_t pin)
{
    uint8_t channel = 0;
    for (channel = 0; channel < channelCount; channel++)
        if (pinList[channel] == pin)
            break;
    return channel;
}

void analogWrite(uint8_t pin, uint16_t value)
{
    // do nothing if channel can't be found
    uint8_t channel = getChannel(pin);
    if (channel == channelCount)
        return;

    if (value == 0 || value == 255)
    {
        if (!pinUsed[channel])
        {
            digitalWrite(pin, (value == 255) ? HIGH : LOW);
        }
        else
        {
            digitalWrite(pin, (value == 255) ? HIGH : LOW);
			ledcDetachPin(pin);
            pinUsed[channel] = false;
        }
    }
    else
    {
        if (!pinUsed[channel]) 
        {
            pinUsed[channel] = true;
            ledcSetup(channel, 1000, 8);    // 1KHz 8bit
            ledcWrite(channel, value);
            ledcAttachPin(pin, channel);
        }
        else
        {
            ledcWrite(channel, value);
        }
    }
}

void tone(uint8_t pin, uint16_t freq)
{
    // do nothing if channel can't be found
    uint8_t channel = getChannel(pin);
    if (channel == channelCount)
        return;

    if (!pinUsed[channel]) 
    {
        pinUsed[channel] = true;
        ledcSetup(channel, 1000, 8);    // any value can be used as ledcWriteTone will override them anyway
        ledcWriteTone(channel, freq);
        ledcAttachPin(pin, channel);
    }
    else
    {
        ledcWriteTone(channel, freq);
    }
}

void noTone(uint8_t pin)
{
    // do nothing if channel can't be found
    uint8_t channel = getChannel(pin);
    if (channel == channelCount)
        return;

    if (!pinUsed[channel])
    {
        pinUsed[channel] = true;
        ledcSetup(channel, 1000, 8);    // any value can be used as ledcWriteTone will override them anyway
        ledcWriteTone(channel, 0);
        ledcAttachPin(pin, channel);
    }
    else
    {
        ledcWriteTone(channel, 0);
    }
}