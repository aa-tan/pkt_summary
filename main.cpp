#include "mainclass.h"
#include <QCoreApplication>
#include <QTimer>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    QString version = "1.0.0";
    app.setApplicationVersion(version);

    MainClass myMain;

    QObject::connect(&myMain, SIGNAL(finished()), 
                     &app, SLOT(quit()));
    QObject::connect(&app, SIGNAL(aboutToQuit()), 
                     &myMain, SLOT(aboutToQuitApp()));

    QTimer::singleShot(0, &myMain, SLOT(run()));

    return app.exec();
}


/*
packet types + numbers

Flags - outputs if flag exists

App -t types.csv
Out put bash code friendly exists or not
*/

// int main(int argc, char *argv[])
// {
//     QCoreApplication app(argc, argv);
//     QCoreApplication::setApplicationName("my-copy-program");
//     QCoreApplication::setApplicationVersion("1.0");

//     QCommandLineParser parser;
//     parser.setApplicationDescription("Summarizes packet data");
//     parser.addHelpOption();
//     parser.addVersionOption();
    
//     // An option with a value
//     QCommandLineOption typesFileOption(QStringList() << "t" << "types",
//             QCoreApplication::translate("main", "checks if packet types specified in <file> exists."),
//             QCoreApplication::translate("main", "file"));
//     parser.addOption(typesFileOption);

//     parser.process(app);
//     QString targetFile = parser.value(typesFileOption);
//     qDebug() << targetFile;
//     quit();
// }

// void MainClass::quit()
// {
//     emit finished();
// }


// /**
//  * Shortly after quit() is called the QCoreApplication will signal this routine.
//  * This is a good place to delete any objects that were created in the
//  * constructor and/or to stop any threads.
//  */
// void MainClass::aboutToQuitApp()
// {
// }
