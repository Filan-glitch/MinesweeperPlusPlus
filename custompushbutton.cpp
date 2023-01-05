#include "custompushbutton.h"

CustomPushButton::CustomPushButton()
{

}

//function that sets an icon
void CustomPushButton::setIcon(Icon icon)
{
    switch(icon) {
    case CLEAR: {
        QIcon iconClear(":/unclicked.png");
        iconClear.addFile(":/clear.png", QSize(64,64),QIcon::Disabled,QIcon::Off);
        m_button->setIcon(iconClear);
        m_icon = CLEAR;
        m_role = CLEAR;
        break;
    }
    case BOMB: {
        QIcon iconBomb(":/unclicked.png");
        iconBomb.addFile(":/B.png", QSize(64,64),QIcon::Disabled,QIcon::Off);
        m_button->setIcon(iconBomb);
        m_icon = BOMB;
        m_role = BOMB;
        break;
    }
    case ONE: {
        QIcon iconOne(":/unclicked.png");
        iconOne.addFile(":/1.png", QSize(64,64),QIcon::Disabled,QIcon::Off);
        m_button->setIcon(iconOne);
        m_icon = ONE;
        m_role = ONE;
        break;
    }
    case TWO: {
        QIcon iconTwo(":/unclicked.png");
        iconTwo.addFile(":/2.png", QSize(64,64),QIcon::Disabled,QIcon::Off);
        m_button->setIcon(iconTwo);
        m_icon = TWO;
        m_role = TWO;
        break;
    }
    case THREE: {
        QIcon iconThree(":/unclicked.png");
        iconThree.addFile(":/3.png", QSize(64,64),QIcon::Disabled,QIcon::Off);
        m_button->setIcon(iconThree);
        m_icon = THREE;
        m_role = THREE;
        break;
    }
    case FOUR: {
        QIcon iconFour(":/unclicked.png");
        iconFour.addFile(":/4.png", QSize(64,64),QIcon::Disabled,QIcon::Off);
        m_button->setIcon(iconFour);
        m_icon = FOUR;
        m_role = FOUR;
        break;
    }
    case FIVE: {
        QIcon iconFive(":/unclicked.png");
        iconFive.addFile(":/5.png", QSize(64,64),QIcon::Disabled,QIcon::Off);
        m_button->setIcon(iconFive);
        m_icon = FIVE;
        m_role = FIVE;
        break;
    }
    case SIX: {
        QIcon iconSix(":/unclicked.png");
        iconSix.addFile(":/6.png", QSize(64,64),QIcon::Disabled,QIcon::Off);
        m_button->setIcon(iconSix);
        m_icon = SIX;
        m_role = SIX;
        break;
    }
    case FLAG: {
        QIcon iconFlag(":/F.png");
        m_button->setIcon(iconFlag);
        m_icon = FLAG;
        break;
    }
    }
}

QPushButton *CustomPushButton::button() const
{
    return m_button;
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
        setIcon(ONE);
        break;
    }
    case 2: {
        setIcon(TWO);
        break;
    }
    case 3: {
        setIcon(THREE);
        break;
    }
    case 4: {
        setIcon(FOUR);
        break;
    }
    case 5: {
        setIcon(FIVE);
        break;
    }
    case 6: {
        setIcon(SIX);
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
        if(neighbour->icon() != BOMB && neighbour->icon() != FLAG && neighbour->button()->isEnabled()){
            neighbour->button()->setEnabled(false);
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


//Constructor
CustomPushButton::CustomPushButton(QPushButton *button) : m_button(button)
{
    m_neighbours = new QSet<CustomPushButton*>;
}
