Public Class Command

    Public Const PRIORITY_LOW As Byte = 0 'UDP
    Public Const PRIORITY_MEDIUM As Byte = 1 'TCP
    Public Const PRIORITY_HIGH As Byte = 2 'TCP retry
    Public Const PRIORITY_INDISPENSABLE As Byte = 3 'TCP retry forever

    Public source As String = "Unknown"
    Public destination As String = "Unknown"
    Public priority As Byte = 0
    Public type As Byte
    Public data As Byte()
    Public api As Byte = Remote.latestApi

    Public Sub send()
        Try
            Network.sendCommand(Me)
        Catch ex As Exception
            Logger.add("Unable to send command to " & destination & vbNewLine & ex.ToString)
        End Try
    End Sub

    Public Sub process()
        Try
            If Authentication.isAuthenticated(source, Server.getApp(source).pin) Then
                'If Authentication.isAuthenticated(source, "") Then
                parse()
            Else
                Logger.add("Refused a command from " & source)
            End If
        Catch ex As Exception
            Logger.add("Error while processing command: " & vbNewLine & ex.ToString)
        End Try
    End Sub

    Private Sub parse()
        'Detect used API version
        Dim commandByte As Byte = data(0)

        If commandByte.Equals(ApiV3.cmd_command + 1) Then
            'Newer API than available
            api = Remote.latestApi
            'TODO: Notify user
            'TODO: Check for updates
        ElseIf commandByte.Equals(ApiV3.cmd_command) Then
            'Android version 2.1.0 and up
            api = 3
        ElseIf commandByte.Equals(ApiV2.cmd_command) Then
            'Android version 1.8.0 - 2.0.3
            api = 2
        Else
            'Older implementations
            api = 1
        End If

        'Logger.add("Detected API version: " & api)
        Remote.processCommand(Me)
    End Sub

    Public Function dataAsString() As String
        Return System.Text.Encoding.UTF8.GetString(data)
    End Function

    Public Sub log()
        Logger.add(source & ": " & dataAsString())
    End Sub

End Class
