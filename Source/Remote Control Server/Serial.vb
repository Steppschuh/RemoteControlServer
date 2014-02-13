Module Serial

    'This module can be used to control an arduino using the remote control app

    Public Const commandStart As Char = "<"
    Public Const commandEnd As Char = ">"

    Public serialPort As IO.Ports.SerialPort

    Public Function openSerialPort(ByVal name As String) As Boolean
        Try
            serialPort = My.Computer.Ports.OpenSerialPort(name)
        Catch ex As Exception
            Logger.add("Unable to open serial port " & serialPortName & vbNewLine & ex.ToString)
        End Try
    End Function

    Public Function closeSerialPort()
        Try
            serialPort.Close()
        Catch ex As Exception
        End Try
    End Function

    Public Sub sendMessage(ByVal message As String)
        Try
            If serialPort Is Nothing Then
                openSerialPort(Settings.serialPortName)
            End If
            If Not serialPort.IsOpen Then
                openSerialPort(Settings.serialPortName)
            End If
            serialPort.WriteLine(message)
            Logger.add("Message sent to " & serialPortName & ": " & message)
        Catch ex As Exception
            Logger.add("Unable to send message to " & serialPortName & vbNewLine & ex.ToString)
        End Try
    End Sub

    Public Sub sendCommand(ByVal command As Command)
        sendMessage(commandStart & command.dataAsString & commandEnd)
    End Sub

End Module
