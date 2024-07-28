#include <iostream>
#include <unordered_map>
#include <vector>
#include "User.h"
#include <fstream>

int ID_Number;
unordered_map<string, User> users;
string current_user = "";
string current_name = "";
Chat public_chat;

#include "Functions.h"

using namespace std;

auto main() -> int
{
    ID_Number = 1;
	char input, input_2, proof;
	bool access = false;
	string message = "", enter = "";

	set_data_from_file();

	do
	{
		// Cleanup console window
		system("cls");
		// Select option
		cout << "0 - log in" << endl; // authorization
		cout << "1 - sign up" << endl; // registration
		cout << "q - close Console Chat" << endl; // exit
		cin >> enter;
		if (enter.size() == 1)
			input = enter[0];
		else
			input = '5';

		switch (input)
		{
		case '0':
			access = Authorization();
			break;

		case '1':
			access = Registration();
			break;

		case 'q':
			proof = 'q';
			while (proof != '0' && proof != '1')
			{
				// getting a proof of exiting
				cout << "Are you sure you want to close the Console Chat?" << endl;
				cout << "0 - No" << endl;
				cout << "1 - Yes" << endl;

				enter = "";
				cin >> enter;
				if (enter.size() == 1)
					proof = enter[0];
				else
					proof = '5';

				if (proof != '0' && proof != '1')
				{
					cout << "Incorrect data." << endl;
					system("pause");
					system("cls");
				}
			}
			if (proof == '0')
				input = 'f'; // Change value of "input" to stay in Console_chat
			break;

		default:
			cout << "Incorrect data." << endl;
			system("pause");
		}

		if (input != 'q' && access)
		{
			access = true;
			do
			{
				// Cleanup console window
				system("cls");
				cout << current_user << endl;
				cout << "0 - exit" << endl;
				cout << "1 - chat" << endl;	

				cin >> enter;
				if (enter.size() == 1)
					input_2 = enter[0];
				else
					input_2 = '5';
				//int chats_count = users[current_user].get_chats_count(), id;
				switch (input_2)
				{
				case '0':
					cout << "Are you sure you want to exit?" << endl;
					cout << "0 - No" << endl;
					cout << "1 - Yes" << endl;
					cin >> proof;
					
					if (proof == '1')
						access = false;
					
					break;

				case '1':
					connection();
					system("cls");
					cout << current_user << endl;
					cout << "Enter \"end\" to close chat." << endl;
					cout << "----------------------------------------" << endl;
					TCP_Communication();
					access = false;
					break;

				default:
					cout << "Incorrect data." << endl;
					system("pause");
				}
			} while (access);
			
		}
	} while (input != 'q');
	return 0;
}
