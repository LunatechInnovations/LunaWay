#include "segwayplotter.h"
#include "ui_segwayplotter.h"
#include <QTime>
#include <QDebug>

/* Constructor
 */
segwayPlotter::segwayPlotter( QWidget *parent ) : QMainWindow( parent ), ui( new Ui::segwayPlotter )
{
    ui->setupUi( this );

    plotting = false;
    n_samples = 0.0f;

    //Setup plot widget
    ui->wdgTrend->addGraph( ui->wdgTrend->xAxis, ui->wdgTrend->yAxis );
    ui->wdgTrend->graph( 0 )->setPen( QPen( Qt::blue ) );
    ui->wdgTrend->graph( 0 )->setLineStyle( QCPGraph::lsLine );
    ui->wdgTrend->graph( 0 )->setName( "Trend1" );

    ui->wdgTrend->addGraph( ui->wdgTrend->xAxis, ui->wdgTrend->yAxis );
    ui->wdgTrend->graph( 1 )->setPen( QPen( Qt::red ) );
    ui->wdgTrend->graph( 1 )->setLineStyle( QCPGraph::lsLine );
    ui->wdgTrend->graph( 1 )->setName( "Trend2" );

    ui->wdgTrend->addGraph( ui->wdgTrend->xAxis, ui->wdgTrend->yAxis );
    ui->wdgTrend->graph( 2 )->setPen( QPen( Qt::green ) );
    ui->wdgTrend->graph( 2 )->setLineStyle( QCPGraph::lsLine );
    ui->wdgTrend->graph( 2 )->setName( "Trend3" );

    ui->wdgTrend->xAxis->setVisible( true );
    ui->wdgTrend->xAxis->setAutoTicks( false );
    ui->wdgTrend->xAxis->setTickLabels( false );
    ui->wdgTrend->xAxis->setTickStep( 1.0f );
    ui->wdgTrend->xAxis->setTicks( false );
    ui->wdgTrend->xAxis->setScaleType( QCPAxis::stLinear );
    ui->wdgTrend->xAxis->setRange( 0.0f, 1.0f );
    ui->wdgTrend->xAxis->setLabel( "n Samples" );

    ui->wdgTrend->yAxis->setScaleType( QCPAxis::stLinear );
    ui->wdgTrend->yAxis->setVisible( true );
    ui->wdgTrend->yAxis->setTickLabels( true );
    ui->wdgTrend->yAxis2->setScaleType( QCPAxis::stLinear );
    ui->wdgTrend->yAxis2->setVisible( true );
    ui->wdgTrend->yAxis2->setTickLabels( true );

    ui->wdgTrend->setInteractions( 0 );

    //Setup plot server
    connect( &ps, SIGNAL( clientConnected( QString ) ), this, SLOT( logMessage( QString ) ) );
    connect( &ps, SIGNAL( error( QString ) ), this, SLOT( logMessage( QString ) ) );
    connect( &ps, SIGNAL( newValue(QVector<double>) ), this, SLOT( plotValues( QVector<double> ) ) );

    //Setup replot timer
    connect( &replot_timer, SIGNAL( timeout() ), this, SLOT( replot_timer_timeout() ) );
}

/* Destructor
 */
segwayPlotter::~segwayPlotter()
{
    delete ui;
}

/* logMessage
 *
 * Slot for logging a timestamp and a message in the text browser.
 */
void segwayPlotter::logMessage( QString msg )
{
    QString time = QTime::currentTime().toString( "hh:mm:ss" );
    ui->tbLog->append( time + " " + msg );
}

/* on_buttStartServer_clicked
 *
 * Slot for start server button.
 * Toggles server on/off
 */
void segwayPlotter::on_buttStartServer_clicked()
{
    if( ps.isRunning() ) //Turn OFF server
    {
        ps.stop();
        ui->buttStartServer->setText( "Start Server" );
        ui->buttStartPlot->setEnabled( false );

        if( plotting )
            on_buttStartPlot_clicked();

        ui->spnPort->setEnabled( true );
        ui->txtSendCmd->setEnabled( false );
        ui->buttSend->setEnabled( false );
    }
    else        //Turn ON server
    {
        if( ps.start( ui->spnPort->value() ) )
        {
            ui->txtSendCmd->setEnabled( true );
            ui->buttSend->setEnabled( true );

            ui->buttStartServer->setText( "Stop Server" );

            ui->buttStartPlot->setEnabled( true );

            ui->spnPort->setEnabled( false );
        }
    }
}

void segwayPlotter::on_buttStartPlot_clicked()
{
    if( plotting ) //Turn OFF plotting
    {
            ui->buttStartPlot->setText( "Start Plot" );
            ui->dspnMaxY->setEnabled( true );
            ui->dspnMinY->setEnabled( true );
            ui->spnNSamples->setEnabled( true );
            ui->dspnUpdateHz->setEnabled( true );
            ui->wdgTrend->setInteractions( QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables );
            plotting = false;
            replot_timer.stop();
    }
    else        //Turn ON plotting
    {
            n_samples = 0.0f;
            ui->wdgTrend->xAxis->setRange( 0.0f, 1.0f );

            ui->wdgTrend->graph( 0 )->clearData();
            ui->wdgTrend->graph( 1 )->clearData();
            ui->wdgTrend->graph( 2 )->clearData();

            ui->wdgTrend->yAxis->setRangeUpper( ui->dspnMaxY->value() );
            ui->wdgTrend->yAxis2->setRangeUpper( ui->dspnMaxY->value() );
            ui->wdgTrend->yAxis->setRangeLower( ui->dspnMinY->value() * -1.0f );
            ui->wdgTrend->yAxis2->setRangeLower( ui->dspnMinY->value() * -1.0f );
            ui->wdgTrend->setInteractions( 0 ); //Disable interactions while plotting

            ui->buttStartPlot->setText( "Stop Plot" );
            ui->dspnMaxY->setEnabled( false );
            ui->dspnMinY->setEnabled( false );
            ui->spnNSamples->setEnabled( false );
            ui->dspnUpdateHz->setEnabled( false );
            plotting = true;
            replot_timer.start( (int)((1 / ui->dspnUpdateHz->value()) * 1000.0f) );
    }
}

void segwayPlotter::plotValues( QVector<double> values )
{
    if( plotting )
    {
        if( values.count() > 0 )
            ui->wdgTrend->graph( 0 )->addData( n_samples, values[0] );
        if( values.count() > 1 )
            ui->wdgTrend->graph( 1 )->addData( n_samples, values[1] );
        if( values.count() > 2 )
            ui->wdgTrend->graph( 2 )->addData( n_samples, values[2] );

        n_samples += 1.0f;
    }
}

void segwayPlotter::replot_timer_timeout()
{
    if( plotting )
    {
        //Update range for x-axis
        if( n_samples < (double)ui->spnNSamples->value() )
            ui->wdgTrend->xAxis->setRange( 0.0f, n_samples );
        else
            ui->wdgTrend->xAxis->setRange( n_samples - (double)ui->spnNSamples->value(), n_samples );

        //Replot
        ui->wdgTrend->replot();
    }
}

void segwayPlotter::on_buttSend_clicked()
{
    if( ui->txtSendCmd->text().count() > 0 )
        ps.sendCommand( ui->txtSendCmd->text() );
}
