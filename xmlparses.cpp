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
 * \file xmlparses.cpp
 *
 * \brief contains the implementation of the XMLParser class.
 *
 * ---
 */


#include "xmlparses.h"

XMLParser::XMLParser()
{
}


XMLParser::~XMLParser()
{
    this->clXmlDoc.clear();
}


bool XMLParser::readXmlFromString (const QString qstrXmlFile)
{
    // declaration of variables
    bool bRetValue = true;

    if (qstrXmlFile.size() == 0)
    {
        bRetValue = false;
    }
    else
    {
        try
        {
            this->clXmlDoc.clear();
            bRetValue = this->clXmlDoc.setContent(qstrXmlFile);
        }
        catch (...)
        {
            bRetValue = false;
        }
    }

    return (bRetValue);
}


bool XMLParser::findXmlNode (QVector<QString> qvecXmlPath, const unsigned int uiIndex, QDomNode& clTargetNode)
{
    // declaration of variables
    bool          bRetValue = true;
    unsigned int  uiPathCount;
    unsigned int  uiIndexCount;
    int           iErrCondition = 0;
    QDomNode      clTmpNode;
    QDomNodeList  clTmpNodeList;

    clTargetNode.clear();
    clTmpNode.clear();

    if (qvecXmlPath.size() > 0)
    {
        for (uiPathCount = 0; uiPathCount < (unsigned int)qvecXmlPath.size(); uiPathCount++)
        {
            if (clTmpNode.isNull() == true)
            {
                clTmpNodeList = this->clXmlDoc.elementsByTagName(qvecXmlPath[uiPathCount]);
            }
            else
            {
                clTmpNodeList = clTmpNode.toElement().elementsByTagName(qvecXmlPath[uiPathCount]);
            }

            if (clTmpNodeList.size() > 0)
            {
                if (((int)uiPathCount == (qvecXmlPath.size() - 1)) &&
                    (uiIndex     != 0xFFFFFFFF))
                {
                    for (uiIndexCount = 0; uiIndexCount < (unsigned int)clTmpNodeList.size(); uiIndexCount++)
                    {
                        if (clTmpNodeList.at(uiIndexCount).attributes().namedItem("id").nodeValue().toUInt() == uiIndex)
                        {
                            clTargetNode = clTmpNodeList.at(uiIndexCount);
                        }
                    }

                    if (clTargetNode.isNull() == true)
                    {
                        bRetValue = false;
                        iErrCondition = -1;
                    }
                }
                else
                {
                    clTmpNode = clTmpNodeList.at(0);
                }

            }
            else
            {
                bRetValue = false;
                iErrCondition = -2;
                break;
            }
        }
    }
    else
    {
        bRetValue = false;
        iErrCondition = -3;
    }

    if (bRetValue == false)
    {
        #ifdef C_XML_PARSER_DEBUG_MSG
            printf ("[%s] findXmlNode (...) error condition: %i\n", C_XML_PARSER_NAME, iErrCondition);
        #endif
    }



    return (bRetValue);
}


bool XMLParser::getSubNodeValue (const QString qstrSubNodeName, QDomNode clRootNode, QString& qstrNodeValue)
{
    // declaration of variables
    bool          bRetValue = true;
    QDomNodeList  clTmpNodeList;

    qstrNodeValue.clear();

    if (clRootNode.isNull() == true)
    {
        clTmpNodeList = this->clXmlDoc.elementsByTagName(qstrSubNodeName);
    }
    else
    {
        clTmpNodeList = clRootNode.toElement().elementsByTagName(qstrSubNodeName);
    }

    if (clTmpNodeList.size() > 0)
    {
        qstrNodeValue = clTmpNodeList.at(0).firstChild().nodeValue();
    }
    else
    {
        bRetValue = false;
    }

    return (bRetValue);
}


bool XMLParser::getSubNodeValue (const QString qstrSubNodeName, QDomNode clRootNode, unsigned int& uiNodeValue)
{
    // declaration of variables
    bool    bRetValue = true;
    QString qstrTemp;

    bRetValue = this->getSubNodeValue(qstrSubNodeName, clRootNode, qstrTemp);
    if (bRetValue == true)
    {
        uiNodeValue = qstrTemp.toUInt(&bRetValue);
    }

    if (bRetValue == false)
    {
        uiNodeValue = 0;
    }

    return (bRetValue);
}


bool XMLParser::getSubNodeValue (const QString qstrSubNodeName, QDomNode clRootNode, int& iNodeValue)
{
    // declaration of variables
    bool    bRetValue = true;
    QString qstrTemp;

    bRetValue = this->getSubNodeValue(qstrSubNodeName, clRootNode, qstrTemp);
    if (bRetValue == true)
    {
        iNodeValue = qstrTemp.toInt(&bRetValue);
    }

    if (bRetValue == false)
    {
        iNodeValue = 0;
    }

    return (bRetValue);
}


bool XMLParser::getSubNodeValue (const QString qstrSubNodeName, QDomNode clRootNode, bool& bNodeValue)
{
    // declaration of variables
    bool    bRetValue = true;
    int     iTemp;
    QString qstrTemp;

    bRetValue = this->getSubNodeValue(qstrSubNodeName, clRootNode, qstrTemp);
    if (bRetValue == true)
    {
        iTemp = qstrTemp.toInt(&bRetValue);
        switch (iTemp)
        {
            case 0:
                {
                    bNodeValue = false;
                    break;
                }

            case 1:
                {
                    bNodeValue = true;
                    break;
                }

            default:
                {
                    bRetValue = false;
                }
        }
    }

    if (bRetValue == false)
    {
        bNodeValue = false;
    }

    return (bRetValue);
}


bool XMLParser::getSubNodeValue (const QString qstrSubNodeName, QDomNode clRootNode, double& dNodeValue)
{
    // declaration of variables
    bool    bRetValue = true;
    QString qstrTemp;

    bRetValue = this->getSubNodeValue(qstrSubNodeName, clRootNode, qstrTemp);
    if (bRetValue == true)
    {
        dNodeValue = qstrTemp.toDouble(&bRetValue);
    }

    if (bRetValue == false)
    {
        dNodeValue = 0.0;
    }

    return (bRetValue);
}


bool XMLParser::setSubNodeValue (const QString qstrSubNodeName, QDomNode* pclRootNode, const QString qstrNodeValue)
{
    // declaration of variables
    bool          bRetValue = true;
    QDomNodeList  clTmpNodeList;

    if (pclRootNode->isNull() == true)
    {
        clTmpNodeList = this->clXmlDoc.elementsByTagName(qstrSubNodeName);
    }
    else
    {
        clTmpNodeList = pclRootNode->toElement().elementsByTagName(qstrSubNodeName);
    }

    if (clTmpNodeList.size() > 0)
    {
        clTmpNodeList.at(0).firstChild().setNodeValue(qstrNodeValue);
    }
    else
    {
        bRetValue = false;
    }

    return (bRetValue);
}


QDomNodeList XMLParser::findNodes (const QString qstrNodeName, QDomNode *pclRootNode)
{
    // declaration of variables
    QDomNodeList clNodeList;

    if (pclRootNode != NULL)
    {
        clNodeList = pclRootNode->toElement().elementsByTagName(qstrNodeName);
    }
    else
    {
        clNodeList = this->clXmlDoc.elementsByTagName(qstrNodeName);
    }

    return (clNodeList);
}


bool XMLParser::getAttributeValue (const QString qstrAttributeName, QDomNode& clNode, QString& qstrValue)
{
    // declaration of variables
    bool bRetValue = false;

    qstrValue.clear();

    if (clNode.attributes().contains(qstrAttributeName) == true)
    {
        qstrValue = clNode.attributes().namedItem(qstrAttributeName).nodeValue();
        bRetValue = true;
    }

    return (bRetValue);
}


bool XMLParser::writeXmlToString (QString& qstrXmlFile)
{
    // declaration of variables
    bool bRetValue = true;

    qstrXmlFile.clear();
    qstrXmlFile = this->clXmlDoc.toString(2);

    return (bRetValue);
}


bool XMLParser::insertXmlFile (QDomNode* pclNode, const QString& qstrXmlFile)
{
    // declaration of variables
    bool bRetValue = true;

    QDomDocument clDocTemp;
    clDocTemp.clear();
    clDocTemp.setContent (qstrXmlFile);

    pclNode->parentNode ().parentNode ().insertAfter (clDocTemp.documentElement(), pclNode->parentNode ());

    clDocTemp.clear ();

    return (bRetValue);
}

