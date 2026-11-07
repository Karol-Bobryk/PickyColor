#include "mainwindow.h"
#include <QBrush>
#include <QGraphicsScene>
#include <QLinearGradient>
#include <QSlider>
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , color_controller{new BindableColorController()}
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    bindTextSliderInputs(BindableColorController::ColorInput::Cyan,
                         ui->edit_cyan, //
                         ui->slider_cyan);
    bindTextSliderInputs(BindableColorController::ColorInput::Magenta,
                         ui->edit_magenta,
                         ui->slider_magenta);
    bindTextSliderInputs(BindableColorController::ColorInput::Yellow,
                         ui->edit_yellow,
                         ui->slider_yellow);
    bindTextSliderInputs(BindableColorController::ColorInput::Black,
                         ui->edit_black,
                         ui->slider_black);
    bindTextSliderInputs(BindableColorController::ColorInput::Red,
                         ui->edit_red, //
                         ui->slider_red);
    bindTextSliderInputs(BindableColorController::ColorInput::Green,
                         ui->edit_green,
                         ui->slider_green);
    bindTextSliderInputs(BindableColorController::ColorInput::Blue,
                         ui->edit_blue, //
                         ui->slider_blue);

    connect(color_controller,
            &BindableColorController::colorUpdated,
            this,
            &MainWindow::updateColorDisplay);
}

void MainWindow::bindTextSliderInputs(BindableColorController::ColorInput color,
                                      QLineEdit *edit,
                                      QSlider *slider)
{
    color_controller->addHandler(
        std::move(color_controller->bindableColor(color).addNotifier([this, color, edit, slider]() {
            int val = color_controller->color(color);
            slider->setValue(val);
            edit->setText(QString::number(val));
        })));

    connect(slider, &QSlider::valueChanged, [this, color](int v) {
        color_controller->setColor(v, color);
    });

    connect(ui->edit_red, &QLineEdit::textEdited, [this, color](const QString &text) {
        color_controller->setColor(text.toInt(), color);
    });
}
void MainWindow::updateColorDisplay(QColor color)
{
    QPalette p = ui->color_display->palette();
    p.setColor(ui->color_display->backgroundRole(), color);
    ui->color_display->setPalette(p);
}

MainWindow::~MainWindow()
{
    delete ui;
}
