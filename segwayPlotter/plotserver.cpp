#include "plotserver.h"
#include <QDebug>
#include <QTcpSocket>
#include <QStringList>

PlotServer::PlotServer(QObject *parent) : QObject(parent), timeout_count( 0 )
{
	has_connection = false;
	server = new QTcpServer( this );

	_socket = NULL;

	connect( server, SIGNAL( newConnection() ), this, SLOT( newConnection() ) );
	connect( &socketTimeout, SIGNAL( timeout() ), this, SLOT( checkTimeout() ) );

	socketTimeout.start( 100 );
}

void PlotServer::newConnection()
{
	if( _socket != NULL )
	{
		if( _socket->isOpen() )
		{
			emit error( "Blocked incomming connection." );
			return;
		}
	}

	QTcpSocket *socket = server->nextPendingConnection();
	if( socket == NULL )
		return;

	_socket = socket;
	connect( _socket, SIGNAL( readyRead() ), this, SLOT( socketReceive() ) );

	has_connection = true;

	QString peer = socket->peerAddress() == QHostAddress::Null ? "unknown" : socket->peerAddress().toString();
	emit clientConnected( "Incomming connection from: " + peer );
}

void PlotServer::socketReceive()
{
	timeout_count = 0;

	QByteArray rx = _socket->readAll();

	split_string( rx );
}

bool PlotServer::start( int port )
{
	if( !server->listen( QHostAddress::Any, port ) )
	{
		emit error( "Could not start server" );
		return false;
	}

	return true;
}

void PlotServer::stop()
{
	if( _socket != NULL )
		_socket->close();

	server->close();
}

bool PlotServer::isRunning()
{
	return server->isListening();
}

void PlotServer::split_string( QString str )
{
	QStringList rows = str.split( '\n' );
	for( int i = 0; i < rows.count(); i++ )
	{
		QVector<double> values;

		QStringList cols = rows[i].split( ';' );
		for( int j = 0; j < cols.count(); j++ )
		{
			bool ok = false;
			double val = cols[j].toDouble( &ok );
			if( ok )
				values.append( val );
		}

		if( values.count() > 0 )
			emit newValue( values );
	}
}

void PlotServer::checkTimeout()
{
	if( server->isListening() && _socket != NULL )
	{
		if( _socket->isOpen() )
		{
			if( timeout_count > 50 )
			{
				_socket->abort();
				_socket->close();
				emit error( "Client disconnected." );
                has_connection = false;
			}
			else
			{
				timeout_count++;
			}
		}
		else
		{
			timeout_count = 0;
		}
	}
	else
	{
		timeout_count = 0;
	}
}

void PlotServer::sendCommand( QString cmd )
{
    if( !has_connection )
    {
        error( "No client connected." );
        return;
    }

    if( _socket == NULL )
    {
        error( "Socket is NULL." );
        return;
    }

    if( (_socket->write( cmd.toStdString().c_str()) < cmd.count() ) )
    {
        error( "ERROR: Send." );
    }

}
