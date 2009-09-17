#include <QString>
#include "manualtester.h"
#include <QFile>
int main(int argc, char *argv[])
{
    if ( argc != 4 ) {
        std::cout << std::endl << "Usage: " << argv[0] << " pathToQtLauncher pathToTestDir pathToEntryFile" << std::endl << std::endl;
        return 0;
    } else {
        QApplication a(argc, argv);
        a.setQuitOnLastWindowClosed(false);
        QString launcher( argv[1] );
        QString testDir(argv[2]);
        QStringList fileList;
        QFile file(argv[3]);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
            std::cout << argv[3] <<": No such file or directory!";
            return 1;
        }
        QTextStream in(&file);
        while (!in.atEnd()){
            fileList << in.readLine();
        }
        ManualTester tester(&a);
        tester.setup(launcher,testDir,fileList);
        tester.start();
        return a.exec();
    }
}
