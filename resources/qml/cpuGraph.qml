import QtQuick
import QtQuick.Controls
import QtCharts

Item {
  width: 600
  height: 300

  property var coreSeries: []
  property bool qmlReady: false

  ChartView {
    id: cpuChart
    anchors.fill: parent
    antialiasing: true
    theme: ChartView.ChartThemeDark

    ValueAxis {
      id: xAxis
      min: 0
      max: 60
      labelsFont.pointSize: 8
    }

    ValueAxis {
      id: yAxis
      min: 0
      max: 100
      labelsFont.pointSize: 8
    }

    // Total CPU Line
    LineSeries {
      id: cpuSeries
      axisX: xAxis
      axisY: yAxis
      name: "Total CPU"
      color: "#00ff00"
    }

    // Per-core CPU Lines loader
    Loader {
      id: coreLoader
      active: qmlReady
      sourceComponent: coreComponent
    }

    Component.onCompleted: {
      qmlReady = true
    }

    Component {
      id: coreComponent
      Repeater {
        model: resourceMonitor ? resourceMonitor.cpuCoreUsages.length : 0
        LineSeries {
          axisX: xAxis
          axisY: yAxis
          name: "CPU" + (index + 1)
          color: Qt.hsla((index * 0.15) % 1.0, 1.0, 0.5, 1)
          Component.onCompleted: {
            coreSeries.push(this)
            console.log("Core series ready:", index)
          }
        }
      }
    }

    Connections {
      target: resourceMonitor
      function onCpuCoreUsagesChanged() {
        if (!qmlReady) return;
        console.log("QML ready. Reloading core graph.")
        coreSeries = []
        coreLoader.active = false
        coreLoader.active = true
      }
    }

    Timer {
      interval: 500
      running: true
      repeat: true
      onTriggered: {
        let total = resourceMonitor.cpuUsage || 0

        // Update Total CPU
        for (let i = 0; i < cpuSeries.count; i++) {
          let pt = cpuSeries.at(i)
          cpuSeries.replace(i, pt.x + 1, pt.y)
        }
        if (cpuSeries.count > 0 && cpuSeries.at(0).x > 60)
          cpuSeries.remove(0)
        cpuSeries.append(0, total)

        // Update per-core series
        for (let i = 0; i < coreSeries.length; ++i) {
          let s = coreSeries[i];
          let val = resourceMonitor.cpuCoreUsages[i] || 0

          for (let j = 0; j < s.count; j++) {
            let pt = s.at(j)
            s.replace(j, pt.x + 1, pt.y)
          }
          if (s.count > 0 && s.at(0).x > 60)
            s.remove(0)
          s.append(0, val)
        }
      }
    }
  }
}
