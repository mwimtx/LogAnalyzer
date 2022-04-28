
// self include protection
#ifndef UTILS_H
#define UTILS_H

// includes
#include <string>
#include <cstdio>
#include <climits>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <sys/time.h>
#include <errno.h>
#include <iostream>
#include <fstream>
#include <stdint.h>
#include <cmath>

#include <iostream>
#include <unistd.h>
#include <valarray>
#include <termios.h>
#include <fcntl.h>
#include <sys/stat.h>

#include "globalincludes.h"

#include <QThread>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QTextStream>
#include <QDebug>


// contants and defines

/**
 * These constants define the name and activation of debug messages
 */
#define C_UTILS_NAME        "Utils"
#define C_UTILS_DEBUG_MSG   1



/**
 * \class Timer
 *
 * \brief This class contains helper functions for platform independent timers and sleeps.
 *
 * This class implements some helper functions, that provide platform independent
 * (Linux / Windows / ...) support for sleeping and timers.
 *
 * \date 2015-10-16
 * \author MWI
 */
class Timer : public QThread
{
    public:

        /**
         * \brief pauses the calling thread for n seconds
         *
         * -
         *
         * @param[in] const unsigned long& ulSecs = seconds to sleep
         *
         * \date 21.05.2012
         * \author MWI
         */
        static void sleep(const unsigned long& ulSecs)
        {
            QThread::sleep(ulSecs);
        }

        /**
         * \brief pauses the calling thread for n milli seconds
         *
         * -
         *
         * @param[in] const unsigned long& ulMSecs = milli seconds to sleep
         *
         * \date 21.05.2012
         * \author MWI
         */
        static void msleep(const unsigned long& ulMSecs)
        {
            QThread::msleep(ulMSecs);
        }

        /**
         * \brief pauses the calling thread for n micro seconds
         *
         * -
         *
         * @param[in] const unsigned long& ulUSecs = micro seconds to sleep
         *
         * \date 21.05.2012
         * \author MWI
         */
        static void usleep(const unsigned long& ulUSecs)
        {
            QThread::usleep(ulUSecs);
        }
};


/**
 * \class Utils
 *
 * \brief This class contains a lot of utility functions.
 *
 * These functions can be used in all software modules.
 *
 * \author Martin Wilde <Martin.Wilde@apextoolgroup.com>
 * \date 2015-10-16
 */class Utils
{
    public:

        /**
         * \brief This function class constructor.
         *
         * Does nothing till now.
         *
         * \date 2015-10-16
         * \author MWI
         */
        Utils(void);

        /**
         * \brief This function will execute a shell command.
         *
         * This fucntion will execute the shell command beeing handed over as paramater.
         * It will return the result as string to the calling function.
         *
         * @param[in] const std::string& strShellCmd = string containing the shell command
         * @param[in] std::string& strCmdResult = string that shall contain the command result
         * @param[out] int = 0: OK / -1: failed to execute shell command
         *
         * \date 2015-10-16
         * \author MWI
         */
        static bool execShellCmd (const std::string& strShellCmd, std::string& strCmdResult)
        {
            // delcaration of variables
            int         iIndex;
            bool        bRetValue = true;
            FILE*       pfCommand;

            // execute shell command
            try
            {
                char arcReadBuffer [1024];
                strCmdResult = "";
                pfCommand    = popen (strShellCmd.c_str (), "r");
                if (pfCommand != NULL)
                {
                    while (!feof (pfCommand))
                    {
                        iIndex = fread (arcReadBuffer, 1, 512, pfCommand);
                        if (iIndex > 0)
                        {
                            strCmdResult.append (arcReadBuffer, iIndex);
                        }
                    }
                    pclose (pfCommand);
                }
                else
                {
                    #ifdef C_UTILS_DEBUG_MSG
                        printf ("[%s] error executing shell command - cannot open pipe!\n", C_UTILS_NAME);
                        fflush (stdout);
                    #endif
                    strCmdResult = "";
                    bRetValue    = false;
                }
            }
            catch (...)
            {
                // error in command execution
                #ifdef C_UTILS_DEBUG_MSG
                    printf ("[%s] error executing shell command - execption!\n", C_UTILS_NAME);
                    fflush (stdout);
                #endif
                strCmdResult = "";
                bRetValue    = false;
            }

            return (bRetValue);
        }

        /**
         * \brief This function converts a time in seconds into a string.
         *
         * The output string will have the following format:<br
         * DDD HH:MM:SS<br>
         * with:<br>
         * DDD: number of days<br>
         * HH : hours<br>
         * MM : minutes<br>
         * SS : seconds<br>
         *
         * @param[in] int iNumSecs = number of seconds
         * \return std::string  = converted string
         *
         * \date 2015-10-30
         * \author MWI
         */
        static std::string getTimeString (int iNumSecs)
        {
            // delcaration of variables
            int     iTemp;
            double  dTemp;
            QString qstrTemp;
            QString qstrRetValue = "";

            if (iNumSecs < 0)
            {
                qstrRetValue = "---         ";
            }
            else
            {
                dTemp     = iNumSecs;
                dTemp    /= 86400.0;
                iTemp     = floor(dTemp);
                iNumSecs -= 86400 * iTemp;
                qstrTemp  = QString::number (iTemp);
                while (qstrTemp.size() < 3)
                {
                    qstrTemp.prepend("0");
                }
                qstrRetValue = qstrTemp + " ";

                dTemp     = ((double)iNumSecs) / 3600.0;
                iTemp     = floor(dTemp);
                iNumSecs -= 3600 * iTemp;
                qstrTemp  = QString::number (iTemp);
                while (qstrTemp.size() < 2)
                {
                    qstrTemp.prepend("0");
                }
                qstrRetValue += qstrTemp + ":";

                dTemp     = ((double)iNumSecs) / 60.0;
                iTemp     = floor(dTemp);
                iNumSecs -= 60 * iTemp;
                qstrTemp  = QString::number (iTemp);
                while (qstrTemp.size() < 2)
                {
                    qstrTemp.prepend("0");
                }
                qstrRetValue += qstrTemp + ":";

                qstrTemp  = QString::number (iNumSecs);
                while (qstrTemp.size() < 2)
                {
                    qstrTemp.prepend("0");
                }
                qstrRetValue += qstrTemp;

            }

            return (qstrRetValue.toStdString());
        }


        /**
         * \brief Converts the disk space into a string.
         *
         * The output string will be in format:<br>
         * NNGB/MB/kB - MMM.MM%
         *
         * @param[in] const int iDiskFreeSpace = free space in kByte
         * @param[in] const int iDiskSize = size of disk in kByte
         * \return std::string  = converted string
         *
         * \date 2015-10-30
         * \author MWI
         */
        static std::string getDiskSpaceString (const int iDiskFreeSpace, const int iDiskSize)
        {
            // declaration of variables
            std::string strRetValue = "";
            double      dTemp;

            if ((iDiskFreeSpace < 0) || (iDiskSize < 0))
            {
                strRetValue = "N/A            ";
            }
            else
            {
                if (iDiskFreeSpace < 1000)
                {
                    strRetValue  = QString::number (iDiskFreeSpace, 'f', 2).toStdString();
                    strRetValue += "kB ";
                }
                else if ((iDiskFreeSpace >= 1000) && (iDiskFreeSpace < 1000000))
                {
                    dTemp        = ((double) iDiskFreeSpace) / 1024.0;
                    strRetValue  = QString::number (dTemp, 'f', 2).toStdString();
                    strRetValue += "MB ";
                }
                else
                {
                    dTemp        = ((double) iDiskFreeSpace) / 1024.0 / 1024.0;
                    strRetValue  = QString::number (dTemp, 'f', 2).toStdString();
                    strRetValue += "GB ";
                }

                dTemp        = (((double) iDiskFreeSpace) * 100.0 / ((double)iDiskSize));
                strRetValue += QString::number (dTemp, 'f', 1).toStdString() + "%";

                strRetValue += " free";
            }


            while (strRetValue.size() < 21)
            {
                strRetValue.push_back(' ');
            }

            return (strRetValue);
        }


        /**
         * \brief Converts the number of bytes into human readable size.
         *
         * The output string will be in format:<br>
         * X.YYGB/MB/kB
         *
         * @param[in] const long long int iNumBytes = number of bytes
         * \return std::string  = converted string
         *
         * \date 2016-03-17
         * \author MWI
         */
        static std::string getSizeString (const long long int iNumBytes)
        {
            // declaration of variables
            std::string strRetValue = "";
            double      dTemp;

            if (iNumBytes < 0)
            {
                strRetValue = "N/A            ";
            }
            else
            {
                if (iNumBytes < (1024 * 1024))
                {
                    dTemp        = ((double) iNumBytes) / 1024.0;
                    strRetValue  = QString::number (dTemp, 'f', 2).toStdString();
                    strRetValue += " kB";
                }
                else if ((iNumBytes >= (1024.0 * 1024.0)) && (iNumBytes < (1024.0 * 1024.0 * 1024.0)))
                {
                    dTemp        = ((double) iNumBytes) / (1024.0 * 1024.0);
                    strRetValue  = QString::number (dTemp, 'f', 2).toStdString();
                    strRetValue += " MB";
                }
                else
                {
                    dTemp        = ((double) iNumBytes) / (1024.0 * 1024.0 * 1024.0);
                    strRetValue  = QString::number (dTemp, 'f', 2).toStdString();
                    strRetValue += " GB";
                }
            }


            while (strRetValue.size() < 21)
            {
                strRetValue.push_back(' ');
            }

            return (strRetValue);
        }


        /**
         * \brief creates a random number value.
         *
         * Care is taken, that the upper border value is NOT exceeded.
         *
         * @param[in] const unsigned int uiMaxNumber = maximum value for output value
         * @param[in] const bool bSigned = output value is signed / unsigned
         * \return int = random number value
         *
         * \date 2015-10-30
         * \author MWI
         */
        static int getRandomNumber (const unsigned int uiMaxNumber, const bool bSigned)
        {
            // declaration of variables
            int    iRetValue;
            double dTemp;

            srand (time(NULL));

            dTemp  = rand ();
            dTemp /= RAND_MAX;
            dTemp *= uiMaxNumber;

            if (bSigned == true)
            {
                if ((rand () % 2) == 0)
                {
                    dTemp *= -1.0;
                }
            }

            iRetValue = (int) dTemp;

            return (iRetValue);
        }


        /**
         * \brief Computes the time series parameters like min/max and DC offset.
         *
         * Receives the time series buffer as std::vector<int>. It will use a
         * 5 points median filter for spike reduction.
         *
         * @param[in] const std::vector<int>& vecTSData = TS data buffer
         * @param[out] int& iMin = value to store minimum amplitude value
         * @param[out] int& iMax = value to store maximum amplitude value
         * @param[out] int& iDCOffset = value to store DC offset value
         * \return bool = true: OK / false: failed to compute TS parameters
         *
         * \date 2016-01-18
         * \author MWI
         */
        static bool getTSParameters (const std::vector<int>& vecTSData, int& iMin, int& iMax, int& iDCOffset)
        {
            // declaration of variables
            bool          bRetValue = true;
            int           iTemp;
            long long int lliOffset = 0;
            unsigned int  uiCounter;
            unsigned int  uiCounter2;
            unsigned int  uiIndex;
            std::vector<int> vecMedian;

            vecMedian.resize(5);
            uiIndex = floor (vecMedian.size() / 2.0);

            iMin      = INT_MAX;
            iMax      = INT_MIN;
            iDCOffset = 0;

            if (vecTSData.size() < vecMedian.size())
            {
                bRetValue = false;
            }
            else
            {

                for (uiCounter = 0; uiCounter < (vecTSData.size() - vecMedian.size()); uiCounter++)
                {
                    for (uiCounter2 = 0; uiCounter2 < vecMedian.size(); uiCounter2++)
                    {
                        vecMedian [uiCounter2] = vecTSData [uiCounter + uiCounter2];
                    }
                    std::sort(vecMedian.begin(), vecMedian.end(), std::greater<int>());
                    iTemp = vecMedian [uiIndex];

                    if (iTemp > iMax)
                    {
                        iMax = iTemp;
                    }

                    if (iTemp < iMin)
                    {
                        iMin = iTemp;
                    }

                    lliOffset += iTemp;
                }

                lliOffset /= uiCounter;
                iDCOffset  = (int)lliOffset;
            }

            return (bRetValue);
        }


        /**
         * \brief This function checks, if n percent are bigger / smaller than a border value.
         *
         * It takes the complete TS data buffer and sorts it in ascending order.
         * Afterwards it executes 2 checks:<br>
         *<br>
         *1.) n percent < border 1<br>
         *    -->can be used to check, if gain should be switched up<br>
         *2.) n percent > broder 2<br>
         *    -->can be used to check, if gain should be switched down<br>
         *<br>
         *
         * @param[in] const std::vector<int>& vecTSData = TS data buffer
         * @param[in] const int iBorder1 = border value 1
         * @param[in] const double dBorder1Percent = percentage value for border 1 check
         * @param[out] bool& bBorder1Exceeded = reference to boolean value for border 1 check
         * @param[in] const int iBorder2 = border value 2
         * @param[in] const double dBorder2Percent = percentage value for border 2 check
         * @param[out] bool& bBorder2Exceeded = reference to boolean value for border 2 check
         * \return bool = true: OK / false: failed to compute TS parameters
         *
         * \date 2016-01-19
         * \author MWI
         */
        static bool checkTSBorderValues (std::vector<int> vecTSData,
                                         const int iBorder1, const double dBorder1Percent, bool& bBorder1Exceeded,
                                         const int iBorder2, const double dBorder2Percent, bool& bBorder2Exceeded)
        {
            // declaration of variables
            bool         bRetValue     = true;
            int          iBorder1Index = 0;
            int          iBorder2Index = 0;
            double       dBorder1Index = 0;
            double       dBorder2Index = 0;
            unsigned int uiCounter;

            bBorder1Exceeded = false;
            bBorder2Exceeded = false;

            std::sort(vecTSData.begin(), vecTSData.end(), std::greater<int>());

            #ifdef C_UTILS_DEBUG_MSG
                printf ("[%s] checking TS data buffer border values ...\n", C_UTILS_NAME);
                fflush (stdout);
            #endif

            for (uiCounter = 0; uiCounter < vecTSData.size(); uiCounter++)
            {
                if (abs(vecTSData [uiCounter]) < iBorder1)
                {
                    iBorder1Index++;
                }

                if (abs (vecTSData [uiCounter]) < iBorder2)
                {
                    iBorder2Index++;
                }
            }

            // border 2 should compute the values > border value, therefore subtract here from
            // vector size to get the number of samples > border value
            iBorder2Index = vecTSData.size() - iBorder2Index;

            dBorder1Index = (((double)iBorder1Index) / ((double)vecTSData.size()));
            dBorder2Index = (((double)iBorder2Index) / ((double)vecTSData.size()));

            if (dBorder1Index >= dBorder1Percent)
            {
                bBorder1Exceeded = true;
            }

            if (dBorder2Index >= dBorder2Percent)
            {
                bBorder2Exceeded = true;
            }

            #ifdef C_UTILS_DEBUG_MSG
                printf ("[%s] border 1 check: %1.4f / %1.4f = %i\n", C_UTILS_NAME, dBorder1Index, dBorder1Percent, bBorder1Exceeded);
                printf ("[%s] border 2 check: %1.4f / %1.4f = %i\n", C_UTILS_NAME, dBorder2Index, dBorder2Percent, bBorder2Exceeded);
                fflush (stdout);
            #endif

            return (bRetValue);
        }


        /**
         * \brief Tries to read the Type ID and serial number from the NAND flash file.
         *
         * The Clock/GPS and ADU-08e BB boards store the type identifier and serial number inside the
         * NAND flash. This file is read during boot-up and stored inside the path defined by qstrTypeIDPath.
         * The function will try to read the file from that location and extract Type ID and serial number.
         *
         *
         * @param[in] const QString qstrTypeIDPath = path to binary file extracted from NAND flash
         * @param[out] unsigned int& uiTypeID = variable to store Type ID
         * @param[out] unsigned int& uiSerial = variable to store serial number
         * \return bool = true: OK / false: failed to read Type ID and serial number
         *
         * \date 2016-07-14
         * \author MWI
         */
        static bool getTypeID (const QString qstrTypeIDPath, unsigned int& uiTypeID, unsigned int& uiSerial)
        {
            // declaration of variables
            bool        bRetValue = false;
            QFile       qfTypeID;
            QByteArray  byTemp;

            qfTypeID.setFileName(qstrTypeIDPath);
            if (qfTypeID.exists() == true)
            {
                if (qfTypeID.open(QIODevice::ReadOnly) == true)
                {
                    byTemp = qfTypeID.readAll();
                    if (byTemp.size() == 8)
                    {
                        bRetValue = true;
                        uiTypeID  = byTemp [0];
                        uiTypeID  = uiTypeID << 8;
                        uiTypeID |= byTemp [1];
                        uiTypeID  = uiTypeID << 8;
                        uiTypeID |= byTemp [2];
                        uiTypeID  = uiTypeID << 8;
                        uiTypeID |= byTemp [3];

                        uiSerial  = byTemp [4];
                        uiSerial  = uiSerial << 8;
                        uiSerial |= byTemp [5];
                        uiSerial  = uiSerial << 8;
                        uiSerial |= byTemp [6];
                        uiSerial  = uiSerial << 8;
                        uiSerial |= byTemp [7];
                    }

                    qfTypeID.close();
                }
            }

            return (bRetValue);
        }


        /**
         * \brief Updates the Type ID and serial number by updating the contents of the NAND flash.
         *
         * This is done by updating the contents of the TypeID.bin file inside the /tmp/Nand directory
         * with the new type ID and serial number. Afterwards the contents of /tmp/Nand is zipped.
         * Finally the new NAND flash blob is created out of the length of the blob (4 bytes Length + size
         * of archive) and written to /dev/mtd5.
         *
         * @param[in] const QString qstrTypeIDPath = path to binary file extracted from NAND flash
         * @param[out] unsigned int uiTypeID = new type ID
         * @param[out] unsigned int uiSerial = new serial number
         * \return bool = true: OK / false: failed to read Type ID and serial number
         *
         * \date 2016-07-14
         * \author MWI
         */
        static bool setTypeID (const QString qstrTypeIDPath, const unsigned int uiTypeID, const unsigned int uiSerial)
        {
            // declaration of variables
            bool         bRetValue = false;
            QFile        qfTypeID;
            QFileInfo    qfiTypeID;
            QFile        qfNandLength;
            QByteArray   byTemp;
            std::string  strCmd;
            std::string  strResp;
            unsigned int uiTemp;

            byTemp.resize(8);
            byTemp [0] = (unsigned char) ((uiTypeID & 0xFF000000) >> 24);
            byTemp [1] = (unsigned char) ((uiTypeID & 0x00FF0000) >> 16);
            byTemp [2] = (unsigned char) ((uiTypeID & 0x0000FF00) >>  8);
            byTemp [3] = (unsigned char) ((uiTypeID & 0x000000FF) >>  0);

            byTemp [4] = (unsigned char) ((uiSerial & 0xFF000000) >> 24);
            byTemp [5] = (unsigned char) ((uiSerial & 0x00FF0000) >> 16);
            byTemp [6] = (unsigned char) ((uiSerial & 0x0000FF00) >>  8);
            byTemp [7] = (unsigned char) ((uiSerial & 0x000000FF) >>  0);

            qfiTypeID.setFile(qstrTypeIDPath);
            if (qfiTypeID.absoluteDir().exists() == false)
            {
                qfiTypeID.absoluteDir().mkdir(qfiTypeID.absolutePath());
            }

            qfTypeID.setFileName(qstrTypeIDPath);
            if (qfTypeID.exists() == true)
            {
                qfTypeID.remove();
            }

            if (qfTypeID.open(QIODevice::WriteOnly) == true)
            {
                if (qfTypeID.write(byTemp) == byTemp.size())
                {
                    qfTypeID.flush();
                    strCmd = "/home/Scripts/createNandArchive.sh " + qfiTypeID.absolutePath().toStdString() + " " + "Nand.tgz";
                    if (execShellCmd(strCmd, strResp) == true)
                    {
                        #ifdef C_UTILS_DEBUG_MSG
                            printf ("[%s] command.: \"%s\"\n", C_UTILS_NAME, strCmd.c_str());
                            printf ("[%s] response:\n%s\n", C_UTILS_NAME, strResp.c_str());
                            fflush (stdout);
                        #endif

                        // get file size of archive and write to disk
                        qfiTypeID.setFile(qfiTypeID.absolutePath() + "/../Nand.tgz");
                        if (qfiTypeID.size() > 0)
                        {
                            uiTemp = qfiTypeID.size();
                            byTemp.resize(4);
                            byTemp [0] = (unsigned char) ((uiTemp & 0xFF000000) >> 24);
                            byTemp [1] = (unsigned char) ((uiTemp & 0x00FF0000) >> 16);
                            byTemp [2] = (unsigned char) ((uiTemp & 0x0000FF00) >>  8);
                            byTemp [3] = (unsigned char) ((uiTemp & 0x000000FF) >>  0);

                            qfNandLength.setFileName(qfiTypeID.absolutePath() + "/NandLength.bin");
                            if (qfNandLength.exists() == true)
                            {
                                qfNandLength.remove();
                            }

                            if (qfNandLength.open(QIODevice::WriteOnly) == true)
                            {
                                if (qfNandLength.write(byTemp) == byTemp.size())
                                {
                                    qfNandLength.flush();

                                    strCmd = "/home/Scripts/writeNand.sh " + qfiTypeID.absolutePath().toStdString() + " NandLength.bin Nand.tgz";
                                    if (execShellCmd(strCmd, strResp) == true)
                                    {
                                        #ifdef C_UTILS_DEBUG_MSG
                                            printf ("[%s] command.: \"%s\"\n", C_UTILS_NAME, strCmd.c_str());
                                            printf ("[%s] response:\n%s\n", C_UTILS_NAME, strResp.c_str());
                                            fflush (stdout);
                                        #endif
                                        bRetValue = true;
                                    }
                                }
                            }
                        }
                    }
                }
                qfTypeID.close();
            }

            return (bRetValue);
        }


        /**
         * \brief asks for type ID and serial number and update NAND flash contents.
         *
         * ---
         *
         * \return bool = true: OK / false: failed to update Type ID and serial number
         *
         * \date 2016-07-14
         * \author MWI
         */
        static bool updateTypeID (void)
        {
            // declaration of variables
            bool         bRetValue = true;
            unsigned int uiBoardType;
            unsigned int uiSerial;
            QTextStream  qstInput (stdin);
            QString      qstrTemp;


            qDebug () << "update type ID ...";
            qDebug () << "select board type:";
            qDebug () << "[1]    ADU-08e BB";
            qDebug () << "[2]    ADU-08e Clock/GPS";
            qDebug () << "[else] exit";
            qDebug () << "-->";
            qstrTemp = qstInput.readLine();

            if (qstrTemp.toUInt() == 1)
            {
                uiBoardType = C_ADU08_BB_DEF_TYPE_ID;
            }
            else if (qstrTemp.toUInt() == 2)
            {
                uiBoardType = C_ADU08_CLOCK_DEF_TYPE_ID;
            }
            else
            {
                return (false);
            }

            qDebug () << "select serial number:";
            qDebug () << "-->";
            qstrTemp = qstInput.readLine();
            uiSerial = qstrTemp.toUInt();

            printf ("Type ID: 0x%8.8x\n", uiBoardType);
            printf ("Serial.: 0x%8.8x\n", uiSerial);

            bRetValue = setTypeID(C_TYPE_ID_PATH, uiBoardType, uiSerial);

            return (bRetValue);
        }

        static bool openFile (const QString qstrFileName, int& iFileHandle, const bool bReadOnly = false)
        {
            if (bReadOnly == true)
            {
                iFileHandle = open (qstrFileName.toStdString().c_str(), O_RDONLY);
            }
            else
            {
                iFileHandle = open (qstrFileName.toStdString().c_str(), O_RDWR);
            }

            if (iFileHandle <= 0)
            {
                return (false);
            }
            else
            {
                return (true);
            }
        }


        static void closeFile (int& iFileHandle)
        {
            close (iFileHandle);
        }


        /**
         * \brief wrapper for checking unsigned long long int ==
         *
         * ---
         *
         * \return bool = true: both number are equal / false: number are not equal
         *
         * \date 2020-07-13
         * \author MWI
         */
        static bool uint64AEqualB (unsigned long long int ullA, unsigned long long int ullB)
        {
            // declaration of variables
            bool                    bRetValue = true;
            unsigned long long int  ullTemp;
            unsigned int            uiAHigh;
            unsigned int            uiALow;
            unsigned int            uiBHigh;
            unsigned int            uiBLow;

            ullTemp = ullA;
            ullTemp = (ullTemp & 0xFFFFFFFF00000000);
            ullTemp = ullTemp >> 32;
            uiAHigh = (unsigned int) (ullTemp & 0x00000000FFFFFFFF);
            uiALow  = (unsigned int) (ullA    & 0x00000000FFFFFFFF);

            ullTemp = ullB;
            ullTemp = (ullTemp & 0xFFFFFFFF00000000);
            ullTemp = ullTemp >> 32;
            uiBHigh = (unsigned int) (ullTemp & 0x00000000FFFFFFFF);
            uiBLow  = (unsigned int) (ullB    & 0x00000000FFFFFFFF);

            /*
            printf ("[Utils]: A: 0x%8.8x %8.8x\n", uiAHigh, uiALow);
            printf ("[Utils]: B: 0x%8.8x %8.8x\n", uiBHigh, uiBLow);
            */

            if ((uiAHigh == uiBHigh) && (uiALow == uiBLow))
            {
                bRetValue = true;
            }
            else
            {
                bRetValue = false;
            }

            /*
            if (bRetValue == true)
            {
                printf ("[Utils]: A == B\n");
            }
            else
            {
                printf ("[Utils]: A != B\n");
            }
            */

            return (bRetValue);
        }

        /**
         * \brief wrapper for checking unsigned long long int >
         *
         * ---
         *
         * \return bool = true: A bigger B / false: A smaller / equal B
         *
         * \date 2020-07-13
         * \author MWI
         */
        static bool uint64ABiggerB (unsigned long long int ullA, unsigned long long int ullB)
        {
            // declaration of variables
            bool                    bRetValue = true;
            unsigned long long int  ullTemp;
            unsigned int            uiAHigh;
            unsigned int            uiALow;
            unsigned int            uiBHigh;
            unsigned int            uiBLow;

            ullTemp = ullA;
            ullTemp = (ullTemp & 0xFFFFFFFF00000000);
            ullTemp = ullTemp >> 32;
            uiAHigh = (unsigned int) (ullTemp & 0x00000000FFFFFFFF);
            uiALow  = (unsigned int) (ullA    & 0x00000000FFFFFFFF);

            ullTemp = ullB;
            ullTemp = (ullTemp & 0xFFFFFFFF00000000);
            ullTemp = ullTemp >> 32;
            uiBHigh = (unsigned int) (ullTemp & 0x00000000FFFFFFFF);
            uiBLow  = (unsigned int) (ullB    & 0x00000000FFFFFFFF);

            /*
            printf ("[Utils]: A: 0x%8.8x %8.8x\n", uiAHigh, uiALow);
            printf ("[Utils]: B: 0x%8.8x %8.8x\n", uiBHigh, uiBLow);
            */

            if (uiAHigh > uiBHigh)
            {
                bRetValue = true;
            }
            else
            {
                if (uiAHigh == uiBHigh)
                {
                    if (uiALow > uiBLow)
                    {
                        bRetValue = true;
                    }
                    else
                    {
                        bRetValue = false;
                    }
                }
                else
                {
                    bRetValue = false;
                }
            }

            /*
            if (bRetValue == true)
            {
                printf ("[Utils]: A > B\n");
            }
            else
            {
                printf ("[Utils]: A <= B\n");
            }
            */

            return (bRetValue);
        }


        /**
         * \brief wrapper for checking unsigned long long int <
         *
         * ---
         *
         * \return bool = true: A smaller B / false: A bigger / equal B
         *
         * \date 2020-07-13
         * \author MWI
         */
        static bool uint64ASmallerB (unsigned long long int ullA, unsigned long long int ullB)
        {
            // declaration of variables
            bool                    bRetValue = true;
            unsigned long long int  ullTemp;
            unsigned int            uiAHigh;
            unsigned int            uiALow;
            unsigned int            uiBHigh;
            unsigned int            uiBLow;

            ullTemp = ullA;
            ullTemp = (ullTemp & 0xFFFFFFFF00000000);
            ullTemp = ullTemp >> 32;
            uiAHigh = (unsigned int) (ullTemp & 0x00000000FFFFFFFF);
            uiALow  = (unsigned int) (ullA    & 0x00000000FFFFFFFF);

            ullTemp = ullB;
            ullTemp = (ullTemp & 0xFFFFFFFF00000000);
            ullTemp = ullTemp >> 32;
            uiBHigh = (unsigned int) (ullTemp & 0x00000000FFFFFFFF);
            uiBLow  = (unsigned int) (ullB    & 0x00000000FFFFFFFF);

            /*
            printf ("[Utils]: A: 0x%8.8x %8.8x\n", uiAHigh, uiALow);
            printf ("[Utils]: B: 0x%8.8x %8.8x\n", uiBHigh, uiBLow);
            */

            if (uiAHigh < uiBHigh)
            {
                bRetValue = true;
            }
            else
            {
                if (uiAHigh == uiBHigh)
                {
                    if (uiALow < uiBLow)
                    {
                        bRetValue = true;
                    }
                    else
                    {
                        bRetValue = false;
                    }
                }
                else
                {
                    bRetValue = false;
                }
            }

            /*
            if (bRetValue == true)
            {
                printf ("[Utils]: A > B\n");
            }
            else
            {
                printf ("[Utils]: A <= B\n");
            }
            */

            return (bRetValue);
        }




};

#endif // UTILS_H
