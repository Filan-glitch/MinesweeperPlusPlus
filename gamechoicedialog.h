#ifndef GAMECHOICEDIALOG_H
#define GAMECHOICEDIALOG_H

#include <QDialog>

namespace Ui {
class GameChoiceDialog;
}

class GameChoiceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GameChoiceDialog(QWidget *parent = nullptr);
    ~GameChoiceDialog();
    enum Choice {EASY, INTERMEDIATE, HARD, CONFUSION1, CONFUSION2, CONFUSION3, BEGINNER1, BEGINNER2};

    GameChoiceDialog::Choice getChoice() const;

private:
    Ui::GameChoiceDialog *ui;
    Choice m_choice = EASY;

private slots:
    void easyClicked(bool checked);
    void intermediateClicked(bool checked);
    void hardClicked(bool checked);
    void confusionEasyClicked(bool checked);
    void confusionIntermediateClicked(bool checked);
    void confusionHardClicked(bool checked);
    void beginnerEasyClicked(bool checked);
    void beginnerIntermediateClicked(bool checked);
};

#endif // GAMECHOICEDIALOG_H
