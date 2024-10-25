// Copyright Epic Games, Inc. All Rights Reserved.

using System.IO;
using System.Reflection.Emit;
using System.Xml.Serialization;
using Internal;
using UnrealBuildTool;

public class UltraLowLatencyAudio : ModuleRules
{
	private string ModulePath => ModuleDirectory;

	private string ThirdPartyPath => Path.GetFullPath(Path.Combine(ModulePath, "../../ThirdParty/"));

	public UltraLowLatencyAudio(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		PrivatePCHHeaderFile = Path.Combine(ModulePath, "Private/JUCEPCH.h");
		
		bUseRTTI = true;
		bEnableExceptions = true;
		
		PublicIncludePaths.AddRange(
			new string[]
			{
				Path.Combine(ModulePath, "Public"),
			}
			);
		
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
			}
			);
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
			}
			);
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
			}
			);
		
		LoadJUCE(Target);
	}

	public void LoadJUCE(ReadOnlyTargetRules Target)
	{
		var isLibrarySupported = false;
		
		if (Target.Platform == UnrealTargetPlatform.Win64)
		{
			isLibrarySupported = true;
			
			var LibrariesPath = Path.Combine(ThirdPartyPath, "JUCE", "Libraries");
			var BuildType = "Release";
			
			Console.WriteLine("JUCE Libraries Path: " + LibrariesPath);

			// Link the JUCE static library
			PublicAdditionalLibraries.Add(Path.Combine(LibrariesPath, BuildType, "JUCEStaticLib.lib"));
			
			PublicIncludePaths.Add(Path.Combine(ThirdPartyPath, "JUCE", "Includes/modules"));
			
			PublicDefinitions.AddRange(new string[]
			{
				"JUCE_WINDOWS=1",
				"JUCE_MAC=0",
				"JUCE_IOS=0",
				"JUCE_ANDROID=0",
				"JUCE_LINUX=0",
				"JUCE_USE_JUCE_AUDIO_DEVICES=1",
				"JUCE_USE_JUCE_AUDIO_PROCESSORS=1"
			});
			
			//PublicDefinitions.Add("JUCE_BSD=0");
			//PublicDefinitions.Add("JUCE_BIG_ENDIAN=0");
			//PublicDefinitions.Add("JUCE_EVENTS_INCLUDE_WINRT_WRAPPER=0");
			//
			//PublicDefinitions.Add("JUCE_DEBUG=0");
			//PublicDefinitions.Add("JUCE_DISABLE_ASSERTIONS=0"); 
			//PublicDefinitions.Add("JUCE_PUSH_NOTIFICATIONS=0");
			//PublicDefinitions.Add("JUCE_DOXYGEN=0");
			//PublicDefinitions.Add("JUCE_ENABLE_LIVE_CONSTANT_EDITOR=0");
			//PublicDefinitions.Add("JUCE_OPENGL_ES=0");
			//
			//PublicDefinitions.Add("JucePlugin_Build_Standalone=0");
			//PublicDefinitions.Add("JUCE_EVENTS_INCLUDE_WIN32_MESSAGE_WINDOW=0");
			//PublicDefinitions.Add("JUCE_GRAPHICS_INCLUDE_COREGRAPHICS_HELPERS=0");
			//PublicDefinitions.Add("JUCE_GUI_BASICS_INCLUDE_SCOPED_THREAD_DPI_AWARENESS_SETTER=0");
			//PublicDefinitions.Add("__ARM_NEON__=0");
			//PublicDefinitions.Add("TARGET_IPHONE_SIMULATOR=0");
			//PublicDefinitions.Add("TARGET_OS_IPHONE=0");
			//PublicDefinitions.Add("JUCE_GCC=0");
			//PublicDefinitions.Add("JUCE_UNIT_TESTS=0");
			//PublicDefinitions.Add("JUCE_MODULE_AVAILABLE_juce_graphics=0");
			//PublicDefinitions.Add("JucePlugin_Enable_ARA=0");

			bEnableUndefinedIdentifierWarnings = false;
		}

		PublicDefinitions.Add($"WITH_JUCE_BINDING={(isLibrarySupported ? 1 : 0)}");
	}
}
