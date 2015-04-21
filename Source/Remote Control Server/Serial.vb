Imports System.Threading

Module Serial

    'This module can be used to control an arduino using the remote control app
    Public currentSerialPortName As String = "auto"
    Public Const commandStart As Char = "<"
    Public Const commandEnd As Char = ">"
    Public Const writeTimeout As Integer = 100

    Public serialPort As IO.Ports.SerialPort

    Public isSending As Boolean = False
    Public shouldListen As Boolean = True

    Public Function openSerialPort(ByVal name As String) As Boolean
        Try
            If name.ToLower = "auto" Then
                If My.Computer.Ports.SerialPortNames.Count > 0 Then
                    name = My.Computer.Ports.SerialPortNames.Item(My.Computer.Ports.SerialPortNames.Count - 1)
                Else
                    Throw New Exception
                End If
            End If
            currentSerialPortName = name
            serialPort = My.Computer.Ports.OpenSerialPort(name)
            serialPort.WriteTimeout = writeTimeout
            startReading()
        Catch ex As Exception
            Logger.add("Unable to open serial port " & currentSerialPortName & ": " & ex.Message)
        End Try
    End Function

    Public Function closeSerialPort()
        Try
            stopReading()
            serialPort.Close()
        Catch ex As Exception
            Logger.add("Unable to close serial port " & currentSerialPortName & ": " & ex.Message)
        End Try
    End Function

    Public Sub sendMessage(ByVal message As String)
        isSending = True
        Try
            If serialPort Is Nothing Then
                openSerialPort(serialPortName)
            End If
            If Not serialPort.IsOpen Then
                openSerialPort(serialPortName)
            End If
            serialPort.WriteLine(message)
            Logger.add("Sent to " & currentSerialPortName & ": " & message)
        Catch ex As Exception
            Logger.add("Unable to send message to " & currentSerialPortName & ": " & ex.Message)
        End Try
        isSending = False
    End Sub

    Public Sub sendMessageAsync(ByVal message As String)
        If Not isSending Then
            Dim initializeThread = New Thread(Sub() sendMessage(message))
            initializeThread.IsBackground = True
            initializeThread.Start()
        End If
    End Sub

    Public Sub startReading()
        shouldListen = True
        Dim readThread = New Thread(AddressOf read)
        readThread.IsBackground = True
        readThread.Start()
    End Sub

    Public Sub stopReading()
        shouldListen = False
    End Sub

    Public Sub read()
        While shouldListen
            Try
                If serialPort Is Nothing Then
                    Exit Try
                End If
                If Not serialPort.IsOpen Then
                    Exit Try
                End If
                Dim message As String = serialPort.ReadLine()
                Logger.add("Received: " & message)
            Catch ex As Exception

            End Try
        End While
    End Sub

    Public Sub sendCommand(ByVal command As Command)
        sendMessage(commandStart & command.dataAsString & commandEnd)
    End Sub

End Module