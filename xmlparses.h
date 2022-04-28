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
 * \file xmlparses.h
 *
 * \brief contains the defintion of the XMLParser class.
 *
 * ---
 */


#ifndef XMLPARSER_H
#define XMLPARSER_H

#include <QString>
#include <QDebug>
#include <QDomDocument>

/**
 * Switch on/off debug output to console.
 */
#define C_XML_PARSER_DEBUG_MSG 1
#define C_XML_PARSER_NAME "XMLParser"


/**
 * \class XMLParser
 *
 * \brief This class to parse all XML files, like XML job files.
 *
 * This is the new base class inside the ADU-08e software to parse all
 * XML files, e.g. job files, status files, ... .
 * It is not based on the deprecated xerces-c API anymore, but on the Qt
 * DOM API.
 * The class shall be used commonly in all ADU-08e system software projects,
 * hence at the same time on CPU board, ADB boards and clock boards.
 *
 * \author Martin Wilde <Martin.Wilde@apextoolgroup.com>
 * \date 2015-08-04
 */
class XMLParser
{
    private:
        /**
         * contains the parsed XML file.
         */
        QDomDocument clXmlDoc;

    public:
        /**
         * \brief This the class contructor.
         *
         * It will create instances of all other needed classes.
         *
         * \author Martin Wilde <Martin.Wilde@apextoolgroup.com>
         * \date 2015-08-04
         */
        XMLParser();

        ~XMLParser();

        /**
         * \brief This function is used to read a XML file from string.
         *
         * It will call the QDomDocument::setContent() function to read the
         * XML file of the String into the QDomDocument clXmlDoc.
         *
         * @param[in] const QString qstrXmlFile = string containing the XML job file
         * \return const bool = true: successfully read file / false: error when parsing file - malformed XML?
         *
         * \author Martin Wilde <Martin.Wilde@apextoolgroup.com>
         * \date 2015-08-04
         */
        bool readXmlFromString (const QString qstrXmlFile);

        /**
         * \brief This function is used to search for a specific XML node.
         *
         * It receives a vector of XML nodes that define the path inside the XML
         * document to the target node. Additionally an Index value can be set to
         * search for a specific node inside a channel list, e.g. inside the channel
         * list:<br>
         *<br>
         * channel id="0"<br>
         * channel id="2"<br>
         * ...<br>
         *<br>
         * If no Index is set,the first node is returned.
         *
         * @param[in] QVector<QString> qvecXmlPath = vector of strings defining the path inside the XML file
         * @param[in] const unsigned int uiIndex = Index (to be used in channel lists), 0xFFFFFFFF if not applied
         * @param[out] QDomNode& clTargetNode = pointer to be set to point to target node
         * \return bool = true: target node was found / false: no such node could be found
         *
         * \author Martin Wilde <Martin.Wilde@apextoolgroup.com>
         * \date 2015-05-27
         */
        bool findXmlNode (QVector<QString> qvecXmlPath, const unsigned int uiIndex, QDomNode &clTargetNode);

        /**
         * \brief This function is used to read the node value of a specific subnode.
         *
         * It receives the name of the target subnode as parameter. Additionally a
         * start node for the search can be defined. If the start node is a NULL node,
         * the subnode is searched on the root node of the document.
         *
         * @param[in] const QString qstrSubNodeName = name of the subnode to be read
         * @param[in] QDomNode& clRootNode = root node, the search shall be started on
         * @param[in] QString& qstrNodeValue = reference to string, the node value shall be stored in
         * \return bool = true: target node was found / false: no such node could be found
         *
         * \author Martin Wilde <Martin.Wilde@apextoolgroup.com>
         * \date 2015-05-27
         */
        bool getSubNodeValue (const QString qstrSubNodeName, QDomNode clRootNode, QString& qstrNodeValue);

        /**
         * \brief This function is used to read the node value of a specific subnode as unsigned int.
         *
         * Wrapper to read unsigned int values.
         *
         * @param[in] const QString qstrSubNodeName = name of the subnode to be read
         * @param[in] QDomNode& clRootNode = root node, the search shall be started on
         * @param[in] unsigned int& uiNodeValue = reference to variable, the node value shall be stored in
         * \return bool = true: target node was found / false: no such node could be found
         *
         * \author Martin Wilde <Martin.Wilde@apextoolgroup.com>
         * \date 2015-05-27
         */
        bool getSubNodeValue (const QString qstrSubNodeName, QDomNode clRootNode, unsigned int& uiNodeValue);

        /**
         * \brief This function is used to read the node value of a specific subnode as int.
         *
         * Wrapper to read int values.
         *
         * @param[in] const QString qstrSubNodeName = name of the subnode to be read
         * @param[in] QDomNode& clRootNode = root node, the search shall be started on
         * @param[in] unsigned int& uiNodeValue = reference to variable, the node value shall be stored in
         * \return bool = true: target node was found / false: no such node could be found
         *
         * \author Martin Wilde <Martin.Wilde@apextoolgroup.com>
         * \date 2015-10-19
         */
        bool getSubNodeValue (const QString qstrSubNodeName, QDomNode clRootNode, int& iNodeValue);

        /**
         * \brief This function is used to read the node value of a specific subnode as int.
         *
         * Wrapper to read boolean values.
         *
         * @param[in] const QString qstrSubNodeName = name of the subnode to be read
         * @param[in] QDomNode& clRootNode = root node, the search shall be started on
         * @param[in] bool& bNodeValue = reference to variable, the node value shall be stored in
         * \return bool = true: target node was found / false: no such node could be found
         *
         * \author Martin Wilde <Martin.Wilde@apextoolgroup.com>
         * \date 2015-10-19
         */
        bool getSubNodeValue (const QString qstrSubNodeName, QDomNode clRootNode, bool& bNodeValue);

        /**
         * \brief This function is used to read the node value of a specific subnode as int.
         *
         * Wrapper to read double values.
         *
         * @param[in] const QString qstrSubNodeName = name of the subnode to be read
         * @param[in] QDomNode& clRootNode = root node, the search shall be started on
         * @param[in] double& dNodeValue = reference to variable, the node value shall be stored in
         * \return bool = true: target node was found / false: no such node could be found
         *
         * \author Martin Wilde <Martin.Wilde@apextoolgroup.com>
         * \date 2015-12-18
         */
        bool getSubNodeValue (const QString qstrSubNodeName, QDomNode clRootNode, double& dNodeValue);

        /**
         * \brief This function is used to write the node value of a specific subnode.
         *
         * It receives the name of the subnode as string, as well as the start node to search for the
         * subnode.
         *
         * @param[in] const QString qstrSubNodeName = name of the subnode to be read
         * @param[in] QDomNode& clRootNode = root node, the search shall be started on
         * @param[in] const QString qstrNodeValue = new value for node
         * \return bool = true: target node was found / false: no such node could be found
         *
         * \author Martin Wilde <Martin.Wilde@apextoolgroup.com>
         * \date 2016-06-27
         */
        bool setSubNodeValue (const QString qstrSubNodeName, QDomNode *pclRootNode, const QString qstrNodeValue);

        /**
         * \brief This function is used to write the XML file to a string.
         *
         * ---
         *
         * @param[in] QString& qstrXmlFile = reference to string the XML shall be written to.
         * \return bool = true: export OK: false: failed to export XML file to string.
         *
         * \author Martin Wilde <Martin.Wilde@apextoolgroup.com>
         * \date 2016-06-27
         */
        bool writeXmlToString (QString& qstrXmlFile);

        /**
         * \brief This function is used to read an attribute value from the node.
         *
         * Tries to find the desired attribute and returns the attributes value, if a fitting
         * one can be found.
         *
         * @param[in] const QString qstrAttributeName = name of attribute to be read
         * @param[in] QDomNode& clNode = node the attribute shall be searched for
         * @param[in] QString& qstrValue = attribute value
         * \return bool = true: attribute was found / fales: no such attribute
         *
         * \author Martin Wilde <Martin.Wilde@apextoolgroup.com>
         * \date 2016-06-15
         */
        bool getAttributeValue (const QString qstrAttributeName, QDomNode& clNode, QString& qstrValue);

        /**
         * \brief This function is used to get a list of all subnodes of a given name.
         *
         * It will return a DomNodeList of all the nodes that are found inside the XML file
         * with the defined node name.
         *
         * @param[in] const QString qstrNodeName = name of the node to be searched for
         * @param[in] QDomNode* pclRootNode = NULL = root node to start search at. use empty node to start at root.
         * \return QDomNodeList = list of all nodes of the given name
         *
         * \author Martin Wilde <Martin.Wilde@apextoolgroup.com>
         * \date 2016-04-05
         */
        QDomNodeList findNodes (const QString qstrNodeName, QDomNode* pclRootNode = NULL);

        /**
         * \brief This function is used to insert a XML File as subpart into the existing file.
         *
         * ---
         *
         * @param[in] QDomNode* pclNode = pointer to insert XML Subfile
         * @param[in] const QString& qstrXmlFile = XML file to be inserted
         * \return bool = true: OK, false: failed to insert XML file
         *
         * \author Martin Wilde <Martin.Wilde@apextoolgroup.com>
         * \date 2016-09-07
         */
        bool insertXmlFile (QDomNode* pclNode, const QString& qstrXmlFile);

};

#endif // XMLPARSER_H
