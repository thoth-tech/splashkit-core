using SplashKitSDK;
using static SplashKitSDK.SplashKit;

string keyTyped = "", keyDown = "", keyUp = "";

void OnKeyTyped(int code)
{
    keyTyped = KeyName((KeyCode)code);
}

void OnKeyDown(int code)
{
    keyDown = KeyName((KeyCode)code);
}

void OnKeyUp(int code)
{
    keyUp = KeyName((KeyCode)code);
}

SetResourcesPath(GlobalSettings.ResourcePath);

RegisterCallbackOnKeyTyped(OnKeyTyped);
RegisterCallbackOnKeyDown(OnKeyDown);
RegisterCallbackOnKeyUp(OnKeyUp);

Rectangle rect = new()
{
    X = 230.0f,
    Y = 50.0f,
    Width = 200.0f,
    Height = 30.0f
};

Window w1 = OpenWindow("Test Input", 600, 600);
Window w2 = OpenWindow("Test Input Window 2", 600, 600);

LoadFont("hara", "hara.ttf");
LoadFont("kochi", "kochi-gothic-subst");

StartReadingText(rect);

SetCurrentWindow(w2);

StartReadingText(rect, "スプラッシュ・キット");

Color back = Color.Wheat;

while (ReadingText(w1) || ReadingText(w2))
{
    ProcessEvents();

    if (QuitRequested() || WindowCloseRequested(w1) || WindowCloseRequested(w2)) break;
    
    SetCurrentWindow(w1);
    
    if (!TextEntryCancelled(w1))
        ClearScreen(Color.White);
    else
        ClearScreen(Color.Peru);
    
    DrawText("Enter english string: ", Color.Navy, "hara", 18, 30, 50);
    DrawCollectedText(Color.Black, FontNamed("hara"), 18, OptionDefaults());
    
    if (!ReadingText(w1))
    {
        DrawText("Read: ", Color.Blue, "hara", 18, 30, 80);
        DrawText(TextInput(w1), Color.Blue, "hara", 18, 30, 110);
    }
    
    string location = "Mouse location: ";
    location += MouseX().ToString() + ":" + MouseY().ToString();
    
    string leftClicked = "Left click status: ";
    leftClicked += MouseClicked(MouseButton.LeftButton).ToString() + " up: " + MouseUp(MouseButton.LeftButton).ToString() + " down: " + MouseDown(MouseButton.LeftButton).ToString();
    
    string rightClicked = "Right click status: ";
    rightClicked += MouseClicked(MouseButton.RightButton).ToString() + " up: " + MouseUp(MouseButton.RightButton).ToString() + " down: " + MouseDown(MouseButton.RightButton).ToString();
    
    string anyKeyInput = "Any keys pressed: ";
    if(AnyKeyPressed()) anyKeyInput += "yes";
    else anyKeyInput += "no";

    string keyDetails = "T key is ";
    if (KeyDown(KeyCode.TKey)) keyDetails += "down";
    if (KeyUp(KeyCode.TKey)) keyDetails += "up";
    if (KeyReleased(KeyCode.TKey)) keyDetails += " - released";
    if (KeyTyped(KeyCode.TKey)) keyDetails += " - typed";
    if (KeyTyped(KeyCode.FKey)) WindowToggleFullscreen(WindowWithFocus());
    if (KeyTyped(KeyCode.BKey)) WindowToggleBorder(WindowWithFocus());

    DrawText(location, Color.Plum, "hara", 14, 18, 200);
    DrawText(leftClicked, Color.Plum, "hara", 14, 18, 220);
    DrawText(rightClicked, Color.Plum, "hara", 14, 18, 240);
    DrawText(anyKeyInput, Color.Plum, "hara", 14, 18, 250);
    DrawText(keyDetails, Color.Plum, "hara", 14, 18, 280);
    DrawText(keyDown, Color.Plum, "hara", 14, 18, 300);
    DrawText(keyUp, Color.Plum, "hara", 14, 18, 320);
    DrawText(keyTyped, Color.Plum, "hara", 14, 18, 340);
    
    SetCurrentWindow(w2);
    
    if (KeyTyped(KeyCode.CKey))
    {
        back = RandomRGBColor(255);
    }
    
    ClearScreen(back);
    DrawText("Enter Japanese string: ", Color.Navy, "hara", 18, 30, 50);
    DrawCollectedText(Color.Black, FontNamed("kochi"), 18, OptionDefaults());
    
    if (!ReadingText(w2))
    {
        DrawText("Read: ", Color.Blue, "hara", 18, 30, 80);
        DrawText(TextInput(w2), Color.Blue, "kochi", 18, 30, 110);
    }
    
    // Get location of mouse in W2
    location = "Mouse location: ";
    location += MouseX().ToString() + ":" + MouseY().ToString();
    
    DrawText(location, Color.Plum, "hara", 14, 18, 200);
    DrawText(leftClicked, Color.Plum, "hara", 14, 18, 220);
    DrawText(rightClicked, Color.Plum, "hara", 14, 18, 240);
    DrawText(anyKeyInput, Color.Plum, "hara", 14, 18, 250);
    DrawText(keyDetails, Color.Plum, "hara", 14, 18, 280);
    DrawText(keyDown, Color.Plum, "hara", 14, 18, 300);
    DrawText(keyUp, Color.Plum, "hara", 14, 18, 320);
    DrawText(keyTyped, Color.Plum, "hara", 14, 18, 340);
    
    RefreshScreen();
}

CloseWindow("Test Input");
CloseWindow("Test Input Window 2");

DeregisterCallbackOnKeyTyped(OnKeyTyped);
DeregisterCallbackOnKeyDown(OnKeyDown);
DeregisterCallbackOnKeyUp(OnKeyUp);

FreeAllFonts();
