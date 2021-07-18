#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <array>
#include <nlohmann/json.hpp>

using nlohmann::json;
using std::cout;
using std::cin;
using std::string;
using std::ofstream;
using std::ifstream;

class quiz_bowl{
public:
	
	static void qb_file_delete(ifstream& qb_file, const char*& fname)
	{
		json j;
		try
		{
			j = json::parse(qb_file);
		}
		catch (json::parse_error e)
		{
			error_text();
			exit(-1);
		}
		bool was_deleted = false;
		qb_file.close();
		ofstream o(fname);
		string term;
		cout << "What term would you like to delete\n";
		cin >> term;
		for (const auto& [key, value] : j.items()) 
		{
			if (term == key) 
			{
				j.erase(j.find(key));
				cout << "Term was deleted\n";
				o << std::setw(4) << j << std::endl;
				was_deleted = true;
			}

		}
		if (!was_deleted) 
		{
			cout << "The term does not exist\n";
			o << std::setw(4) << j << std::endl;

		}
	}

	static void qb_file_view(ifstream& qb_file, const char*& fname)
	{
		json j;
		try
		{
			j = json::parse(qb_file);
		}
		catch (json::parse_error e) 
		{
			cout << e.what() << std::endl;
			error_text();
			exit(-1);
		}
		qb_file.close();
		ofstream o(fname);
		bool term_exists = false;
		cout << "what term would you like to view\n";
		for (const auto& [key, value] : j.items())
		{
			cout << key << '\n';
		}
		cout << '\n';
		cin.ignore();
		string term;
		cin >> term;
		system("cls");
		for (const auto& [key, value] : j.items())
		{
			if (term == key)
			{
				const string value_to_string = value;
				cout << key << " :\n" << value_to_string;
				term_exists = true;
			}
		}
		if (!term_exists)
			cout << "The term doesn't exist!\n";
		o << std::setw(4) << j << std::endl;
	}

	static void qb_file_add(ifstream& qb_file, const char*& fname)
	{
		json j;
		try
		{
			j = json::parse(qb_file);
		}
		catch (json::parse_error e)
		{
			error_text();
			exit(-1);
		}
		qb_file.close();
		string term, fact, multifacts;
		cout << "what term\n";
		cin.ignore();
		std::getline(cin, term);
		cout << "what are the facts?\n";
		int counter = 0;
		while (std::getline(cin, fact))
		{
			if (fact == "stop" && counter != 0)
				break;
			else if ((fact == "stop" && counter == 0) || fact.empty())
				cout << "Do not stop in the first line\n or create an empty space as a fact.";
			multifacts += fact + '\n';
			counter++;
		}
		char already_in_file_answer;
		auto already_in_file = [&j, &term]()
		{
			for (const auto& [key, value] : j.items())
			{
				if (term == key)
				{
					return true;
				}
			}
			return false;
		};

		const auto in_file = already_in_file();
		bool wants_to_be_changed = false;
		if (in_file)
		{

			cout << "the term " << term << " is already registered, would you like to change the definition, press y to change it, else press any other letter\n";
			cin >> already_in_file_answer;
			cin.ignore();
			if (already_in_file_answer == 'y') 
			{
				j[term] = std::move(multifacts);
				cout << "The term's definition has been changed\n";
				wants_to_be_changed = true;
			}
			else
				cout << "the term has not been changed\n";
		}
		else
			j[term] = std::move(multifacts);

		ofstream w(fname);
		if (!in_file || wants_to_be_changed)
		{
			w << std::setw(4) << j << std::endl;
			cout << "The term " << term << " has been added\n";
		}
		else
			w << std::setw(4) << j << std::endl;
	}

private:
	static void error_text() 
	{
		cout << "There was an error parsing the json file\n";
		cout << "please put {} in the json file called qb_facts.json, then add something\n";
	}
};

bool ask_for_continuation() 
{
	cout << "\n\n";
	char answer;
	cout << "do you want to do something else? if so press y, else press any key\n";
	cin >> answer;
	if (answer == 'y') 
	{
		system("cls");
		return true;
	}

	return false;
	

}

int main()
{
	std::array<const char*, 3> const commands = { "view or v", "add or a", "delete or del" };
	bool wants_to_continue = true;
	while (wants_to_continue)
	{
		cout << "which command do you want to use\n";
		for (const auto& command : commands)
			cout << command << '\n';

		cout << '\n';
		string answer;
		cin >> answer;
		const char* file = "C:/Dev/Cpp/QuizBowlHelper/Data/qb_facts.json";
		ifstream quiz_bowl_file(file);

		if (answer == "view" || answer == "v")
		{
			quiz_bowl::qb_file_view(quiz_bowl_file, file);
			wants_to_continue = std::move(ask_for_continuation());
		}
		else if (answer == "add" || answer == "a")
		{
			quiz_bowl::qb_file_add(quiz_bowl_file, file);
			wants_to_continue = std::move(ask_for_continuation());
		}
		else if (answer == "delete" || answer == "del")
		{
			quiz_bowl::qb_file_delete(quiz_bowl_file, file);
			wants_to_continue = std::move(ask_for_continuation());
		}
		else 
		{
			cout << "Invalid argument passed\n";
		}
	}
}
