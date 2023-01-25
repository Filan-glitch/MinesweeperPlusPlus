#include "custompushbutton.h"

CustomPushButton::CustomPushButton(QWidget *parent) : QPushButton(parent)
{
    m_neighbours = new QVector<CustomPushButton*>;
}

//function that sets the icon of the button and edits every important properties
void CustomPushButton::setCustomIcon(Icon icon)
{
    switch(icon) {
    case CLEAR: {
        QIcon iconClear(":/ressources/unclicked.png");
        iconClear.addFile(":/ressources/clear.png", QSize(64,64),QIcon::Disabled,QIcon::Off);
        this->setIcon(iconClear);
        m_icon = CLEAR;
        m_role = CLEAR;
        break;
    }
    case BOMB: {
        QIcon iconBomb(":/ressources/unclicked.png");
        iconBomb.addFile(":/ressources/B.png", QSize(64,64),QIcon::Disabled,QIcon::Off);
        this->setIcon(iconBomb);
        m_icon = BOMB;
        m_role = BOMB;
        break;
    }
    case ONE: {
        QIcon iconOne(":/ressources/unclicked.png");
        iconOne.addFile(":/ressources/1.png", QSize(64,64),QIcon::Disabled,QIcon::Off);
        this->setIcon(iconOne);
        m_icon = ONE;
        m_role = ONE;
        break;
    }
    case TWO: {
        QIcon iconTwo(":/ressources/unclicked.png");
        iconTwo.addFile(":/ressources/2.png", QSize(64,64),QIcon::Disabled,QIcon::Off);
        this->setIcon(iconTwo);
        m_icon = TWO;
        m_role = TWO;
        break;
    }
    case THREE: {
        QIcon iconThree(":/ressources/unclicked.png");
        iconThree.addFile(":/ressources/3.png", QSize(64,64),QIcon::Disabled,QIcon::Off);
        this->setIcon(iconThree);
        m_icon = THREE;
        m_role = THREE;
        break;
    }
    case FOUR: {
        QIcon iconFour(":/ressources/unclicked.png");
        iconFour.addFile(":/ressources/4.png", QSize(64,64),QIcon::Disabled,QIcon::Off);
        this->setIcon(iconFour);
        m_icon = FOUR;
        m_role = FOUR;
        break;
    }
    case FIVE: {
        QIcon iconFive(":/ressources/unclicked.png");
        iconFive.addFile(":/ressources/5.png", QSize(64,64),QIcon::Disabled,QIcon::Off);
        this->setIcon(iconFive);
        m_icon = FIVE;
        m_role = FIVE;
        break;
    }
    case SIX: {
        QIcon iconSix(":/ressources/unclicked.png");
        iconSix.addFile(":/ressources/6.png", QSize(64,64),QIcon::Disabled,QIcon::Off);
        this->setIcon(iconSix);
        m_icon = SIX;
        m_role = SIX;
        break;
    }
    case SEVEN: {
        QIcon iconSeven(":/ressources/unclicked.png");
        iconSeven.addFile(":/ressources/7.png", QSize(64,64),QIcon::Disabled,QIcon::Off);
        this->setIcon(iconSeven);
        m_icon = SEVEN;
        m_role = SEVEN;
        break;
    }
    case EIGHT: {
        QIcon iconEight(":/ressources/unclicked.png");
        iconEight.addFile(":/ressources/8.png", QSize(64,64),QIcon::Disabled,QIcon::Off);
        this->setIcon(iconEight);
        m_icon = EIGHT;
        m_role = EIGHT;
        break;
    }
    case FLAG: {
        QIcon iconFlag(":/ressources/F.png");
        this->setIcon(iconFlag);
        m_icon = FLAG;
        break;
    }
    default: {
        QIcon iconFlag(":/ressources/clear.png");
    }
    }
}

//getter for the mine
bool CustomPushButton::isMine() const
{
    return m_isMine;
}

//set for the mine
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
    case 7: {
        setCustomIcon(SEVEN);
        break;
    }
    case 8: {
        setCustomIcon(EIGHT);
        break;
    }
    }
}

//function to add a neighbour button to the list
void CustomPushButton::addNeighbour(CustomPushButton *neighbour)
{
    m_neighbours->push_back(neighbour);
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

//getter for the icon
CustomPushButton::Icon CustomPushButton::icon() const
{
    return m_icon;
}

//getter for the role
CustomPushButton::Icon CustomPushButton::role() const
{
    return m_role;
}

//overridden function to check right click event to set flags
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

//function that swaps specific icons to confuse
void CustomPushButton::confuse() {
    switch(m_role) {
    case ONE:
        setCustomIcon(EIGHT);
        break;
    case TWO:
        setCustomIcon(SEVEN);
        break;
    case THREE:
        setCustomIcon(SIX);
        break;
    case FIVE:
        setCustomIcon(FOUR);
        break;
    case SIX:
        setCustomIcon(THREE);
        break;
    case SEVEN:
        setCustomIcon(TWO);
        break;
    case EIGHT:
        setCustomIcon(ONE);
        break;
    default:
        break;
    }
}
