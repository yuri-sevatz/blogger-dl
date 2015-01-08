#ifndef APP_HPP
#define APP_HPP

#include <libtwirl/app/boot.hpp>

#include <QMap>
#include <QString>
#include <QStringList>
#include <QTextStream>

class QJsonDocument;
class QJsonArray;
class QJsonObject;
class QJsonValue;

class App : public twirl::Boot {
public:
    App(QCoreApplication & app);
    virtual ~App();
    
    typedef QMap<QString, QString> Dictionary;

    int main();
    
private:
    static QString dictArgs(const QString & output, const Dictionary & dictionary);
    static QString regex(const Dictionary & dictionary);
    
    void help(const QStringList & args);

    QTextStream qtout;
    QTextStream qterr;
    QTextStream qtin;
};

#endif // APP_HPP
