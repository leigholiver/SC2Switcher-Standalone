#pragma once

#include <QTimer>
#include <QMainWindow>
#include <vector>
#include "Observer.h"
#include "SC2State.h"

using namespace std;

class ScoreWriter;
class ScoreTracker : public Observer {
  private:
  	QTimer* timer;
	vector<ScoreWriter*> watchers;
	QMainWindow* mw;

  public:
    explicit ScoreTracker(SC2Data *sc2, QMainWindow* window);
	static ScoreTracker* Current();
	~ScoreTracker();
    std::map <
		std::string,
		std::map<std::string, size_t>
	> scores;
    std::string getScoreString();
	void recordScore(std::string race, std::string result);
	void handleButton(std::string race, std::string result, std::string name);
	void addRandomConfirmMessage(std::string result);
	void addConfirmMessage(player* playerA, player* playerB);
	void attach(ScoreWriter* sw);

  	public slots:
  		void notify(SC2State*& previous, SC2State*& current);
    	void updateText();
    	std::string getName();
};