/**
 * @file
 * @original_author Mark Pallone
 * @date   December, 2015
 * @modified_author Aaron Tan
 * @date   August, 2018
 */

#ifndef MAINCLASS_H
#define MAINCLASS_H

#include <qcstr.hpp>
#include <telemetryfile.hpp>
#include <momadatai.hpp>
#include <gedidatai.hpp>
#include <config699.hpp>
#include <tmmeta.hpp>
#include <mkrcache.hpp>
#include <metamarker.hpp>

#include <QFileInfo>
#include <QDir>
#include <QFile> 
#include <QString> 
#include <QStringList>
#include <QDebug>
#include <QCoreApplication>
#include <QRegExp>
#include <QHash> 
#include <QSet> 
#include <QtCore/qmath.h>

#include <iostream>
#include <stdexcept>
#include <stdlib.h>
#include <math.h>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <set>
#include <cassert>


struct Datapoint
{
    Datapoint() :
       umkid(0), t_utc(0), t_rel(0), value(0), hkid(0), pktIndex(0) {}
    Datapoint(unsigned long long umkid, double t_utc, double t_rel, double value,
              int hkid, unsigned int pktIndex) :
       umkid(umkid), t_utc(t_utc), t_rel(t_rel), value(value), hkid(hkid), pktIndex(pktIndex) {}
    unsigned long long umkid; 
    double t_utc; 
    double t_rel; 
    double value;
    int hkid; 
    unsigned int pktIndex; 
};

// For sorting based on timestamp: 
inline bool datapointLessThan(const Datapoint& d1, const Datapoint& d2 ) 
{ return d1.t_rel < d2.t_rel; }

struct XinaMarkerBinDatapoint
{
    XinaMarkerBinDatapoint() : 
        t_utc(0), t_rel(0), avg(0), min(0), max(0), total(0), numSamples(0) {}
    double t_utc;
    double t_rel;
    double avg;
    double min;
    double max;
    double total;
    int numSamples;
    MomGse::MarkerData* markerDataPtr; 
};

struct XinaMetaMarkerBinDatapoint
{
    XinaMetaMarkerBinDatapoint() : 
        avg(0), min(0), max(0), total(0), numSamples(0) {}
    double avg;
    double min;
    double max;
    double total;
    int numSamples;
};

struct TimeBinDatapoint
{
    TimeBinDatapoint() : 
        t_utc(0), t_rel(0), avg(0), min(0), max(0), total(0), numSamples(0) {}
    double t_utc;
    double t_rel;
    double avg;
    double min;
    double max;
    double total;
    int numSamples;
};

struct CmdLineArgs
{
    QString tmFileName;
    QString outputDir;
    QVector<int> fullyMinedHkids;
    QVector<int> deltaHkids;
    QVector<int> binnedHkids;
    bool allSci;
};

/**
 * The main entry point for the application.
 */
class MainClass : public QObject
{
    Q_OBJECT
public:
    explicit MainClass(QObject *parent = 0);

signals:
    void finished();

public slots:
    void run();
    void aboutToQuitApp();

private:
    void quit();
};

#endif // MAINCLASS_H
