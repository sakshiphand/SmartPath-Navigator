#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCompleter>
#include <QStringList>
#include <QPushButton>

#include "graph/Graph.h"
#include "widgets/MapWidget.h"

QT_BEGIN_NAMESPACE

namespace Ui
{
class MainWindow;
}

QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    explicit MainWindow(
        Graph *graph,
        QWidget *parent = nullptr);

    ~MainWindow() override;


private slots:

    // Find Route button
    void on_pushButton_clicked();

    // Swap Source and Destination
    void swapSourceDestination();


private:

    Ui::MainWindow *ui;

    Graph *graph;

    MapWidget *mapWidget;

    QCompleter *sourceCompleter;

    QCompleter *destinationCompleter;

    QPushButton *swapButton;


    // Setup custom map widget
    void setupMapWidget();

    // Setup place-name autocomplete
    void setupPlaceAutocomplete();

    // Setup swap button
    void setupSwapButton();

    // Display error message
    void showError(
        const QString &message);
};

#endif // MAINWINDOW_H