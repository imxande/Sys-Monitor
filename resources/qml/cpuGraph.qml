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
      labelsFont.pointSize: 8
      min: 0
      max: 60
    }

    ValueAxis {
      id: yAxis
      labelsFont.pointSize: 8
      min: 0
      max: 100
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
