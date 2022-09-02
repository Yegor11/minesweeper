#include "minesweeper.h"

#include <QApplication>
#include "game_field.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    GameField w(20,40);
    w.show();
    return a.exec();
}
