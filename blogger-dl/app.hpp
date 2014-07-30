#ifndef APP_HPP
#define APP_HPP

#include "bootstrap.hpp"

#include <QMap>
#include <QString>
#include <QStringList>
#include <QTextStream>

class QJsonDocument;
class QJsonArray;
class QJsonObject;
class QJsonValue;

class App : public Bootstrap {
public:
    App();
    virtual ~App();
    
    typedef QMap<QString, QString> Dictionary;

protected:
    void main();
    
private:
    static QString dictArgs(const QString & output, const Dictionary & dictionary);
    static QString regex(const Dictionary & dictionary);
    
    void help(const QStringList & args);

    QTextStream qtout;
    QTextStream qterr;
    QTextStream qtin;
};

#endif // APP_HPP
