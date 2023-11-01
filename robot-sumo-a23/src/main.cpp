#include <Arduino.h>
#include <NeoPixelBus.h>

NeoPixelBus<NeoGrbwFeature, NeoSk6812Method> strip(1, 38);

RgbwColor red(128, 0, 0,0);
RgbwColor green(0, 128, 0,0);
RgbwColor blue(0, 0, 128,0);

void setup()
{
    Serial.begin(9600);

    // this resets all the neopixels to an off state
    strip.Begin();
    strip.Show();
}

void loop() 
{ 
    strip.SetPixelColor(0,red);
    strip.Show();
    Serial.print("Rouge\n");
    delay(500);
    strip.SetPixelColor(0,blue);
    strip.Show();
    Serial.print("Bleu\n");
    delay(500);
    strip.SetPixelColor(0,green);
    strip.Show();
    Serial.print("Vert\n");
    delay(500);
} 