public static class GlobalSettings
{
    // This path references resources used by the SplashKit Core integration test suite.
    // By referencing the below field, NuGet test projects are able to share assets with the core tests.
    // To utilise, put "SetResourcesPath(GlobalSettings.ResourcePath);" at the beginning of a test project
    public static readonly string ResourcePath = "../../../../coresdk/src/test/Resources";
}
