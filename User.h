#pragma once
#include <iostream>
#include "sha1.h"
#include <map>
#include "Chat.h"

using namespace std;

class User
{
	string _username;
	uint* _pass_hash;
	int _ID;
	map<int, Chat*> _chats;

public:
	User() : _username(""), _ID(-1), _pass_hash(nullptr) {};
	User(string username, uint* hash, int id) : _username(username), _ID(id), _pass_hash(hash) {};
	~User();
	User& operator=(const User& other);

	void setName(string username);
	void setPassword(char* password, uint pass_length);
	void setPassword(string pass);
	void setPassword(uint* pass);
	void setID(int id);
	string getName() const;
	uint* getPassword() const;
	int getID() const;

	void set_chat(Chat* chat, int position);
	int get_chats_count() const;
	int get_receiver_id(int n);
	void show_chat(int num);
	void add_message(string message, int num);

	friend ostream& operator<<(ostream& output, const User& u);

	void erase();
};


