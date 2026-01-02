using SplashKitSDK;
using static SplashKitSDK.SplashKit;

SetResourcesPath(GlobalSettings.ResourcePath);

Window w1 = OpenWindow("Hello World", 800, 600);

DisplayDialog("Tests", "About to run window tests...", LoadFont("dialog", "hara.ttf"), 14);

Font fnt = LoadFont("hara", "hara.ttf");
Bitmap light = LoadBitmap("light", "on_med.png");

WindowSetIcon(w1, light);

Bitmap test = DownloadBitmap("blah", "http://www.swinburne.edu.au/cwis/php_pages/webapps/marketing/promotiles-v3/assets/img/RgakQ.jpg", 80);

Console.WriteLine("Expecting bad bitmap pointer error: ");
DrawBitmap("fred", 75, 100);

while (!QuitRequested())
{
    ProcessEvents();
    
    DrawBitmap(test, 0, 0);
    DrawBitmap(light, 10, 100);
    DrawBitmap("light", 75, 100);
    
    DrawCircle(Color.Red, 10, 10, 10);
    FillCircle(Color.Green, 50, 10, 10);
    
    DrawText("Hello World", Color.Blue, fnt, 64, 200, 100);
    
    RefreshScreen();
}

FreeBitmap(light);
CloseWindow(WindowNamed("Hello World"));

Delay(500);
