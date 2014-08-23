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
            If Authentication.isAuthenticated(source, Server.getApp(source).pin) Or isBroadcast() Then
                parse()
            Else
                Logger.add("Refused a command from " & source)
                Logger.add("Server protection is active")
            End If
        Catch ex As Exception
            Logger.add("Error while processing command: " & vbNewLine & ex.ToString)
        End Try
    End Sub

    Private Sub parse()
        'Detect used API version
        Try
            Dim commandByte As Byte = data(0)

            If commandByte.Equals(ApiV3.COMMAND_IDENTIFIER + 1) Then
                'Newer API than available
                api = Remote.latestApi
                'TODO: Notify user
                'TODO: Check for updates
            ElseIf commandByte.Equals(ApiV3.COMMAND_IDENTIFIER) Then
                'Android version 2.1.0 and up
                api = 3
            ElseIf commandByte.Equals(ApiV2.COMMAND_IDENTIFIER) Then
                'Android version 1.8.0 - 2.0.3
                api = 2
            Else
                'Older implementations
                api = 1
            End If

            'Logger.add("Detected API version: " & api)
            Network.commandCount += 1
            Remote.processCommand(Me)
        Catch ex As Exception
            Logger.add("Error while parsing command: " & vbNewLine & ex.ToString)
        End Try
    End Sub

    Public Function dataAsString() As String
        Return System.Text.Encoding.UTF8.GetString(data)
    End Function

    Public Sub log()
        Logger.add(source & ": " & dataAsString())
    End Sub

    Public Function isBroadcast() As Boolean
        If ApiV1.isBroadcast(Me) Or ApiV2.isBroadcast(Me) Or ApiV3.isBroadcast(Me) Then
            Return True
        Else
            Return False
        End If
    End Function

End Class
