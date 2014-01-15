#ifndef PLOTSERVER_H
#define PLOTSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTimer>

class PlotServer : public QObject
{
    Q_OBJECT
public:
    explicit PlotServer(QObject *parent = 0);
	bool start( int port );
	void stop();
	bool isRunning();

public slots:
    void sendCommand( QString cmd );

signals:
	void clientConnected( QString msg );
	void error( QString msg );
	void newValue( QVector<double> values );

private slots:
	void newConnection();
	void socketReceive();
	void checkTimeout();

private:
	QTcpServer *server;
	bool has_connection;
	QTcpSocket *_socket;
	void split_string( QString str );
	QTimer socketTimeout;
	int timeout_count;

};

#endif // PLOTSERVER_H
