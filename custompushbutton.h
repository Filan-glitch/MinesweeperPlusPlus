#ifndef CUSTOMPUSHBUTTON_H
#define CUSTOMPUSHBUTTON_H

#include <QPushButton>
#include <QSet>
#include <QMouseEvent>

class CustomPushButton : public QPushButton
{
    Q_OBJECT
public:
    explicit CustomPushButton(QWidget *parent = nullptr);
    enum Icon {CLEAR, ONE, TWO, THREE, FOUR, FIVE, SIX, BOMB, FLAG, GOLD};
    void setCustomIcon(Icon icon);

    bool isMine() const;

    void setIsMine(bool newIsMine);

    void evaluateNeighbours();

    void addNeighbour(CustomPushButton* neighbour);

    void disableNeighbours();

    Icon icon() const;

    Icon role() const;



private:
    QSet<CustomPushButton*>* m_neighbours;
    bool m_isMine = false;
    Icon m_icon = CLEAR;
    Icon m_role = CLEAR;



    // QWidget interface
protected:
    void mousePressEvent(QMouseEvent *event) override;
};

#endif // CUSTOMPUSHBUTTON_H
