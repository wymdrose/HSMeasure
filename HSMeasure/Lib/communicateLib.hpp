#pragma once

#include <QSerialPort>
#include <QSerialPortInfo>

#include <QHostAddress>
#include <QTcpSocket>

namespace _COMMUNICATECLASS
{

	class COM_PORT_ONE
	{
	public:
		COM_PORT_ONE(unsigned int portNo, int baudRate) :mPortNo(portNo), mBaudRate(baudRate)
		{
		//	connect(mpSerial, SIGNAL(readyRead()), this, SLOT(recData()));

			mpSerial->setPortName(QString("COM%1").arg(portNo));
			mpSerial->setBaudRate(baudRate);
			mpSerial->setParity(QSerialPort::NoParity);
			mpSerial->setDataBits(QSerialPort::Data8);
			mpSerial->setStopBits(QSerialPort::OneStop);
			mpSerial->setFlowControl(QSerialPort::NoFlowControl);

		}
		~COM_PORT_ONE()
		{
			mpSerial->close();
			delete mpSerial;
		}

		bool open()
		{
			mpSerial->close();

			qDebug() << "";

			if (mpSerial->open(QIODevice::ReadWrite)) 
			{
				qDebug() << "open com" << mPortNo << " success" << "\r";
				return true;
			}
			else 
			{
				qDebug() << "open com" << mPortNo << " failed" << "\r";
				return false;
			}
		}

		qint64 send(const QString sendData)
		{
			mRecData.clear();

			QByteArray tSend = sendData.toLocal8Bit();

			qDebug() << "";
			qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss:zzz") << mPortNo << "send:" << tSend << "\r";
		
			return mpSerial->write(tSend);
		}

		void recData()
		{

			mpSerial->waitForReadyRead(300);

			QByteArray tRec;
			tRec = mpSerial->readAll();
			
			mRecData = tRec;

			qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss:zzz") << mPortNo << "rec:" << tRec << "\r";
		}

		QString getRec()
		{
			return mRecData;
		}

	private:
		unsigned int mPortNo;
		int mBaudRate;
		QSerialPort *mpSerial = new QSerialPort();
		QString mRecData;
	};



	class TCP_CLIENT
	{
	public:
		TCP_CLIENT(QString ip, int port) : mIp(ip), mPort(port)
		{
			mpQHostAddress = new QHostAddress();
			mpTcpSocket = new QTcpSocket();


		}
		~TCP_CLIENT()
		{
			delete mpTcpSocket;
			delete mpQHostAddress;
		}

		bool connect()
		{
			if (!mpQHostAddress->setAddress(mIp))
			{
				qDebug() << mIp << "server ip address error!";
				return false;
			}

			mpTcpSocket->connectToHost(*mpQHostAddress, mPort);

			qDebug() << mIp << "connect ok.";
			return true;
		}

		void disConnect()
		{
			mpTcpSocket->disconnectFromHost();
	
		}

		void send(const QString msg)
		{
			mRecData.clear();

			qDebug() << "";
			qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss:zzz") << mIp << " send:" << msg << "\r";

			auto re = mpTcpSocket->write(msg.toLatin1(), msg.length());
		}

		
		
		bool getRec()
		{
			if (false == this->mpTcpSocket->waitForReadyRead())
			{ 
				qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss:zzz");
				qDebug() << "Server does not response!";
				return false; 
			}

		
			QByteArray datagram = mpTcpSocket->readAll();;
			
		//	mpTcpSocket->read(datagram.data(), 4);

			QString msg = datagram.data();

			qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss:zzz") << msg;
						
			return true;
		}

		/*
		void slotConnected()
		{
			qDebug() << mIp << "slotConnected success.";
			Sleep(0);
		}

		void slotDisconnected()
		{
			qDebug() << mIp << "slotDisconnected.";
		}
		*/

		QTcpSocket *mpTcpSocket;
	private:
		QString mIp;
		QString userName;
		int mPort;
		QHostAddress *mpQHostAddress;
		
		QString mRecData;
	
	};








};























