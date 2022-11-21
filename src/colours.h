#include <Arduino.h>

struct RGBColour
{
    int Red;
    int Green;
    int Blue;
    String Name;

    String ToString()
    {
        return "RGB(" + String(Red) + "," + String(Green) + "," + String(Blue) + ")";
    }

    String GetName()
    {
        for (auto &&colour : BasicColours)
        {
            if (colour == *this)
            {
                return colour.Name;
            }
        }

        for (auto &&colour : ExtendedColours)
        {
            if (colour == *this)
            {
                return colour.Name;
            }
        }
        return "Unknown";
    }

    RGBColour(int red, int green, int blue)
    {
        if (red > 255)
        {
            red = 255;
        }
        Red = red;

        if (green > 255)
        {
            green = 255;
        }
        Green = green;

        if (blue > 255)
        {
            blue = 255;
        }
        Blue = blue;

        if (Name.isEmpty())
        {
            Name = GetName();
        }
    }

    RGBColour(String name, int red, int green, int blue)
    {
        Name = name;
        RGBColour(red, green, blue);
    }

    RGBColour Invert() const
    {
        return RGBColour(255 - Red, 255 - Green, 255 - Blue);
    }

    bool operator==(const RGBColour &other) const
    {
        return Red == other.Red && Green == other.Green && Blue == other.Blue;
    }

    RGBColour operator+(const RGBColour &other) const
    {
        return RGBColour(Red + other.Red, Green + other.Green, Blue + other.Blue);
    }

    RGBColour operator-(const RGBColour &other) const
    {
        return RGBColour(Red - other.Red, Green - other.Green, Blue - other.Blue);
    }

    HSVColour ToHSV() const
    {
        float r = Red / 255.0;
        float g = Green / 255.0;
        float b = Blue / 255.0;

        float max = std::max(r, std::max(g, b));
        float min = std::min(r, std::min(g, b));

        float h = 0;
        float s = 0;
        float v = max;

        float d = max - min;
        s = max == 0 ? 0 : d / max;

        if (max == min)
        {
            h = 0; // achromatic
        }
        else
        {
            if (max == r)
            {
                h = (g - b) / d + (g < b ? 6 : 0);
            }
            else if (max == g)
            {
                h = (b - r) / d + 2;
            }
            else if (max == b)
            {
                h = (r - g) / d + 4;
            }
            h /= 6;
        }

        return HSVColour(h, s, v);
    }
};

struct HSVColour
{
    int Hue;
    int Saturation;
    int Value;
    String Name;

    HSVColour(int hue)
    {
        HSVColour(hue, 100, 100);
    }

    HSVColour(int hue, int saturation, int value)
    {
        if (hue > 360)
            hue = 360;
        if (saturation > 100)
            saturation = 100;
        if (value > 100)
            value = 100;
        Hue = hue;
        Saturation = saturation;
        Value = value;
        Name = GetName();
    }

    HSVColour Invert() const
    {
        return HSVColour(360 - Hue, 100 - Saturation, 100 - Value);
    }

    HSVColour operator+(const HSVColour &other) const
    {
        return HSVColour(Hue + other.Hue, Saturation + other.Saturation, Value + other.Value);
    }

    HSVColour operator-(const HSVColour &other) const
    {
        return HSVColour(Hue - other.Hue, Saturation - other.Saturation, Value - other.Value);
    }

    RGBColour ToRGB() const
    {
        int c = Value * Saturation;
        int x = c * (1 - abs((Hue / 60) % 2 - 1));
        int m = Value - c;

        int r = 0;
        int g = 0;
        int b = 0;

        if (Hue >= 0 && Hue < 60)
        {
            r = c;
            g = x;
            b = 0;
        }
        else if (Hue >= 60 && Hue < 120)
        {
            r = x;
            g = c;
            b = 0;
        }
        else if (Hue >= 120 && Hue < 180)
        {
            r = 0;
            g = c;
            b = x;
        }
        else if (Hue >= 180 && Hue < 240)
        {
            r = 0;
            g = x;
            b = c;
        }
        else if (Hue >= 240 && Hue < 300)
        {
            r = x;
            g = 0;
            b = c;
        }
        else if (Hue >= 300 && Hue < 360)
        {
            r = c;
            g = 0;
            b = x;
        }

        return RGBColour((r + m) * 255, (g + m) * 255, (b + m) * 255);
    }

    String ToString() const
    {
        return String(Hue) + "," + String(Saturation) + "," + String(Value);
    }

    String GetName() const
    {
        switch (Hue)
        {
        case 30:
            return "Orange";
            break;
        case 60:
            return "Yellow";
            break;
        case 90:
            return "Lime"; // chartreuse
            break;
        case 120:
            return "Green";
            break;
        case 150:
            return "Aqua"; // spring green
            break;
        case 180:
            return "Blue"; // cyan
            break;
        case 210:
            return "Purple"; // azure
            break;
        case 240:
            return "Magenta"; // blue
            break;
        case 270:
            return "Pink"; // violet
            break;
        case 300:
            return "Crimson"; // magneta
            break;
        case 330:
            return "Maroon"; // rose
            break;
        default:
            return "Red";
            break;
        }
    }
};

struct HSVList
{
    HSVColour colours[13] = {{0}, {30}, {60}, {90}, {120}, {150}, {180}, {210}, {240}, {270}, {300}, {330}, {360}};
    int count = 13;

    HSVColour Get(int index)
    {
        return colours[index];
    }

    HSVColour Get(String name)
    {
        for (int i = 0; i < count; i++)
        {
            if (name.equalsIgnoreCase(colours[i].GetName()))
            {
                return colours[i];
            }
        }
        throw "Colour {" + name + "} not found";
    }

    HSVColour GetRandom()
    {
        return colours[random(count)];
    }

    HSVColour GetNext(HSVColour colour)
    {
        for (int i = 0; i < count; i++)
        {
            if (colours[i].Hue == colour.Hue)
            {
                if (i == count - 1)
                {
                    return colours[0];
                }
                else
                {
                    return colours[i + 1];
                }
            }
        }
    }

    HSVColour GetPrevious(HSVColour colour)
    {
        for (int i = 0; i < count; i++)
        {
            if (colours[i].Hue == colour.Hue)
            {
                if (i == 0)
                {
                    return colours[count - 1];
                }
                else
                {
                    return colours[i - 1];
                }
            }
        }
    }

    String GetColourNameList(String prefix = "", String separator = ",") const
    {
        String list = "";
        for (int i = 0; i < count; i++)
        {
            list += prefix + colours[i].GetName();
            if (i < count - 1)
            {
                list += separator;
            }
        }
        return list;
    }
} HSVColourList;

const RGBColour BasicColours[]{
    {"Red", 255, 0, 0},
    {"Green", 0, 255, 0},
    {"Blue", 0, 0, 255},
    {"Yellow", 255, 255, 0},
    {"Cyan", 0, 255, 255},
    {"Magenta", 255, 0, 255},
    {"White", 255, 255, 255},
    {"Black", 0, 0, 0}};

const RGBColour ExtendedColours[]{
    {"AliceBlue", 240, 248, 255},
    {"AntiqueWhite", 250, 235, 215},
    {"Aqua", 0, 255, 255},
    {"Aquamarine", 127, 255, 212},
    {"Azure", 240, 255, 255},
    {"Beige", 245, 245, 220},
    {"Bisque", 255, 228, 196},
    {"Black", 0, 0, 0},
    {"BlanchedAlmond", 255, 235, 205},
    {"Blue", 0, 0, 255},
    {"BlueViolet", 138, 43, 226},
    {"Brown", 165, 42, 42},
    {"BurlyWood", 222, 184, 135},
    {"CadetBlue", 95, 158, 160},
    {"Chartreuse", 127, 255, 0},
    {"Chocolate", 210, 105, 30},
    {"Coral", 255, 127, 80},
    {"CornflowerBlue", 100, 149, 237},
    {"Cornsilk", 255, 248, 220},
    {"Crimson", 220, 20, 60},
    {"Cyan", 0, 255, 255},
    {"DarkBlue", 0, 0, 139},
    {"DarkCyan", 0, 139, 139},
    {"DarkGoldenRod", 184, 134, 11},
    {"DarkGray", 169, 169, 169},
    {"DarkGrey", 169, 169, 169},
    {"DarkGreen", 0, 100, 0},
    {"DarkKhaki", 189, 183, 107},
    {"DarkMagenta", 139, 0, 139},
    {"DarkOliveGreen", 85, 107, 47},
    {"DarkOrange", 255, 140, 0},
    {"DarkOrchid", 153, 50, 204},
    {"DarkRed", 139, 0, 0},
    {"DarkSalmon", 233, 150, 122},
    {"DarkSeaGreen", 143, 188, 143},
    {"DarkSlateBlue", 72, 61, 139},
    {"DarkSlateGray", 47, 79, 79},
    {"DarkSlateGrey", 47, 79, 79},
    {"DarkTurquoise", 0, 206, 209},
    {"DarkViolet", 148, 0, 211},
    {"DeepPink", 255, 20, 147},
    {"DeepSkyBlue", 0, 191, 255},
    {"DimGray", 105, 105, 105},
    {"DimGrey", 105, 105, 105},
    {"DodgerBlue", 30, 144, 255},
    {"FireBrick", 178, 34, 34},
    {"FloralWhite", 255, 250, 240},
    {"ForestGreen", 34, 139, 34},
    {"Fuchsia", 255, 0, 255},
    {"Gainsboro", 220, 220, 220},
    {"GhostWhite", 248, 248, 255},
    {"Gold", 255, 215, 0},
    {"GoldenRod", 218, 165, 32},
    {"Gray", 128, 128, 128},
    {"Grey", 128, 128, 128},
    {"Green", 0, 128, 0},
    {"GreenYellow", 173, 255, 47},
    {"HoneyDew", 240, 255, 240},
    {"HotPink", 255, 105, 180},
    {"IndianRed", 205, 92, 92},
    {"Indigo", 75, 0, 130},
    {"Ivory", 255, 255, 240},
    {"Khaki", 240, 230, 140},
    {"Lavender", 230, 230, 250},
    {"LavenderBlush", 255, 240, 245},
    {"LawnGreen", 124, 252, 0},
    {"LemonChiffon", 255, 250, 205},
    {"LightBlue", 173, 216, 230},
    {"LightCoral", 240, 128, 128},
    {"LightCyan", 224, 255, 255},
    {"LightGoldenRodYellow", 250, 250, 210},
    {"LightGray", 211, 211, 211},
    {"LightGrey", 211, 211, 211},
    {"LightGreen", 144, 238, 144},
    {"LightPink", 255, 182, 193},
    {"LightSalmon", 255, 160, 122},
    {"LightSeaGreen", 32, 178, 170},
    {"LightSkyBlue", 135, 206, 250},
    {"LightSlateGray", 119, 136, 153},
    {"LightSlateGrey", 119, 136, 153},
    {"LightSteelBlue", 176, 196, 222},
    {"LightYellow", 255, 255, 224},
    {"Lime", 0, 255, 0},
    {"LimeGreen", 50, 205, 50},
    {"Linen", 250, 240, 230},
    {"Magenta", 255, 0, 255},
    {"Maroon", 128, 0, 0},
    {"MediumAquaMarine", 102, 205, 170},
    {"MediumBlue", 0, 0, 205},
    {"MediumOrchid", 186, 85, 211},
    {"MediumPurple", 147, 112, 219},
    {"MediumSeaGreen", 60, 179, 113},
    {"MediumSlateBlue", 123, 104, 238},
    {"MediumSpringGreen", 0, 250, 154},
    {"MediumTurquoise", 72, 209, 204},
    {"MediumVioletRed", 199, 21, 133},
    {"MidnightBlue", 25, 25, 112},
    {"MintCream", 245, 255, 250},
    {"MistyRose", 255, 228, 225},
    {"Moccasin", 255, 228, 181},
    {"NavajoWhite", 255, 222, 173},
    {"Navy", 0, 0, 128},
    {"OldLace", 253, 245, 230},
    {"Olive", 128, 128, 0},
    {"OliveDrab", 107, 142, 35},
    {"Orange", 255, 165, 0},
    {"OrangeRed", 255, 69, 0},
    {"Orchid", 218, 112, 214},
    {"PaleGoldenRod", 238, 232, 170},
    {"PaleGreen", 152, 251, 152},
    {"PaleTurquoise", 175, 238, 238},
    {"PaleVioletRed", 219, 112, 147},
    {"PapayaWhip", 255, 239, 213},
    {"PeachPuff", 255, 218, 185},
    {"Peru", 205, 133, 63},
    {"Pink", 255, 192, 203},
    {"Plum", 221, 160, 221},
    {"PowderBlue", 176, 224, 230},
    {"Purple", 128, 0, 128},
    {"RebeccaPurple", 102, 51, 153},
    {"Red", 255, 0, 0},
    {"RosyBrown", 188, 143, 143},
    {"RoyalBlue", 65, 105, 225},
    {"SaddleBrown", 139, 69, 19},
    {"Salmon", 250, 128, 114},
    {"SandyBrown", 244, 164, 96},
    {"SeaGreen", 46, 139, 87},
    {"SeaShell", 255, 245, 238},
    {"Sienna", 160, 82, 45},
    {"Silver", 192, 192, 192},
    {"SkyBlue", 135, 206, 235},
    {"SlateBlue", 106, 90, 205},
    {"SlateGray", 112, 128, 144},
    {"SlateGrey", 112, 128, 144},
    {"Snow", 255, 250, 250},
    {"SpringGreen", 0, 255, 127},
    {"SteelBlue", 70, 130, 180},
    {"Tan", 210, 180, 140},
    {"Teal", 0, 128, 128},
    {"Thistle", 216, 191, 216},
    {"Tomato", 255, 99, 71},
    {"Turquoise", 64, 224, 208},
    {"Violet", 238, 130, 238},
    {"Wheat", 245, 222, 179},
    {"White", 255, 255, 255},
    {"WhiteSmoke", 245, 245, 245},
    {"Yellow", 255, 255, 0},
    {"YellowGreen", 154, 205, 50}};
