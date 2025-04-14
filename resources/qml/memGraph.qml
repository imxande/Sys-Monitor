import QtQuick
import QtQuick.Controls
import QtCharts

Item {
  width: 600
  height: 300


  ChartView {
    id: memChart
    anchors.fill: parent
    antialiasing: true
    theme: ChartView.ChartThemeDark

    ValueAxis {
      id: xAxis
      min: 0
      max: 60
      titleText: "Time (s)"
    }

    ValueAxis {
      id: yAxis
      min: 0
      max: 100
      titleText: "Memory Usage (%)"
    }

    LineSeries {
      id: memSeries
      axisX: xAxis
      axisY: yAxis
      name: "Memory"
    }

    Timer {
      interval: 1000
      running: true
      repeat: true
      onTriggered: {
        let memVal = resourceMonitor.memoryUsage || 0
        if (memSeries.count > 60) 
          memSeries.remove(0)
        memSeries.append(memSeries.count, memVal)
      }
    }
  }
}
