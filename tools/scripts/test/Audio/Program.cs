using SplashKitSDK;
using static SplashKitSDK.SplashKit;

SetResourcesPath(GlobalSettings.ResourcePath);

Console.WriteLine("Starting Audio Tests...");

Console.WriteLine($"    Audio should be ready? {AudioReady()}");

Console.WriteLine("    Closing audio...");
CloseAudio();

Console.WriteLine($"    Audio should not be ready? {AudioReady()}");

Delay(500);

Console.WriteLine("    Opening audio...");
OpenAudio();

Console.WriteLine($"    Is audio ready? {AudioReady()}");

Console.WriteLine($"    Have not loaded a sound yet. Is a sound effect loaded? {HasSoundEffect("test")}");

SoundEffect s1 = LoadSoundEffect("test", "test.ogg");

Console.WriteLine($"    Loaded sound effect. Is there a sound effect loaded? {HasSoundEffect("test")}");
Console.WriteLine("    Playing loaded sound effect");
PlaySoundEffect(s1, 1, 1.0f);

Delay(2000);

Console.WriteLine("    Downloading sound effect...");
DownloadSoundEffect("text message 2", "http://soundbible.com/grab.php?id=2155&type=wav", 80);
Console.WriteLine("    Playing downloaded sound effect twice");
PlaySoundEffect("text message 2", 2, 0.8);

Delay(5000);

Console.WriteLine("    Downloading music...");
DownloadMusic("music", "http://cdn.pixabay.com/download/audio/2025/10/29/audio_dcca153626.mp3?filename=classical-royal-british-music-427631.mp3", 80);
Console.WriteLine("    Playing downloaded music for 5 seconds");
PlayMusic("music");

Delay(5000);
StopMusic();

Console.WriteLine("    Playing named sound effect once at full volume");
PlaySoundEffect(SoundEffectNamed("test"), 1, 1.0f);

Delay(2000);

Console.WriteLine("    Playing sound effect twice");
PlaySoundEffect(SoundEffectNamed("test"), 2);

Delay(5000);

Console.WriteLine("    Playing sound effect");
PlaySoundEffect(SoundEffectNamed("test"), 1);
Delay(1000);
Console.WriteLine("    Stopping sound effect");
StopSoundEffect(SoundEffectNamed("test1"));

SoundEffect s2 = LoadSoundEffect("dancing", "dancingFrog.wav");
Console.WriteLine("    Fading out sound effect over 2 seconds");
PlaySoundEffect(s2);
FadeSoundEffectOut(s2, 2000);
Delay(3000);
Console.WriteLine("    Fading out all sound effects over 5 seconds");
PlaySoundEffect(s2);
FadeAllSoundEffectsOut(5000);

Delay(6000);

FreeSoundEffect(s1);

LoadSoundEffect("test2", "error.wav");
LoadSoundEffect("test3", "30248__streety__sword7.flac");

FreeAllSoundEffects();

LoadMusic("dance", "dancingFrog.wav");

Console.WriteLine("    Playing music...");
PlayMusic(MusicNamed("dance"), 1, 1.0f);

Delay(5000);

Console.WriteLine("    Pausing music...");
PauseMusic();

Delay(2000);

Console.WriteLine("    Resuming music...");
ResumeMusic();

Delay(5000);

Console.WriteLine("    Stopping music...");
StopMusic();

Delay(2000);
Console.WriteLine("    Fading in music over 2 secs...");
FadeMusicIn("dance", 2000);
Delay(5000);
Console.WriteLine("    Fading out music over 5 secs...");
FadeMusicOut(5000);
Delay(5000);

Console.WriteLine("    Closing audio...");
CloseAudio();

Console.WriteLine($"    Is audio ready? {AudioReady()}");

Console.WriteLine("Audio Tests Ended");
Delay(5000);
