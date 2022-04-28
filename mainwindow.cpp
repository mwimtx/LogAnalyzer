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

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dbinterface.h"
#include "xmlparses.h"

#include <QSqlQuery>
#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QDomDocument>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    qvecHWStatus.clear ();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pbReadLog_clicked(void)
{
    QSqlQuery   clQuery;
    int         iCounter;
    QDomNode    clTargetNode;
    QString     qstrTemp1;
    QString     qstrTemp2;
    QVector<QString> qvecXmlPath;

    // HW Status
    qstrTemp1 = ui->teIP->toPlainText ();
    DBInterface* pclDB = new DBInterface ("aduuser", "neptun", "mcpdb", qstrTemp1, 3306, "mcpdb");
    clQuery = pclDB->execQuery ("select message from log where component=\'HW_STATUS\' order by timestamp asc;");

    if (clQuery.size() <= 0)
    {
        delete (pclDB);
        pclDB = new DBInterface ("aduuser", "neptun", "mcpdb07", qstrTemp1, 3306, "mcpdb07");
        clQuery = pclDB->execQuery ("select message from log where component=\'HW_STATUS\' order by timestamp asc;");
    }

    if (clQuery.size() > 0)
    {
        clQuery.first();
        this->qvecHWStatus.clear();
        for (iCounter = 0; iCounter < clQuery.size(); iCounter++)
        {
            //qDebug () << "message:" << clQuery.value(0);
            XMLParser*  pclXML = new XMLParser ();
            sHWStatus_t sMsg;

            pclXML->readXmlFromString(clQuery.value(0).toString());

            clTargetNode.clear();
            pclXML->getSubNodeValue ("date", clTargetNode, qstrTemp1);
            pclXML->getSubNodeValue ("time", clTargetNode, qstrTemp2);
            qstrTemp1 += " " + qstrTemp2;
            sMsg.sTimeStamp = QDateTime::fromString(qstrTemp1, "yyyy-MM-dd hh:mm:ss").toUTC().toTime_t();

            clTargetNode.clear();
            pclXML->getSubNodeValue ("temp", clTargetNode, qstrTemp1);
            sMsg.dTemperatureSystem = qstrTemp1.toDouble();

            clTargetNode.clear();
            pclXML->getSubNodeValue ("temp_sensor", clTargetNode, qstrTemp1);
            sMsg.dTemperatureSensor = qstrTemp1.toDouble();

            clTargetNode.clear();
            pclXML->getSubNodeValue ("batt_volt", clTargetNode, qstrTemp1);
            sMsg.dBattVoltage = qstrTemp1.toDouble() / 10.0;

            clTargetNode.clear();
            pclXML->getSubNodeValue ("batt1_curr", clTargetNode, qstrTemp1);
            sMsg.dBattCurr1 = qstrTemp1.toDouble() / 100.0;

            clTargetNode.clear();
            pclXML->getSubNodeValue ("batt2_curr", clTargetNode, qstrTemp1);
            sMsg.dBattCurr2 = qstrTemp1.toDouble() / 100.0;

            QDateTime clStartDate (this->ui->startDate->date());
            QDateTime clStopDate  (this->ui->stopDate->date ());
            if ((sMsg.sTimeStamp > clStartDate.toSecsSinceEpoch()) &&
                (sMsg.sTimeStamp < clStopDate.toSecsSinceEpoch ()))
            {
                this->qvecHWStatus.push_back(sMsg);
            }

            delete (pclXML);

            clQuery.next();
        }
        delete (pclDB);


        // GPS Status
        qstrTemp1 = ui->teIP->toPlainText ();
        DBInterface* pclDB = new DBInterface ("aduuser", "neptun", "mcpdb", qstrTemp1, 3306, "mcpdb");
        clQuery = pclDB->execQuery ("select message from log where component=\'GPS_STATUS\' order by timestamp asc;");
        if (clQuery.size() > 0)
        {
            clQuery.first();
            this->qvecGPStatus.clear();
            for (iCounter = 0; iCounter < clQuery.size(); iCounter++)
            {
                //qDebug () << "message:" << clQuery.value(0);
                XMLParser*  pclXML = new XMLParser ();
                sGPStatus_t sMsg;

                pclXML->readXmlFromString(clQuery.value(0).toString());

                clTargetNode.clear();
                pclXML->getSubNodeValue ("date", clTargetNode, qstrTemp1);
                pclXML->getSubNodeValue ("time", clTargetNode, qstrTemp2);
                qstrTemp1 += " " + qstrTemp2;
                sMsg.sTimeStamp = QDateTime::fromString(qstrTemp1, "yyyy-MM-dd hh:mm:ss").toUTC().toTime_t();

                clTargetNode.clear();
                pclXML->getSubNodeValue ("num_sats", clTargetNode, qstrTemp1);
                sMsg.dNumSats = qstrTemp1.toDouble();

                clTargetNode.clear();
                pclXML->getSubNodeValue ("sync_state", clTargetNode, qstrTemp1);
                sMsg.dSyncState = qstrTemp1.toDouble();

                this->qvecGPStatus.push_back(sMsg);

                delete (pclXML);

                clQuery.next();

            }
        }
        delete (pclDB);


        this->plotHwStatusGraphs (this->qvecHWStatus);
        this->plotGPSStatusGraphs(this->qvecGPStatus);

    }
}


void MainWindow::on_pbReadFromMeasDoc_clicked (void)
{
    int iCounter;
    QFile qfMeasDoc;
    QString qstrMeasDocName;
    QString qstrMeasDoc;
    QString qstrTemp1;
    QString qstrTemp2;
    QDomDocument qddXml;
    QDomNodeList qdlNodes1;
    QDomNodeList qdlNodes2;
    QDomNodeList qdlNodes3;

    qstrMeasDocName = QFileDialog::getOpenFileName (this, "Open MeasDoc XML", "", "*.xml (*.xml)");
    qfMeasDoc.setFileName(qstrMeasDocName);
    if (qfMeasDoc.open(QIODevice::ReadOnly) == false)
    {
        QMessageBox::warning(this, "Unable to open MeasDoc XML", "Unable to open MeasDoc XML file!");
    }
    else
    {
        qstrMeasDoc.clear ();
        qstrMeasDoc = qfMeasDoc.readAll();

        qddXml.clear();
        qddXml.setContent(qstrMeasDoc);
        qdlNodes1 = qddXml.elementsByTagName("message");

        this->qvecGPStatus.clear();
        this->qvecHWStatus.clear();
        for (iCounter = 0; iCounter < qdlNodes1.size(); iCounter++)
        {
            qdlNodes2 = qdlNodes1.at(iCounter).toElement().elementsByTagName("component");
            if (qdlNodes2.size() > 0)
            {
                if (qdlNodes2.at(0).firstChild().nodeValue().contains("HW_STATUS"))
                {
                    sHWStatus_t sMsg;

                    // timestamp
                    qdlNodes2 = qdlNodes1.at(iCounter).toElement().elementsByTagName("date");
                    if (qdlNodes2.size() > 0)
                    {
                        qstrTemp1 = qdlNodes2.at(0).firstChild().nodeValue();
                    }
                    qdlNodes2 = qdlNodes1.at(iCounter).toElement().elementsByTagName("time");
                    if (qdlNodes2.size() > 0)
                    {
                        qstrTemp2 = qdlNodes2.at(0).firstChild().nodeValue();
                    }
                    qstrTemp1 += " " + qstrTemp2;
                    sMsg.sTimeStamp = QDateTime::fromString(qstrTemp1, "yyyy-MM-dd hh:mm:ss").toUTC().toTime_t();

                    // battery voltage
                    qdlNodes2 = qdlNodes1.at(iCounter).toElement().elementsByTagName("batt_volt");
                    if (qdlNodes2.size() > 0)
                    {
                        qstrTemp1 = qdlNodes2.at(0).firstChild().nodeValue();
                    }
                    sMsg.dBattVoltage = qstrTemp1.toDouble() / 10.0;

                    // temperatrue
                    qdlNodes2 = qdlNodes1.at(iCounter).toElement().elementsByTagName("temp");
                    if (qdlNodes2.size() > 0)
                    {
                        qstrTemp1 = qdlNodes2.at(0).firstChild().nodeValue();
                    }
                    sMsg.dTemperatureSystem = qstrTemp1.toDouble();

                    // temperatrue
                    qdlNodes2 = qdlNodes1.at(iCounter).toElement().elementsByTagName("temp_sensor");
                    if (qdlNodes2.size() > 0)
                    {
                        qstrTemp1 = qdlNodes2.at(0).firstChild().nodeValue();
                    }
                    sMsg.dTemperatureSensor = qstrTemp1.toDouble();

                    // battery current 1
                    qdlNodes2 = qdlNodes1.at(iCounter).toElement().elementsByTagName("batt1_curr");
                    if (qdlNodes2.size() > 0)
                    {
                        qstrTemp1 = qdlNodes2.at(0).firstChild().nodeValue();
                    }
                    sMsg.dBattCurr1 = qstrTemp1.toDouble() / 100.0;

                    // battery current 2
                    qdlNodes2 = qdlNodes1.at(iCounter).toElement().elementsByTagName("batt2_curr");
                    if (qdlNodes2.size() > 0)
                    {
                        qstrTemp1 = qdlNodes2.at(0).firstChild().nodeValue();
                    }
                    sMsg.dBattCurr2 = qstrTemp1.toDouble() / 100.0;

                    this->qvecHWStatus.push_back(sMsg);
                }
                else if (qdlNodes2.at(0).firstChild().nodeValue().contains("GPS_STATUS"))
                {
                    sGPStatus_t sMsg;

                    // timestamp
                    qdlNodes2 = qdlNodes1.at(iCounter).toElement().elementsByTagName("date");
                    if (qdlNodes2.size() > 0)
                    {
                        qstrTemp1 = qdlNodes2.at(0).firstChild().nodeValue();
                    }
                    qdlNodes2 = qdlNodes1.at(iCounter).toElement().elementsByTagName("time");
                    if (qdlNodes2.size() > 0)
                    {
                        qstrTemp2 = qdlNodes2.at(0).firstChild().nodeValue();
                    }
                    qstrTemp1 += " " + qstrTemp2;
                    sMsg.sTimeStamp = QDateTime::fromString(qstrTemp1, "yyyy-MM-dd hh:mm:ss").toUTC().toTime_t();

                    // number of satellites
                    qdlNodes2 = qdlNodes1.at(iCounter).toElement().elementsByTagName("num_sats");
                    if (qdlNodes2.size() > 0)
                    {
                        qstrTemp1 = qdlNodes2.at(0).firstChild().nodeValue();
                    }
                    sMsg.dNumSats = qstrTemp1.toDouble();

                    // sync status
                    qdlNodes2 = qdlNodes1.at(iCounter).toElement().elementsByTagName("sync_state");
                    if (qdlNodes2.size() > 0)
                    {
                        qstrTemp1 = qdlNodes2.at(0).firstChild().nodeValue();
                    }
                    sMsg.dSyncState = qstrTemp1.toDouble();

                    this->qvecGPStatus.push_back(sMsg);
                }
            }
        }

        this->plotHwStatusGraphs (this->qvecHWStatus);
        this->plotGPSStatusGraphs(this->qvecGPStatus);
    }
}


bool MainWindow::plotHwStatusGraphs (QVector<sHWStatus_t> qvecHWStatus)
{
    bool            bRetValue = true;
    int             iCounter;
    QVector<double> qvecXAxis;
    QVector<double> qvecYAxis;
    unsigned int    uiTemp;

    QPen clPen = QPen (Qt::blue);
    clPen.setWidth (3);

    // get time values
    qvecXAxis.clear();
    for (iCounter = 0; iCounter < qvecHWStatus.size(); iCounter++)
    {
        uiTemp = qvecHWStatus [iCounter].sTimeStamp - qvecHWStatus [0].sTimeStamp;
        qvecXAxis.push_back(uiTemp / 86400.0);
    }

    // plot system temperarure
    qvecYAxis.clear();
    for (iCounter = 0; iCounter < qvecHWStatus.size(); iCounter++)
    {
        qvecYAxis.push_back(qvecHWStatus [iCounter].dTemperatureSystem);
    }
    this->drawPlot(ui->plotTemperature,
                   "System Temperature [degree C]",
                   "Time Since Start [days]",
                   "System Temperature [degree C]",
                   qvecXAxis, qvecYAxis);

    // plot sensor temperarure
    qvecYAxis.clear();
    for (iCounter = 0; iCounter < qvecHWStatus.size(); iCounter++)
    {
        qvecYAxis.push_back(qvecHWStatus [iCounter].dTemperatureSensor);
    }
    this->drawPlot(ui->plotTemperatureSensor,
                   "Sensor Temperature [degree C]",
                   "Time Since Start [days]",
                   "Sensor Temperature [degree C]",
                   qvecXAxis, qvecYAxis);

    // plot battery voltage
    qvecYAxis.clear();
    for (iCounter = 0; iCounter < qvecHWStatus.size(); iCounter++)
    {
        qvecYAxis.push_back(qvecHWStatus [iCounter].dBattVoltage);
    }
    this->drawPlot(ui->plotBatteryVoltage,
                   "Battery Voltage [V]",
                   "Time Since Start [days]",
                   "Battery Voltage [V]",
                   qvecXAxis, qvecYAxis);

    // plot power consumption
    qvecYAxis.clear();
    for (iCounter = 0; iCounter < qvecHWStatus.size(); iCounter++)
    {
        qvecYAxis.push_back(qvecHWStatus [iCounter].dBattCurr1);
        qvecYAxis.back() += qvecHWStatus [iCounter].dBattCurr2;
        qvecYAxis.back() *= qvecHWStatus [iCounter].dBattVoltage;
    }
    this->drawPlot(ui->plotPowerConsumption,
                   "Power Consumption [W]",
                   "Time Since Start [days]",
                   "Power Consumption [W]",
                   qvecXAxis, qvecYAxis);

    return (bRetValue);
}


bool MainWindow::plotGPSStatusGraphs (QVector<sGPStatus_t> qvecGPStatus)
{
    bool            bRetValue = true;
    int             iCounter;
    QVector<double> qvecXAxis;
    QVector<double> qvecYAxis1;
    QVector<double> qvecYAxis2;
    unsigned int    uiTemp;

    QPen clPen = QPen (Qt::blue);
    clPen.setWidth (3);

    // get time values
    qvecXAxis.clear();
    for (iCounter = 0; iCounter < qvecGPStatus.size(); iCounter++)
    {
        uiTemp = qvecGPStatus [iCounter].sTimeStamp - qvecGPStatus [0].sTimeStamp;
        qvecXAxis.push_back(uiTemp / 86400.0);
    }

    qvecYAxis1.clear();
    for (iCounter = 0; iCounter < qvecGPStatus.size(); iCounter++)
    {
        qvecYAxis1.push_back(qvecGPStatus [iCounter].dNumSats);
        qvecYAxis2.push_back(qvecGPStatus [iCounter].dSyncState);
    }

    ui->plotGPS->clearGraphs ();
    ui->plotGPS->addGraph    ();
    ui->plotGPS->graph       (0)->addData (qvecXAxis, qvecYAxis1);
    ui->plotGPS->graph       (0)->setName ("Number of Satellites");
    ui->plotGPS->graph       (0)->setVisible (true);
    ui->plotGPS->graph       (0)->setPen   (clPen);
    ui->plotGPS->graph       (0)->setBrush (Qt::NoBrush);
    ui->plotGPS->graph       (0)->rescaleAxes(true);
    ui->plotGPS->graph       (0)->rescaleAxes(true);
    ui->plotGPS->graph       (0)->keyAxis   ()->setLabel ("Time Since Start [days]");
    ui->plotGPS->graph       (0)->valueAxis ()->setLabel ("");

    ui->plotGPS->addGraph    ();
    clPen.setColor(Qt::green);
    ui->plotGPS->graph       (1)->addData (qvecXAxis, qvecYAxis2);
    ui->plotGPS->graph       (1)->setName ("Sync Status");
    ui->plotGPS->graph       (1)->setVisible (true);
    ui->plotGPS->graph       (1)->setPen   (clPen);
    ui->plotGPS->graph       (1)->setBrush (Qt::NoBrush);
    ui->plotGPS->graph       (1)->rescaleAxes(true);
    ui->plotGPS->graph       (1)->rescaleAxes(true);
    ui->plotGPS->graph       (1)->keyAxis   ()->setLabel ("Time Since Start [days]");
    ui->plotGPS->graph       (1)->valueAxis ()->setLabel ("");

    ui->plotGPS->xAxis->setScaleType (QCPAxis::stLinear);
    ui->plotGPS->yAxis->setScaleType (QCPAxis::stLinear);
    ui->plotGPS->yAxis->setAutoSubTicks   (true);
    ui->plotGPS->legend->removeItem(ui->plotGPS->legend->itemCount()-1);
    if (ui->plotGPS->plotLayout()->rowCount () <= 1)
    {
        ui->plotGPS->plotLayout()->insertRow (0);
        ui->plotGPS->plotLayout()->addElement(0, 0, new QCPPlotTitle(ui->plotGPS, "GPS Status"));
    }
    ui->plotGPS->legend->setVisible      (true);
    ui->plotGPS->legend->setFont(QFont   ("Helvetica",9));
    ui->plotGPS->setLocale               (QLocale(QLocale::English, QLocale::UnitedKingdom));

    ui->plotGPS->setInteractions  (QCP::iRangeDrag | QCP::iRangeZoom);
    ui->plotGPS->xAxis->rescale();

    ui->plotGPS->repaint     ();
    ui->plotGPS->replot      ();

    return (bRetValue);
}


bool MainWindow::drawPlot (QCustomPlot* pclPlot, const QString qstrHeading, const QString qstrXAxis, const QString qstrYAxis, QVector<double> qvecXAxis, QVector<double> qvecYAxis)
{
    bool            bRetValue = true;
    int             iCounter;

    QPen clPen = QPen (Qt::blue);
    clPen.setWidth (3);

    pclPlot->clearGraphs ();
    pclPlot->addGraph    ();
    pclPlot->graph       (0)->addData (qvecXAxis, qvecYAxis);
    pclPlot->graph       (0)->setName (qstrYAxis);
    pclPlot->graph       (0)->setVisible (true);
    pclPlot->graph       (0)->setPen   (clPen);
    pclPlot->graph       (0)->setBrush (Qt::NoBrush);
    pclPlot->graph       (0)->rescaleAxes(true);
    pclPlot->graph       (0)->rescaleAxes(true);
    pclPlot->graph       (0)->keyAxis   ()->setLabel (qstrXAxis);
    pclPlot->graph       (0)->valueAxis ()->setLabel (qstrYAxis);
    pclPlot->xAxis->setScaleType (QCPAxis::stLinear);
    pclPlot->yAxis->setScaleType (QCPAxis::stLinear);
    pclPlot->yAxis->setAutoSubTicks   (true);
    pclPlot->legend->removeItem(pclPlot->legend->itemCount()-1);
    if (pclPlot->plotLayout()->rowCount () <= 1)
    {
        pclPlot->plotLayout()->insertRow (0);
        pclPlot->plotLayout()->addElement(0, 0, new QCPPlotTitle(pclPlot, qstrHeading));
    }
    pclPlot->legend->setVisible      (true);
    pclPlot->legend->setFont(QFont   ("Helvetica",9));
    pclPlot->setLocale               (QLocale(QLocale::English, QLocale::UnitedKingdom));

    pclPlot->setInteractions  (QCP::iRangeDrag | QCP::iRangeZoom);
    pclPlot->xAxis->rescale();

    pclPlot->repaint     ();
    pclPlot->replot      ();

    return (bRetValue);
}
