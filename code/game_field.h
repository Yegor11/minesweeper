#ifndef GAMEFIELD_H
#define GAMEFIELD_H

#include <QWidget>
#include <QGridLayout>
#include <vector>
#include <memory>
#include "sector.h"
#include <functional>

class GameField : public QWidget
{
    Q_OBJECT
    using SectorPtr = std::unique_ptr<Sector>;
public:
    GameField(const size_t height, const size_t width, QWidget *parent = nullptr);
    void new_game();

signals:

private slots:
    void left_click(const size_t x, const size_t y);
    void right_click(const size_t x, const size_t y);

private:
    void boooom(const size_t x, const size_t y);
    bool win();

    void GameOver(bool win);

    void for_each_sector_near(const size_t x, const size_t y, std::function<void(SectorPtr&)> foo);
    QGridLayout m_layout;
    std::vector<std::vector<SectorPtr>> m_sectors;
    size_t m_height;
    size_t m_width;
};

#endif // GAMEFIELD_H
