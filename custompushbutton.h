#ifndef CUSTOMPUSHBUTTON_H
#define CUSTOMPUSHBUTTON_H

#include <QPushButton>
#include <QSet>

class CustomPushButton
{
public:
    explicit CustomPushButton();
    CustomPushButton(QPushButton *button);
    enum Icon {CLEAR, ONE, TWO, THREE, FOUR, FIVE, SIX, BOMB, FLAG};
    void setIcon(Icon icon);

    QPushButton *button() const;

    bool isMine() const;

    void setIsMine(bool newIsMine);

    void evaluateNeighbours();

    void addNeighbour(CustomPushButton* neighbour);

    void disableNeighbours();

    Icon icon() const;

    Icon role() const;

private:
    QPushButton* m_button;
    QSet<CustomPushButton*>* m_neighbours;
    bool m_isMine = false;
    Icon m_icon = CLEAR;
    Icon m_role = CLEAR;

};

#endif // CUSTOMPUSHBUTTON_H
