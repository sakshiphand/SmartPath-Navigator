# 🗺️ SmartPath Navigator

SmartPath Navigator is a **C++ route-planning desktop application** that finds the shortest path between locations using **Dijkstra's Algorithm** and the **A* Search Algorithm**.

The application uses **OpenStreetMap (OSM)** map data and provides an interactive **Qt-based graphical interface** for selecting locations, choosing routing algorithms, selecting travel modes, and visualizing the calculated route.

---

## ✨ Features

- 📍 Search source and destination locations
- 🔎 Location autocomplete suggestions
- 🔄 Swap source and destination
- 🗺️ OpenStreetMap (OSM) based road network
- 🚗 Car travel mode
- 🚲 Bike travel mode
- 🚶 Walk travel mode
- 🧭 Dijkstra's shortest path algorithm
- ⭐ A* search algorithm
- 📏 Route distance calculation
- ⏱️ Estimated travel time
- 🛣️ Graphical route visualization
- 🟢 Source and 🔴 destination markers

---

## 🧠 Algorithms

### Dijkstra's Algorithm

Dijkstra's algorithm finds the shortest path by exploring nodes based on the minimum accumulated distance from the source.

It guarantees the shortest path when all edge weights are non-negative.

### A* Search Algorithm

A* improves pathfinding by using a heuristic to guide the search toward the destination.

The geographical distance between nodes can be used as the heuristic for route planning.

---

## 🛠️ Technologies Used

| Technology | Purpose |
|---|---|
| C++ | Core application and algorithms |
| Qt | Desktop graphical user interface |
| CMake | Project build configuration |
| OpenStreetMap | Road and location data |
| TinyXML2 | Parsing OSM XML data |
| STL | Graph and algorithm data structures |

---

## 📂 Project Structure

```text
SmartPath-Navigator/
│
├── algorithms/
│   ├── AStar.cpp
│   ├── AStar.h
│   ├── Dijkstra.cpp
│   └── Dijkstra.h
│
├── graph/
│   ├── Graph.cpp
│   ├── Graph.h
│   ├── Node.cpp
│   └── Node.h
│
├── osm/
│   ├── OSMParser.cpp
│   ├── OSMParser.h
│   └── map.osm
│
├── widgets/
│   ├── MapWidget.cpp
│   └── MapWidget.h
│
├── xml/
│   ├── tinyxml2.cpp
│   └── tinyxml2.h
│
├── main.cpp
├── mainwindow.cpp
├── mainwindow.h
├── mainwindow.ui
├── CMakeLists.txt
└── README.md
```

---

## ⚙️ How It Works

1. OpenStreetMap data is loaded from `map.osm`.
2. OSM nodes and roads are converted into a graph.
3. Named locations are indexed for searching.
4. The user selects a source and destination.
5. The locations are connected to the road network.
6. Dijkstra or A* calculates the shortest route.
7. The route is displayed graphically on the map.
8. Distance and estimated travel time are displayed.

---

## 🚦 Travel Modes

SmartPath Navigator currently supports:

- **Car**
- **Bike**
- **Walk**

The selected travel mode is used to estimate the approximate travel time for the calculated route.

---

## 🔮 Future Improvements

- Compare Dijkstra and A* execution time
- Display number of nodes explored by each algorithm
- Zoom and pan support
- Improved map visualization
- Additional routing options
- Larger OpenStreetMap datasets
- Turn-by-turn route information

---

## 👩‍💻 Author

**Sakshi Phand**

Computer Engineering Student  
Interested in C++, Data Structures & Algorithms, AI/ML and Software Development

---

## 📌 Project Status

🚧 **Under Active Development**

SmartPath Navigator is currently being improved with additional routing and algorithm-comparison features.
