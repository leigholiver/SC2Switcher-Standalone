#include <iostream>
#include <fstream>
#include "ScoreWriter.h"
#include "FileScoreWriter.h"
using namespace std;

FileScoreWriter::FileScoreWriter(ScoreTracker *tracker): ScoreWriter(tracker){}

void FileScoreWriter::notify(std::string scoreString) {
	ofstream settingsFile("scores.txt");
	if (settingsFile.is_open()) {
		settingsFile << scoreString;
		settingsFile.close();
	}
}