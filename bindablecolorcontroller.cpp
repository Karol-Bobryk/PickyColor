#include "bindablecolorcontroller.h"

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