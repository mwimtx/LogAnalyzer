/***************************************************************************
**                                                                        **
**  Log Analyzer - Metronix ADU-XX system log analyzer                    **
**  Copyright (C) 2019-2022 metronix GmbH                                 **
**                                                                        **
**  This program is free software: you can redistribute it and/or modify  **
**  it under the terms of the GNU General Public License as published by  **
**  the Free Software Foundation, either version 3 of the License, or     **
**  (at your option) any later version.                                   **
**                                                                        **
**  This program is distributed in the hope that it will be useful,       **
**  but WITHOUT ANY WARRANTY; without even the implied warranty of        **
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         **
**  GNU General Public License for more details.                          **
**                                                                        **
**  You should have received a copy of the GNU General Public License     **
**  along with this program.  If not, see http://www.gnu.org/licenses/.   **
**                                                                        **
** Parts of the software are linked against the Open Source Version of Qt **
** Qt source code copy is available under https://www.qt.io               **
** Qt source code modifications are non-proprietary,no modifications made **
** https://www.qt.io/licensing/open-source-lgpl-obligations               **
****************************************************************************
**           Author: metronix geophysics                                  **
**  Website/Contact: www.metronix.de                                      **
**                                                                        **
**                                                                        **
****************************************************************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "qcustomplot.h"

namespace Ui {
class MainWindow;
}

typedef struct
{
    double dBattVoltage;
    double dBattCurr1;
    double dBattCurr2;
    double dTemperatureSystem;
    double dTemperatureSensor;
    time_t sTimeStamp;
} sHWStatus_t;


typedef struct
{
    double dNumSats;
    double dSyncState;
    time_t sTimeStamp;
} sGPStatus_t;

class MainWindow : public QMainWindow
{
    Q_OBJECT

private slots:
    void on_pbReadLog_clicked (void);
    void on_pbReadFromMeasDoc_clicked (void);

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    QVector<sHWStatus_t> qvecHWStatus;
    QVector<sGPStatus_t> qvecGPStatus;

    bool plotHwStatusGraphs  (QVector<sHWStatus_t> qvecHWStatus);
    bool plotGPSStatusGraphs (QVector<sGPStatus_t> qvecGPStatus);

    bool drawPlot (QCustomPlot* pclPlot, const QString qstrHeading, const QString qstrXAxis, const QString qstrYAxis, QVector<double> qvecXAxis, QVector<double> qvecYAxis);

};

#endif // MAINWINDOW_H
