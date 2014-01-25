Public Class App

    Public ip As String = "Unknown"
    Public license As License = New License
    Public appVersion As String = "Unknown"
    Public appName As String = "Unknown"
    Public osVersion As String = "Unknown"
    Public deviceName As String = "Unknown"
    Public status As String = "Unknown"

    Public detectedOs As Byte = 0

    Public lastControl As String = "Unknown"
    Public lastCommand As String = "Unknown"

    Public isConnected As Boolean = False
    Public isActive As Boolean = False


    Public Sub onConnect()
        isConnected = True
        isActive = True
        Logger.add(deviceName & " connected")
        gui.showNotification("App connected", deviceName & " has connected to the Remote Control Server")
    End Sub

    Public Sub onDisconnect()
        isConnected = False
        isActive = False
        Logger.add(deviceName & " disconnected")
    End Sub

    Public Sub onPause()
        isActive = False
        Logger.add(deviceName & " paused the connection")
    End Sub

    Public Sub onResume()
        isActive = False
        Logger.add(deviceName & " resumed the connection")
    End Sub

    Public Sub onBroadCast(ByRef command As Command)
        Try
            Logger.add("Connection request from " & ip)
            If Authentication.isAuthenticated(ip) Then
                'App ip is whitelisted
                Logger.add("Allowing to connect")
                answerBroadCast(command)
            Else
                If Settings.usePin Then
                    'Request pin
                    Logger.add("Requesting Pin")
                    requestPin(command)
                Else
                    'Block request
                    Logger.add("Connection blocked")
                    refuseBroadCast(command)
                    gui.showNotification("Connection blocked", "A connection attempt from " & ip & " has been blocked.")
                End If
            End If
        Catch ex As Exception

        End Try
    End Sub

    Public Sub answerBroadCast(ByRef command As Command)
        Select Case command.api
            Case 3
                ApiV3.answerBroadCast(Me)
            Case 1, 2
                ApiV2.answerBroadCast(Me)
            Case Else
                ApiV3.answerBroadCast(Me)
        End Select
    End Sub

    Public Sub refuseBroadCast(ByRef command As Command)
        Select Case command.api
            Case 3
                ApiV3.refuseBroadCast(Me)
            Case 1, 2
                ApiV2.refuseBroadCast(Me)
            Case Else
                ApiV3.refuseBroadCast(Me)
        End Select
    End Sub

    Public Sub requestPin(ByRef command As Command)
        Select Case command.api
            Case 3
                ApiV3.requestPin(Me)
            Case 1, 2
                ApiV2.requestPin(Me)
            Case Else
                ApiV3.requestPin(Me)
        End Select
    End Sub

    Public Sub detectOs()
        If osVersion.ToLower.Contains("android") Then
            detectedOs = Settings.OS_ANDROID
        ElseIf osVersion.ToLower.Contains("bb") Then
            detectedOs = Settings.OS_BLACKBERRY
        ElseIf osVersion.ToLower.Contains("ios") Then
            detectedOs = Settings.OS_IOS
        Else
            detectedOs = Settings.OS_DEFAULT
        End If
    End Sub

End Class
