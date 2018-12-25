#pragma once
#include <vector>
#include <string>
#include <algorithm>

#include "Constants.h"

using namespace std;

class Config {
	public:
		Config();
		static Config* Current();
		void load();
		void save();
		std::string escape_json(const std::string &s);

		std::string ipAddr;
		vector<std::string> usernames;
		vector<std::string> recentUsernames;

		std::string updateURL;
		std::string updateDescription;
		void checkForUpdates();
		vector<std::string> webhookURLList;

		std::string scoreString;
		bool scoresEnabled;
		bool popupsEnabled;


	private:
		static Config* _instance;
};