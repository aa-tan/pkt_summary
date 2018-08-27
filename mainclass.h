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
// #include <QRegExp>
// #include <QHash> 
// #include <QSet> 
// #include <QtCore/qmath.h>

#include <stdexcept>
#include <stdlib.h>
#include <math.h>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <set>
#include <cassert>
#include <fstream>

struct CmdLineArgs
{
    QString tmFileName;
    QString checkFileName;
    QVector<int> aPids;
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
