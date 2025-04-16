import QtQuick
import QtQuick.Controls
import QtCharts

Item {
  width: 600
  height: 300

  ChartView {
    id: diskChart
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
      max: 4 * 1024 * 1024 // up to 4MiB/s for now 
      titleText: "Bytes/s"
    }

    LineSeries {
      id: readSeries
      axisX: xAxis
      axisY: yAxis
      name: "Read"
      color: "#00bfff"
    }

    LineSeries {
      id: writeSeries
      axisX: xAxis
      axisY: yAxis
      name: "Write"
      color: "#ff6600"
    }

    Timer {
      interval: 1000
      running: true
      repeat: true
      onTriggered: {
        readSeries.append(readSeries.count, resourceMonitor.readRate || 0)
        writeSeries.append(writeSeries.count, resourceMonitor.writeRate || 0)

        if (readSeries.count > 60) 
          readSeries.remove(0)
          
        if (writeSeries.count > 60) 
          writeSeries.remove(0)
      }
    }
  }
}
