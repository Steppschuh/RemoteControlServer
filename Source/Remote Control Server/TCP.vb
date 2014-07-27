Imports System
Imports System.IO
Imports System.Net
Imports System.Net.Sockets
Imports System.Text
Imports System.Threading
Imports System.Windows.Threading

Public Class TCP

    Public Const portReceive As Integer = 1925
    Public Const portSend As Integer = 1927

    Public Const buffer As Integer = 1024
    Public Const receiveTimeout As Integer = 2000

    Public Const retries As Integer = 3
    Public Const retryTimeout As Integer = 1100
    Public Const sendTimeout As Integer = 1000

    Public tcpClient As TcpClient
    Public tcpListener As TcpListener

    Public isListening As Boolean = False
    Private listenTimer As DispatcherTimer

    Public Sub initialize()
        Logger.add("Initializing TCP")
        startListener()
    End Sub

    Public Function sendData(ByRef command As Command) As Boolean
        Dim received As Boolean = False
        Try
            If command.data.Length < 300 Then
                Logger.add("Sending command: " & command.dataAsString())
            Else
                Logger.add("Sending long command")
            End If


            Dim startTime As Long = My.Computer.Clock.TickCount

            tcpClient = New System.Net.Sockets.TcpClient()
            tcpClient.Client.SendTimeout = sendTimeout
            tcpClient.Connect(command.destination, portSend)
            Dim networkStream As NetworkStream = tcpClient.GetStream()

            networkStream.Write(command.data, 0, command.data.Length)
            tcpClient.Close()
            received = True

            Dim endTime As Long = My.Computer.Clock.TickCount
            Dim difTime As Long = endTime - startTime

            Logger.add("Success (" & difTime.ToString & "ms needed)")
        Catch ex As Exception
            Logger.add("Unable to send command to " & command.destination & ":" & portSend)
            Logger.add(ex.ToString)
        End Try
        Return received
    End Function

    Public Sub sendDataRetry(ByRef command As Command)
        For i As Integer = 0 To retries - 1
            If sendData(command) Then
                Exit For
            End If
        Next
    End Sub

    Public Sub sendDataUntilReceived(ByRef command As Command)
        Dim received As Boolean
        Do Until received
            received = sendData(command)
            Thread.Sleep(retryTimeout)
        Loop
    End Sub

    Public Sub startListener()
        Try
            If Not isListening Then
                isListening = True
                tcpListener = New TcpListener(IPAddress.Any, portReceive)
                tcpListener.Start()

                listenTimer = New DispatcherTimer
                AddHandler listenTimer.Tick, AddressOf listenTimerTick
                listenTimer.Interval = New TimeSpan(10000)
                listenTimer.Start()

                Logger.add("TCP listener started at port " & portReceive)
            Else
                Logger.add("TCP listener already running at port " & portReceive)
            End If
        Catch ex As Exception
            isListening = False
            Logger.add("Error while starting TCP listener:")
            Logger.add(ex.ToString)
        End Try
    End Sub

    Public Sub stopListener()
        isListening = False
    End Sub

    Private Sub listenTimerTick(ByVal sender As Object, ByVal e As EventArgs)
        listen()

        If Not isListening Then
            listenTimer.Stop()
        End If
    End Sub


    Public Sub listen()
        Try
            If tcpListener.Pending = True Then
                tcpClient = tcpListener.AcceptTcpClient
                tcpClient.ReceiveBufferSize = buffer

                Dim localEndPoint As Net.IPEndPoint = tcpClient.Client.LocalEndPoint
                Dim remoteEndPoint As Net.IPEndPoint = tcpClient.Client.RemoteEndPoint

                Dim Reader As New BinaryReader(tcpClient.GetStream)
                Try
                    Dim message_data As Byte()
                    message_data = Reader.ReadBytes(tcpClient.ReceiveBufferSize)

                    Dim command As New Command
                    command.source = remoteEndPoint.Address.ToString
                    command.destination = localEndPoint.Address.ToString
                    command.data = message_data
                    command.process()

                    'Logger.add("LocalEndPoint: " & command.destination)
                    'Logger.add("RemoteEndPoint: " & command.source)

                    Reader.Close()
                Catch ex As Exception
                    Logger.add("Error while parsing TCP data:")
                    Logger.add(ex.ToString)
                End Try
            End If
        Catch ex As Exception
            Logger.add("Error while receiving TCP data:")
            Logger.add(ex.ToString)
        End Try
    End Sub

End Class
