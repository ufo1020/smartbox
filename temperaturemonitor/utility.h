#ifndef UTILITY
#define UTILITY

#include <QTextStream>
#include <QFile>

namespace SmartBox {

template <class T> bool readFile(QFile* file, T& result)
{
    if (!file->exists()){
        return false;
    }

    if (file->isOpen()){
        file->close();
    }

    if (!file->open(QIODevice::ReadOnly | QIODevice::Text)) {
        return false;
    }

    QTextStream in(file);
    in >> result;
    file->close();
    return true;
}

template <class T> bool writeFile(QFile* file, T input)
{
    if (!file->exists()){
        return false;
    }

    if (file->isOpen()){
        file->close();
    }

    if (!file->open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        return false;
    }

    QTextStream out(file);
    out << input;
    file->close();
    return true;
}
}
#endif // UTILITY

