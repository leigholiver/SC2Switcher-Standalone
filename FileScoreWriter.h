#pragma once

#include "ScoreWriter.h"
#include "ScoreTracker.h"

class FileScoreWriter : public ScoreWriter {
	
	public:
		explicit FileScoreWriter(ScoreTracker* tracker);

	public slots:
		void notify(std::string scoreString);
};