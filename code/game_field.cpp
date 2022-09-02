#include "game_field.h"
#include <QMessageBox>
#include <QApplication>

GameField::GameField(const size_t height, const size_t width, QWidget *parent)
    : QWidget{parent}
    , m_height{height}
    , m_width{width}
{
    m_sectors.resize(m_height);

    for(size_t i = 0; i < m_height; ++i)
    {
        m_sectors[i].reserve(m_width);
        for(size_t j = 0; j < m_width; ++j)
        {
            m_sectors[i].push_back(std::make_unique<Sector>(j, i));
            m_layout.addWidget(m_sectors[i][j].get(), i, j);
            QObject::connect(m_sectors[i][j].get(), &Sector::left_click, this, &GameField::left_click);
            QObject::connect(m_sectors[i][j].get(), &Sector::right_click, this, &GameField::right_click);
        }
    }

    setLayout(&m_layout);
    setFixedSize(m_width * 25, m_height * 25);
    m_layout.setSpacing(0);
    m_layout.setContentsMargins(0, 0, 0, 0);
    new_game();
}

void GameField::new_game()
{
    for(size_t i = 0; i < m_height; ++i)
    {
        for(size_t j = 0; j < m_width; ++j)
        {
            m_sectors[i][j]->set_state(Sector::State::Close);
            m_sectors[i][j]->set_count(rand() % 10 == 0 ? -1 : 0);

            if(m_sectors[i][j]->count() == -1)
            {
                m_sectors[i][j]->set_state(Sector::State::Question);
            }

        }
    }

    for(size_t i = 0; i < m_height; ++i)
    {
        for(size_t j = 0; j < m_width; ++j)
        {
            if(m_sectors[i][j]->count() == 0)
            {
                for_each_sector_near(j, i,
                    [&center = m_sectors[i][j]](SectorPtr& sector)
                    {
                        if(sector->count() == -1)
                        {
                            center->set_count(center->count() + 1);
                        }
                    }
                );
            }
        }
    }
}

void GameField::left_click(const size_t x, const size_t y)
{
    switch(m_sectors[y][x]->state())
    {
    case Sector::State::Close:
    case Sector::State::Question:
        if(m_sectors[y][x]->count() == -1)
        {
            boooom(x, y);
        }
        else
        {
            m_sectors[y][x]->set_state(static_cast<Sector::State>(m_sectors[y][x]->count()));
            if(m_sectors[y][x]->count() == 0)
            {
                for_each_sector_near(x, y,
                    [this](SectorPtr& sector)
                    {
                        left_click(sector->x(), sector->y());
                    }
                );
            }
        }
        if(win())
        {
            GameOver(true);
        }
        break;
    default:
        break;
    }
}

void GameField::right_click(const size_t x, const size_t y)
{
    switch(m_sectors[y][x]->state())
    {
    case Sector::State::Close:
        m_sectors[y][x]->set_state(Sector::State::Flag);
        break;
    case Sector::State::Flag:
        m_sectors[y][x]->set_state(Sector::State::Question);
        break;
    case Sector::State::Question:
        m_sectors[y][x]->set_state(Sector::State::Close);
        break;
    default:
        break;
    }
}

void GameField::for_each_sector_near(const size_t x, const size_t y, std::function<void(SectorPtr&)> foo)
{
    const std::vector<std::pair<int, int>> shifts =
        {
            {-1, -1},
            {0, -1},
            {1, -1},
            {1, 0},
            {1, 1},
            {0, 1},
            {-1, 1},
            {-1, 0},
        };

        for(const auto& shift : shifts )
        {
            const int shiftX = x + shift.first;
            const int shiftY = y + shift.second;

            if(shiftY >= 0 && shiftY < m_height
               && shiftX >= 0 && shiftX < m_width)
            {
                foo(m_sectors[shiftY][shiftX]);
            }
        }
}

void GameField::boooom(const size_t x, const size_t y)
{
    for(size_t i = 0; i < m_height; ++i)
    {
        for(size_t j = 0; j < m_width; ++j)
        {
            if(m_sectors[i][j]->count() == -1)
            {
                if(m_sectors[i][j]->state() == Sector::State::Flag)
                {
                    m_sectors[i][j]->set_state(Sector::State::MinaDeactivated);
                }
                else
                {
                    m_sectors[i][j]->set_state(Sector::State::Mina);
                }
            }
        }
    }
    m_sectors[y][x]->set_state(Sector::State::MinaRed);
    GameOver(false);
}

bool GameField::win()
{
    for(size_t i = 0; i < m_height; ++i)
    {
        for(size_t j = 0; j < m_width; ++j)
        {
            if(m_sectors[i][j]->count() != -1)
            {
                switch (m_sectors[i][j]->state()) {
                    case Sector::State::Close:
                    case Sector::State::Flag:
                    case Sector::State::Question:
                            return false;
                }
            }
        }
    }
    return true;
}

void GameField::GameOver(bool win)
{
    const QString status = win ? "won" : "lose";
    QMessageBox msgBox;
    msgBox.setText("You " + status + "! Play again?");
    msgBox.addButton("Yes", QMessageBox::ButtonRole::AcceptRole);
    msgBox.addButton("No", QMessageBox::ButtonRole::RejectRole);
    int result = msgBox.exec();
    if(result == QMessageBox::ButtonRole::AcceptRole)
    {
        new_game();
    }
    else
    {
        QApplication::exit(0);
    }
}
