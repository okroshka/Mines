#include "Cell.hpp"

#include "Field.hpp"



Cell::Cell(int x, int y):QObject()
{
    m_x = x;
    m_y = y;

    m_haveMine = false;
    m_open = false;
    m_mark = MarkNothing;
    m_marked = false;
    emit markChanged(m_mark);
}

int Cell::minesAround() const
{
    int mines = 0;
    for (Cell *cell : getNeighbors()) {
        if (cell->haveMine()) {
            ++mines;
        }
    }

    return mines;
}

void Cell::setHaveMine(bool haveMine)
{
    m_haveMine = haveMine;
}

void Cell::open()
{
    if (m_open)
    {
        return;
    }
    m_open = true;

    emit opened(x(), y());

    if (minesAround() == 0)
    {
        for (Cell *cell : getNeighbors())
        {
            cell->open();
        }
    }

}

void Cell::reset()
{
    m_open = false;
    m_haveMine = false;
    m_mark = MarkNothing;
    emit markChanged(m_mark);
}

void Cell::toggleMark()
{
    switch (m_mark)
    {
        case MarkNothing:
            m_mark = MarkFlagged;
            m_marked = true;
            break;
        case MarkFlagged:
            m_mark = MarkQuestioned;
            m_marked = false;
           break;
        case MarkQuestioned:
            m_mark = MarkNothing;
            break;
    }
    emit markChanged(m_mark);
}

void Cell::tryToOpenAround()
{
    int excl = 0;
    int mines = minesAround();

    for (Cell *cell : getNeighbors()) // тут ищем поля по вектору, где стоят воскл.знаки
    {
        if (cell->mark() == MarkFlagged)
        {
            ++excl;
        }
    }

    if (mines == excl) // сравниваем значения
    {
        for (Cell *cell : getNeighbors())
        {
            if (cell->mark() != MarkFlagged) // если на поле отметка НЕ воскл.знак
            {
                cell->open(); // открываем
            }
        }
    }
}

void Cell::setNeighbors(const QVector<Cell *> &neighbors)
{
    m_neighbors = neighbors;
}
