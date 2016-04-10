#ifndef FIELD_HPP
#define FIELD_HPP

#include <QVector>
#include <QObject>

class Cell;
class QGraphicsRectItem;

class Field: public QObject
{
    Q_OBJECT
public:
    enum State
    {
        StateIdle,
        StateStarted,
        StateEnded
    };

    Field();


    void setSize(int width, int height);
    void setNumberOfMines(int number);

    void generate(int x, int y);
    void prepare();
    bool isGenerated();
    void lose();

    int width() const { return m_width; }
    int height() const { return m_height; }
    int numberOfMines() const { return m_numberOfMines; }
    int numberOfFlags() const { return m_numberOfFlags; }
    State state() const { return m_state; }

    Cell *cellAt(int x, int y) const;

signals:
    void stateChanged(State newState);
    void numberOfFlagsChanged(int newNumbOfFlags);

protected slots:
    void onCellOpened(int x, int y);
    void onCellMarkChanged();

private:
    QVector<Cell*> m_cells;

    int m_width;
    int m_height;

    int m_numberOfMines;
    int m_numberOfFlags;
    int m_numberOfOpenedCells;
    bool m_generated;
    QGraphicsRectItem *m_fieldItem;

    State m_state;

    void win();
    void setState(State name);
};

#endif // FIELD_HPP
