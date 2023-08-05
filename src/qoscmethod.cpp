#include <qoscmethod.h>
#include <QMetaObject>

QOscMethod::QOscMethod(const QString& addr) : addr(addr) {
    this->compilePattern();
}

QOscSlotMethod::QOscSlotMethod(const QString& addr, QObject* obj, const char* slot) :
    QOscMethod(addr),
    _obj(obj),
    _slot(slot)
{

}

void QOscSlotMethod::call(const QOscMessage& msg)
{
    QMetaObject::invokeMethod(_obj, _slot, Qt::DirectConnection, Q_ARG(QOscMessage, msg));
}

bool QOscMethod::match(QString pattern) {
    return matcher.match(pattern).hasMatch();
}

void QOscMethod::compilePattern() {
    // Set pattern to be address
    QString copy = addr;

    copy.replace('.', "\\.");
    copy.replace('(', "\\(");
    copy.replace(')', "\\)");
    copy.replace('*', "[^\\s]*");
    copy.replace("[!", "[^");
    copy.replace('?',  '.');

    QRegularExpression reg(R"({(([^,]+)(,([^,]+))+)})");
    QRegularExpressionMatch m;
    while((m = reg.match(copy)).hasMatch())
    {
        auto content = QRegularExpression::escape(m.captured(1));
        content.replace("\\,", "|");
        copy.replace(m.capturedStart(), m.capturedLength(), "(" + content + ")");
    }

    copy.prepend('^');
    copy.append('$');

    matcher.setPattern(copy);

    if(!matcher.isValid()) {
        pattern.clear();
    }
}