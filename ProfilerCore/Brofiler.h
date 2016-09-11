#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Compiler detection and adaptations
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if defined __clang__ || defined __GNUC__

//	Force inline
#	define BRO_INLINE __attribute__((always_inline))

#elif defined _MSC_VER || defined __INTEL_COMPILER

//	Force inline
#	define BRO_INLINE __forceinline

#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Pre-processor meta-programming
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define BRO_CONCAT_IMPL(x, y) x##y
#define BRO_CONCAT(x, y) BRO_CONCAT_IMPL(x, y)

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Common includes
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include <cstdint>

typedef unsigned int uint;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// OS specific parts
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef LINUX64

#define WINAPI
typedef void* PVOID;

typedef uint32_t DWORD;
#endif

#if !defined(USE_PROFILER) && (!defined(_FINALRELEASE) || defined(EMULATE_DEVELOPER_FINAL_RELEASE))
	#define USE_PROFILER 1
#endif

#if USE_PROFILER
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// EXPORTS 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef PROFILER_EXPORTS
#define BROFILER_API __declspec(dllexport)
#else
#define BROFILER_API //__declspec(dllimport)
#endif
#pragma region Colors.h
namespace Profiler
{
	// Source: http://msdn.microsoft.com/en-us/library/system.windows.media.colors(v=vs.110).aspx
	// Image:  http://i.msdn.microsoft.com/dynimg/IC24340.png
	enum class Color : uint32_t
	{
		Null = 0x00000000,
		AliceBlue = 0xFFF0F8FF,
		AntiqueWhite = 0xFFFAEBD7,
		Aqua = 0xFF00FFFF,
		Aquamarine = 0xFF7FFFD4,
		Azure = 0xFFF0FFFF,
		Beige = 0xFFF5F5DC,
		Bisque = 0xFFFFE4C4,
		Black = 0xFF000000,
		BlanchedAlmond = 0xFFFFEBCD,
		Blue = 0xFF0000FF,
		BlueViolet = 0xFF8A2BE2,
		Brown = 0xFFA52A2A,
		BurlyWood = 0xFFDEB887,
		CadetBlue = 0xFF5F9EA0,
		Chartreuse = 0xFF7FFF00,
		Chocolate = 0xFFD2691E,
		Coral = 0xFFFF7F50,
		CornflowerBlue = 0xFF6495ED,
		Cornsilk = 0xFFFFF8DC,
		Crimson = 0xFFDC143C,
		Cyan = 0xFF00FFFF,
		DarkBlue = 0xFF00008B,
		DarkCyan = 0xFF008B8B,
		DarkGoldenRod = 0xFFB8860B,
		DarkGray = 0xFFA9A9A9,
		DarkGreen = 0xFF006400,
		DarkKhaki = 0xFFBDB76B,
		DarkMagenta = 0xFF8B008B,
		DarkOliveGreen = 0xFF556B2F,
		DarkOrange = 0xFFFF8C00,
		DarkOrchid = 0xFF9932CC,
		DarkRed = 0xFF8B0000,
		DarkSalmon = 0xFFE9967A,
		DarkSeaGreen = 0xFF8FBC8F,
		DarkSlateBlue = 0xFF483D8B,
		DarkSlateGray = 0xFF2F4F4F,
		DarkTurquoise = 0xFF00CED1,
		DarkViolet = 0xFF9400D3,
		DeepPink = 0xFFFF1493,
		DeepSkyBlue = 0xFF00BFFF,
		DimGray = 0xFF696969,
		DodgerBlue = 0xFF1E90FF,
		FireBrick = 0xFFB22222,
		FloralWhite = 0xFFFFFAF0,
		ForestGreen = 0xFF228B22,
		Fuchsia = 0xFFFF00FF,
		Gainsboro = 0xFFDCDCDC,
		GhostWhite = 0xFFF8F8FF,
		Gold = 0xFFFFD700,
		GoldenRod = 0xFFDAA520,
		Gray = 0xFF808080,
		Green = 0xFF008000,
		GreenYellow = 0xFFADFF2F,
		HoneyDew = 0xFFF0FFF0,
		HotPink = 0xFFFF69B4,
		IndianRed = 0xFFCD5C5C,
		Indigo = 0xFF4B0082,
		Ivory = 0xFFFFFFF0,
		Khaki = 0xFFF0E68C,
		Lavender = 0xFFE6E6FA,
		LavenderBlush = 0xFFFFF0F5,
		LawnGreen = 0xFF7CFC00,
		LemonChiffon = 0xFFFFFACD,
		LightBlue = 0xFFADD8E6,
		LightCoral = 0xFFF08080,
		LightCyan = 0xFFE0FFFF,
		LightGoldenRodYellow = 0xFFFAFAD2,
		LightGray = 0xFFD3D3D3,
		LightGreen = 0xFF90EE90,
		LightPink = 0xFFFFB6C1,
		LightSalmon = 0xFFFFA07A,
		LightSeaGreen = 0xFF20B2AA,
		LightSkyBlue = 0xFF87CEFA,
		LightSlateGray = 0xFF778899,
		LightSteelBlue = 0xFFB0C4DE,
		LightYellow = 0xFFFFFFE0,
		Lime = 0xFF00FF00,
		LimeGreen = 0xFF32CD32,
		Linen = 0xFFFAF0E6,
		Magenta = 0xFFFF00FF,
		Maroon = 0xFF800000,
		MediumAquaMarine = 0xFF66CDAA,
		MediumBlue = 0xFF0000CD,
		MediumOrchid = 0xFFBA55D3,
		MediumPurple = 0xFF9370DB,
		MediumSeaGreen = 0xFF3CB371,
		MediumSlateBlue = 0xFF7B68EE,
		MediumSpringGreen = 0xFF00FA9A,
		MediumTurquoise = 0xFF48D1CC,
		MediumVioletRed = 0xFFC71585,
		MidnightBlue = 0xFF191970,
		MintCream = 0xFFF5FFFA,
		MistyRose = 0xFFFFE4E1,
		Moccasin = 0xFFFFE4B5,
		NavajoWhite = 0xFFFFDEAD,
		Navy = 0xFF000080,
		OldLace = 0xFFFDF5E6,
		Olive = 0xFF808000,
		OliveDrab = 0xFF6B8E23,
		Orange = 0xFFFFA500,
		OrangeRed = 0xFFFF4500,
		Orchid = 0xFFDA70D6,
		PaleGoldenRod = 0xFFEEE8AA,
		PaleGreen = 0xFF98FB98,
		PaleTurquoise = 0xFFAFEEEE,
		PaleVioletRed = 0xFFDB7093,
		PapayaWhip = 0xFFFFEFD5,
		PeachPuff = 0xFFFFDAB9,
		Peru = 0xFFCD853F,
		Pink = 0xFFFFC0CB,
		Plum = 0xFFDDA0DD,
		PowderBlue = 0xFFB0E0E6,
		Purple = 0xFF800080,
		Red = 0xFFFF0000,
		RosyBrown = 0xFFBC8F8F,
		RoyalBlue = 0xFF4169E1,
		SaddleBrown = 0xFF8B4513,
		Salmon = 0xFFFA8072,
		SandyBrown = 0xFFF4A460,
		SeaGreen = 0xFF2E8B57,
		SeaShell = 0xFFFFF5EE,
		Sienna = 0xFFA0522D,
		Silver = 0xFFC0C0C0,
		SkyBlue = 0xFF87CEEB,
		SlateBlue = 0xFF6A5ACD,
		SlateGray = 0xFF708090,
		Snow = 0xFFFFFAFA,
		SpringGreen = 0xFF00FF7F,
		SteelBlue = 0xFF4682B4,
		Tan = 0xFFD2B48C,
		Teal = 0xFF008080,
		Thistle = 0xFFD8BFD8,
		Tomato = 0xFFFF6347,
		Turquoise = 0xFF40E0D0,
		Violet = 0xFFEE82EE,
		Wheat = 0xFFF5DEB3,
		White = 0xFFFFFFFF,
		WhiteSmoke = 0xFFF5F5F5,
		Yellow = 0xFFFFFF00,
		YellowGreen = 0xFF9ACD32,
	};
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}
#pragma endregion

#pragma region Event.h
namespace Profiler
{
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct BROFILER_API ThreadDescription
{
	uint32_t threadID;
	const char* name;

	ThreadDescription(const char* threadName = "MainThread");
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern "C" BROFILER_API int64_t GetTime();
BROFILER_API int64_t GetTimeMicroSeconds();
BROFILER_API void NextFrame();
BROFILER_API bool IsActive();
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct EventDescription;
struct Frame;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct EventTime
{
	int64_t start;
	int64_t finish;

	BRO_INLINE void Start() { start  = GetTime(); }
	BRO_INLINE void Stop() 	{ finish = GetTime(); }
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct EventData : public EventTime
{
	const EventDescription* description;
	EventData* link;
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct GLEventData : public EventData
{
	uint32_t glStartTimeQuery;
	uint32_t glStopTimeQuery;
	void*    glContext;
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct BROFILER_API EventDescription
{
	bool isSampling{ false };

	// HOT  \\
	// Have to place "hot" variables at the beginning of the class (here will be some padding)
	// COLD //

	const char* name{ "" };
	const char* file{ "" };
	uint32_t line{ 0 };
	uint32_t index{ 0 };
	Color color{ Color::Null };

	static EventDescription* Create(const char* eventName, const char* fileName, const unsigned long fileLine, const Color eventColor = Color::Null);
	static EventDescription* CreateGL(const char* eventName, const char* fileName, const unsigned long fileLine, const Color eventColor = Color::Null);

private:
	friend class EventDescriptionBoard;

	EventDescription();
	~EventDescription() = default;
	EventDescription(const EventDescription&) = delete;
	EventDescription& operator=(const EventDescription&) = delete;
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct BROFILER_API Event
{
	/// Data associated with the event
	EventData* data;

	static EventData* Start(const EventDescription& description);
	static void Stop(EventData& data);

	Event(const EventDescription& description)
	{
		data = Start(description);
	}

	~Event()
	{
		if (data)
		{
			Stop(*data);
		}
	}
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct BROFILER_API GLEvent
{
	/// Data associated with the event
	GLEventData* data;

	static GLEventData* Start(const EventDescription& description);
	static void Stop(GLEventData& data);

	GLEvent(const EventDescription& description)
	{
		data = Start(description);
	}

	~GLEvent()
	{
		if (data)
		{
			Stop(*data);
		}
	}
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct BROFILER_API GLFrameEvent : public GLEvent
{
	GLFrameEvent(const EventDescription& description)
	: GLEvent(description)
	{
	}

	~GLFrameEvent()
	{
		CollectGLMarkers();
	}

	/// Collect all the GL markers that are already processed
	static void CollectGLMarkers();
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct BROFILER_API Category : public Event
{
	Category(const EventDescription& description)
	: Event(description)
	{
		if (data)
		{
			Register(*data);
		}
	}

	/// Register some event data as a category
	/// @param data event data to register
	static void Register(EventData& data);
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}
#pragma endregion

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Add a profile range 
#define BROFILER_EVENT(NAME) \
	static Profiler::EventDescription* BRO_CONCAT(autogenerated_description_, __LINE__) = Profiler::EventDescription::Create( NAME, __FILE__, __LINE__ ); \
	Profiler::Event                    BRO_CONCAT(autogenerated_event_, __LINE__)( *(BRO_CONCAT(autogenerated_description_, __LINE__)) );

/// Add a profile range 
#define BROFILER_EVENT_GL(NAME) \
	static Profiler::EventDescription* BRO_CONCAT(autogenerated_description_, __LINE__) = Profiler::EventDescription::CreateGL( NAME, __FILE__, __LINE__ ); \
	Profiler::GLEvent                  BRO_CONCAT(autogenerated_event_, __LINE__)( *(BRO_CONCAT(autogenerated_description_, __LINE__)) );

#if defined __clang__ || defined __GNUC__
#	define PROFILE BROFILER_EVENT(__PRETTY_FUNCTION__)
#elif defined _MSC_VER || defined __INTEL_COMPILER
#	define PROFILE BROFILER_EVENT(__FUNCSIG__)
#endif

/// Profile a single piece of code
#define BROFILER_INLINE_EVENT(NAME, CODE) { BROFILER_EVENT(NAME) CODE; }

/// Add a profile range with a color
#define BROFILER_CATEGORY(NAME, COLOR) \
	static Profiler::EventDescription* BRO_CONCAT(autogenerated_description_, __LINE__) = Profiler::EventDescription::Create( NAME, __FILE__, __LINE__, COLOR ); \
	Profiler::Category                 BRO_CONCAT(autogenerated_event_, __LINE__)( *(BRO_CONCAT(autogenerated_description_, __LINE__)) );

/// Profile a permanent, system-thread
#define BROFILER_FRAME(FRAME_NAME) \
	static Profiler::ThreadDescription currentFrameDescription(FRAME_NAME); \
	Profiler::NextFrame(); \
	BROFILER_EVENT("Frame")

/// Profile a permanent rendering thread
#define BROFILER_FRAME_GL(FRAME_NAME) \
	static Profiler::ThreadDescription currentFrameDescription(FRAME_NAME); \
	Profiler::NextFrame(); \
	static Profiler::EventDescription* BRO_CONCAT(autogenerated_description_, __LINE__) = Profiler::EventDescription::CreateGL( "Frame", __FILE__, __LINE__ ); \
	Profiler::GLFrameEvent             BRO_CONCAT(autogenerated_event_, __LINE__)( *(BRO_CONCAT(autogenerated_description_, __LINE__)) );

/// Profile a worker thread
#define BROFILER_THREAD(FRAME_NAME) Profiler::ThreadDescription currentFrameDescription(FRAME_NAME);

#else
#define BROFILER_EVENT(NAME)
#define PROFILE
#define BROFILER_INLINE_EVENT(NAME, CODE) { CODE; }
#define BROFILER_CATEGORY(NAME, COLOR)
#define BROFILER_FRAME(NAME)
#define BROFILER_THREAD(FRAME_NAME)
#endif
