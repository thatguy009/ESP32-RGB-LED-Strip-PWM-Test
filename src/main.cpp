#include <Arduino.h>
#include <colours.h>

#define green_Ch 0
#define red_Ch 1
#define blue_Ch 2

#define PWM1_Res 8
#define pwm_Freq 1000
#define PWN_Delay 10
#define PWM_Sleep 100

const int redPin = 17;
const int greenPin = 18;
const int bluePin = 16;

HSVColour currentColour = HSVColourList.GetRandom();

void setup()
{
  Serial.begin(115200);
  digitalWrite(redPin, LOW);
  digitalWrite(greenPin, LOW);
  digitalWrite(bluePin, LOW);

  ledcAttachPin(greenPin, green_Ch);
  ledcSetup(green_Ch, pwm_Freq, PWM1_Res);

  ledcAttachPin(redPin, red_Ch);
  ledcSetup(red_Ch, pwm_Freq, PWM1_Res);

  ledcAttachPin(bluePin, blue_Ch);
  ledcSetup(blue_Ch, pwm_Freq, PWM1_Res);

  if (Serial.availableForWrite())
  {
    Serial.println("Serial is ready");
  }
}

void updateRGB()
{
  RGBColour rgb = currentColour.ToRGB();

  ledcWrite(red_Ch, rgb.Red);
  ledcWrite(green_Ch, rgb.Green);
  ledcWrite(blue_Ch, rgb.Blue);

  if (Serial.availableForWrite())
  {
    Serial.print("Set Colour to: " + currentColour.ToString());
  }
}

String ColourNamesPretty = HSVColourList.GetColourNameList("     ", ",\n");

void loop()
{
  if (Serial.available())
  {
    Serial.println("Serial available, Awaiting input");
    Serial.println();
    Serial.println("Valid values are:");
    Serial.println();
    Serial.println("  - RGB([0-255],[0-255],[0-255])");
    Serial.println();
    Serial.println("  - HSV([0-360])");
    Serial.println("  - HSV([0-360],[0-100],[0-100])");
    Serial.println();
    Serial.println("  - Colour names: ");
    Serial.println(ColourNamesPretty);

    Serial.println("Enter a colour value");

    String input = Serial.readStringUntil('\n');
    input.trim();
    Serial.println("Received: " + input);

    if (input.startsWith("RGB"))
    {
      String r = input.substring(3, input.indexOf(","));
      String g = input.substring(input.indexOf(",") + 1, input.lastIndexOf(","));
      String b = input.substring(input.lastIndexOf(",") + 1);
      r.trim();
      g.trim();
      b.trim();
      Serial.println("R: " + r + " G: " + g + " B: " + b);
      currentColour = RGBColour(r.toInt(), g.toInt(), b.toInt()).ToHSV();
    }
    else if (input.startsWith("HSV"))
    {
      String h = input.substring(3, input.indexOf(","));
      String s = "100";
      String v = "100";
      h.trim();
      if (input.indexOf(",") == -1)
      {
        currentColour = HSVColour(h.toInt(), 100, 100);
      }
      else
      {
        s = input.substring(input.indexOf(",") + 1, input.lastIndexOf(","));
        v = input.substring(input.lastIndexOf(",") + 1);
        s.trim();
        v.trim();
      }

      Serial.println("H: " + h + " S: " + s + " V: " + v);
      currentColour = HSVColour(h.toInt(), s.toInt(), v.toInt());
    }
    else if (ColourNamesPretty.indexOf(input) != -1 && input.length() > 2)
    {
      currentColour = HSVColourList.Get(input);
    }
    else
    {
      Serial.println("Invalid input {" + input + "}, or serial timeout expired");
      currentColour = HSVColourList.GetRandom();
    }
  }
  else
  {
    currentColour = HSVColourList.GetNext(currentColour);
  }

  updateRGB();
}
