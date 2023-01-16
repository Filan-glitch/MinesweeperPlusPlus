#include "custompushbutton.h"

CustomPushButton::CustomPushButton(QWidget *parent) : QPushButton(parent)
{
    m_neighbours = new QSet<CustomPushButton*>;
}

//function that sets an icon
void CustomPushButton::setCustomIcon(Icon icon)
{
    switch(icon) {
    case CLEAR: {
        QIcon iconClear(":/unclicked.png");
        iconClear.addFile(":/clear.png", QSize(64,64),QIcon::Disabled,QIcon::Off);
        this->setIcon(iconClear);
        m_icon = CLEAR;
        m_role = CLEAR;
        break;
    }
    case BOMB: {
        QIcon iconBomb(":/unclicked.png");
        iconBomb.addFile(":/B.png", QSize(64,64),QIcon::Disabled,QIcon::Off);
        this->setIcon(iconBomb);
        m_icon = BOMB;
        m_role = BOMB;
        break;
    }
    case ONE: {
        QIcon iconOne(":/unclicked.png");
        iconOne.addFile(":/1.png", QSize(64,64),QIcon::Disabled,QIcon::Off);
        this->setIcon(iconOne);
        m_icon = ONE;
        m_role = ONE;
        break;
    }
    case TWO: {
        QIcon iconTwo(":/unclicked.png");
        iconTwo.addFile(":/2.png", QSize(64,64),QIcon::Disabled,QIcon::Off);
        this->setIcon(iconTwo);
        m_icon = TWO;
        m_role = TWO;
        break;
    }
    case THREE: {
        QIcon iconThree(":/unclicked.png");
        iconThree.addFile(":/3.png", QSize(64,64),QIcon::Disabled,QIcon::Off);
        this->setIcon(iconThree);
        m_icon = THREE;
        m_role = THREE;
        break;
    }
    case FOUR: {
        QIcon iconFour(":/unclicked.png");
        iconFour.addFile(":/4.png", QSize(64,64),QIcon::Disabled,QIcon::Off);
        this->setIcon(iconFour);
        m_icon = FOUR;
        m_role = FOUR;
        break;
    }
    case FIVE: {
        QIcon iconFive(":/unclicked.png");
        iconFive.addFile(":/5.png", QSize(64,64),QIcon::Disabled,QIcon::Off);
        this->setIcon(iconFive);
        m_icon = FIVE;
        m_role = FIVE;
        break;
    }
    case SIX: {
        QIcon iconSix(":/unclicked.png");
        iconSix.addFile(":/6.png", QSize(64,64),QIcon::Disabled,QIcon::Off);
        this->setIcon(iconSix);
        m_icon = SIX;
        m_role = SIX;
        break;
    }
    case FLAG: {
        QIcon iconFlag(":/F.png");
        this->setIcon(iconFlag);
        m_icon = FLAG;
        break;
    }
    }
}

bool CustomPushButton::isMine() const
{
    return m_isMine;
}

void CustomPushButton::setIsMine(bool newIsMine)
{
    m_isMine = newIsMine;
}

//function, that evaluates how many mines are nearby and then assigns the specific icon
void CustomPushButton::evaluateNeighbours()
{
    if(m_isMine) return;
    int counter = 0;
    for(CustomPushButton* neighbour : *m_neighbours) {
        if(neighbour->isMine()) {
            counter++;
        }
    }
    switch(counter) {
    case 1: {
        setCustomIcon(ONE);
        break;
    }
    case 2: {
        setCustomIcon(TWO);
        break;
    }
    case 3: {
        setCustomIcon(THREE);
        break;
    }
    case 4: {
        setCustomIcon(FOUR);
        break;
    }
    case 5: {
        setCustomIcon(FIVE);
        break;
    }
    case 6: {
        setCustomIcon(SIX);
        break;
    }
    }
}

//function to add a neighbour button to the list
void CustomPushButton::addNeighbour(CustomPushButton *neighbour)
{
    m_neighbours->insert(neighbour);
}

//function, that disables all neighbours, used for clear buttons to open up upto a number button
void CustomPushButton::disableNeighbours()
{
    for(CustomPushButton* neighbour : *m_neighbours) {
        if(neighbour->icon() != BOMB && neighbour->icon() != FLAG && neighbour->isEnabled()){
            neighbour->setEnabled(false);
            if(neighbour->icon() == CLEAR) {
                neighbour->disableNeighbours();
            }
        }
    }
}

CustomPushButton::Icon CustomPushButton::icon() const
{
    return m_icon;
}

CustomPushButton::Icon CustomPushButton::role() const
{
    return m_role;
}

void CustomPushButton::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::RightButton) {
        emit customContextMenuRequested(QPoint());
    } else if(event->button() == Qt::LeftButton) {
        if(m_icon == FLAG) return;
        if(isEnabled()) {
            setEnabled(false);
        } else {
            setEnabled(true);
        }
        emit clicked();
    }
}
