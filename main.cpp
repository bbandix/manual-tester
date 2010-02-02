#include <QString>
#include "manualtester.h"
#include <QFile>
#include <stdio.h>
int main(int argc, char *argv[])
{
    QString launcher;
    QString testDir;
    QString pwd;
    QFile file("manualtester.conf");
    if ( argc == 3 ) {
        launcher = QString( argv[1] );
        testDir = QString( argv[2] );
    }
    QApplication a(argc, argv);
    a.setQuitOnLastWindowClosed(false);
    QStringList fileList;

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        std::cout << argv[3] <<": No such file or directory!\n\nRun ./generateTestFile.sh before running this!";
        return 1;
    }
    QTextStream in(&file);
    pwd = in.readLine();
    if (launcher.isEmpty()) launcher = in.readLine();
    else in.readLine();
    if (testDir.isEmpty()) testDir = in.readLine();
    else in.readLine();
    while (!in.atEnd()){
        fileList << in.readLine();
    }
    fprintf(stderr,"launcher = %s\n",launcher.toAscii().data());
    fprintf(stderr,"testDir = %s\n",testDir.toAscii().data());
    ManualTester tester(&a);
    tester.setup(pwd, launcher,testDir,fileList);
    tester.start();
    return a.exec();
}
