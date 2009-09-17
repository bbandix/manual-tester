#include "manualtester.h"

ManualTester::ManualTester(QApplication *a)
        : QWidget()
{
    app = a;
    launcher = new QProcess(this);
    timer = new QTimer(this);
    message = new QMessageBox(this);
    actual = 0;
    message->setText("The document has been modified.");
    message->setInformativeText("Do you want to save your changes?");
    message->setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Abort);
    message->setDefaultButton(QMessageBox::Discard);
    message->move(0,0);
    log.setFileName("log.txt");
    logging = log.open(QIODevice::WriteOnly | QIODevice::Text);
    logger.setDevice(&log);
    connect( this, SIGNAL( optionSignal(Option) ), SLOT( handleOption(Option) ) );
    connect( this, SIGNAL( continueSignal() ), SLOT( start() ) );
    connect( timer, SIGNAL( timeout() ), this, SLOT( requestOption() ) );
    connect( timer, SIGNAL( timeout() ), message, SLOT( show() ) );
}

void ManualTester::start(){
    if (!QFile::exists(program)){
        qDebug()<< "\n" << program <<":\tProgram does not exist!" << "\n";
        exit(1);
    }

    if (actual < fileList.size()) {
        run( fileList[actual] );
    } else {
        handleOption(Abort);
    }
}

void ManualTester::copy() {
    QFileInfo info(fileList[actual]);
    QStringList path = info.absoluteDir().absolutePath().split("LayoutTests");
    QString actual(testDir.absolutePath()+path.last()+"/"
                   +info.fileName().split(".").first()+"-actual.txt");
    QString expected(path.first()+"LayoutTests/platform/qt"+path.last()+"/"
                     +info.fileName().split(".").first()+"-expected.txt");

    if(QFile::remove(expected) && logging) logger<<"rm "<<expected<<"\n";
    if(QFile::copy(actual,expected) && logging) logger<<"cp "<<actual<<" "<<expected<<"\n";

}

void ManualTester::run(const QString &arg){
    launcher->start(program, QStringList()<<arg);
    if ( !launcher->waitForStarted() )
        return;
    timer->start(500);
}

void ManualTester::requestOption(){
    int opt = message->exec();
    switch (opt) {
    case QMessageBox::Save:
        emit optionSignal(Save);
        break;
    case QMessageBox::Discard:
        emit optionSignal(Discard);
        break;
    case QMessageBox::Abort:
        emit optionSignal(Abort);
        break;
    default:
        // should never be reached
        break;
    }
}

void ManualTester::setup(const QString& pgrm, const QString& tstDir, const QStringList& files){
    program = pgrm;
    fileList = files;
    testDir.setPath(tstDir);
}

void ManualTester::handleOption(Option opt){
    actual++;
    launcher-> close();
    switch (opt){
    case Save:
        if (actual < fileList.size()) {
            copy();
        } else {
            handleOption(Abort);
        }
        break;
    case Discard:
        if (actual < fileList.size()) {
          if (logging) logger<<"Skipped: "<<fileList[actual]<<"\n";
          qDebug()<< logging;
        }
        break;
    case Abort:
        launcher->close();
        app->setQuitOnLastWindowClosed(true);
        if (actual < fileList.size()) {
          if (logging) logger<<"Aborted at: "<<fileList[actual]<<"\n";
        }
        logger.flush();
        log.close();
        exit(0);
        break;
    default:
        // should never be reached
        break;
    }
    emit continueSignal();
}

ManualTester::~ManualTester()
{
    delete launcher;
    delete timer;
    delete message;
}
