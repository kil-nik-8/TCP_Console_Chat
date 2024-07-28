#pragma once
#include <iostream>

using namespace std;

class Chat
{
public:
	Chat() : _messages("") {};
	friend ostream& operator<<(ostream& output, const Chat& chat);

	void SetMessages(string message);
	void addMessage(string message, string username);

	void clear();
private:
	string _messages;
};

