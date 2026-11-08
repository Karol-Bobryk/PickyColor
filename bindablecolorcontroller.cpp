#include "bindablecolorcontroller.h"
#include <qdebug.h>

BindableColorController::BindableColorController()
{
    connect(this, &BindableColorController::rgbUpdated, [this](QColor rgb) {
        setCmyk(toCmyk(rgb), false);
        emit colorUpdated(rgb);
    });

    connect(this, &BindableColorController::cmykUpdated, [this](QColor cmyk) {
        setRgb(toRgb(cmyk), false);
        emit colorUpdated(toRgb(cmyk));
    });
}

QColor BindableColorController::toRgb(QColor cmyk)
{
    int black = cmyk.black();
    if (black == 255)
        return QColor(0, 0, 0);

    int red = (255 - cmyk.cyan()) * (255 - black) / 255;
    int green = (255 - cmyk.magenta()) * (255 - black) / 255;
    int blue = (255 - cmyk.yellow()) * (255 - black) / 255;
    return QColor(red, green, blue);
}

QColor BindableColorController::toCmyk(QColor rgb)
{
    // this is stupid but qt has cmyk values 0-256 instead of 0-100
    // and I'm not planning on writing a compatibility layer

    QColor cmyk;

    int black = 255 - std::max(rgb.red(), std::max(rgb.green(), rgb.blue()));

    if (black == 255) {
        cmyk.setCmyk(0, 0, 0, 255);
        return cmyk;
    }

    int denominator = 255 - black;
    int cyan = (255 - rgb.red() - black) * 255 / denominator;
    int magenta = (255 - rgb.green() - black) * 255 / denominator;
    int yellow = (255 - rgb.blue() - black) * 255 / denominator;

    cmyk.setCmyk(cyan, magenta, yellow, black);
    return cmyk;
}

void BindableColorController::setRgb(QColor rgb, bool update)
{
    this->red_value = rgb.red();
    this->green_value = rgb.green();
    this->blue_value = rgb.blue();
    if (update)
        emit rgbUpdated(getRgb());
}

void BindableColorController::setCmyk(QColor cmyk, bool update)
{
    this->cyan_value = cmyk.cyan();
    this->magenta_value = cmyk.magenta();
    this->yellow_value = cmyk.yellow();
    this->black_value = cmyk.black();
    if (update)
        emit cmykUpdated(getCmyk());
}
int BindableColorController::color(ColorInput color) const
{
    switch (color) {
    case ColorInput::Cyan:
        return cyan();
    case ColorInput::Magenta:
        return magenta();
    case ColorInput::Yellow:
        return yellow();
    case ColorInput::Black:
        return black();
    case ColorInput::Red:
        return red();
    case ColorInput::Green:
        return green();
    case ColorInput::Blue:
        return blue();
    default:
        return 0; // shouldnt happen :V
    }
}
void BindableColorController::setColor(int value, ColorInput color)
{
    switch (color) {
    case ColorInput::Cyan:
        setCyan(value);
        break;
    case ColorInput::Magenta:
        setMagenta(value);
        break;
    case ColorInput::Yellow:
        setYellow(value);
        break;
    case ColorInput::Black:
        setBlack(value);
        break;
    case ColorInput::Red:
        setRed(value);
        break;
    case ColorInput::Green:
        setGreen(value);
        break;
    case ColorInput::Blue:
        setBlue(value);
        break;
    default:
        return; // shouldnt happen :V
    }
}

QBindable<int> BindableColorController::bindableColor(ColorInput color)
{
    switch (color) {
    case ColorInput::Cyan:
        return &cyan_value;
    case ColorInput::Magenta:
        return &magenta_value;
    case ColorInput::Yellow:
        return &yellow_value;
    case ColorInput::Black:
        return &black_value;
    case ColorInput::Red:
        return &red_value;
    case ColorInput::Green:
        return &green_value;
    case ColorInput::Blue:
        return &blue_value;
    default:
        throw std::runtime_error("Unexpected ColorInput type");
    }
}

void BindableColorController::addHandler(QPropertyNotifier &&notifier)
{
    handlers.push_back(std::move(notifier));
}