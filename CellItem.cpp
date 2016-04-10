#include "CellItem.hpp"

#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSimpleTextItem>
#include <QPainter>

#include "Cell.hpp"

const int CellItem::cellSize = 32;

CellItem::CellItem(Cell *cell, QGraphicsItem *parent) :
    QGraphicsItem(parent)
{
    m_cell = cell;
    m_text = new QGraphicsSimpleTextItem(this);

    if (m_cell->isOpen())
    {
       int numbMine = m_cell->minesAround();
       m_text->setText(QString::number(numbMine));
    }
    else
    {
        QFont f = m_text->font();
        f.setPixelSize(cellSize - 4);
        m_text->setFont(f);
    }
}

QRectF CellItem::boundingRect() const
{
    return QRectF(0, 0, cellSize, cellSize);
}

void CellItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    //painter->setRenderHint(QPainter::Antialiasing);
    //painter->setRenderHint(QPainter::HighQualityAntialiasing);

    painter->drawRect(0, 0, cellSize, cellSize);

    static const int border = 4;

    if (m_cell->isOpen())
    {
      if (m_cell->haveMine())
      {
         m_text->setText("+");
         painter->fillRect(border, border, cellSize - border * 2, cellSize - border * 2, Qt::red);
      }
      else
      {
          if (m_cell->minesAround() > 0)
          {
              m_text->setText(QString::number(m_cell->minesAround()));
          }
      }
    }
    else
    {
        painter->fillRect(border, border, cellSize - border * 2, cellSize - border * 2, Qt::lightGray);
        m_text->setText(QString());
    }

    switch (m_cell->mark())
    {
        case Cell::MarkNothing:
           //m_text->setText("");
           break;
        case Cell::MarkFlagged:
           m_text->setText("!");
           break;
        case Cell::MarkQuestioned:
           m_text->setText("?");
           break;
    }

    /*if(m_cell->mark() == m_cell->MarkFlagged)
    {
        m_text->setText("!");
    }
    if(m_cell->mark() == m_cell->MarkQuestioned)
    {
        m_text->setText("?");
    }*/
    m_text->setPos((cellSize - m_text->boundingRect().width()) / 2, (cellSize - m_text->boundingRect().height()) / 2);
}

void CellItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    event->accept();
}

void CellItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        if (m_cell->isOpen())
        {
            m_cell->tryToOpenAround();
        }
        else
        {
            m_cell->open();
        }
    }

    if (event->button() == Qt::RightButton)
    {
        m_cell->toggleMark();
    }

    update();
}
