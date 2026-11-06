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

    bind_text_slider_inputs(BindableColorController::ColorInput::Cyan,
                            ui->edit_cyan,
                            ui->slider_cyan);
    bind_text_slider_inputs(BindableColorController::ColorInput::Magenta,
                            ui->edit_magenta,
                            ui->slider_magenta);
    bind_text_slider_inputs(BindableColorController::ColorInput::Yellow,
                            ui->edit_yellow,
                            ui->slider_yellow);
    bind_text_slider_inputs(BindableColorController::ColorInput::Black,
                            ui->edit_black,
                            ui->slider_black);
    bind_text_slider_inputs(BindableColorController::ColorInput::Red,
                            ui->edit_red, //
                            ui->slider_red);
    bind_text_slider_inputs(BindableColorController::ColorInput::Green,
                            ui->edit_green,
                            ui->slider_green);
    bind_text_slider_inputs(BindableColorController::ColorInput::Blue,
                            ui->edit_blue,
                            ui->slider_blue);
}

void MainWindow::bind_text_slider_inputs(BindableColorController::ColorInput color,
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

MainWindow::~MainWindow()
{
    delete ui;
}
