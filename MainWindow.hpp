#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class QGraphicsScene;
class Field;
class QGraphicsSimpleTextItem;
class QGraphicsRectItem;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void newGame();

protected:
    void resizeEvent(QResizeEvent *event);

protected slots:
    void updateSceneScale();
    void onFieldNumberOfFlagsChanged(int number);
    void onFieldStateChanged();

private slots:
    void on_actionNewGame_triggered();

private:
    Ui::MainWindow *ui;
    QGraphicsScene *m_scene;
    Field *m_field;
    QGraphicsSimpleTextItem *m_gameStateText;
    QGraphicsRectItem *m_gameStateRect;
};

#endif // MAINWINDOW_H
