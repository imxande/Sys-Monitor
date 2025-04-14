import QtQuick
import QtQuick.Controls
import QtCharts

Item {
  width: 600
  height: 300

  ChartView {
    id: cpuChart
    anchors.fill: parent
    antialiasing: true
    theme: ChartView.ChartThemeDark

    ValueAxis {
      id: xAxis
      min: 0
      max: 60
      titleText: "Time(s)"
    }

    ValueAxis {
      id: yAxis
      min: 0
      max: 100
      titleText: "CPU Usage (%)"
    }

    LineSeries {
      id: cpuSeries
      axisX: xAxis
      axisY: yAxis
      name: "Total CPU"
    }

    Timer {
      interval: 1000
      running: true
      repeat: true
      onTriggered: {
        let cpuVal = resourceMonitor.cpuUsage || 0
        if (cpuSeries.count > 60)
          cpuSeries.remove(0)
        cpuSeries.append(cpuSeries.count, cpuVal)
      }
    }
  }
}
