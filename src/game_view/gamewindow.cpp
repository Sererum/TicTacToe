#include "gamewindow.h"
#include "ui_gamewindow.h"

QPoint GameWindow::coordsToCells(QPoint coords) const
{
    int x = coords.x(), y = coords.y() - menuHeight;
    return QPoint(x / SIZE_CELL, y / SIZE_CELL);
}

bool GameWindow::isFinalMove(QPoint &move)
{
    int row = move.y(), col = move.x();

    if (isFinalHorMove(row, col, nextMove))
        return true;

    if (isFinalVerMove(row, col, nextMove))
        return true;

    if (isFinalMainMove(row, col, nextMove))
        return true;

    if (isFinalAddMove(row, col, nextMove))
        return true;

    return false;
}

bool GameWindow::isFinalHorMove(int row, int col, Mark mark)
{
    int left = qMax(0, col - 4), right = qMin(countCells, col + 5);
    int count = 0;

    for (int i = left; i < right; ++i)
    {
        count += ((field[row][i] == mark) ? 1 : -count);

        if (count == 5)
        {
            for (int j = i; j > i - 5; --j)
                winCells[i - j] = std::move(QPoint(row, j));

            return true;
        }
    }
    return false;
}

bool GameWindow::isFinalVerMove(int row, int col, Mark mark)
{
    int up = qMax(0, row - 4), down = qMin(countCells, row + 5);
    int count = 0;

    for (int i = up; i < down; ++i)
    {
        count += ((field[i][col] == mark) ? 1 : -count);
        if (count == 5)
        {
            for (int j = i; j > i - 5; --j)
                winCells[i - j] = std::move(QPoint(j, col));

            return true;
        }
    }
    return false;
}

bool GameWindow::isFinalMainMove(int row, int col, Mark mark)
{
    int left = qMax(0, col - 4), right = qMin(countCells, col + 5);
    int up = qMax(0, row - 4), down = qMin(countCells, row + 5);
    int left_up = qMax(left - col, up - row), down_right = qMin(down - row, right - col);
    int count = 0;

    for (int i = left_up; i < down_right; ++i)
    {
        count += ((field[row + i][col + i] == mark) ? 1 : -count);
        if (count == 5)
        {
            for (int j = i; j > i - 5; --j)
                winCells[i - j] = std::move(QPoint(row + j, col + j));

            return true;
        }
    }
    return false;
}

bool GameWindow::isFinalAddMove(int row, int col, Mark mark)
{
    int left = qMax(0, col - 4), right = qMin(countCells, col + 5);
    int up = qMax(0, row - 4), down = qMin(countCells, row + 5);
    int down_left = qMax(left - col, row - down + 1), right_up = qMin(right - col, row - up + 1);
    int count = 0;

    for (int i = down_left; i < right_up; ++i)
    {
        count += ((field[row - i][col + i] == mark) ? 1 : -count);
        if (count == 5)
        {
            for (int j = i; j > i - 5; --j)
                winCells[i - j] = std::move(QPoint(row - j, col + j));

            return true;
        }
    }
    return false;
}

void GameWindow::setWindowSize(Ui::GameWindow *ui, int countCells)
{
    menuHeight = ui->menubar->height();
    int toolHeight = ui->statusBar->height();

    int sizeField = SIZE_CELL * countCells;
    setFixedSize(sizeField + 1, sizeField + menuHeight + toolHeight + 1);
}

void GameWindow::initGameFields(Ui::GameWindow *ui, int countCells)
{
    field = QVector<QVector<Mark>>(countCells, QVector<Mark>(countCells, None));
    winCells = QVector<QPoint>(WIN_LEN, QPoint(0, 0));

    moves.resize(countCells * countCells);
    moveIndex = -1;

    gameOver = false;

    nextMove = ((randint(0, 1)) ? Zero : Cross);
    setStatusOfMove("Первый ход: ");
}

void GameWindow::setStatusOfMove(const QString &prefix)
{
    QString mark = (nextMove == Zero) ? "нолики" : "крестики";
    ui->statusBar->showMessage(prefix + mark);
}

int GameWindow::randint(int start, int end)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(start, end);

    return dis(gen);
}

void GameWindow::connectMenu(Ui::GameWindow *ui)
{
    connect(ui->game_restart, &QAction::triggered, this, &GameWindow::restartGame);
    connect(ui->game_to_menu, &QAction::triggered, this, &GameWindow::moveToMenu);
    connect(ui->game_cancel, &QAction::triggered, this, &GameWindow::cancelMove);
}

GameWindow::GameWindow(int countCells, QDialog *parent)
    : QMainWindow(nullptr), menuWindow(parent), countCells(countCells), ui(new Ui::GameWindow)
{
    ui->setupUi(this);
    setWindowSize(ui, countCells);
    initGameFields(ui, countCells);
    connectMenu(ui);
}

GameWindow::~GameWindow() { delete ui; }

void GameWindow::mousePressEvent(QMouseEvent *event)
{
    if (event->button() != Qt::LeftButton || gameOver)
        return;

    QPoint pos = coordsToCells(event->pos());

    if (field[pos.y()][pos.x()] != None)
        return;

    field[pos.y()][pos.x()] = nextMove;
    moves[++moveIndex] = pos;
    gameOver = isFinalMove(pos);

    if (gameOver)
    {
        setStatusOfMove("Победитель: ");
    }
    else
    {
        nextMove = GET_NEXT_MARK(nextMove);
        setStatusOfMove("Следующий ход: ");
    }

    update();
}

void GameWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setBrush(BACK_COLOR);
    painter.setPen(FOREGROUND_COLOR);

    for (int row = 0; row < countCells; ++row)
    {
        for (int col = 0; col < countCells; ++col)
        {
            drawCell(painter, row, col);
        }
    }

    if (gameOver)
    {
        for (int i = 0; i < 5; ++i)
        {
            QPoint &winCell = winCells[i];
            int row = winCell.x(), col = winCell.y();
            drawWinCell(painter, row, col);
        }
    }
}

void GameWindow::drawCell(QPainter &painter, int row, int col) const
{
    QRect cell(col * SIZE_CELL, row * SIZE_CELL + menuHeight, SIZE_CELL, SIZE_CELL);
    painter.drawRect(cell);

    const Mark &mark = field[row][col];
    if (mark != None)
        drawMark(painter, mark, cell);
}

void GameWindow::drawWinCell(QPainter &painter, int row, int col) const
{
    painter.setPen(QPen(Qt::green));
    drawCell(painter, row, col);
    painter.setPen(QPen(FOREGROUND_COLOR));
}

void GameWindow::drawMark(QPainter &painter, Mark mark, QRect &rect) const
{
    int markSize = SIZE_CELL * 0.5;
    int shift = (SIZE_CELL - markSize) / 2;
    QRect markRect(rect.left() + shift, rect.top() + shift, markSize, markSize);

    const QPen oldPen = painter.pen();

    setPen(painter, BOARD_COLOR, 7);
    DRAW_MARK(mark);

    setPen(painter, oldPen.color(), 3);
    DRAW_MARK(mark);

    painter.setPen(oldPen);
}

void GameWindow::drawCross(QPainter &painter, QRect &rect) const
{
    painter.drawLine(rect.topLeft(), rect.bottomRight());
    painter.drawLine(rect.topRight(), rect.bottomLeft());
}

void GameWindow::drawZero(QPainter &painter, QRect &rect) const
{
    QPoint center = rect.center();
    int radius = rect.width() / 2;
    painter.drawEllipse(center, radius, radius);
}

void GameWindow::setPen(QPainter &painter, const QColor &color, int width) const
{
    QPen pen(color);
    pen.setWidth(width);
    painter.setPen(pen);
}

void GameWindow::cancelMove()
{
    if (gameOver || moveIndex < 0)
        return;
    
    QPoint &lastMove = moves[moveIndex--];
    field[lastMove.y()][lastMove.x()] = None;
    nextMove = GET_NEXT_MARK(nextMove);
    setStatusOfMove("Следующий ход: ");
    update();
}

void GameWindow::moveToMenu()
{
    menuWindow->show();
    destroy();
}

void GameWindow::restartGame()
{
    initGameFields(ui, countCells);
    update();
}