#include "sector.h"

#include <QMouseEvent>
#include <QPainter>
#include <QImage>

Sector::Sector(const size_t x, const size_t y, QWidget *parent)
    : QWidget{parent}
    , m_x{x}
    , m_y{y}
{

}

void Sector::mouseReleaseEvent(QMouseEvent *event)
{
    if(event)
    {
        if(event->button() == Qt::LeftButton)
        {
            emit left_click(m_x, m_y);
        }
        if(event->button() == Qt::RightButton)
        {
            emit right_click(m_x, m_y);
        }
    }
}

void Sector::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    QImage spriteshit("D:\\cpp_advanced_05_21\\examples\\minesweeper\\data\\spriteshit.png");

    int row = 3;
    int column = 3;
    switch (m_state) {
    case State::Zero:
        row = 0; column = 0;
        break;
    case State::One:
        row = 0; column = 1;
        break;
    case State::Two:
        row = 0; column = 2;
        break;
    case State::Three:
        row = 0; column = 3;
        break;
    case State::Four:
        row = 1; column = 0;
        break;
    case State::Five:
        row = 1; column = 1;
        break;
    case State::Six:
        row = 1; column = 2;
        break;
    case State::Seven:
        row = 1; column = 3;
        break;
    case State::Eight:
        row = 2; column = 0;
        break;
    case State::Close:
        row = 2; column = 1;
        break;
    case State::MinaRed:
        row = 2; column = 2;
        break;
    case State::Flag:
        row = 2; column = 3;
        break;
    case State::MinaDeactivated:
        row = 3; column = 0;
        break;
    case State::Question:
        row = 3; column = 1;
        break;
    case State::Mina:
        row = 3; column = 2;
        break;
    }

    QRect target(0, 0, width(), height());
    const int spriteWidth = spriteshit.width() / 4;
    const int spriteHeight = spriteshit.height() / 4;
    QRect source(column * spriteWidth, row * spriteHeight, spriteWidth, spriteHeight);
    painter.drawImage(target, spriteshit, source);
}

void Sector::set_state(const State val)
{
    m_state = val;
    update();
}
