#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "bindablecolorcontroller.h"

#include <QLineEdit>
#include <QMainWindow>
#include <QProperty>
#include <QPropertyNotifier>
#include <QSlider>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private:
    void bindTextSliderInputs(BindableColorController::ColorInput color,
                              QLineEdit *edit,
                              QSlider *slider);
    Ui::MainWindow *ui;
    BindableColorController *color_controller;
    void updateColorDisplay(QColor color);
};
#endif // MAINWINDOW_H
