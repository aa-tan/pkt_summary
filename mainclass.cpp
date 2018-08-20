#include "mainclass.h"
#include <momamission.hpp>
#include <gedimission.hpp>

#include <backports/qcommandlineparser.h>

MomGse::MomDataInterpretationFactory momaDataInterpFactory;
GediGse::GediDataInterpretationFactory gediDataInterpFactory;

using GseLib::DataInterpretation;
using GseLib::TmMeta;
using GseLib::TmDatabaseRow;
using GseLib::TMPacketPtr;
using GseLib::TelemetryFile;

using MomGse::MetaMarker;

CmdLineArgs parseArguments(const QStringList & arguments)
{
    QCommandLineParser parser;
    parser.setApplicationDescription("Summarizes TM data");
    parser.addHelpOption();
    parser.addVersionOption();

    const QCommandLineOption fileOption(QStringList() << "f" << "file",
                                        "Path to TM file to extract data from", "FILE");
    parser.addOption(fileOption);

    const QCommandLineOption fullOption("full", "IDs of HK items to be fully mined", "IDs");
    parser.addOption(fullOption);

    parser.process(arguments);

    CmdLineArgs results;

    results.tmFileName = parser.value(fileOption);
    // No option provided, search the current directory
    if (results.tmFileName.isEmpty())
    {
       TmMeta tmMeta(QDir::current().absolutePath());
       if (tmMeta.isValid())
          results.tmFileName = tmMeta.tmFileName();
    }
    const QFileInfo tmFileInfo(results.tmFileName);
    if (!tmFileInfo.exists() || !tmFileInfo.isFile())
    {
        std::cerr << "tmfile does not exist: " << results.tmFileName << "\n\n";
        parser.showHelp(1);
    }

    // results.fullyMinedHkids = extractHkIds(parser.value(fullOption));

    return results;
}

MainClass::MainClass(QObject *parent) :
    QObject(parent)
{
}

void MainClass::run()
{
    CmdLineArgs args = parseArguments(qApp->arguments());

    // GseLib::TelemetryFilePtr tmFile = createTmFile(args.tmFileName);
    // const MomGse::MarkerCache markerCache(new GseLib::TelemetryFile());

    // updateHkidsToBeMined(args);

    // QHash<int, QVector<Datapoint> > datapointHash;
    // extractData(tmFile, markerCache, args.binnedHkids, datapointHash, args.allSci);

    // if (!datapointHash.isEmpty())
    // {
    //     if (isMomaMission(tmFile->tmMeta()))
    //         outputMomaFormat(args, tmFile, markerCache, args.binnedHkids, datapointHash);
    //     else if (isGediMission(tmFile->tmMeta()))
    //         outputGediFormat(args, tmFile, markerCache, args.binnedHkids, datapointHash);
    //     else
    //         std::cerr << "Invalid mission: " << tmFile->tmMeta().mission().toStdString();
    // }
    // else
    // {
    //     std::cerr << "No data found for given HKIDs" << std::endl;
    // }

    quit();
}

/**
 * Triggers the application to quit by signaling to the QApplication instance.
 */
void MainClass::quit()
{
    emit finished();
}

// /**
//  * Shortly after quit() is called the QCoreApplication will signal this routine.
//  * This is a good place to delete any objects that were created in the
//  * constructor and/or to stop any threads.
//  */
void MainClass::aboutToQuitApp()
{
}
