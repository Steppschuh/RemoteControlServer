Imports System
Imports System.IO
Imports System.Net
Imports System.Net.Sockets
Imports System.Windows.Threading
Imports System.Threading

Public Class UDP

    Public Const port As Integer = 1926
    Public Const timeout As Integer = 2000

    Public udpClient As UdpClient

    Public isListening As Boolean = False
    Private listenThread As Thread

    Public Sub initialize()
        Logger.add("Initializing UDP")

        startListener()
    End Sub

    Public Sub sendData(ByRef command As Command)

    End Sub

    Public Sub startListener()
        Try
            If Not isListening Then
                isListening = True

                udpClient = New UdpClient(port)
                udpClient.Client.ReceiveTimeout = timeout

                listenThread = New Thread(New ThreadStart(AddressOf listen))
                listenThread.Start()

                Logger.add("UDP listener started at port " & port)
            Else
                Logger.add("UDP listener already running at port " & port)
            End If
        Catch ex As Exception
            isListening = False
            Logger.add("Error while starting UDP listener:")
            Logger.add(ex.ToString)
        End Try
    End Sub

    Public Sub stopListener()
        isListening = False
    End Sub

    Public Sub listen()
        Do While isListening
            Try
                Dim message_data As Byte()
                message_data = udpClient.Receive(Network.anyIpEndPoint)
                Try
                    If Not message_data Is Nothing Then
                        Dim command As Command = New Command
                        command.source = Remote.lastCommand.source
                        command.destination = Remote.lastCommand.destination
                        command.data = message_data
                        command.process()
                    End If
                Catch ex As Exception
                    Logger.add("Error while receiving UDP data:")
                    Logger.add(ex.ToString)
                End Try
            Catch ex As Exception
                'udpClient.Receive timed out, this is alright
            End Try
        Loop
    End Sub
End Class
