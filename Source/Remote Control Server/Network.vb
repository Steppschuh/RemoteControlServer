Imports System.Net
Imports System.Threading
Imports System.IO

Public Module Network

    Public Const localHost As String = "127.0.0.1"
    Public hostIps As List(Of String)
    Public anyIpEndPoint As New System.Net.IPEndPoint(System.Net.IPAddress.Any, 0)

    Public tcp As TCP
    Public udp As UDP

    Public commandCount As Long = 0

    Public Sub initialize()
        Logger.add("Initializing network")

        hostIps = New List(Of String)
        getHostIps()

        tcp = New TCP
        tcp.initialize()

        udp = New UDP
        udp.initialize()

        Dim listenThread As Thread = New Thread(New ThreadStart(AddressOf checkListenersRunning))
        listenThread.Start()
    End Sub

    Public Function checkListenersRunning() As Boolean
        If tcp.isListening And udp.isListening Then
            Return True
        Else
            Logger.add("Network initialization failed, not all listeners could be started.")
            Server.gui.showErrorDialog("Error", "Network initialization failed, not all listeners could be started." _
                                  & vbNewLine & "Make sure that only one instance of the Remote Control Server is running.")
            Return False
        End If
    End Function

    Public Sub getHostIps()
        Logger.add("Active Wi-Fi network adapters:")

        For Each ipAddress As IPAddress In Dns.GetHostByName(Dns.GetHostName()).AddressList
            Dim ip As String = ipAddress.ToString
            hostIps.Add(ip)
            Logger.add(" - " & ip)
        Next
    End Sub

    Public Function getServerIp() As String
        Dim ip As String = "Unknown"
        Try
            ip = hostIps(hostIps.Count - 1)

            If hostIps.Contains(Remote.lastCommand.destination) Then
                ip = lastCommand.destination
            End If
        Catch ex As Exception

        End Try
        Return ip
    End Function

    Private Sub sendCommandThread(ByVal command As Command)
        Select Case command.priority
            Case RemoteControlServer.Command.PRIORITY_LOW
                udp.sendData(command)
            Case RemoteControlServer.Command.PRIORITY_MEDIUM
                tcp.sendData(command)
            Case RemoteControlServer.Command.PRIORITY_HIGH
                tcp.sendDataRetry(command)
            Case RemoteControlServer.Command.PRIORITY_INDISPENSABLE
                tcp.sendDataUntilReceived(command)
            Case Else
                tcp.sendData(command)
        End Select
    End Sub

    Public Sub sendCommand(ByVal command As Command)
        Dim sendThread As Thread = New Thread(Sub() sendCommandThread(command))
        sendThread.Start()
    End Sub

    Public Sub getWebRequestAsync(ByVal url As String, ByVal callback As Func(Of String, Boolean))
        Dim requestThread As Thread = New Thread(Sub() getWebRequest(url, callback))
        requestThread.Start()
    End Sub

    Private Sub getWebRequest(ByVal url As String, ByVal callback As Func(Of String, Boolean))
        callback(getWebRequest(url))
    End Sub

    Public Function getWebRequest(ByVal url As String) As String
        Dim result = ""
        Try
            Dim request As WebRequest = WebRequest.Create(url)
            request.Timeout = 5000

            Dim response As WebResponse = request.GetResponse()
            Dim dataStream As Stream = response.GetResponseStream()
            Dim reader As New StreamReader(dataStream)
            Dim responseFromServer As String = reader.ReadToEnd()
            result = responseFromServer
            reader.Close()
            response.Close()
        Catch ex As Exception
            Logger.add("Unable to get webrequest: " & url)
        End Try
        Return result
    End Function

    Public Sub loadInBrowser(ByVal url As String)
        Server.gui.loadInBrowser(url)
    End Sub

    Public Function isValidIp(ByVal ip As String) As Boolean
        Try
            System.Net.IPAddress.Parse(ip)
            Return True
        Catch ex As Exception
            Return False
        End Try
    End Function

End Module
