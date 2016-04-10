#include "MainWindow.hpp"
#include "ui_MainWindow.h"

#include "Field.hpp"

#include "CellItem.hpp"

#include <QGLWidget>
#include <QGraphicsScene>
#include <QTimer>

static const int fieldBorderWidth = CellItem::cellSize / 2;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_scene = new QGraphicsScene();
    m_gameStateText = new QGraphicsSimpleTextItem();
    QFont g = m_gameStateText->font();
    g.setPixelSize(CellItem::cellSize - 4);
    m_gameStateText->setFont(g);
    m_gameStateText->setZValue(2);
    m_scene->addItem(m_gameStateText);
    m_field = new Field();
    m_field->setSize(8, 8);
    m_field->setNumberOfMines(10);
    m_gameStateRect->setZValue(1);

    QGLFormat f = QGLFormat::defaultFormat();
    f.setSampleBuffers(true);
    f.setSamples(4);

    ui->graphicsView->setViewport(new QGLWidget(f));
    ui->graphicsView->setScene(m_scene);

    for (int y = 0; y < m_field->height(); ++y) {
        for (int x = 0; x < m_field->width(); ++x) {
            m_scene->addItem(new CellItem(m_field->cellAt(x, y)));
        }
    }
    newGame();
    connect(m_field, SIGNAL(numberOfFlagsChanged(int)), this, SLOT(onFieldNumberOfFlagsChanged(int)));
    connect(m_field, SIGNAL(stateChanged(State)), this, SLOT(onFieldStateChanged()));
}

MainWindow::~MainWindow()
{
    delete ui;
    MainWindow::newGame();
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QTimer::singleShot(0, this, SLOT(updateSceneScale()));
}

void MainWindow::updateSceneScale()
{
    ui->graphicsView->fitInView(m_scene->sceneRect(), Qt::KeepAspectRatio);
}

void MainWindow::newGame()
{
    m_field->prepare();
    m_scene->update();
    m_gameStateText->setText("");
}

void MainWindow::on_actionNewGame_triggered()
{
    MainWindow::newGame();
}

void MainWindow::onFieldNumberOfFlagsChanged(int number)
{
    ui->minesLabel->setText(QString("%1/%2").arg(number).arg(m_field->numberOfMines()));
}

void MainWindow::onFieldStateChanged()
{
    if (m_field->state() == Field::StateEnded) {
        m_gameStateText->setText("Game over");
        m_gameStateText->setPos((m_scene->width() - m_gameStateText->boundingRect().width()) / 2,
                               (m_scene->height() - m_gameStateText->boundingRect().height()) / 2);
        m_gameStateText->setVisible(true);
    } else {
        m_gameStateText->setVisible(false);
    }
}
