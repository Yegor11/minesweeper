#ifndef SECTOR_H
#define SECTOR_H

#include <QWidget>

class Sector : public QWidget
{
    Q_OBJECT
public:
    enum class State
    {
        Zero = 0,
        One,
        Two,
        Three,
        Four,
        Five,
        Six,
        Seven,
        Eight,
        Close,
        MinaRed,
        Flag,
        MinaDeactivated,
        Question,
        Mina
    };

    Sector(const size_t x, const size_t y, QWidget *parent = nullptr);

    size_t x() { return m_x; }
    size_t y() { return m_y; }
    State state() { return m_state; }
    void set_state(const State val);

    int count() { return m_count; }
    void set_count(int val) { m_count = val; }

signals:
    void left_click(const size_t x, const size_t y);
    void right_click(const size_t x, const size_t y);

private:
    void mouseReleaseEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent*) override;

    int m_count;
    State m_state;
    size_t m_x;
    size_t m_y;
};

#endif // SECTOR_H
