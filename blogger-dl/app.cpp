#include "app.hpp"

#include <QDebug>
#include <QApplication>
#include <QDir>
#include <QImageReader>
#include <QUrl>
#include <QRegExp>

#include <libblogger/include/config.h>
#include <libblogger/client.hpp>

using namespace blogger;

App::App(QCoreApplication & app) : Boot(app),
    qtout(stdout),
    qterr(stderr),
    qtin(stdin) {
}

App::~App() {}

void App::help(const QStringList & args) {
    qDebug() << "Blogspot Shell Script";
    qDebug() << "Usage:" << args.value(0);
    qDebug() << "\t-a address" << "\t" << "Address of a blog site or post";
    qDebug() << "\t-o output" << "\t" << "Output template (includes selectors)";
    qDebug() << "\t-d direction" << "\t" << "Direction [prev|next]";
    qDebug() << "\t-s simulate" << "\t" << "Simulate, no saving";
    qDebug() << "\t-c continue" << "\t" << "Continue on duplicate filename";
    qDebug() << "\t-i ignore-errors" << "\t" << "Ignore errors";
    qDebug() << "\t-q quiet" << "\t" << "Suppress verbose output";
    qDebug() << "\t";
    qDebug() << "\tSelectors:";
    qDebug() << "\t\t" << "%(domain)s" << "\t" << "Domain of the blog";
    qDebug() << "\t\t" << "%(title)s" << "\t" << "Title of the blog post";
    qDebug() << "\t\t" << "%(file)s" << "\t" << "Filename of the download";
    qDebug() << "\t\t" << "%(ext)s" << "\t" << "Extension of the download";
    qDebug() << "\t\t" << "%(format)s" << "\t" << "Format after the download";
    qDebug() << "";
    qDebug() << "\t-h" << "\t\t" << "Show this help message";
    qDebug() << "";
    qDebug() << "Examples:";
    qDebug() << "\t" << args.value(0) << "";
}

static inline App::Dictionary dict(
    const QString & domain,
    const QString & imageUrl,
    const QString & blogId,
    const QString & postId,
    const QString & title,
    const QString & file,
    const QString & base,
    const QString & ext,
    const QString & count,
    const QString & format
) {
    App::Dictionary dictionary;
    dictionary.insert("%(domain)s", domain);
    dictionary.insert("%(imageUrl)s", imageUrl);
    dictionary.insert("%(blogId)s", blogId);
    dictionary.insert("%(postId)s", postId);
    dictionary.insert("%(title)s", title);
    dictionary.insert("%(file)s", file);
    dictionary.insert("%(base)s", base);
    dictionary.insert("%(ext)s", ext);
    dictionary.insert("%(count)s", count);
    dictionary.insert("%(format)s", format);
    return dictionary;
}

int App::main() {
    const QStringList args = QApplication::arguments();
    Client client;
    QUrl url;
    QString output = QString("%(postId)s - %(title)s").append(QDir::separator()).append("%(file)s");
    enum Direction {
      None,
      Prev,
      Next,
    };
    Direction direction = None;
    bool write = true;
    bool abort = true;
    bool cease = true;
    bool noisy = true;

    if (args.size() < 2) {
        help(args);
        return EXIT_FAILURE;
    }

    for(int argi = 0; argi < args.size(); argi++) {
        const QString arg = args.value(argi);
        if (arg == "-a") {
            url = args.value(++argi);
        } else if (arg == "-o") {
            output = args.value(++argi);
        } else if (arg == "-d") {
	    const QString val = args.value(++argi);
	    direction =
		val == "prev" ? Prev :
		val == "next" ? Next :
		None;
	} else if (arg == "-s") {
            write = false;
        } else if (arg == "-c") {
            cease = false;
        } else if (arg == "-i") {
            abort = false;
        } else if (arg == "-q") {
            noisy = false;
        } else if (arg == "-h" || arg == "-help" || arg == "--help" || arg == "/?") {
            help(args);
            return EXIT_SUCCESS;
        }
    }
    
    if (url.isEmpty()) {
        qterr << "Please enter a url (-a)";
        return EXIT_FAILURE;;
    } else if (url.scheme().isEmpty()) {
        url.setScheme("http");
    }
    
    while (!url.isEmpty()) {
        qtout.flush();
        qterr.flush();
        Q_FOREACH(const Blog & blog, client.open(url)) {
            qtout << blog.url().toDisplayString() << "\n";
            if (noisy) {
                qtout << blog.title() << "\n";
            }
            typedef QMap<QString,qulonglong> FrequencyMap;
            FrequencyMap occurences;
            Q_FOREACH(const QUrl & image, blog.images()) {
                qtout.flush();
                qterr.flush();
                if (noisy) {
                    qtout << image.toDisplayString() << "\n";
                }
                const QByteArray data = client.save(image);
                if (data.isEmpty()) {
                    qterr << "Failed - " << image.toDisplayString() << "\n";
                    if (abort) {
                        qtout.flush();
                        qterr.flush();
                        return EXIT_FAILURE;;
                    }
                } else {
                    const QFileInfo src(image.fileName());
                    const QString format = QImageReader(QDataStream(data).device()).format();
                    
                    if (format.isEmpty()) {
                        qterr << "Format - Unknown image header: " << image.toDisplayString() << "\n";
                        if (abort) {
                            qtout.flush();
                            qterr.flush();
                            return EXIT_FAILURE;;
                        }
                    }
                    
                    const QString countSource = src.fileName();
                    FrequencyMap::iterator occurence = occurences.find(countSource);
                    const QString count = QString::number(
                        (*(
                            occurence == occurences.end() ? 
                            occurences.insert(countSource, 0) : 
                            occurence
                        ))++
                    );
                                        
                    const QFileInfo dst(
                        dictArgs(output, dict(
                            blog.url().authority(),
                            blog.imageUrl().toEncoded(),
                            blog.blogId(),
                            blog.postId(),
                            blog.title(),
                            src.fileName(),
                            src.baseName(),
                            src.completeSuffix(),
                            count,
                            format
                        ))
                    );
                    
                    if (dst.exists()) {
                        qterr << "Exists - " << dst.filePath() << "\n";
                        if (cease) {
                            qtout.flush();
                            qterr.flush();
                            return EXIT_FAILURE;;
                        }
                    } else if (write) {
                        if (dst.path().isEmpty() || QDir().mkpath(dst.path())) {
                            QFile fd(dst.filePath());
                            fd.open(QIODevice::WriteOnly);
                            fd.write(data);
                            if (fd.error()) {
                                qterr << "Error - " << dst.filePath() << "\n";
                                if (abort) {
                                    qtout.flush();
                                    qterr.flush();
                                    return EXIT_FAILURE;;
                                }
                            } else if (noisy) {
                                qterr << "Saved - " << dst.filePath() << "\n";
                            }
                        } else {
                            qterr << "Path - " << dst.path() << "\n";
                            if (abort) {
                                qtout.flush();
                                qterr.flush();
                                return EXIT_FAILURE;;
                            }
                        }
                    } else if (noisy) {
                        qtout << "Pretend - " << dst.filePath() << "\n";
                    }
                }
            }
        }
        url =
            direction == Prev ? client.older() :
            direction == Next ? client.newer() :
            QUrl();
    }
    qtout.flush();
    qterr.flush();
    return EXIT_SUCCESS;
}

inline QString App::regex(const QMap<QString, QString> & dictionary) {
    QString ret;
    QMap<QString, QString>::const_iterator iter = dictionary.begin();
    if (iter != dictionary.end()) {
        ret.append(QRegExp::escape((iter++).key()));
        while(iter != dictionary.end()) {
            ret.append("|").append(QRegExp::escape((iter++).key()));
        }
    }
    return ret;
}

inline QString App::dictArgs(const QString & output, const Dictionary & dictionary) {
    QString ret;
    const QRegExp lookup(regex(dictionary), Qt::CaseSensitive, QRegExp::RegExp2);
    int start = 0;

    do {
	const int next = lookup.indexIn(output, start);
	if (next >= start) {
	    ret += output.mid(start, (next - start));
	    const QString match = lookup.cap(0);
            start = next + match.length();
	    ret += dictionary.value(lookup.cap(0));
        } else {
            break;
        }
    } while (true);

    if (start < output.size()) {
        ret += output.right(output.size() - start);
    }

    return ret;
}


