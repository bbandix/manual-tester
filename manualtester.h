#ifndef MANUALTESTER_H
#define MANUALTESTER_H

#include <QProcess>
#include <QMessageBox>
#include <QTimer>
#include <QStringList>
#include <QDir>
#include <QApplication>
#include <QtDebug>
#include <iostream>

class ManualTester : public QWidget
{
    Q_OBJECT
private:
    QApplication *app;
    QMessageBox *message;
    QProcess *launcher;
    QProcess *runtest;
    QProcess *showpng;
    QTimer *timer;
    QString tester;
    QString program;
    QString tempFile;
    QDir testDir;
    QStringList fileList;
    QTextStream logger;
    QFile log;
    bool logging;
    int actual;
    void copy();
    enum Option {
        Save = 0,
        Discard = 1,
        Abort = 2
    };
private slots:
    void requestOption();
    void handleOption(Option);
signals:
    void optionSignal(Option);
    void continueSignal();
public slots:
    void start();
public:
    ManualTester(QApplication*);
    ~ManualTester();
    void setup(const QString&, const QString&, const QString&, const QStringList&);
    void run(const QString&);
};

#endif // MANUALTESTER_H
