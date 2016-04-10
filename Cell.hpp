#ifndef CELL_HPP
#define CELL_HPP

#include <QVector>
#include <QObject>

class Field;

class Cell: public QObject
{
    Q_OBJECT
public:
    enum Mark
    {
        MarkNothing,
        MarkFlagged,
        MarkQuestioned
    };

    Cell(int x, int y);

    int x() const { return m_x; }
    int y() const { return m_y; }

    int minesAround() const;

    bool haveMine() const { return m_haveMine; }
    bool isMarked() const { return m_marked; }
    void setHaveMine(bool haveMine);

    bool isOpen() const { return m_open; }
    void open();
    void reset();
    Mark mark() const { return m_mark; }
    void toggleMark();
    void tryToOpenAround();

    QVector<Cell *> getNeighbors() const { return m_neighbors; }
    void setNeighbors(const QVector<Cell*> &neighbors);
    signals:
        void opened(int x, int y);
        void markChanged(Mark newMark);

private:
    int m_x;
    int m_y;
    Mark m_mark;
    QVector<Cell*> m_neighbors;

    bool m_haveMine;
    bool m_open;
    bool m_marked;
};

#endif // CELL_HPP
