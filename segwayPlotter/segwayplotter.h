#ifndef SEGWAYPLOTTER_H
#define SEGWAYPLOTTER_H

#include <QMainWindow>
#include "plotserver.h"

namespace Ui {
class segwayPlotter;
}

class segwayPlotter : public QMainWindow
{
    Q_OBJECT

public:
    explicit segwayPlotter( QWidget *parent = 0 );
    ~segwayPlotter();

private:
    Ui::segwayPlotter *ui;
    PlotServer ps;
	bool plotting;
    QTimer replot_timer;
    double n_samples;

private slots:
    void logMessage( QString msg );
    void on_buttStartServer_clicked();
	void plotValues( QVector<double> values );
	void on_buttStartPlot_clicked();
    void replot_timer_timeout();
    void on_buttSend_clicked();
};

#endif // SEGWAYPLOTTER_H
