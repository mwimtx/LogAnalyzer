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
 * \file dbinterface.h
 *
 * \brief contains the defintion of the DBInterface class.
 *
 * ---
 */

#ifndef DBINTERFACE_H
#define DBINTERFACE_H

#include <QSqlDatabase>



#define C_DB_IFACE_NAME "DBInterface"

#define C_DB_IFACE_DEBUG_MSG 1


/**
 * default values for database access parameters.
 */
#define C_DB_IFACE_USER_NAME    "aduuser"   ///< default database user name
#define C_DB_IFACE_PASSWORD     "neptun"    ///< default database password
#define C_DB_IFACE_DB_NAME      "mcpdb"     ///< default database name to be connected to
#define C_DB_IFACE_HOST         "127.0.0.1" ///< default database host to be connected to
#define C_DB_IFACE_PORT         3306        ///< default database port to be used for the connection



/**
 * \class DBInterface
 *
 * \brief This is the database interface class.
 *
 * It can be used to access a MySQL / ProgreSQL database and execute SQL
 * operations on the target database.
 * It uses the Qt database interface as backend.
 *
 * \author Martin Wilde <Martin.Wilde@apextoolgroup.com>
 * \date 2015-06-03
 */
class DBInterface
{
    protected:

        /**
         * contains the username to be used to access the database.
         */
        QString qstrUserName;

        /**
         * password to access the database.
         */
        QString qstrPassword;

        /**
         * database namespace
         */
        QString qstrDBName;

        /**
         * hostname
         */
        QString qstrHostname;

        /**
         * port number
         */
        int iPortNumber;

        /**
         * connection name
         */
        QString qstrConnName;

        /**
         * database connection class
         */
        QSqlDatabase clDBConn;


    public:

        /**
         * \brief This is the class constructor.
         *
         * It will create an instance of QSqlDatabase and connect to the target database, using
         * username, password, hostname and port number that is handed over as parameter to this
         * function.
         *
         * @param[in] const QString qstrUserNameRef = user name to connect to database
         * @param[in] const QString qstrPasswordRef = password to connect to database
         * @param[in] const QString qstrDBNameRef = name of target database
         * @param[in] const QString qstrHostnameRef = host name to connect to
         * @param[in] const int iPortNumberRef = port number to be used for connection
         * @param[in] const QString qstrConnNameRef = connection name (used inside Qt)
         *
         */
        DBInterface(const QString qstrUserNameRef, const QString qstrPasswordRef, const QString qstrDBNameRef, const QString qstrHostnameRef, const int iPortNumberRef, const QString qstrConnNameRef);

        /**
         * \brief This is the class desrtuctor.
         *
         * It will close the database connection.
         *
         */
        ~DBInterface (void);

        /**
         * \brief Executes a SQL statement on the database.
         *
         * The result is returned as QSqlQuery class instance. The calling class needs to check on its
         * own for the correct results.
         * NOTE, that the index inside the result is defined by the order of the fields to be read, e.g.<br>
         *<br>
         * select index, date, xml_data from system;<br>
         *        0      1     2<br>
         *<br>
         *
         * @param[in] const QString qstrSqlStatement = statement to be executed
         * \return QSqlQuery = result of SQL statement execution
         */
        QSqlQuery execQuery (const QString qstrSqlStatement);

        /**
         * \brief This function can be used to check, if the database is connected.
         *
         * directly returns the value of the QSqlDatabase object.
         *
         * \return bool = true: connected / false: not connected
         */
        bool isConnected (void);

        /**
         * \brief reopens the database connection.
         *
         * ---
         *
         * \return bool = true: connected again / false: failed to reopen connection
         */
        bool reopenDB (void);

};

#endif // DBINTERFACE_H
