Imports System.Windows.Forms

Module ApiV1

    Public Const cmd_info_device_name As String = "[cmd_15]"
    Public Const cmd_info_device_osversion As String = "[cmd_16]"
    Public Const cmd_info_app_name As String = "[cmd_17]"
    Public Const cmd_info_app_version As String = "[cmd_18]"
    Public Const cmd_info_app_mode As String = "[cmd_19]"


    Public Const cmd_connect As String = "[cmd_connect]"
    Public Const cmd_disconnect As String = "[cmd_disconnect]"
    Public Const cmd_lizens As String = "[cmd_lizens]"
    Public Const cmd_version As String = "[cmd_version]"

    Public Const cmd_mouse_string As String = "[cmd_mouse]"
    Public Const cmd_keyboard_string As String = "[cmd_keyboard]"
    Public Const cmd_media_string As String = "[cmd_music]"
    Public Const cmd_slideshow_string As String = "[cmd_slide]"
    Public Const cmd_process_string As String = "[cmd_process]"
    Public Const cmd_scroll_string As String = "[cmd_scroll]"
    Public Const cmd_broadcast_string As String = "[cmd_broadcast]"
    Public Const cmd_google_string As String = "[cmd_google]"
    Public Const cmd_shortcut_string As String = "[cmd_short]"

    Public readableCommand As String = "Unknown"

    Public Sub parseCommand(ByRef command As Command)
        Dim cmd As String = command.dataAsString
        Dim value As String = getCommandValue(cmd)
        Dim app As App = Server.getApp(command.source)

        readableCommand = "Unknown"
        If cmd.Contains(cmd_mouse_string) Then
            parseMouseCommand(value)
            Logger.add("Mouse: " & value)
        ElseIf cmd.Contains(cmd_keyboard_string) Then
            parseKeyboardCommand(cmd)
            Logger.add("Keyboard: " & readableCommand)
        ElseIf cmd.Contains(cmd_media_string) Then
            If (app.license.isProVersion) Then
                parseMediaCommand(cmd)
            Else
                app.license.requestUpgrade()
            End If
            Logger.add("Media: " & readableCommand)
        ElseIf cmd.Contains(cmd_slideshow_string) Then
            If (app.license.isProVersion) Then
                parseSlideshowCommand(cmd)
            Else
                app.license.requestUpgrade()
            End If
            Logger.add("Slideshow: " & readableCommand)
        ElseIf cmd.Contains(cmd_scroll_string) Then
            parseScrollCommand(cmd)
            Logger.add("Scroll: " & readableCommand)
        ElseIf cmd.Contains(cmd_shortcut_string) Then
            parseShortcutCommand(cmd)
            Logger.add("Shortcut: " & readableCommand)
        ElseIf cmd.Contains(cmd_process_string) Then
            Try
                Process.Start(value)
            Catch ex As Exception
            End Try
            Logger.add("Process: " & value)
        ElseIf cmd.Contains(cmd_google_string) Then
            Try
                Process.Start("http://google.de/search?q=" & value)
            Catch ex As Exception
            End Try
            Logger.add("Google: " & value)

            'Connection
        ElseIf cmd.Contains(cmd_broadcast_string) Then
            command.source = value
            app = Server.getApp(value)
            app.onBroadCast(command)
        ElseIf cmd.Contains(cmd_connect) Then
            app.deviceName = value
            Logger.add("Device name set to " & value)
            app.onConnect()
        ElseIf cmd.Contains(cmd_disconnect) Then
            app = Server.getApp(value)
            app.onDisconnect()

            'Info
        ElseIf cmd.Contains(cmd_info_device_name) Then
            app.deviceName = value
            Logger.add("Device name set to " & value)
        ElseIf cmd.Contains(cmd_info_device_osversion) Then
            app.osVersion = value
            app.detectOs()
            Logger.add("Device OS version set to " & value)
        ElseIf cmd.Contains(cmd_info_app_name) Then
            app.appName = value
            Logger.add("App name set to " & value)
        ElseIf cmd.Contains(cmd_info_app_version) Or cmd.Contains(cmd_version) Then
            app.appVersion = value
            Logger.add("App version set to " & value)
        ElseIf cmd.Contains(cmd_info_app_mode) Or cmd.Contains(cmd_lizens) Then
            Dim license As New License
            If value.ToLower.Equals("pro") Or value.ToLower.Equals("trial") Then
                license.isProVersion = True
            Else
                license.isProVersion = False
            End If
            app.license = license
            Logger.add("App license set to " & value)
        Else
            Logger.add("Unknown ApiV1: " + cmd)
        End If

    End Sub

    Public Function getCommandValue(ByVal cmd As String) As String
        Dim value As String
        value = cmd.Substring(InStr(cmd, "]"))
        value = value.Replace(vbLf, "")
        Return value
    End Function

    Public Sub parseMouseCommand(ByVal cmd As String)
        Mouse.parseMouse(cmd)
    End Sub

    Public Sub parseKeyboardCommand(ByVal cmd As String)
        If cmd.Contains("<") And cmd.Contains(">") Then
            If cmd.Contains("<Enter>") Then
                readableCommand = "Enter"
                Keyboard.sendKeyPress(Keys.Enter)
                Exit Sub
            ElseIf cmd.Contains("<Back>") Then
                readableCommand = "Backspace"
                Keyboard.sendKeyPress(Keys.Back)
                Exit Sub
            ElseIf cmd.Contains("<tab>") Then
                readableCommand = "Tab"
                Keyboard.sendKeyPress(Keys.Tab)
                Exit Sub
            ElseIf cmd.Contains("<caps>") Then
                readableCommand = "Capslock"
                Keyboard.sendKeyPress(Keys.CapsLock)
                Exit Sub
            ElseIf cmd.Contains("<ctrl>") Then
                readableCommand = "Control"
                If cmd.Contains("down") Then
                    sendKeyDown(Keys.ControlKey)
                Else
                    sendKeyUp(Keys.ControlKey)
                End If
                Exit Sub
            ElseIf cmd.Contains("<alt>") Then
                readableCommand = "Alt"
                If cmd.Contains("down") Then
                    'SendKeyDown(Keys.Alt)
                    sendKeyDown(&H1)
                Else
                    'SendKeyUp(Keys.Alt)
                    sendKeyUp(&H1)
                End If
                Exit Sub
            ElseIf cmd.Contains("<shift>") Then
                readableCommand = "Shift"
                If cmd.Contains("down") Then
                    sendKeyDown(Keys.ShiftKey)
                Else
                    sendKeyUp(Keys.ShiftKey)
                End If
            ElseIf cmd.Contains("<del>") Then
                readableCommand = "Delete"
                Keyboard.sendKeyPress(Keys.Delete)
            ElseIf cmd.Contains("<win>") Then
                readableCommand = "Windows"
                Keyboard.sendKeyPress(Keys.LWin)
                Exit Sub
            ElseIf cmd.Contains("<esc>") Then
                readableCommand = "Escape"
                Keyboard.sendKeyPress(Keys.Escape)
                Exit Sub
            ElseIf cmd.Contains("<end>") Then
                readableCommand = "End"
                Keyboard.sendKeyPress(Keys.End)
                Exit Sub
            ElseIf cmd.Contains("<tab>") Then
                readableCommand = "Tab"
                Keyboard.sendKeyPress(Keys.Tab)
                Exit Sub
            ElseIf cmd.Contains("<ins>") Then
                readableCommand = "Insert"
                Keyboard.sendKeyPress(Keys.Insert)
                Exit Sub
            ElseIf cmd.Contains("<up>") Then
                readableCommand = "Up"
                Keyboard.sendKeyPress(Keys.Up)
                Exit Sub
            ElseIf cmd.Contains("<down>") Then
                readableCommand = "Down"
                Keyboard.sendKeyPress(Keys.Down)
                Exit Sub
            ElseIf cmd.Contains("<left>") Then
                readableCommand = "Left"
                Keyboard.sendKeyPress(Keys.Left)
                Exit Sub
            ElseIf cmd.Contains("<right>") Then
                readableCommand = "Right"
                Keyboard.sendKeyPress(Keys.Right)
                Exit Sub
            End If
        Else
            Try
                readableCommand = getCommandValue(cmd)
                Keyboard.sendEachKey(readableCommand)
            Catch ex As Exception
                Logger.add(ex.ToString)
            End Try
        End If
    End Sub

    Public Sub parseMediaCommand(ByVal cmd As String)
        Dim value As String = getCommandValue(cmd)
        If value = "play" Then
            readableCommand = "Play"
            Media.playMedia()
        ElseIf value = "stop" Then
            readableCommand = "Stop"
            Media.stopMedia()
        ElseIf value = "next" Then
            readableCommand = "Next"
            Media.nextMedia()
        ElseIf value = "prev" Then
            readableCommand = "Previous"
            Media.previousMedia()
        ElseIf value = "volup" Then
            readableCommand = "Volume up"
            Media.volumeUp()
        ElseIf value = "voldown" Then
            readableCommand = "Volume down"
            Media.volumeDown()
        ElseIf value = "mute" Then
            readableCommand = "Mute"
            Media.volumeMute()
        ElseIf value = "launch" Then
            readableCommand = "Launch player"
            Media.launchPlayer()
        End If
    End Sub

    Public Sub parseScrollCommand(ByVal cmd As String)
        Dim value As String = getCommandValue(cmd)
        If value = "zoomin" Then
            readableCommand = "Zoom in"
            Mouse.zoom(1, 1)
        ElseIf value = "zoomout" Then
            readableCommand = "Zoom out"
            Mouse.zoom(-1, 1)
        ElseIf value = "back" Then
            readableCommand = "Back"
            Keyboard.sendKeyPress(Keys.BrowserBack)
        ElseIf value = "forward" Then
            readableCommand = "Forward"
            Keyboard.sendKeyPress(Keys.BrowserForward)
        ElseIf value = "pageup" Then
            readableCommand = "Page up"
            Keyboard.sendKeyPress(Keys.PageUp)
        ElseIf value = "pagedown" Then
            readableCommand = "Page down"
            Keyboard.sendKeyPress(Keys.PageDown)
        ElseIf value = "cancel" Then
            readableCommand = "Cancel"
            Keyboard.sendKeyPress(Keys.BrowserStop)
        ElseIf value = "refresh" Then
            readableCommand = "Refresh"
            Keyboard.sendKeyPress(Keys.BrowserRefresh)
        ElseIf value = "fullexit" Then
            readableCommand = "Exit fullscreen"
            Keyboard.sendKeyPress(Keys.Escape)
        ElseIf value = "fullscreen" Then
            readableCommand = "Fullscreen"
            Keyboard.sendKeyPress(Keys.F11)
        End If
    End Sub

    Public Sub parseShortcutCommand(ByVal cmd As String)
        Dim value As String = getCommandValue(cmd)
        If value = "desktop" Then
            readableCommand = "Show desktop"
            Keyboard.sendShortcut(New List(Of Key) From {Keys.LWin, Keys.D})
        ElseIf value = "close" Then
            readableCommand = "Close"
            'Doesn't work in the .Net Framework:
            'Keyboard.sendShortcut(New List(Of Key) From {Keys.Alt, Keys.F4})
            Keyboard.sendKeys("%{F4}")
        ElseIf value = "copy" Then
            readableCommand = "Copy"
            Keyboard.sendShortcut(New List(Of Key) From {Keys.ControlKey, Keys.C})
        ElseIf value = "paste" Then
            readableCommand = "Paste"
            Keyboard.sendShortcut(New List(Of Key) From {Keys.ControlKey, Keys.V})
        ElseIf value = "selectall" Then
            readableCommand = "Select all"
            Keyboard.sendShortcut(New List(Of Key) From {Keys.ControlKey, Keys.A})
        ElseIf value = "undo" Then
            readableCommand = "Undo"
            Keyboard.sendShortcut(New List(Of Key) From {Keys.ControlKey, Keys.Z})
        ElseIf value = "standby" Then
            readableCommand = "Standby"
            Keyboard.standby()
        ElseIf value = "shutdown" Then
            readableCommand = "Shutdown"
            Keyboard.shutdown()
        End If
    End Sub

    Public Sub parseSlideshowCommand(ByVal cmd As String)
        Dim value As String = getCommandValue(cmd)
        If value = "pause" Then
            readableCommand = "Pause slideshow"
            Keyboard.sendKeyPress(Keys.B)
        ElseIf value = "next" Then
            readableCommand = "Next slide"
            Keyboard.sendKeyPress(Keys.Right)
        ElseIf value = "prev" Then
            readableCommand = "Previous slide"
            Keyboard.sendKeyPress(Keys.Left)
        End If
    End Sub

End Module
