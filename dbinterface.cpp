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

/**
 * \file dbinterface.cpp
 *
 * \brief contains the implementation of the DBInterface class.
 *
 * ---
 */


#include "dbinterface.h"
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>

DBInterface::DBInterface(const QString qstrUserNameRef, const QString qstrPasswordRef, const QString qstrDBNameRef, const QString qstrHostnameRef, const int iPortNumberRef, const QString qstrConnNameRef)
{
    this->qstrUserName = qstrUserNameRef;
    this->qstrPassword = qstrPasswordRef;
    this->qstrDBName   = qstrDBNameRef;
    this->qstrHostname = qstrHostnameRef;
    this->qstrConnName = qstrConnNameRef;
    this->iPortNumber  = iPortNumberRef;

    // call reopen function to remove any old database connection
    (void) this->reopenDB();
}


DBInterface::~DBInterface (void)
{
    this->clDBConn.close();
    this->clDBConn = QSqlDatabase();
    this->clDBConn.removeDatabase(this->qstrConnName);
    #ifdef C_DB_IFACE_DEBUG_MSG
        printf ("[%s] database connection closed: \"%s\".\n", C_DB_IFACE_NAME, this->qstrConnName.toStdString().c_str());
    #endif
}


QSqlQuery DBInterface::execQuery (const QString qstrSqlStatement)
{
    QSqlQuery clSQLQuery = this->clDBConn.exec(qstrSqlStatement);

    // check, if database connection is still alive. if not,
    // reopen database connection and execute query again.
    if (clSQLQuery.size() <= 0)
    {
        if ((clSQLQuery.lastError().text ().contains("server has gone away") == true))
        {
            #ifndef MCP_DEBUG
                printf ("[%s]: lost connection to server: %s\n", C_DB_IFACE_NAME, clSQLQuery.lastError().text ().toStdString().c_str());
            #endif

            clSQLQuery.clear();
            if (this->reopenDB() == true)
            {
                clSQLQuery = this->clDBConn.exec(qstrSqlStatement);
            }
        }
    }

    return (clSQLQuery);
}


bool DBInterface::isConnected (void)
{
    return (this->clDBConn.isOpen());
}


bool DBInterface::reopenDB (void)
{
    bool bRetValue = true;

    #ifdef C_DB_IFACE_DEBUG_MSG
        printf ("[%s] re-opening database: \"%s\"\n", C_DB_IFACE_NAME, this->qstrConnName.toStdString().c_str());
    #endif

    this->clDBConn.close();
    this->clDBConn = QSqlDatabase();
    this->clDBConn.close();
    this->clDBConn.removeDatabase(this->qstrConnName);
    this->clDBConn.removeDatabase(this->clDBConn.connectionName());

    clDBConn = QSqlDatabase::addDatabase("QMYSQL", this->qstrConnName);
    clDBConn.setHostName    (this->qstrHostname);
    clDBConn.setPort        (this->iPortNumber);
    clDBConn.setDatabaseName(this->qstrDBName);
    clDBConn.setUserName    (this->qstrUserName);
    clDBConn.setPassword    (this->qstrPassword);

    if (clDBConn.open () == false)
    {
        #ifdef C_DB_IFACE_DEBUG_MSG
            printf ("[%s] database could not be opened: \"%s\"\n", C_DB_IFACE_NAME, this->qstrConnName.toStdString().c_str());
            printf ("[%s] --> %s\n", C_DB_IFACE_NAME,clDBConn.lastError().text().toStdString().c_str());
        #endif
        bRetValue = false;
    }
    else
    {
        #ifdef C_DB_IFACE_DEBUG_MSG
            printf ("[%s] successfully opened database: \"%s\"\n", C_DB_IFACE_NAME, this->qstrConnName.toStdString().c_str());
        #endif
    }

    return (bRetValue);
}
