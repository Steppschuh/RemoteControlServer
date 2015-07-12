Imports System.Threading

Module ApiV2

    'API v2 introduced commands that were just plain byte arrays sent from a device to the server

    Public Const COMMAND_IDENTIFIER As Byte = 128

    Public Const cmd_connect As Byte = 10
    Public Const cmd_disconnect As Byte = 11
    Public Const cmd_pause As Byte = 12
    Public Const cmd_resume As Byte = 13
    Public Const cmd_control As Byte = 14
    Public Const cmd_pin As Byte = 15
    Public Const cmd_broadcast As Byte = 16

    Public Const cmd_media As Byte = 20
    Public Const cmd_mouse As Byte = 21
    Public Const cmd_pointer As Byte = 22
    Public Const cmd_slideshow As Byte = 23
    Public Const cmd_motion As Byte = 24
    Public Const cmd_keyboard As Byte = 25
    Public Const cmd_scroll As Byte = 26
    Public Const cmd_request As Byte = 27
    Public Const cmd_laser As Byte = 28
    Public Const cmd_custom As Byte = 29

    Public Const cmd_media_play As Byte = 0
    Public Const cmd_media_stop As Byte = 1
    Public Const cmd_media_prev As Byte = 2
    Public Const cmd_media_next As Byte = 3
    Public Const cmd_media_volup As Byte = 4
    Public Const cmd_media_voldown As Byte = 5
    Public Const cmd_media_volmute As Byte = 6
    Public Const cmd_media_launch As Byte = 7

    Public Const cmd_mouse_down As Byte = 0
    Public Const cmd_mouse_up As Byte = 1
    Public Const cmd_mouse_down_2 As Byte = 2
    Public Const cmd_mouse_up_2 As Byte = 3
    Public Const cmd_mouse_move As Byte = 4
    Public Const cmd_mouse_left_down As Byte = 5
    Public Const cmd_mouse_left_up As Byte = 6
    Public Const cmd_mouse_right_down As Byte = 7
    Public Const cmd_mouse_right_up As Byte = 8
    Public Const cmd_mouse_scroll As Byte = 9
    Public Const cmd_mouse_set As Byte = 10
    Public Const cmd_mouse_left_click As Byte = 11
    Public Const cmd_mouse_right_click As Byte = 12

    Public Const cmd_pointer_start As Byte = 0
    Public Const cmd_pointer_end As Byte = 1
    Public Const cmd_pointer_move As Byte = 2
    Public Const cmd_pointer_calibrate As Byte = 3

    Private Const cmd_request_screen As Byte = 0
    Private Const cmd_request_screen_full As Byte = 1
    Private Const cmd_request_screen_next As Byte = 2
    Private Const cmd_request_connect As Byte = 3
    Private Const cmd_request_pin As Byte = 4

    Public readableCommand As String = "Unknown"

    Public Function isBroadcast(ByVal command As Command) As Boolean
        If command.data.Equals(New Byte() {COMMAND_IDENTIFIER, cmd_broadcast}) Then
            Return True
        Else
            Return False
        End If
    End Function

    Public Sub requestPin(ByRef app As App)
        Dim command As New Command
        command.source = Network.getServerIp()
        command.destination = app.ip
        command.priority = RemoteControlServer.Command.PRIORITY_HIGH
        command.data = New Byte() {COMMAND_IDENTIFIER, cmd_request, cmd_request_pin}
        command.send()
    End Sub

    Public Sub answerBroadCast(ByRef app As App)
        'Send server name back
        Dim command As New Command
        command.source = Network.getServerIp()
        command.destination = app.ip
        command.priority = RemoteControlServer.Command.PRIORITY_HIGH
        command.data = Converter.stringToByte(Server.gui.userName)
        command.send()
    End Sub

    Public Sub refuseBroadCast(ByRef app As App)
        'Ignore broadcast
    End Sub

    Public Sub parseCommand(ByRef command As Command)
        'Get remote type
        Dim typeBit As Byte = command.data(1)
        If typeBit < 20 Then
            'General
            parseGeneralCommand(command)
        Else
            'Remote
            parseRemoteCommand(command)
        End If
    End Sub

    Private Sub parseGeneralCommand(ByRef command As Command)
        Dim app As App = Server.getApp(command.source)

        Select Case command.data(1)
            Case cmd_connect
                'App checks if server is reachable
                app.onConnect()
            Case cmd_disconnect
                'App closed
                app.onDisconnect()
            Case cmd_pause
                'Remote inactive
                app.onPause()
            Case cmd_resume
                'Remote resumed
                app.onResume()
            Case cmd_pin
                'App authenticates
                app.pin = Converter.byteToString(command.data, 2)
            Case cmd_control
                'Remote changed
                Select Case command.data(2)
                    Case cmd_media
                        app.lastControl = "Media"
                    Case cmd_mouse
                        app.lastControl = "Mouse"
                    Case cmd_pointer
                        app.lastControl = "Pointer"
                    Case cmd_slideshow
                        app.lastControl = "Slideshow"
                    Case cmd_motion
                        app.lastControl = "Motion"
                    Case cmd_keyboard
                        app.lastControl = "Keyboard"
                    Case cmd_scroll
                        app.lastControl = "Scroll"
                    Case cmd_request
                        app.lastControl = "Request"
                    Case cmd_laser
                        app.lastControl = "Laser"
                    Case cmd_custom
                        app.lastControl = "Custom"
                    Case Else
                        app.lastControl = "Unknown"
                End Select
                Logger.add("Control set to " & app.lastControl)
            Case Else
                Logger.add("Unknown general command")
        End Select
    End Sub

    Private Sub parseRemoteCommand(ByRef command As Command)
        Select Case command.data(1)
            Case cmd_mouse
                parseMouseCommand(command)
            Case cmd_pointer
                parsePointerCommand(command)
            Case cmd_media
                parseMediaCommand(command)
            Case cmd_request
                parseRequest(command)
            Case cmd_laser
                parseLaserCommand(command)
            Case cmd_custom
                parseCustomCommand(command)
            Case Else
                Logger.add("Unknown remote command")
        End Select
    End Sub

    Private Sub parseMouseCommand(ByRef command As Command)
        Select Case command.data(2)
            Case cmd_mouse_move
                readableCommand = "Move cursor"
                MouseV2.parseCursorMove(command.data)
            Case cmd_mouse_set
                readableCommand = "Set cursor"
                MouseV2.parseCursorSet(command.data)
            Case cmd_mouse_scroll
                readableCommand = "Scroll"
                MouseV2.parseScroll(command.data)
            Case Else
                readableCommand = "Click"
                MouseV2.parseClick(command.data)
        End Select
    End Sub

    Private Sub parsePointerCommand(ByRef command As Command)
        Select Case command.data(2)
            Case cmd_pointer_move
                readableCommand = "Move"
                MouseV2.parsePointer(command.data)
                Logger.add("Pointer: " & readableCommand)
            Case cmd_pointer_start
                readableCommand = "Show"
                Server.pointer.showPointer()
                Logger.add("Pointer: " & readableCommand)
            Case cmd_pointer_end
                readableCommand = "Hide"
                Server.pointer.hidePointer()
                Logger.add("Pointer: " & readableCommand)
            Case cmd_pointer_calibrate
                readableCommand = "Calibrate"
                MouseV2.calibratePointer(command.data)
                Logger.add("Pointer: " & readableCommand)
        End Select
    End Sub

    Private Sub parseLaserCommand(ByRef command As Command)
        readableCommand = "Move"
        MouseV2.parseLaser(command.data)
        Logger.add("Laser: " & readableCommand)
    End Sub

    Private Sub parseMediaCommand(ByRef command As Command)
        Select Case command.data(2)
            Case cmd_media_play
                readableCommand = "Play"
                Media.playMedia()
            Case cmd_media_stop
                readableCommand = "Stop"
                Media.stopMedia()
            Case cmd_media_prev
                readableCommand = "Previous"
                Media.previousMedia()
            Case cmd_media_next
                readableCommand = "Next"
                Media.nextMedia()
            Case cmd_media_voldown
                readableCommand = "Volume down"
                Media.volumeDown()
            Case cmd_media_volup
                readableCommand = "Volume up"
                Media.volumeUp()
            Case cmd_media_volmute
                readableCommand = "Volume mute"
                Media.volumeMute()
            Case cmd_media_launch
                readableCommand = "Launch player"
                Media.launchPlayer()
            Case Else
                readableCommand = "Unknown"
        End Select
        Logger.add("Media: " & readableCommand)
    End Sub

    Private Sub parseRequest(ByRef command As Command)
        Select Case command.data(2)
            Case cmd_request_screen
                readableCommand = "Screenshot (normal)"
                Screenshot.sendScreenshot(False)
            Case cmd_request_screen_next
                readableCommand = "Toggle screen"
                Screenshot.toogleScreen()
            Case cmd_request_screen_full
                readableCommand = "Screenshot (full)"
                Screenshot.sendScreenshot(True)
        End Select
        Logger.add("Request: " & readableCommand)
    End Sub

    Private Sub parseCustomCommand(ByRef command As Command)
        Select Case command.data(2)
            Case 1
                readableCommand = "Button 1"
                Serial.sendMessage("<01>")
            Case 2
                readableCommand = "Button 2"
                Serial.sendMessage("<02>")
            Case 3
                readableCommand = "Button 3"
                Serial.sendMessage("<03>")
            Case 4
                readableCommand = "Button 4"
                Serial.sendMessage("<04>")
            Case 5
                readableCommand = "Button 5"
                Serial.sendMessage("<05>")
            Case 6
                readableCommand = "Button 6"
                Serial.sendMessage("<06>")
            Case 7
                readableCommand = "Button 7"
                Serial.sendMessage("<07>")
            Case 8
                readableCommand = "Button 8"
                Serial.sendMessage("<08>")
            Case 9
                readableCommand = "Button 9"
                Serial.sendMessage("<09>")
            Case 0
                readableCommand = "Button 0"
                Serial.sendMessage("<00>")
            Case 10
                readableCommand = "Volume up"
                Serial.sendMessage("<10>")
            Case 11
                readableCommand = "Volume down"
                Serial.sendMessage("<11>")
            Case 12
                readableCommand = "Scrollbar move"
                If (MouseV2.X_Def = 0) Then
                    MouseV2.X_Def = command.data(3)
                Else
                    MouseV2.X_Rel = MouseV2.X_Def - command.data(3)
                    If MouseV2.X_Rel < MouseV2.X_New - 10 Or MouseV2.X_Rel > MouseV2.X_New + 10 Then
                        MouseV2.X_New = MouseV2.X_Rel
                        Dim value As Integer = 64 + MouseV2.X_Rel
                        If value > 127 Then
                            value = 127
                        ElseIf value < 0 Then
                            value = 0
                        End If
                        readableCommand = "Scrollbar " & value
                        Serial.sendMessage("<12" & Chr(value) & ">")
                    End If
                End If
            Case 13
                readableCommand = "Scrollbar down"
                MouseV2.X_Def = command.data(3)
                Serial.sendMessage("<13>")
            Case 14
                readableCommand = "Scrollbar up"
                MouseV2.X_Def = 0
                Serial.sendMessage("<14>")
            Case Else
                readableCommand = "Unknown"
        End Select
        Logger.add("Custom: " & readableCommand)
    End Sub

End Module
