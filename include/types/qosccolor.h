#ifndef QOSCCOLOR_H
#define QOSCCOLOR_H

#include "QOSC_global.h"
#include "qoscabstracttype.h"
#include <QColor>

class QOSC_EXPORT QOSCColor : public QOSCAbstractType
{
public:
    QOSCColor() : QOSCAbstractType(QOSC::ColorType) {}
    QOSCColor(const QOSCColor& copy) = default;
    QOSCColor(QOSCColor&& move) = default;

    inline QOSCColor(const QColor& c) : QOSCAbstractType(QOSC::ColorType), _c(c) {}
    inline QOSCColor(QColor&& c) : QOSCAbstractType(QOSC::ColorType), _c(c) {}

    inline operator QColor() const { return _c; }

    QOSCColor& operator=(const QOSCColor& i) = default;
    QOSCColor& operator=(QOSCColor&& i) = default;

    inline QOSCColor& operator=(const QColor& c) { _c = c; return *this; };
    inline QOSCColor& operator=(QColor&& c) { _c = c; return *this; };

    inline void writeTypeTag(QIODevice* dev) const override { dev->putChar('r'); }
    inline void writeData(QIODevice* dev) const override
    {
        quint32 rgba = ((_c.red()   & 0xFF) << 24) +
                       ((_c.green() & 0xFF) << 16) +
                       ((_c.blue()  & 0xFF) << 8)  +
                        (_c.alpha() & 0xFF);
        QOSC::writeHelper(dev, rgba);
    }

    inline void readData(QIODevice* dev) override
    {
        quint32 rgba;
        QOSC::readHelper(dev, &rgba);
        char r, g, b, a;

        r = (rgba >> 24) & 0xFF;
        g = (rgba >> 16) & 0xFF;
        b = (rgba >>  8) & 0xFF;
        a = (rgba      ) & 0xFF;

        _c = QColor(r, g, b, a);
    }

private:
    QColor _c;
};

#endif // QOSCCOLOR_H
