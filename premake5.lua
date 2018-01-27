local isWindows = os.ishost("windows")

workspace "Module"
location "Generated_%{_ACTION}"
language "C++"
rtti "Off"
exceptionhandling "Off"

configurations { "Debug", "Release" }

if isWindows then
	buildoptions { "/Zc:threadSafeInit-" }

	platforms { "Win32", "x64" }

	filter "action:vs2015"
	toolset "v140_xp"
	filter {}

	filter "action:vs2017"
	toolset "v141_xp"
	filter {}

	defines { "WIN32", "_WINDOWS", "WIN32_LEAN_AND_MEAN", "NOMINMAX", "_USE_MATH_DEFINES=1", "_CRT_RAND_S=1" }

	filter { "kind:SharedLib" }
	defines { "_USRDLL" }
	flags { "NoManifest" }
	flags { "NoImportLib" }

	targetname("$(ProjectName)_$(PlatformTarget)")

	filter { "kind:SharedLib", "configurations:Debug" }
	targetname("$(ProjectName)_d_$(PlatformTarget)")

	filter { "kind:StaticLib" }
	defines { "_LIB" }
end

filter { "configurations:Debug" }
defines { "DEBUG=1", "_DEBUG=1", "SDL_ASSERT_LEVEL=3" }
symbols "On"

filter { "configurations:Release" }
defines { "NDEBUG" }
optimize "On"
if isWindows then
	flags { "NoIncrementalLink", "LinkTimeOptimization" }
	flags { "StaticRuntime" }
	linkoptions { "/LTCG" }
end

if isWindows then
	filter { "platforms:Win32" }
	system "Windows"
	architecture "x86"

	filter { "platforms:x64" }
	system "Windows"
	architecture "x64"
end

filter {}

targetdir ("Lib/%{cfg.architecture}/%{cfg.buildcfg}")
objdir ("!Generated_%{_ACTION}/obj/%{prj.name}/%{cfg.architecture}/%{cfg.buildcfg}")

if isWindows then
	filter "action:vs*"
	defines { "_CRT_SECURE_NO_DEPRECATE", "_CRT_SECURE_NO_WARNINGS", "_CRT_NONSTDC_NO_WARNINGS" }
	targetdir ("Bin")
	objdir ("!Generated_%{_ACTION}/obj/$(ProjectName)/%{cfg.architecture}/$(Configuration)")
	filter("action:vs2015 or vs2017")
	targetdir ("Bin")
	objdir ("!Generated_%{_ACTION}/obj/$(ProjectName)/$(PlatformTarget)/$(Configuration)")
end

filter {}

includedirs { "Third Party/SDL/include" }
includedirs { "Third Party/ModuleSDK" }

project("ModuleSDK")
	kind "StaticLib"

	vpaths {
	   ["Header Files"] = {"Third Party/ModuleSDK/*.h"},
	   ["Source Files"] = {"Third Party/ModuleSDK/*.cpp"}
	}

	files { "Third Party/ModuleSDK/**.h", "Third Party/ModuleSDK/**.cpp" }

project("Hello World")
	kind "SharedLib"

	vpaths {
	   ["Header Files"] = {"Hello World/*.h"},
	   ["Resource Files"] = {"Hello World/*.rc"},
	   ["Source Files"] = {"Hello World/*.c", "Hello World/*.cpp"}
	}

	pchheader("StdInc.h")
	pchsource("Hello World/StdInc.cpp")

	implibname "$(OutDir)$(TargetName)"

	files { "Hello World/**.h", "Hello World/**.c", "Hello World/**.cpp" }

	links { "ModuleSDK" }
