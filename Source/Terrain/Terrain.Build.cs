// Fill out your copyright notice in the Description page of Project Settings.
using System.IO;
using UnrealBuildTool;

public class Terrain : ModuleRules
{
    private string ModulePath
    {
        get { return ModuleDirectory; }
    }


    private string ThirdPartyPath
    {
        get { return Path.GetFullPath(Path.Combine(ModulePath, "../../ThirdParty/")); }
    }

    public Terrain(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "ProceduralMeshComponent" });

		PrivateDependencyModuleNames.AddRange(new string[] {  });

        LoadDelaunayTriangulation(Target);
        LoadDelaunayTriangulation3D(Target);
        // Uncomment if you are using Slate UI
        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
    }
    //public Terrain(ReadOnlyTargetRules Target) : base(Target)
    //{
        
    //}

    public bool LoadDelaunayTriangulation(ReadOnlyTargetRules Target)
    {
        bool isLibrarySupported = false;

        if ((Target.Platform == UnrealTargetPlatform.Win64) || (Target.Platform == UnrealTargetPlatform.Win32))
        {
            isLibrarySupported = true;

            string PlatformString = (Target.Platform == UnrealTargetPlatform.Win64) ? "x64" : "x86";
            string LibrariesPath = Path.Combine(ThirdPartyPath, "DelaunayTriangulation", "Libraries");

            /*
            test your path with:
            using System; // Console.WriteLine("");
            Console.WriteLine("... LibrariesPath -> " + LibrariesPath);
            */

            PublicAdditionalLibraries.Add(Path.Combine(LibrariesPath, "DelabellaLib" + ".lib"));
        }

        if (isLibrarySupported)
        {
            // Include path
            PublicIncludePaths.Add(Path.Combine(ThirdPartyPath, "DelaunayTriangulation", "Includes"));
        }

        Definitions.Add(string.Format("WITH_DELAUNAY_TRIANGULATION_BINDING={0}", isLibrarySupported ? 1 : 0));

        return isLibrarySupported;
    }


    public bool LoadDelaunayTriangulation3D(ReadOnlyTargetRules Target)
    {
        bool isLibrarySupported = false;

        if ((Target.Platform == UnrealTargetPlatform.Win64) || (Target.Platform == UnrealTargetPlatform.Win32))
        {
            isLibrarySupported = true;

            string PlatformString = (Target.Platform == UnrealTargetPlatform.Win64) ? "x64" : "x86";
            string LibrariesPath = Path.Combine(ThirdPartyPath, "DelaunayTriangulation3D", "Libraries");

            PublicAdditionalLibraries.Add(Path.Combine(LibrariesPath, "SimpleDelaunay3D" + ".lib"));
        }

        if (isLibrarySupported)
        {
            // Include path
            PublicIncludePaths.Add(Path.Combine(ThirdPartyPath, "DelaunayTriangulation3D", "Includes"));
        }

        Definitions.Add(string.Format("WITH_DELAUNAY_TRIANGULATION_BINDING={0}", isLibrarySupported ? 1 : 0));

        return isLibrarySupported;
    }


}
