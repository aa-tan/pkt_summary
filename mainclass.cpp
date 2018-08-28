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

QVector<int> extractHkIds(const QString& hkidArg)
{
    QVector<int> hkids;
    const QStringList tokens = hkidArg.split(',', QString::SkipEmptyParts);
    foreach (const QString & token, tokens)
    {
        bool success; 
        int hkid = token.toInt(&success);

        if (success)
        {
            if (!hkids.contains(hkid))
                hkids.push_back(hkid); 
        }
        else
        {
            // Assume a range of HKIDs, such as "5-10"
            QStringList highAndLow = token.split('-');
            if (highAndLow.size() != 2)
            {
                qDebug() << token << " <-- invalid range";
                throw std::invalid_argument("invalid range of HKIDs");  
            }

            bool low_is_ok;
            bool high_is_ok;
            int low_hkid  = highAndLow[0].toInt(&low_is_ok); 
            int high_hkid = highAndLow[1].toInt(&high_is_ok); 

            if ((!low_is_ok || !high_is_ok) || (low_hkid >= high_hkid))
            {
                qDebug() << token << " <-- invalid range";
                throw std::invalid_argument("invalid range of HKIDs");  
            }

            for (int curr_hkid = low_hkid; curr_hkid <= high_hkid; curr_hkid++)
            {
                if (!hkids.contains(curr_hkid))
                    hkids.push_back(curr_hkid); 
            }
        }
    }

    return hkids;
}


CmdLineArgs parseArguments(const QStringList & arguments)
{
    QCommandLineParser parser;
    parser.setApplicationDescription("Summarizes TM packet data");
    parser.addHelpOption();
    parser.addVersionOption();

    const QCommandLineOption fileOption(QStringList() << "f" << "file",
                                        "Path to TM file to extract data from", "FILE");
    parser.addOption(fileOption);

    const QCommandLineOption apidOption(QStringList() << "a" << "apid", "List of APIDs to check if exists in TM file. Ex. 1,2,3 or 1-3", "IDs");
    parser.addOption(apidOption);

    parser.process(arguments);

    CmdLineArgs results;

    // Sets tmFileName to flagged name
    results.tmFileName = parser.value(fileOption);
    // No option provided, search the current directory
    if (results.tmFileName.isEmpty())
    {
       TmMeta tmMeta(QDir::current().absolutePath());
       if (tmMeta.isValid())
          results.tmFileName = tmMeta.tmFileName();
    }
    //Checks for existance of file
    const QFileInfo tmFileInfo(results.tmFileName);
    if (!tmFileInfo.exists() || !tmFileInfo.isFile())
    {
        std::cerr << "tmfile does not exist: " << results.tmFileName << "\n\n";
        parser.showHelp(1);
    }
    
    results.aPids = extractHkIds(parser.value(apidOption));
    return results;
}

bool isMomaMission(const TmMeta & tmMeta)
{
    return tmMeta.mission().compare("moma", Qt::CaseInsensitive) == 0;
}

bool isGediMission(const TmMeta & tmMeta)
{
    return tmMeta.mission().compare("gedi", Qt::CaseInsensitive) == 0;
}

GseLib::TelemetryFilePtr createTmFile(const QString& tmFileName)
{
    QFileInfo tmFileInfo(tmFileName);
    TmMeta tmMeta(tmFileInfo.absolutePath());
    if (!tmMeta.isValid())
    {
       std::cerr << "Valid tm.meta is required" << std::endl;
       exit(1);
    }

    if (isMomaMission(tmMeta))
    {
       GseLib::Config699::setDefaultGroup(GseLib::Config699::MOMA_GROUP);
       DataInterpretation::setFactory(&momaDataInterpFactory);
       DataInterpretation::current().loadDatabase(tmMeta.patchFiles());
       TelemetryFile::setSharedCache(new MomGse::MomTelemetryCache);
       TelemetryFile::sharedCache()->setPacketFormat(GseLib::TelemetryCacheBase::TomPacketFormat);
       TelemetryFile::setSharedCacheFile(tmFileInfo.absoluteFilePath().toStdString());
    }
    else if (isGediMission(tmMeta))
    {
       GseLib::Config699::setDefaultGroup(GseLib::Config699::GEDI_GROUP);
       DataInterpretation::setFactory(&gediDataInterpFactory);
       DataInterpretation::current().loadDatabase(tmMeta.patchFiles());
       TelemetryFile::setSharedCache(new GediGse::GediTelemetryCache);
       TelemetryFile::sharedCache()->setPacketFormat(GseLib::TelemetryCacheBase::CcsdsPacketFormat);
       TelemetryFile::setSharedCacheFile(tmFileInfo.absoluteFilePath().toStdString());
    }
    else
    {
       std::cerr << "Unsupported mission: " << tmMeta.mission().toStdString() << std::endl;
       exit(1);
    }

    return GseLib::TelemetryFilePtr(new GseLib::TelemetryFile);
}


void summarizeData(GseLib::TelemetryFilePtr tmFile, QVector<int> aPids)
{
    //
    // Creates a map type_map that stores the type and # of ocurrences
    // Reads from tmFile and adds packet to map
    //
    TMPacketPtr pkt;
    std::map<int, int> type_map;
    std::vector<int> v;
    while ((pkt = tmFile->getNext()) != tmFile->end())
    {
        int type = pkt->type();
        if (type_map.count(type)){
            type_map[type] ++;
        }
        else {
            type_map[type] = 1;
        }
    }
    // If aPids is empty, output all found types
    // Else, iterate through aPids, check if it exists in type_map, output
    // If not found, exit()
    if(aPids.isEmpty()){
        for(std::map<int,int>::iterator it = type_map.begin(); it != type_map.end(); ++it) {
            std::cout << it->first << ": " << type_map[it->first] << "\n";
        }
    }
    else {
        bool flag = false;
        for(int i=0; i < aPids.size(); i++){
            if(type_map.count(aPids[i])){
                std::cout << aPids[i] << ": " << type_map[aPids[i]] << "\n";
            }
            else {
                flag = true;
                std::cout << aPids[i] << ": " << 0 << "\n";
            }
        }
        if(flag){
            return false;
        }
    }
    return true;
}
 
MainClass::MainClass(QObject *parent) :
    QObject(parent)
{
}

void MainClass::run()
{
    CmdLineArgs args = parseArguments(qApp->arguments());
    GseLib::TelemetryFilePtr tmFile = createTmFile(args.tmFileName);
    if (!summarizeData(tmFile, args.aPids)){
        ::exit(1);
    }
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
