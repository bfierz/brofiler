#include <cstring>
#include "Event.h"
#include "Core.h"
#include "Thread.h"
#include "EventDescriptionBoard.h"

namespace Profiler
{
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static CriticalSection lock;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
EventDescription* EventDescription::Create(const char* eventName, const char* fileName, const unsigned long fileLine, const Color eventColor /*= Color::Null*/)
{
	CRITICAL_SECTION(lock)
	EventDescription* result = EventDescriptionBoard::Get().CreateDescription();
	result->name = eventName;
	result->file = fileName;
	result->line = fileLine;
	result->color = eventColor;
	return result;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
EventDescription* EventDescription::CreateGL(const char* eventName, const char* fileName, const unsigned long fileLine, const Color eventColor /*= Color::Null*/)
{
	CRITICAL_SECTION(lock)
	EventDescription* result = EventDescriptionBoard::Get().CreateDescription();
	result->name = eventName;
	result->file = fileName;
	result->line = fileLine;
	result->color = eventColor;
	return result;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
EventDescription::EventDescription()
{
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
EventData* Event::Start(const EventDescription& description)
{
	EventData* result = nullptr;

	if (EventStorage* storage = Core::storage)
	{
		result = &storage->NextEvent();
		result->description = &description;
		result->link = nullptr;
		result->Start();

		if (description.isSampling)
		{
			AtomicIncrement(&storage->isSampling);
		}
	}
	return result;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Event::Stop(EventData& data)
{
	data.Stop();

	if (data.description->isSampling)
	{
		if (EventStorage* storage = Core::storage)
			AtomicDecrement(&storage->isSampling);
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
GLEventData* GLEvent::Start(const EventDescription& description)
{
	BRO_ASSERT(wglGetCurrentContext(), "Need an attached OpenGL context");

	GLEventData* result = nullptr;

	if (EventStorage* storage = Core::storage)
	{
		// Load GL function points
		if (!glGenQueries)
		{
			glGenQueries    = (PFNGLGENQUERIESPROC)    wglGetProcAddress("glGenQueries");
			glDeleteQueries = (PFNGLDELETEQUERIESPROC) wglGetProcAddress("glDeleteQueries");
			glQueryCounter  = (PFNGLQUERYCOUNTERPROC)  wglGetProcAddress("glQueryCounter");
			glGetQueryObjectiv   = (PFNGLGETQUERYOBJECTIVPROC)   wglGetProcAddress("glGetQueryObjectiv");
			glGetQueryObjecti64v = (PFNGLGETQUERYOBJECTI64VPROC) wglGetProcAddress("glGetQueryObjecti64v");
		}
		if (!glGenQueries)
			return nullptr;

		auto hostEvent = &storage->NextEvent();
		hostEvent->description = &description;
		hostEvent->Start();

		result = &storage->NextGLEvent();
		result->description = &description;
		result->link = hostEvent;

		hostEvent->link = result;

		// Allocate GL queries
		//if (result->glStartTimeQuery == 0)
		{
			glGenQueries(1, &result->glStartTimeQuery);
			glGenQueries(1, &result->glStopTimeQuery);
		}

		// Place a start marker into the GPU command queue
		glQueryCounter(result->glStartTimeQuery, GL_TIMESTAMP);
		
		if (description.isSampling)
		{
			AtomicIncrement(&storage->isSampling);
		}
	}
	return result;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GLEvent::Stop(GLEventData& data)
{
	BRO_ASSERT(wglGetCurrentContext(), "Need an attached OpenGL context");

	if (data.link)
		data.link->Stop();

	// Place a stop marker into the GPU command queue
	if (glQueryCounter)
		glQueryCounter(data.glStopTimeQuery, GL_TIMESTAMP);

	if (data.description->isSampling)
	{
		if (EventStorage* storage = Core::storage)
			AtomicDecrement(&storage->isSampling);
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GLFrameEvent::CollectGLMarkers()
{
	BRO_ASSERT(wglGetCurrentContext(), "Need an attached OpenGL context")

	if (EventStorage* storage = Core::storage)
	{
		if (!glGetQueryObjectiv)
			return;

		storage->glEventBuffer.ForEach([&](GLEventData& data)
		{
			if (data.glStartTimeQuery == 0)
				return;

			// Check if the GPU has already catched up with the CPU
			GLint stopTimerAvailable;
			glGetQueryObjectiv(data.glStopTimeQuery, GL_QUERY_RESULT_AVAILABLE, &stopTimerAvailable);
			if (stopTimerAvailable == GL_TRUE)
			{
				// Store the context in the data
				data.glContext = wglGetCurrentContext();

				// Fetch available GPU timers
				glGetQueryObjecti64v(data.glStartTimeQuery, GL_QUERY_RESULT, &data.start);
				glGetQueryObjecti64v(data.glStopTimeQuery, GL_QUERY_RESULT, &data.finish);

				glDeleteQueries(1, &data.glStartTimeQuery);
				glDeleteQueries(1, &data.glStopTimeQuery);

				data.glStartTimeQuery = 0;
				data.glStopTimeQuery = 0;
			}
		});
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
OutputDataStream & operator<<(OutputDataStream &stream, const EventDescription &ob)
{
	uint8_t flags = (ob.isSampling ? 0x1 : 0);
	return stream << ob.name << ob.file << ob.line << (uint32_t) ob.color << flags;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
OutputDataStream& operator<<(OutputDataStream& stream, const EventTime& ob)
{
	return stream << ob.start << ob.finish;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
OutputDataStream& operator<<(OutputDataStream& stream, const EventData& ob)
{
	return stream << (EventTime)(ob) << ob.description->index;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
OutputDataStream& operator<<(OutputDataStream& stream, const GLEventData& ob)
{
	return stream << (EventData)(ob) << (uint64_t) ob.glContext << ob.glStartTimeQuery << ob.glStopTimeQuery;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Category::Register(EventData& data)
{
	if (EventStorage* storage = Core::storage)
	{
		storage->RegisterCategory(data);
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}
