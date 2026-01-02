using SplashKitSDK;
using static SplashKitSDK.SplashKit;

void TestDrawingOnNewWindow()
{
    Bitmap userImage;

    userImage = CreateBitmap ("user_image", 20, 20);
    SaveBitmap(userImage, "0");
    ClearBitmap (userImage, Color.BrightGreen);
    SaveBitmap(userImage, "1");
    FillRectangleOnBitmap (userImage, Color.Black, 0, 0, 10, 10);
    SaveBitmap(userImage, "2");

    Window myWindow = OpenWindow ("Black TL+BR", 200, 200);
    ClearWindow (myWindow, Color.White);
    FillRectangleOnBitmap (userImage, Color.Black, 10, 10, 10, 10);
    SaveBitmap(userImage, "3");

    DrawBitmapOnWindow (myWindow, userImage, 0, 0);
    SaveBitmap(userImage, "4");
    RefreshWindow(myWindow);
    
    Bitmap userImage1;
    
    userImage1 = CreateBitmap ("user_image1", 20, 20);
    ClearBitmap (userImage1, Color.Black);
    FillRectangleOnBitmap (userImage1, Color.BrightGreen, 10, 0, 10, 10);
    FillRectangleOnBitmap (userImage1, Color.BrightGreen, 0, 10, 10, 10);
    
    DrawBitmapOnWindow (myWindow, userImage1, 30, 0);
    SaveBitmap(userImage, "4");
    
    // Get pixel from bitmap
    Color bitmapClr0 = GetPixel(userImage1, 0, 0);
    Color bitmapClr1 = GetPixel(userImage1, 0, 10);
    string bitmapClrStr0 = "Clr@0,0 " + ColorToString(bitmapClr0);
    string bitmapClrStr1 = "Clr@0,10 " + ColorToString(bitmapClr1);
    DrawTextOnWindow(myWindow, bitmapClrStr0, Color.Black, 0, 20);
    DrawTextOnWindow(myWindow, bitmapClrStr1, Color.Black, 0, 30);
    RefreshWindow(myWindow);

    Delay(5000);

    CloseWindow(myWindow);
    FreeBitmap(userImage);
}

void TestClipping(Window w1)
{
    ClearWindow(w1, Color.WhiteSmoke);
    DrawText("White Smoke Bottom Right", Color.Black, 10, 280);
    SetClip(w1, RectangleFrom(0, 0, 250, 250));
    FillRectangle(StringToColor("#ff0000ff"), 0, 0, 300, 300, OptionDrawTo(w1));
    DrawText("Testing Clipping", Color.Black, 10, 10);
    DrawText("Red Top Left", Color.Black, 10, 20);
    
    PushClip(RectangleFrom(50, 50, 250, 250));
    FillRectangle(StringToColor("#00ff00ff"), 0, 0, 300, 300, OptionDrawTo(w1));
    
    PushClip(RectangleFrom(145, 0, 10, 300));
    FillRectangle(Color.Gold, 0, 0, 300, 300, OptionDrawTo(w1));
    
    PopClip();
    PushClip(RectangleFrom(0, 145, 300, 10));
    FillRectangle(Color.Gold, 0, 0, 300, 300, OptionDrawTo(w1));
    
    ResetClip();
    
    RefreshScreen();
    Delay(3000);
    
    Bitmap bmp = CreateBitmap("bmp", 100, 100);
    SetClip(bmp, RectangleFrom(0, 0, 75, 75));
    FillRectangle(Color.Red, 0, 0, 100, 100, OptionDrawTo(bmp));
    PushClip(bmp, RectangleFrom(0, 0, 50, 50));
    FillRectangle(Color.Green, 0, 0, 100, 100, OptionDrawTo(bmp));
    
    PushClip(bmp, RectangleFrom(20, 0, 10, 100));
    FillRectangle(Color.Gold, 0, 0, 100, 100, OptionDrawTo(bmp));
    PopClip(bmp);
    
    PushClip(bmp, RectangleFrom(0, 20, 100, 10));
    FillRectangle(Color.Gold, 0, 0, 100, 100, OptionDrawTo(bmp));
    PopClip(bmp);
    
    PushClip(bmp, RectangleFrom(0, 0, 25, 25));
    FillRectangle(Color.Blue, 0, 0, 100, 100, OptionDrawTo(bmp));
    ResetClip(bmp);
    
    ClearWindow(w1, Color.Silver);
    DrawText("B > G > R from top left of bmp", Color.Black, 10, 10);
    DrawBitmap(bmp, 100, 100);
    SaveBitmap(bmp, "bmp");
    RefreshScreen();
    Delay(3000);
}

SetResourcesPath(GlobalSettings.ResourcePath);

Console.WriteLine("Checking the number of displays and their details");
Console.WriteLine($"There are {NumberOfDisplays()} displays");
Console.WriteLine("------------------------");

for (int i = 0; i < NumberOfDisplays(); i++)
{
    Display d = DisplayDetails((uint)i);
    Console.WriteLine($"Display {i}");
    Console.WriteLine($"  name: {DisplayName(d)}");
    Console.WriteLine($"     @: {DisplayX(d)}, {DisplayY(d)}");
    Console.WriteLine($"     s: {DisplayWidth(d)}x{DisplayHeight(d)}");
    Console.WriteLine("------------------------");
}

TestDrawingOnNewWindow();

Window w1 = OpenWindow("Testing Graphics", 300, 300);

TestClipping(w1);

// Open a second window next to the first for testing on_window functions
Point2D w1Pos = WindowPosition(w1);
Window w2 = OpenWindow("Testing Second Window", 300, 300);
MoveWindowTo(w2, (int)(w1Pos.X + 350), (int)w1Pos.Y);

Color inClr = StringToColor("#ffeebbaa");

Color clr, clr2;

while ( !(WindowCloseRequested(w1) || WindowCloseRequested(w2)) )
{
    ProcessEvents();
    
    // First window
    for (int x = 0; x < WindowWidth(w1); x++)
    {
        for (int y = 0; y < WindowHeight(w1); y++)
        {
            clr = HSBColor(x / (WindowWidth(w1) * 1.0f), y / (WindowHeight(w1) * 1.0f), y / (WindowHeight(w1) * 1.0f));
            DrawPixel(clr, x, y);
        }
    }
    
    FillRectangle(inClr, 100, 100, 100, 100);
    
    clr = GetPixel(MousePosition());
    
    string clrString   = "Color " + ColorToString(clr);
    string rString     = "Red   " + RedOf(clr).ToString();
    string gString     = "Green " + GreenOf(clr).ToString();
    string bString     = "Blue  " + BlueOf(clr).ToString();
    string aString     = "Alpha " + AlphaOf(clr).ToString();
    string hString     = "Hue   " + HueOf(clr).ToString();
    string sString     = "Sat   " + SaturationOf(clr).ToString();
    string briString   = "Bri   " + BrightnessOf(clr).ToString();

    DrawText(clrString, Color.Black, 20, 180);
    DrawText(rString, Color.Black, 20, 190);
    DrawText(gString, Color.Black, 20, 200);
    DrawText(bString, Color.Black, 20, 210);
    DrawText(aString, Color.Black, 20, 220);
    DrawText(hString, Color.Black, 20, 240);
    DrawText(sString, Color.Black, 20, 250);
    DrawText(briString, Color.Black, 20, 260);
    
    //  Second window
    for (int x = 0; x < WindowWidth(w2); x++)
    {
        for (int y = 0; y < WindowHeight(w2); y++)
        {
            clr2 = HSBColor(x / (WindowWidth(w2) * 1.0f), y / (WindowHeight(w2) * 1.0f), y / (WindowHeight(w2) * 1.0f));
            DrawPixelOnWindow(w2, clr2, x, y);
        }
    }
    
    FillRectangleOnWindow(w2, Color.Magenta, 100, 100, 100, 100);
    
    clr2 = GetPixel(w2, MousePosition());
    
    string clrString2   = "Color " + ColorToString(clr2);
    string rString2     = "Red   " + RedOf(clr2).ToString();
    string gString2     = "Green " + GreenOf(clr2).ToString();
    string bString2     = "Blue  " + BlueOf(clr2).ToString();
    string aString2     = "Alpha " + AlphaOf(clr2).ToString();
    string hString2     = "Hue   " + HueOf(clr2).ToString();
    string sString2     = "Sat   " + SaturationOf(clr2).ToString();
    string briString2   = "Bri   " + BrightnessOf(clr2).ToString();
    
    DrawTextOnWindow(w2, clrString2, Color.Black, 20, 180);
    DrawTextOnWindow(w2, rString2, Color.Black, 20, 190);
    DrawTextOnWindow(w2, gString2, Color.Black, 20, 200);
    DrawTextOnWindow(w2, bString2, Color.Black, 20, 210);
    DrawTextOnWindow(w2, aString2, Color.Black, 20, 220);
    DrawTextOnWindow(w2, hString2, Color.Black, 20, 240);
    DrawTextOnWindow(w2, sString2, Color.Black, 20, 250);
    DrawTextOnWindow(w2, briString2, Color.Black, 20, 260);

    RefreshScreen();
}

CloseAllWindows();
