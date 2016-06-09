#include "EventDescriptionBoard.h"
#include "Event.h"

namespace Profiler
{
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static CriticalSection lock;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
EventDescriptionBoard& EventDescriptionBoard::Get()
{ 
	return instance;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void EventDescriptionBoard::SetSamplingFlag( int index, bool flag )
{ 
	CRITICAL_SECTION(lock)
	BRO_VERIFY(index < (int)board.size(), "Invalid EventDescription index", return);

	if (index < 0)
	{
		for (EventDescription* desc : board)
		{
			desc->isSampling = flag;
		}
	} else
	{
		board[index]->isSampling = flag;
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool EventDescriptionBoard::HasSamplingEvents() const
{
	CRITICAL_SECTION(lock)
	for (const EventDescription* desc : board)
	{
		if (desc->isSampling)
		{
			return true;
		}
	}

	return false;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const std::vector<EventDescription*>& EventDescriptionBoard::GetEvents() const
{
	return board;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
EventDescriptionBoard::~EventDescriptionBoard()
{
	for (EventDescription* desc : board)
	{
		delete desc;
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
EventDescription* EventDescriptionBoard::CreateDescription()
{
	CRITICAL_SECTION(lock)
	EventDescription* desc = new EventDescription();
	desc->index = (unsigned long)board.size();
	board.push_back(desc);
	return desc;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
OutputDataStream& operator << ( OutputDataStream& stream, const EventDescriptionBoard& ob)
{
	CRITICAL_SECTION(lock)
	const std::vector<EventDescription*>& events = ob.GetEvents();

	stream << (uint32)events.size();

	for ( const EventDescription *desc : events )
	{
		stream << *desc;
	}

	return stream;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Profiler::EventDescriptionBoard EventDescriptionBoard::instance;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}