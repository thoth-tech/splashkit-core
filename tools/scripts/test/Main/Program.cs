using System.Diagnostics;
using SplashKitSDK;

int opt = 0;

// Get the target framework for this program
string? targFramework = System.AppContext.TargetFrameworkName;  // Output example: ".NETCoreApp,Version=v9.0"
string? targFrameworkVersion = targFramework?.Remove(0, 21);    // Extracts version number (e.g. "9.0")

// Get the specific runtime framework (e.g. ".NET 9.0.11")
string runtimeFramework = System
    .Runtime
    .InteropServices
    .RuntimeInformation
    .FrameworkDescription;

// Get the SplashKit NuGet package version
string? nVersion = typeof(SplashKit)?
    .Assembly?
    .GetName()?
    .Version?
    .ToString();

// Get the project directory, to list all available projects
string solutionDir = Directory
    .GetParent(Directory.GetCurrentDirectory())?
    .ToString() ?? "";

do
{
    // Display test options
    Console.WriteLine("-----------------------------------------------------");
    Console.WriteLine($" NuGet Dev Tests, using SplashKit NuGet {nVersion}");
    Console.WriteLine($" Targeting .NET {targFrameworkVersion} // Runtime {runtimeFramework}");
    Console.WriteLine("-----------------------------------------------------");
    Console.WriteLine("  R: Reload tests");
    Console.WriteLine(" -1: Quit");

    // Search the solution directory for projects
    var projectPaths = Directory.EnumerateFiles(solutionDir, "*.csproj", SearchOption.AllDirectories);
    // Populate a list of all project names (except this one)
    List<string> projectNames = new();
    foreach (var project in projectPaths)
    {
        if (project.Contains("Main.csproj")) continue;
        projectNames.Add(Path.GetFileNameWithoutExtension(project));
    }
    projectNames.Sort();

    // Print project names
    for (int i = 0; i < projectNames.Count; i++)
        Console.WriteLine($"  {i}: {projectNames[i]}");

    Console.WriteLine("-----------------------------------------------------");
    Console.Write(" Select test to run: ");

    // Collect input
    string? input = Console.ReadLine();
    bool inputIsValid = int.TryParse(input, out opt) && opt >= 0 && opt < projectNames.Count;

    // Run selected program
    if (inputIsValid)
    {
        string selectedProject = projectNames[opt];
        var projectPath = Path.Combine(solutionDir, selectedProject, $"{selectedProject}.csproj");
        
        // Set process options
        var startInfo = new ProcessStartInfo
        {
            FileName = "dotnet",
            Arguments = $"run --project {projectPath} -f net{targFrameworkVersion}"
        };
        // Start process
        System.Console.WriteLine($"\nRunning {selectedProject}...\n\n");
        Process.Start(startInfo)?.WaitForExit();
        Console.WriteLine("\n");
    }
    
} while (opt != -1);
