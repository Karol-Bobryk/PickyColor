#ifndef BINDABLECOLORCONTROLLER_H
#define BINDABLECOLORCONTROLLER_H

#include <QObject>
#include <QProperty>

class BindableColorController : public QObject
{
    Q_OBJECT
public:
    enum class ColorInput { Cyan, Magenta, Yellow, Black, Red, Green, Blue };

    BindableColorController() = default;
    BindableColorController(const BindableColorController &) = delete;

    int red() const { return red_value; }
    int green() const { return green_value; }
    int blue() const { return blue_value; }
    int cyan() const { return cyan_value; }
    int magenta() const { return magenta_value; }
    int yellow() const { return yellow_value; }
    int black() const { return black_value; }

    int color(ColorInput color) const;

    void setRed(int red_value) { this->red_value = red_value; }
    void setGreen(int green_value) { this->green_value = green_value; }
    void setBlue(int blue_value) { this->blue_value = blue_value; }
    void setCyan(int cyan_value) { this->cyan_value = cyan_value; }
    void setMagenta(int magenta_value) { this->magenta_value = magenta_value; }
    void setYellow(int yellow_value) { this->yellow_value = yellow_value; }
    void setBlack(int black_value) { this->black_value = black_value; }

    void setColor(int value, ColorInput color);

    void addHandler(QPropertyNotifier &&notifier);

    QBindable<int> bindableRed() { return &red_value; }
    QBindable<int> bindableGreen() { return &green_value; }
    QBindable<int> bindableBlue() { return &blue_value; }
    QBindable<int> bindableCyan() { return &cyan_value; }
    QBindable<int> bindableMagenta() { return &magenta_value; }
    QBindable<int> bindableYellow() { return &yellow_value; }
    QBindable<int> bindableBlack() { return &black_value; }

    QBindable<int> bindableColor(ColorInput color);

private:
    // rgb
    QProperty<int> red_value{0};
    QProperty<int> green_value{0};
    QProperty<int> blue_value{0};

    // cmyk
    QProperty<int> cyan_value{0};
    QProperty<int> magenta_value{0};
    QProperty<int> yellow_value{0};
    QProperty<int> black_value{0};

    std::vector<QPropertyNotifier> handlers;
signals:
};

#endif // BINDABLECOLORCONTROLLER_H
