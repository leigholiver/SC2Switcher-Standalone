#include <curl/curl.h>
#include <jansson.h>
#include "Config.h"
#include <iostream>
#include <fstream>
#include <QDebug>
#include <algorithm>
#include <sstream>
#include <iomanip>

Config* Config::_instance = new Config();

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
	((std::string*)userp)->append((char*)contents, size * nmemb);
	return size * nmemb;
}

Config::Config() : 
ipAddr(std::string("localhost")),
scoreString(std::string("vT: ${tw}-${tl}\nvZ: ${zw}-${zl}\nvP: ${pw}-${pl}"))
{ }

Config* Config::Current() {
   	return _instance;
}

void Config::load() {
	string line;
	ifstream settingsFile("webhook.json");
	if (settingsFile.is_open()) {
		getline(settingsFile, line);
		settingsFile.close();

		json_error_t error;
		json_t* root = json_loads(line.c_str(), 0, &error);
		if (!root) {
			return;
		}

		ipAddr = json_string_value(json_object_get(root, "ipAddr"));
		popupsEnabled = json_boolean_value(json_object_get(root, "popupsEnabled"));
		scoresEnabled = json_boolean_value(json_object_get(root, "scoresEnabled"));
		scoreString = json_string_value(json_object_get(root, "scoreString"));

		vector<std::string> seen;
		json_t* jsonusernames = json_object_get(root, "usernames");
		for (size_t i = 0; i < json_array_size(jsonusernames); i++) {
			string un = json_string_value(json_array_get(jsonusernames, i));
			if (std::find(seen.begin(), seen.end(), un) == seen.end() && un != "") {
				usernames.push_back(un);
				seen.push_back(un);
			}
		}

		seen.clear();
		json_t* webhookURLListJSON = json_object_get(root, "webhookURLList");
		for (size_t i = 0; i < json_array_size(webhookURLListJSON); i++) {
			string un = json_string_value(json_array_get(webhookURLListJSON, i));
			if (std::find(seen.begin(), seen.end(), un) == seen.end() && un != "") {
				webhookURLList.push_back(un);
				seen.push_back(un);
			}
		}
	}
	else {
		qDebug("settings file not open");
	}
}

void Config::save() {
	ofstream settingsFile("webhook.json");
	if (settingsFile.is_open()) {
		std::string out = "{";
		out += "\"ipAddr\": \"" + escape_json(ipAddr) + "\", ";

		out += "\"usernames\": [";
		for (size_t i = 0; i < usernames.size(); i++) {
			if(i > 0) {
				out += ", ";
			}
			out += "\"" + escape_json(usernames[i]) + "\"";
		}
		out += "], ";


		out += "\"webhookURLList\": [";
		for (size_t i = 0; i < webhookURLList.size(); i++) {
			if(i > 0) {
				out += ", ";
			}
			out += "\"" + escape_json(webhookURLList[i]) + "\"";
		}
		out += "],";

		out += "\"scoreString\": \"" + escape_json(scoreString) + "\","; // 
		out += "\"scoresEnabled\": ";
		out += (scoresEnabled ? "true" : "false");
		out += ",";
		
		out += "\"popupsEnabled\": ";
		out += (popupsEnabled ? "true" : "false");
		out += "";

		out += "}";
		settingsFile << out;
		settingsFile.close();
	}
}

void Config::checkForUpdates() {
	CURL *curl;
	CURLcode res;
 	std::string response;
 	
 	curl = curl_easy_init();
	if (curl) {
		std::string reqURL = "https://api.github.com/repos/leigholiver/SC2Switcher-Standalone/releases/latest";

		struct curl_slist *chunk = NULL;
 
		curl_easy_setopt(curl, CURLOPT_URL, reqURL.c_str());
		curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, 500);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
		
		chunk = curl_slist_append(chunk, "User-Agent: SC2Switcher-Standalone");
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);

		res = curl_easy_perform(curl);
		curl_easy_cleanup(curl);
		if (res != CURLE_OK) {
			return;
		}
	}

	json_error_t error;
	json_t* root = json_loads(response.c_str(), 0, &error);
	if (!root) {
		return;
	}

	json_t* url = json_object_get(root, "tag_name");
	if (!url) {
		return;
	}

	const char *urlText = json_string_value(url);
	float latestVer = stof(urlText);
	float currentVer = 0.1;
	if(latestVer > currentVer) {
		json_t* url2 = json_object_get(root, "html_url");
		const char *urlText2 = json_string_value(url2);
		updateURL = urlText2;

		json_t* patch = json_object_get(root, "body");
		const char *patchText = json_string_value(patch);
		updateDescription = patchText;
	}
}

std::string Config::escape_json(const std::string &s) {
	std::ostringstream o;
	for (auto c = s.cbegin(); c != s.cend(); c++) {
		if (*c == '"' || *c == '\\' || ('\x00' <= *c && *c <= '\x1f')) {
			o << "\\u"
				<< std::hex << std::setw(4) << std::setfill('0') << (int)*c;
		}
		else {
			o << *c;
		}
	}
	return o.str();
}