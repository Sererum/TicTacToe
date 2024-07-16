#include "menuwindow.h"
#include "ui_menuwindow.h"

void MenuWindow::showWarningMessage(const QString &text)
{
    QMessageBox *message = new QMessageBox(this);
    message->setIcon(QMessageBox::Warning);
    message->setWindowTitle("Предупреждение");
    message->setStyleSheet("QLabel{font-size: 18px; qproperty-alignment: AlignCenter;} QPushButton{font-size: 18px;}");
    message->setText(text);
    message->exec();
    delete message;
}

void MenuWindow::setFigure(QLabel *label, const QString &pathToFig) const
{
    QPixmap pixmap(pathToFig);
    pixmap = pixmap.scaled(SIZE_MENU_FIGS, SIZE_MENU_FIGS, Qt::KeepAspectRatio);
    label->setPixmap(pixmap);
}

MenuWindow::MenuWindow(QWidget *parent)
    : QDialog(parent), ui(new Ui::MenuWindow)
{
    //TODO
    //this->setStyleSheet("background-color: #e9fbd3; \ncolor: #000000");
    ui->setupUi(this);

    setFigure(ui->LeftImageLabel, ":/images/Cross.png");
    setFigure(ui->RightImageLabel, ":/images/Zero.png");
}

MenuWindow::~MenuWindow()
{
    delete ui;
}

void MenuWindow::onCloseButtonClicked()
{
    QMessageBox::StandardButton reply 
        = QMessageBox::question(this, "Выход", "Точно ли вы хотите выйти из приложения?",
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply  == QMessageBox::Yes)
        QCoreApplication::quit();
}

void MenuWindow::onStartButtonClicked() {
    QString value = ui->SizeFieldEdit->text();

    bool isNumber;
    int numValue = value.toInt(&isNumber);

    if (isNumber == false) {
        showWarningMessage("Размер поля должен быть числом!");
        return;
    } else if (numValue < 5 || numValue > 25) {
        showWarningMessage("Размер поля должен находиться в диапазоне от 5 до 25");
        return;
    }

    hide();
    game = new GameWindow(numValue, this);
    game->show();
}
