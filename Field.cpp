#include "Field.hpp"

#include "Cell.hpp"

#include <QDebug>

Field::Field():QObject()
{
   m_numberOfOpenedCells = 0;
   m_state = StateIdle;
   emit stateChanged(m_state);
}

void Field::setSize(int width, int height)
{
    setState(StateIdle);
    m_width = width;
    m_height = height;

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            Cell *cell = new Cell(x, y);
            connect(cell, SIGNAL(opened(int,int)), this, SLOT(onCellOpened(int,int)));
            connect(cell, SIGNAL(markChanged(Mark)), this, SLOT(onCellMarkChanged()));
            m_cells.append(cell);
        }
    }
}

void Field::setNumberOfMines(int number)
{
    m_numberOfMines = number;
}

void Field::generate(int x, int y)
{
    setState(StateStarted);
    Cell *banned = cellAt(x, y);
    QVector<Cell*> bannedCells = banned->getNeighbors();
    bannedCells.append(banned);
    qsrand(10);
    int minesToPlace = m_numberOfMines;

    while (minesToPlace > 0)
    {
        Cell *cell = m_cells.at(qrand() % m_cells.count());

        if (cell->haveMine())
        {
               continue;
        }
        if (bannedCells.contains(cell))
        {
            continue;
        }

        cell->setHaveMine(true);
        --minesToPlace;
    }
    m_generated = true;
}


Cell *Field::cellAt(int x, int y) const
{
    if (x < 0 || x >= m_width) {
        return 0;
    }
    if (y < 0 || y >= m_height) {
        return 0;
    }

    return m_cells.at(x + y * m_width);
}

void Field::onCellOpened(int x, int y)
{
    ++m_numberOfOpenedCells;
    if (cellAt(x, y)->haveMine())
    {
        lose();
        setState(StateEnded);
        emit stateChanged(m_state);
    }

    if (m_numberOfOpenedCells == m_cells.count() - m_numberOfMines)
    {
        win();
        setState(StateEnded);
        emit stateChanged(m_state);
    }

    if (!isGenerated())
    {
        generate(x, y);
    }
}

bool Field::isGenerated()
{
    return m_generated;
}

void Field::lose()
{
    foreach (Cell *cell, m_cells)
    {
        cell->open();
    }
}

void Field::win()
{
    qDebug() << "Win!";
}

void maybeAddCell(QVector<Cell*> *vector, Cell *cell)
{
    if (cell) {
        vector->append(cell);
    }
}

void Field::prepare()
{
    foreach (Cell *cell, m_cells)
    {
        cell->reset();

        QVector<Cell*> neighbors;
        for (int x = cell->x() - 1; x <= cell->x() + 1; ++x)
        {
            maybeAddCell(&neighbors, cellAt(x, cell->y() - 1));
            maybeAddCell(&neighbors, cellAt(x, cell->y() + 1));
        }
            maybeAddCell(&neighbors, cellAt(cell->x() - 1, cell->y()));
            maybeAddCell(&neighbors, cellAt(cell->x() + 1, cell->y()));

            cell->setNeighbors(neighbors);
    }
    m_numberOfOpenedCells = 0;
    m_generated = false;
    m_numberOfFlags = 0;
}

void Field::onCellMarkChanged()
{
    int mark = 0;
    foreach (Cell *cell, m_cells)
    {
        if (cell->isMarked())
        {
            ++mark;
        }
    }
    m_numberOfFlags = mark;
    emit numberOfFlagsChanged(m_numberOfFlags);
}

void Field::setState(State name)
{
    m_state = name;
}
