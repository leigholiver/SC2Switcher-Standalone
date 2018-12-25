#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "SC2Data.h"
#include "Webhook.h"
#include "Config.h"
#include "ScoreTracker.h"
#include "FileScoreWriter.h"

Webhook* wh = nullptr;
ScoreTracker* st = nullptr;
FileScoreWriter* fsw = nullptr;
Config* config = Config::Current();

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
	config->load();
	config->checkForUpdates();
	SC2Data::Instance = new SC2Data();
	wh = new Webhook(SC2Data::Instance);
	st = new ScoreTracker(SC2Data::Instance, this);
	fsw = new FileScoreWriter(st);
	
	isLoading = true;
	for (size_t i = 0; i < config->usernames.size(); i++) {
		if (config->usernames[i] != "") {
			ui->userNames->addItem(config->usernames[i].c_str());
		}
	}

	ui->ipAddr->setText(QString::fromStdString(config->ipAddr));
	if (config->updateURL != "") {
		ui->downloadURL->setText(QString::fromStdString("<a href=\"" + config->updateURL + "\">Update Available. Click here to download.</a>"));
		ui->downloadURL->setTextFormat(Qt::RichText);
		ui->downloadURL->setTextInteractionFlags(Qt::TextBrowserInteraction);
		ui->downloadURL->setOpenExternalLinks(true);
		ui->patchNotesLabel->setText(config->updateDescription.c_str());
	}
	for (size_t i = 0; i < config->webhookURLList.size(); i++) {
		ui->webhookURLList->addItem(config->webhookURLList[i].c_str());
	}

	ui->textTemplate->setText(QString::fromStdString(config->scoreString));
	if (config->scoresEnabled) {
		ui->scoresEnabled->setChecked(true);
	}
	else {
		ui->scoresEnabled->setChecked(false);
	}
	if (config->popupsEnabled) {
		ui->popupsEnabled->setChecked(true);
	}
	else {
		ui->popupsEnabled->setChecked(false);
	}

	isLoading = false;

	timer = new QTimer(this);
	QObject::connect(timer, &QTimer::timeout, this, [=]{ updateForm(); });
	timer->start(1000);
}

void MainWindow::updateForm() {
	isLoading = true;
	ui->userNames->clear();
	for (size_t i = 0; i < config->usernames.size(); i++) {
		if (config->usernames[i] != "") {
			ui->userNames->addItem(config->usernames[i].c_str());
		}
	}
	
	ui->recentUsernames->clear();
	for (size_t i = 0; i < config->recentUsernames.size(); i++) {
		if (config->recentUsernames[i] != "") {
			ui->recentUsernames->addItem(config->recentUsernames[i].c_str());
		}
	}

	isLoading = false;
}

MainWindow::~MainWindow()
{
	delete wh;
    delete ui;
}

void MainWindow::on_ipAddr_textChanged(const QString& text) {
	if(!isLoading) {
		config->ipAddr = (string)text.toUtf8().constData();
	}
}
void MainWindow::on_addUsernameButton_clicked() {
	if(!isLoading) {
		// add the username to our usernames
		string username = ui->usernameLine->text().toUtf8().constData();

		if (std::find(config->usernames.begin(), config->usernames.end(), username) == config->usernames.end() && username != "") {
			config->usernames.push_back(username);		// add it to the ui list
			ui->userNames->addItem(ui->usernameLine->text());
		}

		// clear the username input
		ui->usernameLine->setText("");
	}
}
void MainWindow::on_removeUsernameButton_clicked() {
	if(!isLoading) {
		// remove the list item from our usernames
		config->usernames.erase(std::remove(config->usernames.begin(), config->usernames.end(), ui->usernameLine->text().toUtf8().constData()), config->usernames.end());

		// remove the list item from the ui list
		// have to iterate backwards because the size of the list changes as items are removed
		// also store the name first as the value of usernameLine changes as items are removed and new items selected
		string searchName = ui->usernameLine->text().toUtf8().constData();
		for (int i = ui->userNames->count()-1; i >= 0; i--) {
			QListWidgetItem *item = ui->userNames->item(i);
			if (item->text() == QString::fromStdString(searchName)) {
				int row = ui->userNames->row(item);
				delete ui->userNames->takeItem(row);
			}
		}

		// clear the username box if its empty
		// otherwise box will hold selected item
		if (ui->userNames->count() == 0) {
			ui->usernameLine->setText("");
		}
	}
}
void MainWindow::on_userNames_itemSelectionChanged() {
	if(!isLoading) {
		if (ui->userNames->currentItem() != NULL && ui->userNames->currentItem()->text() != NULL) {
			ui->usernameLine->setText(ui->userNames->currentItem()->text());
		}
	}
}
void MainWindow::on_recentUsernames_itemSelectionChanged() {
	if(!isLoading) {
		if (ui->recentUsernames->currentItem() != NULL && ui->recentUsernames->currentItem()->text() != NULL) {
			ui->usernameLine->setText(ui->recentUsernames->currentItem()->text());
		}
	}
}
void MainWindow::on_addURLButton_clicked() {
	if(!isLoading) {
		string username = ui->webhookEnterGame->text().toUtf8().constData();
		config->webhookURLList.push_back(username);
		ui->webhookURLList->addItem(ui->webhookEnterGame->text());
		ui->webhookEnterGame->setText("");
	}
}

void MainWindow::on_removeURLButton_clicked() {
	if(!isLoading) {
		config->webhookURLList.erase(std::remove(config->webhookURLList.begin(), config->webhookURLList.end(), ui->webhookEnterGame->text().toUtf8().constData()), config->webhookURLList.end());

		string searchName = ui->webhookEnterGame->text().toUtf8().constData();
		for (int i = ui->webhookURLList->count()-1; i >= 0; i--) {
			QListWidgetItem *item = ui->webhookURLList->item(i);
			if (item->text() == QString::fromStdString(searchName)) {
				int row = ui->webhookURLList->row(item);
				delete ui->webhookURLList->takeItem(row);
			}
		}

		if (ui->webhookURLList->count() == 0) {
			ui->webhookEnterGame->setText("");
		}
	}
}

void MainWindow::on_webhookURLList_itemSelectionChanged() {
	if(!isLoading) {
		if (ui->webhookURLList->currentItem() != NULL && ui->webhookURLList->currentItem()->text() != NULL) {
			ui->webhookEnterGame->setText(ui->webhookURLList->currentItem()->text());
		}
	}
}

void MainWindow::on_textTemplate_textChanged() {
	if (!isLoading) {
		if (config->scoreString != (string)ui->textTemplate->toPlainText().toUtf8().constData()) {
			config->scoreString = (string)ui->textTemplate->toPlainText().toUtf8().constData();
		}
	}
}

void MainWindow::on_scoresEnabled_stateChanged(int state) {
	if (!isLoading) {
		if (state == Qt::Checked) {
			config->scoresEnabled = true;
		}
		else {
			config->scoresEnabled = false;
		}
	}
}

void MainWindow::on_popupsEnabled_stateChanged(int state) {
	if (!isLoading) {
		if (state == Qt::Checked) {
			config->popupsEnabled = true;
		}
		else {
			config->popupsEnabled = false;
		}
	}
}

void MainWindow::on_tWinPlus_clicked() {
	ScoreTracker* st = ScoreTracker::Current();
	if (!isLoading && st) {
		st->scores["Terr"]["Victory"] = st->scores["Terr"]["Victory"] + 1;
		st->updateText();
		//ui->scoresLabel->setText(st->getScoreString().c_str());
	}
}

void MainWindow::on_tWinMinus_clicked() {
	ScoreTracker* st = ScoreTracker::Current();
	if (!isLoading) {
		if (st->scores["Terr"]["Victory"] > 0) {
			st->scores["Terr"]["Victory"] = st->scores["Terr"]["Victory"] - 1;
			st->updateText();
			//ui->scoresLabel->setText(st->getScoreString().c_str());
		}
	}
}

void MainWindow::on_tLossPlus_clicked() {
	ScoreTracker* st = ScoreTracker::Current();
	if (!isLoading) {
		st->scores["Terr"]["Defeat"] = st->scores["Terr"]["Defeat"] + 1;
		st->updateText();
		//ui->scoresLabel->setText(st->getScoreString().c_str());
	}
}

void MainWindow::on_tLossMinus_clicked() {
	ScoreTracker* st = ScoreTracker::Current();
	if (!isLoading) {
		if (st->scores["Terr"]["Defeat"] > 0) {
			st->scores["Terr"]["Defeat"] = st->scores["Terr"]["Defeat"] - 1;
			st->updateText();
			//ui->scoresLabel->setText(st->getScoreString().c_str());
		}
	}
}

void MainWindow::on_zWinPlus_clicked() {
	ScoreTracker* st = ScoreTracker::Current();
	if (!isLoading) {
		st->scores["Zerg"]["Victory"] = st->scores["Zerg"]["Victory"] + 1;
		st->updateText();
		//ui->scoresLabel->setText(st->getScoreString().c_str());
	}
}

void MainWindow::on_zWinMinus_clicked() {
	ScoreTracker* st = ScoreTracker::Current();
	if (!isLoading) {
		if (st->scores["Zerg"]["Victory"] > 0) {
			st->scores["Zerg"]["Victory"] = st->scores["Zerg"]["Victory"] - 1;
			st->updateText();
			//ui->scoresLabel->setText(st->getScoreString().c_str());
		}
	}
}

void MainWindow::on_zLossPlus_clicked() {
	ScoreTracker* st = ScoreTracker::Current();
	if (!isLoading) {
		st->scores["Zerg"]["Defeat"] = st->scores["Zerg"]["Defeat"] + 1;
		st->updateText();
		//ui->scoresLabel->setText(st->getScoreString().c_str());
	}
}

void MainWindow::on_zLossMinus_clicked() {
	ScoreTracker* st = ScoreTracker::Current();
	if (!isLoading) {
		if (st->scores["Zerg"]["Defeat"] > 0) {
			st->scores["Zerg"]["Defeat"] = st->scores["Zerg"]["Defeat"] - 1;
			st->updateText();
			//ui->scoresLabel->setText(st->getScoreString().c_str());
		}
	}
}

void MainWindow::on_pWinPlus_clicked() {
	ScoreTracker* st = ScoreTracker::Current();
	if (!isLoading) {
		st->scores["Prot"]["Victory"] = st->scores["Prot"]["Victory"] + 1;
		st->updateText();
		//ui->scoresLabel->setText(st->getScoreString().c_str());
	}
}

void MainWindow::on_pWinMinus_clicked() {
	ScoreTracker* st = ScoreTracker::Current();
	if (!isLoading) {
		if (st->scores["Prot"]["Victory"] > 0) {
			st->scores["Prot"]["Victory"] = st->scores["Prot"]["Victory"] - 1;
			st->updateText();
			//ui->scoresLabel->setText(st->getScoreString().c_str());
		}
	}
}

void MainWindow::on_pLossPlus_clicked() {
	ScoreTracker* st = ScoreTracker::Current();
	if (!isLoading) {
		st->scores["Prot"]["Defeat"] = st->scores["Prot"]["Defeat"] + 1;
		st->updateText();
		//ui->scoresLabel->setText(st->getScoreString().c_str());
	}
}

void MainWindow::on_pLossMinus_clicked() {
	ScoreTracker* st = ScoreTracker::Current();
	if (!isLoading) {
		if (st->scores["Prot"]["Defeat"] > 0) {
			st->scores["Prot"]["Defeat"] = st->scores["Prot"]["Defeat"] - 1;
			st->updateText();
			//ui->scoresLabel->setText(st->getScoreString().c_str());
		}
	}
}