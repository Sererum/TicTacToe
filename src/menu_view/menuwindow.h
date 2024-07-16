#pragma once

#include <QWidget>
#include <QDialog>
#include <QLabel>
#include <QPixmap>
#include <QString>
#include <QMessageBox>

#include "game_view/gamewindow.h"
#include "constants.h"

namespace Ui
{
    class MenuWindow;
}

class MenuWindow : public QDialog
{
    Q_OBJECT

private:
    Ui::MenuWindow *ui;
    GameWindow *game;

    void showWarningMessage(const QString &text);
    void setFigure(QLabel *label, const QString &pathToFig) const;

public:
    explicit MenuWindow(QWidget *parent = nullptr);
    ~MenuWindow();

private slots:
    void onStartButtonClicked();
    void onCloseButtonClicked();
};