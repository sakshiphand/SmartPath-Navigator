#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "algorithms/Dijkstra.h"
#include "algorithms/AStar.h"

#include <QMessageBox>
#include <QVBoxLayout>
#include <QCompleter>
#include <QStringList>
#include <QAbstractItemView>
#include <QPushButton>

#include <cmath>
#include <vector>
#include <algorithm>

// =====================================================
// CONSTRUCTOR
// =====================================================

MainWindow::MainWindow(
    Graph *graph,
    QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow),
    graph(graph),
    mapWidget(nullptr),
    sourceCompleter(nullptr),
    destinationCompleter(nullptr),
    swapButton(nullptr)
{
    ui->setupUi(this);

    // =================================================
    // MODERN UI STYLE
    // =================================================

    this->setStyleSheet(R"(

        QMainWindow {
            background-color: #F5F7FA;
        }

        QLineEdit {
            background-color: #FFFFFF;
            color: #111111;

            border: 1px solid #BFC5CC;
            border-radius: 6px;

            padding-left: 10px;
            padding-right: 10px;

            font-size: 14px;
            min-height: 30px;
        }

        QLineEdit:hover {
            border: 1px solid #7C8795;
        }

        QLineEdit:focus {
            background-color: #FFFFFF;
            color: #111111;

            border: 2px solid #2563EB;
        }

        QComboBox {
            background-color: #FFFFFF;
            color: #111111;

            border: 1px solid #BFC5CC;
            border-radius: 6px;

            padding-left: 10px;
            padding-right: 8px;

            font-size: 14px;
            min-height: 30px;
        }

        QComboBox:hover {
            border: 1px solid #2563EB;
        }

        QComboBox:focus {
            border: 1px solid #2563EB;
        }

        QPushButton {
            background-color: #2563EB;
            color: #FFFFFF;

            border: none;
            border-radius: 7px;

            padding: 10px 22px;

            font-size: 15px;
            font-weight: bold;

            min-height: 25px;
        }

        QPushButton:hover {
            background-color: #1D4ED8;
        }

        QPushButton:pressed {
            background-color: #1E40AF;
        }

        QLabel {
            color: #111827;
            background-color: transparent;
        }

    )");

    // =================================================
    // PLACEHOLDERS
    // =================================================

    ui->lineEdit->setPlaceholderText(
        "Enter source location");

    ui->lineEdit_2->setPlaceholderText(
        "Enter destination location");

    // =================================================
    // AUTOCOMPLETE
    // =================================================

    setupPlaceAutocomplete();

    // =================================================
    // MAP
    // =================================================

    setupMapWidget();

    // =================================================
    // SWAP BUTTON
    // =================================================

    setupSwapButton();

    // =================================================
    // ALGORITHM COMBO BOX
    // =================================================

    ui->comboBox->clear();

    ui->comboBox->addItem(
        "Dijkstra");

    ui->comboBox->addItem(
        "A*");

    // =================================================
    // TRAVEL MODE
    // =================================================

    ui->comboBox_2->clear();

    ui->comboBox_2->addItem(
        "Car");

    ui->comboBox_2->addItem(
        "Bike");

    ui->comboBox_2->addItem(
        "Walk");

    // =================================================
    // INITIAL LABELS
    // =================================================

    ui->label_7->setText(
        "Distance: --");

    ui->label_8->setText(
        "Estimated Time: --");

    ui->label_9->setText(
        "Algorithm Used: --");

    ui->label_10->setText(
        "Status: Ready");
}


// =====================================================
// DESTRUCTOR
// =====================================================

MainWindow::~MainWindow()
{
    delete ui;
}


// =====================================================
// SETUP AUTOCOMPLETE
// =====================================================

void MainWindow::setupPlaceAutocomplete()
{
    QStringList placeList;

    if (graph != nullptr)
    {
        std::vector<std::string> places =
            graph->getPlaceNames();

        for (const std::string &place : places)
        {
            placeList.append(
                QString::fromStdString(place));
        }
    }

    // Sort alphabetically
    placeList.sort(
        Qt::CaseInsensitive);

    // =================================================
    // SOURCE COMPLETER
    // =================================================

    sourceCompleter =
        new QCompleter(
            placeList,
            this);

    sourceCompleter->setCaseSensitivity(
        Qt::CaseInsensitive);

    sourceCompleter->setFilterMode(
        Qt::MatchContains);

    sourceCompleter->setCompletionMode(
        QCompleter::PopupCompletion);

    sourceCompleter->setMaxVisibleItems(
        10);

    ui->lineEdit->setCompleter(
        sourceCompleter);

    sourceCompleter->popup()->setStyleSheet(R"(

        QListView {
            background-color: white;
            color: #111111;

            border: 1px solid #BFC5CC;

            font-size: 14px;

            padding: 3px;
        }

        QListView::item {
            color: #111111;
            background-color: white;

            min-height: 28px;

            padding-left: 8px;
        }

        QListView::item:hover {
            background-color: #E8F0FE;
            color: #111111;
        }

        QListView::item:selected {
            background-color: #2563EB;
            color: white;
        }

    )");

    // =================================================
    // DESTINATION COMPLETER
    // =================================================

    destinationCompleter =
        new QCompleter(
            placeList,
            this);

    destinationCompleter->setCaseSensitivity(
        Qt::CaseInsensitive);

    destinationCompleter->setFilterMode(
        Qt::MatchContains);

    destinationCompleter->setCompletionMode(
        QCompleter::PopupCompletion);

    destinationCompleter->setMaxVisibleItems(
        10);

    ui->lineEdit_2->setCompleter(
        destinationCompleter);

    destinationCompleter->popup()->setStyleSheet(R"(

        QListView {
            background-color: white;
            color: #111111;

            border: 1px solid #BFC5CC;

            font-size: 14px;

            padding: 3px;
        }

        QListView::item {
            color: #111111;
            background-color: white;

            min-height: 28px;

            padding-left: 8px;
        }

        QListView::item:hover {
            background-color: #E8F0FE;
            color: #111111;
        }

        QListView::item:selected {
            background-color: #2563EB;
            color: white;
        }

    )");
}


// =====================================================
// SETUP MAP
// =====================================================

void MainWindow::setupMapWidget()
{
    mapWidget =
        new MapWidget(
            ui->frame);

    QVBoxLayout *layout =
        new QVBoxLayout(
            ui->frame);

    layout->setContentsMargins(
        0,
        0,
        0,
        0);

    layout->setSpacing(
        0);

    layout->addWidget(
        mapWidget);

    mapWidget->setGraph(
        graph);

    ui->label_6->hide();
}


// =====================================================
// SETUP SWAP BUTTON
// =====================================================

void MainWindow::setupSwapButton()
{
    // Only arrow symbol - no "Swap" text
    swapButton =
        new QPushButton(
            QString::fromUtf8("⇅"),
            this);

    swapButton->setObjectName(
        "swapButton");

    // Small button placed between
    // Source and Destination fields
    swapButton->setGeometry(
        1175,
        120,
        42,
        42);

    swapButton->setFixedSize(
        42,
        42);

    swapButton->setCursor(
        Qt::PointingHandCursor);

    swapButton->setToolTip(
        "Swap source and destination");

    // =================================================
    // SWAP BUTTON STYLE
    // =================================================

    swapButton->setStyleSheet(R"(

        QPushButton {
            background-color: #FFFFFF;
            color: #2563EB;

            border: 1px solid #BFC5CC;
            border-radius: 21px;

            padding: 0px;

            font-size: 23px;
            font-weight: bold;

            min-width: 42px;
            max-width: 42px;

            min-height: 42px;
            max-height: 42px;
        }

        QPushButton:hover {
            background-color: #EFF6FF;
            color: #1D4ED8;

            border: 1px solid #2563EB;
        }

        QPushButton:pressed {
            background-color: #DBEAFE;
            color: #1E40AF;
        }

    )");

    connect(
        swapButton,
        &QPushButton::clicked,
        this,
        &MainWindow::swapSourceDestination);
}


// =====================================================
// SWAP SOURCE AND DESTINATION
// =====================================================

void MainWindow::swapSourceDestination()
{
    QString source =
        ui->lineEdit->text();

    QString destination =
        ui->lineEdit_2->text();

    // Swap values
    ui->lineEdit->setText(
        destination);

    ui->lineEdit_2->setText(
        source);

    // Hide autocomplete popups if currently open
    if (sourceCompleter != nullptr)
    {
        sourceCompleter
            ->popup()
            ->hide();
    }

    if (destinationCompleter != nullptr)
    {
        destinationCompleter
            ->popup()
            ->hide();
    }

    // Clear previous route
    if (mapWidget != nullptr)
    {
        mapWidget->clearRoute();
    }

    // Reset result information
    ui->label_7->setText(
        "Distance: --");

    ui->label_8->setText(
        "Estimated Time: --");

    ui->label_9->setText(
        "Algorithm Used: --");

    ui->label_10->setText(
        "Status: Source and destination swapped");
}


// =====================================================
// SHOW ERROR
// =====================================================

void MainWindow::showError(
    const QString &message)
{
    ui->label_10->setText(
        "Status: " + message);

    QMessageBox::warning(
        this,
        "Route Error",
        message);
}


// =====================================================
// FIND ROUTE
// =====================================================

void MainWindow::on_pushButton_clicked()
{
    // =================================================
    // READ INPUT
    // =================================================

    QString sourceText =
        ui->lineEdit
            ->text()
            .trimmed();

    QString destinationText =
        ui->lineEdit_2
            ->text()
            .trimmed();

    if (sourceText.isEmpty() ||
        destinationText.isEmpty())
    {
        showError(
            "Enter source and destination.");

        return;
    }

    if (graph == nullptr)
    {
        showError(
            "Graph is not loaded.");

        return;
    }

    // =================================================
    // RESET PREVIOUS RESULT
    // =================================================

    if (mapWidget != nullptr)
    {
        mapWidget->clearRoute();
    }

    ui->label_7->setText(
        "Distance: --");

    ui->label_8->setText(
        "Estimated Time: --");

    ui->label_9->setText(
        "Algorithm Used: --");

    ui->label_10->setText(
        "Status: Searching...");

    // =================================================
    // PLACE NAMES
    // =================================================

    std::string sourceName =
        sourceText.toStdString();

    std::string destinationName =
        destinationText.toStdString();

    // =================================================
    // SOURCE PLACE
    // =================================================

    long long sourcePlace =
        graph->getPlaceId(
            sourceName);

    if (sourcePlace == -1)
    {
        showError(
            "Source place not found in map.osm.");

        return;
    }

    // =================================================
    // DESTINATION PLACE
    // =================================================

    long long destinationPlace =
        graph->getPlaceId(
            destinationName);

    if (destinationPlace == -1)
    {
        showError(
            "Destination place not found in map.osm.");

        return;
    }

    // =================================================
    // CONNECT TO ROAD NETWORK
    // =================================================

    long long sourceRoad =
        graph->findNearestRoadNode(
            sourcePlace);

    long long destinationRoad =
        graph->findNearestRoadNode(
            destinationPlace);

    if (sourceRoad == -1 ||
        destinationRoad == -1)
    {
        showError(
            "Unable to connect place to road network.");

        return;
    }

    // =================================================
    // SELECT ALGORITHM
    // =================================================

    QString selectedAlgorithm =
        ui->comboBox
            ->currentText();

    std::vector<long long> path;

    double totalDistanceMeters =
        0.0;

    // =================================================
    // DIJKSTRA
    // =================================================

    if (selectedAlgorithm ==
        "Dijkstra")
    {
        path =
            Dijkstra::shortestPath(
                *graph,
                sourceRoad,
                destinationRoad,
                totalDistanceMeters);
    }

    // =================================================
    // A*
    // =================================================

    else if (selectedAlgorithm ==
             "A*")
    {
        path =
            AStar::shortestPath(
                *graph,
                sourceRoad,
                destinationRoad);

        // Calculate A* route distance
        if (!path.empty())
        {
            for (size_t i = 0;
                 i + 1 < path.size();
                 ++i)
            {
                double segmentDistance =
                    graph->calculateDistance(
                        path[i],
                        path[i + 1]);

                if (std::isfinite(
                        segmentDistance))
                {
                    totalDistanceMeters +=
                        segmentDistance;
                }
            }
        }
    }

    else
    {
        showError(
            "Unknown algorithm selected.");

        return;
    }

    // =================================================
    // ROUTE NOT FOUND
    // =================================================

    if (path.empty())
    {
        ui->label_9->setText(
            "Algorithm Used: "
            + selectedAlgorithm);

        showError(
            "No route found between these locations.");

        return;
    }

    // =================================================
    // DRAW ROUTE
    // =================================================

    mapWidget->setRoute(
        path,
        sourceRoad,
        destinationRoad);

    // =================================================
    // DISTANCE
    // =================================================

    double distanceKm =
        totalDistanceMeters /
        1000.0;

    ui->label_7->setText(
        QString(
            "Distance: %1 km")
            .arg(
                distanceKm,
                0,
                'f',
                2));

    // =================================================
    // TRAVEL MODE
    // =================================================

    QString travelMode =
        ui->comboBox_2
            ->currentText();

    double speedKmPerHour =
        30.0;

    if (travelMode.compare(
            "Car",
            Qt::CaseInsensitive) == 0)
    {
        speedKmPerHour =
            30.0;
    }

    else if (travelMode.compare(
                 "Bike",
                 Qt::CaseInsensitive) == 0)
    {
        speedKmPerHour =
            15.0;
    }

    else if (travelMode.compare(
                 "Walk",
                 Qt::CaseInsensitive) == 0)
    {
        speedKmPerHour =
            5.0;
    }

    // =================================================
    // ESTIMATED TIME
    // =================================================

    double hours =
        distanceKm /
        speedKmPerHour;

    int estimatedMinutes =
        static_cast<int>(
            std::ceil(
                hours * 60.0));

    if (estimatedMinutes < 1)
    {
        estimatedMinutes =
            1;
    }

    ui->label_8->setText(
        QString(
            "Estimated Time: %1 min")
            .arg(
                estimatedMinutes));

    // =================================================
    // ALGORITHM USED
    // =================================================

    ui->label_9->setText(
        "Algorithm Used: "
        + selectedAlgorithm);

    // =================================================
    // SUCCESS
    // =================================================

    ui->label_10->setText(
        QString(
            "Status: Route Found (%1 nodes)")
            .arg(
                static_cast<int>(
                    path.size())));
}