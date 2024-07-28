#include "Chat.h"

ostream& operator<<(ostream& output, const Chat& chat)
{
	output << chat._messages;
	return output;
}

void Chat::SetMessages(string message)
{
	if (0 < message.size())
		_messages += message + "\n";
}

void Chat::addMessage(string message, string username)
{
	if (message.size() > 0)
		_messages += username + ": " + message + "\n";
}

void Chat::clear()
{
	_messages = "";
}