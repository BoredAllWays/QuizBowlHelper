#include <iostream>
#include <fstream>
#include <string>
#include <nlohmann/json.hpp>

using nlohmann::json;
using std::cout;
using std::cin;
using std::string;

class quiz_bowl
{
public:

	static void qb_file_view(std::ifstream& qb_file, string fname)
	{
		json j = json::parse(qb_file);
		string term;
		bool term_exists = false;
		cout << "what term would you like to view\n";
		cin >> term;
		for (auto& i : j.items())
		{
			if (term == i.key())
			{
				string val = i.value();
				cout << i.key() << " : " << val;
				term_exists = true;
			}
		}
		if (!term_exists)
			cout << "The term doesn't exist!\n";

		std::ofstream o(fname);
		o << j;
	}

	static void qb_file_add(std::ifstream& qb_file, string fname)
	{
		json x;
		try
		{
			x = json::parse(qb_file);
		}
		catch (...)
		{
			cout << "There was an error parsing the json file\n";
			exit(-1);
		}
		json j = x;
		qb_file.close();
		string term, fact, multifacts;
		cout << "what term\n";
		cin.ignore();
		std::getline(cin, term);
		cout << "what are the facts?\n";
		int counter = 0;
		while (std::getline(cin, fact) && !fact.empty())
		{
			if (fact == "stop" && counter != 0)
				break;
			else if (fact == "stop" && counter == 0)
				cout << "Do not stop in the first line\n";
			multifacts += fact + '\n';
			counter++;
		}
		char already_in_file_answer;
		auto already_in_file = [j, term]()
		{
			for (auto& i : j.items())
			{
				if (term == i.key())
				{
					return true;
				}
			}
			return false;
		};

		bool wants_to_be_changed = false;
		if (already_in_file())
		{

			cout << "the term " << term << " is already registered, would you like to change the defenition, press y to change it, else press any other letter\n";
			cin >> already_in_file_answer;
			cin.ignore();
			switch (already_in_file_answer)
			{
			case 'y':
				j[term] = multifacts;
				cout << "The term's definition has been changed\n";
				wants_to_be_changed = true;
				break;
			default:
				cout << "the term has not been changed\n";
				break;
			}
		}
		else
			j[term] = multifacts;

		std::ofstream w(fname);
		if (!already_in_file() || wants_to_be_changed)
		{
			w << j;
			cout << "The term " << term << " has been added\n";
		}
		else
			w << x;
	}
};


int main()
{
	cout << "say add to add something or view to view something\n";
	string answer;
	cin >> answer;
	string file = "data/qb_facts.json";
	std::ifstream quiz_bowl_file(file);

	if (answer == "view")
	{
		quiz_bowl::qb_file_view(quiz_bowl_file, file);
	}
	else if (answer == "add")
	{
		quiz_bowl::qb_file_add(quiz_bowl_file, file);
	}
	else
	{
		cout << "Invalid argument passed\n";
	}
	return 0;
}