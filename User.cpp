#include "User.h"
#include <cstring>

User::~User()
{
	if (_pass_hash != nullptr)
		delete[] _pass_hash;
	_pass_hash = nullptr;
}

User& User::operator=(const User& other)
{
	_username = other._username;
	_ID = other._ID;
	
	if (_pass_hash != nullptr)
		delete[] _pass_hash;
	
	_pass_hash = new uint[5];
	/*
	memcpy(_pass_hash, other._pass_hash, SHA1HASHLENGTHBYTES);
	*/
	for (int i = 0; i < 5; i++)
		_pass_hash[i] = other._pass_hash[i];

	return *this;
}

void User::setName(string uname)
{
	_username = uname;
}

void User::setPassword(char* password, uint pass_length)
{
	
	if (_pass_hash != nullptr)
		delete[] _pass_hash;
	_pass_hash = new uint[5];
	/*
	memcpy(_pass_hash, sha1(password, pass_length), SHA1HASHLENGTHBYTES);
	*/
	uint* hash = new uint[SHA1HASHLENGTHUINTS];
	memcpy(hash, sha1(password, pass_length), SHA1HASHLENGTHBYTES);
	for (int i = 0; i < 5; i++)
	{
		_pass_hash[i] = hash[i];
	}
	hash = nullptr;
}

void User::setPassword(string pass)
{
	
	if (_pass_hash != nullptr)
		delete[] _pass_hash;
	_pass_hash = new uint[5];
	/*
	char* password = new char[pass.length()];
	for (int i = 0; i < pass.length(); i++)
		password[i] = pass[i];

	memcpy(_pass_hash, sha1(password, pass.length()), SHA1HASHLENGTHBYTES);
	delete[] password;
	*/
	char* password = new char[pass.length()];
	for (int i = 0; i < pass.length(); i++)
		password[i] = pass[i];

	memcpy(_pass_hash, sha1(password, pass.length()), SHA1HASHLENGTHBYTES);
	delete[] password;
}

void User::setPassword(uint* pass)
{
	
	if (_pass_hash != nullptr)
		delete[] _pass_hash;
	_pass_hash = new uint[5];
	/*
	memcpy(_pass_hash, pass, SHA1HASHLENGTHBYTES);
	*/
	for (int i = 0; i < 5; i++)
		_pass_hash[i] = pass[i];
}

void User::setID(int id)
{
	_ID = id;
}

string User::getName() const
{
	return _username;
}

uint* User::getPassword() const
{
	return _pass_hash;
}

int User::getID() const
{
	return _ID;
}

void User::set_chat(Chat* chat, int position)
{
	_chats.insert(make_pair(position, chat));
}

int User::get_chats_count() const
{
	return _chats.size();
}

int User::get_receiver_id(int n)
{
	map<int, Chat*>::iterator it = _chats.begin();
	while (n > 0)
	{
		n--;
		it++;
	}
	return it->first;
}

void User::show_chat(int num)
{
	cout << *_chats[num];
}

void User::add_message(string message, int num)
{
	_chats[num]->addMessage(message, _username);
}

ostream& operator<<(ostream& output, const User& u)
{
	output << "username: " << u._username << endl
		<< "password hash: " << u._pass_hash[0]
		<< endl << u._pass_hash[1]
		<< endl << u._pass_hash[2]
		<< endl << u._pass_hash[3]
		<< endl << u._pass_hash[4] << endl;

	return output;
}

void User::erase()
{
	_username = "";
	
	_pass_hash = nullptr;
	_ID = -1;
	_chats.clear();
}
