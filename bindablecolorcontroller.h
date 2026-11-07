#ifndef BINDABLECOLORCONTROLLER_H
#define BINDABLECOLORCONTROLLER_H

#include <QColor>
#include <QObject>
#include <QProperty>
#include <qdebug.h>

class BindableColorController : public QObject
{
    Q_OBJECT
public:
    enum class ColorInput { Cyan, Magenta, Yellow, Black, Red, Green, Blue };

    BindableColorController();
    BindableColorController(const BindableColorController &) = delete;

    int red() const { return red_value; }
    int green() const { return green_value; }
    int blue() const { return blue_value; }
    int cyan() const { return cyan_value; }
    int magenta() const { return magenta_value; }
    int yellow() const { return yellow_value; }
    int black() const { return black_value; }

    int color(ColorInput color) const;

    QColor getRgb() const { return QColor(red(), green(), blue()); }
    QColor getCmyk() const { return QColor::fromCmyk(cyan(), magenta(), yellow(), black()); }

    void setRed(int red_value, bool update = true)
    {
        this->red_value = red_value;
        if (update)
            emit rgbUpdated(getRgb());
    }

    void setGreen(int green_value, bool update = true)
    {
        this->green_value = green_value;
        if (update)
            emit rgbUpdated(getRgb());
    }

    void setBlue(int blue_value, bool update = true)
    {
        this->blue_value = blue_value;
        if (update)
            emit rgbUpdated(getRgb());
    }

    void setCyan(int cyan_value, bool update = true)
    {
        this->cyan_value = cyan_value;
        if (update)
            emit cmykUpdated(getCmyk());
    }

    void setMagenta(int magenta_value, bool update = true)
    {
        this->magenta_value = magenta_value;
        if (update)
            emit cmykUpdated(getCmyk());
    }

    void setYellow(int yellow_value, bool update = true)
    {
        this->yellow_value = yellow_value;
        if (update)
            emit cmykUpdated(getCmyk());
    }

    void setBlack(int black_value, bool update = true)
    {
        this->black_value = black_value;
        if (update)
            emit cmykUpdated(getCmyk());
    }

    void setColor(int value, ColorInput color);

    void setRgb(QColor rgb, bool update = true);

    void setCmyk(QColor cmyk, bool update = true);

    void addHandler(QPropertyNotifier &&notifier);

    QBindable<int> bindableRed() { return &red_value; }
    QBindable<int> bindableGreen() { return &green_value; }
    QBindable<int> bindableBlue() { return &blue_value; }
    QBindable<int> bindableCyan() { return &cyan_value; }
    QBindable<int> bindableMagenta() { return &magenta_value; }
    QBindable<int> bindableYellow() { return &yellow_value; }
    QBindable<int> bindableBlack() { return &black_value; }

    QBindable<int> bindableColor(ColorInput color);

    static QColor toRgb(QColor cmyk);

    static QColor toCmyk(QColor rgb);

private:
    // rgb
    QProperty<int> red_value{0};
    QProperty<int> green_value{0};
    QProperty<int> blue_value{0};

    // cmyk
    QProperty<int> cyan_value{0};
    QProperty<int> magenta_value{0};
    QProperty<int> yellow_value{0};
    QProperty<int> black_value{255};

    std::vector<QPropertyNotifier> handlers;
signals:
    void rgbUpdated(QColor rgb);
    void cmykUpdated(QColor rgb);
    void colorUpdated(QColor rgb);
};

#endif // BINDABLECOLORCONTROLLER_H
