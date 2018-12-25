#include <QMainWindow>
#include "ui_MainWindow.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    public slots:
	    void on_ipAddr_textChanged(const QString &name);
		void on_addUsernameButton_clicked();
		void on_removeUsernameButton_clicked();
		void on_userNames_itemSelectionChanged();
		void on_recentUsernames_itemSelectionChanged();
		void on_addURLButton_clicked();
		void on_removeURLButton_clicked();
		void on_webhookURLList_itemSelectionChanged();
		void on_textTemplate_textChanged();
		void on_scoresEnabled_stateChanged(int state);
		void on_popupsEnabled_stateChanged(int state);
		void on_tWinPlus_clicked();
		void on_tWinMinus_clicked();
		void on_tLossPlus_clicked();
		void on_tLossMinus_clicked();
		void on_zWinPlus_clicked();
		void on_zWinMinus_clicked();
		void on_zLossPlus_clicked();
		void on_zLossMinus_clicked();
		void on_pWinPlus_clicked();
		void on_pWinMinus_clicked();
		void on_pLossPlus_clicked();
		void on_pLossMinus_clicked();

		void updateForm();
private:
    Ui::MainWindow *ui;
	bool isLoading = false;
	QTimer* timer;
};