#ifndef MINESWEEPER_H
#define MINESWEEPER_H

#include <QWidget>

class Minesweeper : public QWidget
{
    Q_OBJECT

public:
    Minesweeper(QWidget *parent = nullptr);
    ~Minesweeper();
};
#endif // MINESWEEPER_H
