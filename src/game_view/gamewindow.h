#pragma once

#include <QMainWindow>
#include <QDialog>
#include <QVector>
#include <QPoint>
#include <QPainter>
#include <QMouseEvent>

#include <random>
#include "constants.h"

#define DRAW_MARK(mark) (mark == Zero) ? drawZero(painter, markRect) : drawCross(painter, markRect)

namespace Ui {
    class GameWindow;
}

class GameWindow : public QMainWindow
{
    Q_OBJECT

private:
    int countCells;
    int menuHeight;

    Mark nextMove;
    QVector<QVector<Mark>> field;
    QVector<QPoint> winCells;

    QVector<QPoint> moves;
    int moveIndex;

    bool gameOver;

    Ui::GameWindow *ui;
    QDialog *menuWindow;

    QPoint coordsToCells(QPoint coords) const;

    void drawCell(QPainter &painter, int row, int col) const;
    void drawWinCell(QPainter &painter, int row, int col) const;

    void drawMark(QPainter &painter, Mark mark, QRect &rect) const;
    void drawCross(QPainter &painter, QRect &rect) const;
    void drawZero(QPainter &painter, QRect &rect) const;
    void setPen(QPainter &painter, const QColor &color, int width) const;

    bool isFinalMove(QPoint &move);
    bool isFinalHorMove(int row, int col, Mark mark);
    bool isFinalVerMove(int row, int col, Mark mark);
    bool isFinalMainMove(int row, int col, Mark mark);
    bool isFinalAddMove(int row, int col, Mark mark);

    void setWindowSize(Ui::GameWindow *ui, int countCells);
    void initGameFields(Ui::GameWindow *ui, int countCells);
    void setStatusOfMove(const QString &prefix);
    int randint(int start, int end);

    void connectMenu(Ui::GameWindow *ui);

public:
    explicit GameWindow(int countCells, QDialog *parent = nullptr);
    ~GameWindow();

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private slots:
    void cancelMove();
    void restartGame();
    void moveToMenu();
};