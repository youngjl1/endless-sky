/* AI.h
Copyright (c) 2014 by Michael Zahniser

Endless Sky is free software: you can redistribute it and/or modify it under the
terms of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later version.

Endless Sky is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE.  See the GNU General Public License for more details.
*/

#include "Messages.h"

#include <mutex>

using namespace std;

namespace {
	mutex incomingMutex;
	
	vector<string> incoming;
	vector<Messages::Entry> list;
}



// Add a message to the list.
void Messages::Add(const std::string &message)
{
	lock_guard<mutex> lock(incomingMutex);
	incoming.emplace_back(message);
}



// Get the messages for the given game step. Any messages that are too old
// will be culled out, and new ones that have just been added will have
// their "step" set to the given value.
const vector<Messages::Entry> &Messages::Get(int step)
{
	lock_guard<mutex> lock(incomingMutex);
	
	// Load the incoming messages.
	for(const string &message : incoming)
	{
		// For each incoming message, if it exactly matches an existing message,
		// replace that one with this new one.
		auto it = list.begin();
		while(it != list.end())
		{
			// Also erase messages that have reached the end of their lifetime.
			if(it->message == message || it->step < step - 1000)
				it = list.erase(it);
			else
				++it;
		}
		list.emplace_back(step, message);
	}
	incoming.clear();
	return list;
}
