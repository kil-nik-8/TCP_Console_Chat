#pragma once
#include <iostream>
#include <string>
#include <thread>
#include <stdlib.h>
#include <cstring>

#ifdef _WIN32
#include <winsock2.h>
#include <windows.h>
#pragma warning(disable : 4996)
#include <ws2tcpip.h>
#pragma comment (lib, "ws2_32.lib")

#elif defined (__linux__)
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#define SOCKET int
#define INVALID_SOCKET -1
#define closesocket close
#define ZeroMemory bzero
#define SOCKET_ERROR -1
#endif

using namespace std;
#ifdef _WIN32
int getYcoord()
{
	CONSOLE_SCREEN_BUFFER_INFO info_y;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info_y);
	return info_y.dwCursorPosition.Y;
}

int getXcoord()
{
	CONSOLE_SCREEN_BUFFER_INFO info_x;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info_x);
	return info_x.dwCursorPosition.X;
}

void set_cursor(int x = 0, int y = 0)
{
	HANDLE handle;
	COORD coordinates;
	handle = GetStdHandle(STD_OUTPUT_HANDLE);
	coordinates.X = x;
	coordinates.Y = y;
	SetConsoleCursorPosition(handle, coordinates);
}
#endif

string get_login_from_id(int id)
{
	for (const auto& user : users)
	{
		if ((user.second).getID() == id)
		{
			return user.first;
		}
	}
	return "";
}

void tokenize(string const& str, vector<string>& out)
{
    size_t start;
    size_t end = 0;

    while ((start = str.find_first_not_of(' ', end)) != string::npos)
    {
        end = str.find(' ', start);
        out.push_back(str.substr(start, end - start));
    }
}

uint string_to_uint(string str)
{
	uint Ans = 0;
	int q = 1;
	for (int i = str.length() - 1; i >= 0; i--)
	{
		Ans += (int(str[i]) - 48) * q;
		q *= 10;
	}
	return Ans;
}

void set_data_from_file()
{
	fstream file;
	char open_file[400];
	users.clear();
	vector<string> out;
	User user;

	// Open the file with authorization data
	file.open("AuthData.txt");
	// if AuthData.txt doesn't exist
	if (!file.is_open())
	{
		file.close();
		// creating AuthData.txt
		ofstream{ "AuthData.txt" };
		file.open("AuthData.txt");	
	}

	// Move pointer to the end of file
	file.seekg(0, ios_base::end);
	// if file is empty
	if (file.tellg())
	{
		// Move pointer to the begining of file
		file.seekg(0, ios_base::beg);
		while (!file.eof())
		{
			file.getline(open_file, 400);
			if (string(open_file) != "")
			{
				user.erase();
				out.clear();
				tokenize(open_file, out);
				
				// memorize the login
				user.setName(out[6]);
				uint* hash = new uint[5];
				hash[0] = string_to_uint(out[1]);
				hash[1] = string_to_uint(out[2]);
				hash[2] = string_to_uint(out[3]);
				hash[3] = string_to_uint(out[4]);
				hash[4] = string_to_uint(out[5]);
				user.setPassword(hash);

				for (int j = 0; j < 5; j++)
					hash[j] = 0;
				delete[] hash;
				user.setID(ID_Number);

				pair<string, User> myPair = make_pair((string)out[0], user);
				users.insert(myPair);
				user.erase();
				ID_Number++;
				myPair.second.erase();
			}
		}
	}
	file.close();

	// Open the file with messages in public chat
	file.open("Public_Chat.txt");
	// if Public_Chat.txt doesn't exist (similar to AuthData.txt)
	if (!file.is_open())
	{
		file.close();
		ofstream{ "Public_Chat.txt" };
		file.open("Public_Chat.txt");
	}
	file.seekg(0, ios_base::end);
	if (file.tellg())
	{
		file.seekg(0, ios_base::beg);
		while (!file.eof())
		{
			file.getline(open_file, 400);
			public_chat.SetMessages(string(open_file));
		}
	}
	file.close();

	string name = "";
	Chat *chat;
	for (int i = 1; i < ID_Number; i++)
	{
		for (int j = i + 1; j < ID_Number; j++)
		{
			name = "Private_Chat_" + to_string(i) + "_" + to_string(j) + ".txt";
			file.open(name);
			if (!file.is_open())
			{
				chat = new Chat;
				file.close();
				ofstream{ name };
				file.open(name);
				users[get_login_from_id(i)].set_chat(chat, j);
				users[get_login_from_id(j)].set_chat(chat, i);
				chat = nullptr;
			}
			file.seekg(0, ios_base::end);

			if (file.tellg() && file.is_open())
			{
				chat = new Chat;
				file.seekg(ios_base::beg);
				while (!file.eof())
				{
					file.getline(open_file, 400);
					(*chat).SetMessages(string(open_file));
				}
				users[get_login_from_id(i)].set_chat(chat, j);
				users[get_login_from_id(j)].set_chat(chat, i);
				chat = nullptr;
			}
			file.close();
		}
	}
}

#define MESSAGE_BUFFER 1024 // Maximum siae of buffer for sending and receiving
#define PORT 7777 // Port number which we will use for sending and receiving 

int socket_file_descriptor;
char message[MESSAGE_BUFFER];

bool connection()
{
	#ifdef _WIN32
	WSADATA wsData;
	WORD ver = MAKEWORD(2, 2);

	// Loading library winsock
	int wsok = WSAStartup(ver, &wsData);
	if (wsok != 0)
	{
		cerr << "Can't Initialize winsock! Quitig" << endl;
		return false;
	}
	#endif
	// Create a socket
	socket_file_descriptor = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_file_descriptor == -1) {
		cout << "Creation of Socket failed!" << endl;
		return false;
	}

	sockaddr_in serveraddress;
	// Set server address 
	serveraddress.sin_addr.s_addr = inet_addr("127.0.0.1");
	// Set port number
	serveraddress.sin_port = htons(PORT);
	// using IPv4
	serveraddress.sin_family = AF_INET;
	// Set connection with the server
	int cnt = connect(socket_file_descriptor, (struct sockaddr*)&serveraddress, sizeof(serveraddress));
	if (cnt == -1) {
		cout << "Connection with the server failed.!" << endl;
		return false;
	}
	return true;
}

bool Authorization()
{
	string login, pass;
	bool access = true;
	User user;
	cout << "login: ";
	// enter login
	cin >> login;

	if (users.find(login) == users.end())
	{
		cout << "Incorrect login." << endl;
		system("pause");
		return false;
	}

	cout << "password: ";
	// enter password
	cin >> pass;
	char* password = new char[pass.length()];
	for (int i = 0; i < pass.length(); i++)
		password[i] = pass[i];
	unordered_map<string, User>::iterator it = users.find(login);
	user = it->second;
	
	// getting password's hash
	uint* hash1 = sha1(password, pass.length());
	uint* hash2 = user.getPassword();

	// check password's hash
	for (int j = 0; j < 5; j++)
	{
		if (hash1[j] != hash2[j])
			access = false;
	}

	if (access)
	{
		current_name = user.getName();
		user.erase();
		current_user = login;
		return true;
	}
	else
	{
		cout << "Incorrect password." << endl;
		user.erase();
		system("pause");
		return false;
	}
}

bool Registration()
{
	string login, pass, name;
	User user;
	user.erase();
	cout << "Set login: ";
	cin >> login;

	fstream file;

	unordered_map<string, User>::iterator it = users.find(login);
	if (it != users.end())
	{
		cout << "This login is already taken." << endl;
		system("pause");
		return false;
	}
	user.setID(ID_Number++);

	cout << "Set password: ";
	cin >> pass;
	user.setPassword(pass);

	cout << "Set your UserName: ";
	cin >> name;
	user.setName(name);

	cout << user << endl;

	pair<string, User> myPair = make_pair((string)login, user);
	users.insert(myPair);
	for (int i = 1; i < ID_Number - 1; i++)
	{
		string name = "Private_Chat_" + to_string(i) + "_" + to_string(ID_Number - 1) + ".txt";
		Chat* chat = new Chat;
		ofstream{ name };
		users[get_login_from_id(i)].set_chat(chat, ID_Number - 1);
		users[login].set_chat(chat, i);
		chat = nullptr;
	}

	uint* hash = user.getPassword();
	file.open("AuthData.txt", ios::app);
	file << login << " ";
	current_user = login;
	for (int j = 0; j < 5; j++)
	{
		file << hash[j] << " ";
		cout << hash[j] << " ";
	}
	file << name << "\n";
	current_name = name;
	user.erase();
	myPair.second.erase();
	file.close();
	return true;
}

void Communication(int chat_num)
{
	fstream file;
	string message = "";
	if (!chat_num)
	{
		file.open("Public_Chat.txt", ios::app);
		while (message != "CLOSE")
		{
			// Cleanup console window
			system("cls");
			// reminder for user
			cout << "CLOSE - close chat" << endl;
			cout << "--------------------------------"
				<< "-------------------------------" << endl;
			cout << public_chat << endl;
			cout << "Message: ";
			getline(cin, message);
			// cleanup spaces from message
			while (message[0] == ' ')
				message.erase(0, 1);
			if (message != "CLOSE")
			{
				// sending message to the chat
				public_chat.addMessage(message, users[current_user].getName());
				if (message.size() > 0)
					file << users[current_user].getName() + ": " + message + "\n"; // save message into file
			}
		}
		file.close();
	}
	else
	{
		try
		{
			users.at(get_login_from_id(chat_num));
		}
		catch (out_of_range& e)
		{
			cout << "Incorrect data." << endl;
			message = "CLOSE";
			system("pause");
			return;
		}

		string name = "";
		if (users[current_user].getID() < chat_num)
			name = "Private_Chat_" + to_string(users[current_user].getID()) + "_" + to_string(chat_num) + ".txt";
		else
			name = "Private_Chat_" + to_string(chat_num) + "_" + to_string(users[current_user].getID()) + ".txt";
		file.open(name, ios::app);
		while (message != "CLOSE")
		{
			// Cleanup console window
			system("cls");
			// reminder for user
			cout << "CLOSE - close chat" << endl;
			cout << "--------------------------------"
				<< "-------------------------------" << endl;
			users[current_user].show_chat(chat_num);
			cout << endl << "Message: ";
			getline(cin, message);
			// cleanup spaces from message
			while (message[0] == ' ')
				message.erase(0, 1);
			if (message != "CLOSE")
			{
				// sending message to the chat
				users[current_user].add_message(message, chat_num);
				if (message.size() > 0)
					file << users[current_user].getName() + ": " + message + "\n";	// save message into file
			}
		}
		file.close();
	}
}

void listening()
{
	#ifdef _WIN32
	int x = 0, y = getYcoord();
	#endif
	int recieve_bytes;
	while (true)
	{
		ZeroMemory(message, MESSAGE_BUFFER);
		// receiving message
		#ifdef _WIN32
		recieve_bytes = recv(socket_file_descriptor, message, sizeof(message), 0);
		#elif defined(__linux__)
		recieve_bytes = read(socket_file_descriptor, message, sizeof(message));
		#endif
		if (recieve_bytes <= 0)
			break;
		else
		{
			#ifdef _WIN32
			set_cursor(x, y);
			cout << message << endl;
			x = 0, y = getYcoord();
			#elif defined(__linux__)
			cout << message << endl;
			#endif
		}
	}
}

void TCP_Communication()
{
	string str = "";
	// create a thread for listening incoming messages
	thread th_listen(listening);
	#ifdef _WIN32
	int x, y;
	x = getXcoord();
	y = getYcoord();
	cout << endl;
	#endif
	getline(cin, str);
	while (1) {
		ZeroMemory(message, MESSAGE_BUFFER);
		#ifdef _WIN32
		set_cursor(x, y);
		#endif
		// enter our message
		getline(cin, str);
		if (str == "end")
		{
			cout << "Client Exit." << endl;
			th_listen.detach();
			break;
		}
		#ifdef _WIN32
		set_cursor(x, y);
		#endif
		str = current_name + ": " + str;
		for (int i = 0; i < str.length(); i++)
			message[i] = str[i];
		// sending message
		#ifdef _WIN32
		int bytes = send(socket_file_descriptor, message, sizeof(message), 0);
		#elif defined(__linux__)
		int bytes = write(socket_file_descriptor, message, sizeof(message));
		#endif
	}

	// end of connection
	closesocket(socket_file_descriptor);
}
