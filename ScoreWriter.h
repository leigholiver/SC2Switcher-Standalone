#pragma once

#include <QObject>

#include "ScoreTracker.h"

class ScoreWriter : public QObject {
	Q_OBJECT
	ScoreTracker *st;

	public:
		ScoreWriter(ScoreTracker *tracker, QObject* parent = Q_NULLPTR)
			: QObject(parent) {
			st = tracker;
			st->attach(this);
		}
		virtual void notify(std::string scoreString) = 0;
};