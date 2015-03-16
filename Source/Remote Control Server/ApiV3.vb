Imports System.Drawing
Imports System.Threading

Module ApiV3

    'API v3 uses commands similar to API v2 but supports communication from the server to an app

    Public Const COMMAND_IDENTIFIER As Byte = 127

    'first index
    Public Const cmd_connection As Byte = 10
    Public Const cmd_disconnect As Byte = 11
    Public Const cmd_pause As Byte = 12
    Public Const cmd_resume As Byte = 13
    Public Const cmd_control As Byte = 14
    Public Const cmd_pin As Byte = 15
    Public Const cmd_broadcast As Byte = 16
    Public Const cmd_set As Byte = 17
    Public Const cmd_get As Byte = 19
    Public Const cmd_open As Byte = 18


    'second index
    'connect
    Public Const cmd_connection_reachable As Byte = 0
    Public Const cmd_connection_protected As Byte = 1
    Public Const cmd_connection_connect As Byte = 2
    Public Const cmd_connection_disconnect As Byte = 3

    'action
    Public Const cmd_action_up As Byte = 0
    Public Const cmd_action_down As Byte = 1
    Public Const cmd_action_click As Byte = 2
    Public Const cmd_action_process As Byte = 3

    'setter
    Public Const cmd_set_pin As Byte = 0
    Public Const cmd_set_app_version As Byte = 1
    Public Const cmd_set_app_name As Byte = 2
    Public Const cmd_set_os_version As Byte = 3
    Public Const cmd_set_device_name As Byte = 5

    'getter
    Public Const cmd_get_server_version As Byte = 1
    Public Const cmd_get_server_name As Byte = 2
    Public Const cmd_get_os_name As Byte = 4
    Public Const cmd_get_screenshot As Byte = 5
    Public Const cmd_get_screenshots As Byte = 7
    Public Const cmd_get_api_version As Byte = 6

    'mouse
    Public Const cmd_mouse As Byte = 20
    Public Const cmd_mouse_pointers As Byte = 1
    Public Const cmd_mouse_pointers_absolute As Byte = 0
    Public Const cmd_mouse_pointers_absolute_presenter As Byte = 5
    Public Const cmd_mouse_pad_action As Byte = 2
    Public Const cmd_mouse_left_action As Byte = 3
    Public Const cmd_mouse_right_action As Byte = 4

    'keyboard
    Public Const cmd_keyboard As Byte = 21
    Public Const cmd_keyboard_unicode As Byte = 0
    Public Const cmd_keyboard_string As Byte = 1
    Public Const cmd_keyboard_keycode As Byte = 2


    Public Function isBroadcast(ByVal command As Command) As Boolean
        If command.data.Equals(New Byte() {COMMAND_IDENTIFIER, cmd_broadcast}) Then
            Return True
        ElseIf command.data(2) = ApiV3.cmd_connection_reachable Then
            Return True
        Else
            Return False
        End If
    End Function

    Public Function isConnectionCommand(ByVal command As Command) As Boolean
        Select Case command.data(1)
            Case cmd_connection
                Return True
            Case cmd_set
                Return True
        End Select
        Return False
    End Function

    Public Sub requestPin(ByRef app As App)
        'Send pin request
        Dim command As New Command
        command.source = Network.getServerIp()
        command.destination = app.ip
        command.priority = RemoteControlServer.Command.PRIORITY_HIGH

        Dim serverName As Byte() = New Byte() {COMMAND_IDENTIFIER, cmd_connection, cmd_connection_protected}
        command.data = buildCommandData(serverName, Converter.stringToByte(Server.gui.userName))

        command.send()
    End Sub

    Public Sub validatePin(ByRef app As App)
        'Send if pin is valid or not
        Dim command As New Command
        command.source = Network.getServerIp()
        command.destination = app.ip
        command.priority = RemoteControlServer.Command.PRIORITY_HIGH

        If (isAuthenticated(app.ip, app.pin)) Then
            command.data = New Byte() {COMMAND_IDENTIFIER, cmd_connection, cmd_connection_connect}
        Else
            command.data = New Byte() {COMMAND_IDENTIFIER, cmd_connection, cmd_connection_disconnect}
        End If

        command.send()
    End Sub

    Public Sub answerBroadCast(ByRef app As App)
        'Send server name back
        Dim command As New Command
        command.source = Network.getServerIp()
        command.destination = app.ip
        command.priority = RemoteControlServer.Command.PRIORITY_HIGH

        Dim serverName As Byte() = New Byte() {COMMAND_IDENTIFIER, cmd_get, cmd_get_server_name}
        command.data = buildCommandData(serverName, Converter.stringToByte(Server.gui.userName))

        command.send()
    End Sub

    Public Sub refuseBroadCast(ByRef app As App)

    End Sub

    Public Sub parseCommand(ByRef command As Command)
        Dim localCommand As Command = command
        Dim parseThread As Thread = New Thread(Sub() parseCommandThread(localCommand))
        parseThread.Start()
    End Sub

    Public Sub parseCommandThread(ByRef command As Command)
        Select Case command.data(1)
            Case cmd_connection
                parseConnectCommand(command)
            Case cmd_get
                answerGetRequest(command)
            Case cmd_set
                setValue(command)
            Case cmd_mouse
                parseMouseCommand(command)
            Case cmd_keyboard
                parseKeyboardCommand(command)
            Case cmd_open
                parseOpenCommand(command)
            Case Else
                Logger.add("Unknown command: " & Converter.commandToString(command))
        End Select
    End Sub

#Region "Connecting"

    Public Sub parseConnectCommand(ByRef command As Command)
        Dim app As App = Server.getApp(command.source)

        Select Case command.data(2)
            Case cmd_connection_connect
                'App wants to connect with the server       
                app.onConnect()
            Case cmd_connection_disconnect
                'App disconnected from the server       
                app.onDisconnect()
            Case cmd_connection_reachable
                'App checks if server is reachable
                'Reply with the server name

                If command.data.Length > 2 Then
                    'Command data contains source IP
                    app.ip = Converter.byteToString(command.data, 3)
                End If

                'answerBroadCast(app)
                app.onBroadCast(command)
                Logger.add(app.ip & " checked reachability")
            Case Else
                Logger.add("Unknown connection command: " & Converter.commandToString(command))
        End Select
    End Sub

#End Region

#Region "Open"

    Public Sub parseOpenCommand(ByRef command As Command)
        Dim app As App = Server.getApp(command.source)

        Select Case command.data(2)
            Case cmd_action_process
                Dim processString As String = Converter.byteToString(command.data, 3)
                Logger.add("Starting process: " & processString)
                Try
                    Process.Start(processString)
                Catch ex As Exception
                    Logger.add(ex.Message)
                End Try
            Case Else
                Logger.add("Unknown open command: " & Converter.commandToString(command))
        End Select
    End Sub

#End Region

#Region "Get and Set requests"

    Public Sub answerGetRequest(ByRef requestCommand As Command)
        Dim app As App = Server.getApp(requestCommand.source)

        Dim responseCommand As New Command
        responseCommand.source = Network.getServerIp()
        responseCommand.destination = app.ip
        responseCommand.priority = RemoteControlServer.Command.PRIORITY_HIGH

        Dim commandIdentifier As Byte() = New Byte() {COMMAND_IDENTIFIER, cmd_get, requestCommand.data(2)}

        Select Case requestCommand.data(2)
            Case cmd_get_server_version
                'responseCommand.data = buildCommandData(commandIdentifier, Converter.stringToByte(Server.getServerVersionName))
                responseCommand.data = buildCommandData(commandIdentifier, New Byte() {Updater.currentVersionCode})
            Case cmd_get_server_name
                responseCommand.data = buildCommandData(commandIdentifier, Converter.stringToByte(Server.gui.userName))
            Case cmd_get_os_name
                responseCommand.data = buildCommandData(commandIdentifier, Converter.stringToByte(Server.getServerOs))
            Case cmd_get_api_version
                responseCommand.data = buildCommandData(commandIdentifier, New Byte() {3})
            Case cmd_get_screenshot
                parseScreenshotProperties(requestCommand)
                answerScreenGetRequest(requestCommand, responseCommand)
            Case cmd_get_screenshots
                parseScreenshotProperties(requestCommand)
                Screenshot.continueSendingScreenshots = True
                If Not Screenshot.isSendingScreenshot Then
                    Screenshot.keepSendingScreenshots(requestCommand, responseCommand)
                End If
            Case Else
                Logger.add("Unknown get command: " & Converter.commandToString(requestCommand))
        End Select

        If Not responseCommand.data Is Nothing Then
            responseCommand.send()
        End If
    End Sub

    Public Sub parseScreenshotProperties(ByVal requestCommand As Command)
        If requestCommand.data.Length > 3 Then
            Screenshot.lastRequestedWidth = requestCommand.data(3) * 100

            If requestCommand.data.Length > 4 Then
                Screenshot.lastRequestedQuality = requestCommand.data(4)
            Else
                Screenshot.lastRequestedQuality = Settings.screenQualityFull
            End If
        Else
            Screenshot.lastRequestedWidth = 9999
        End If

        'Logger.add("Screenshot requested. Width: " & Screenshot.lastRequestedWidth & " Quality: " & Screenshot.lastRequestedQuality)
    End Sub

    Public Sub answerScreenGetRequest(ByVal requestCommand As Command, ByVal responseCommand As Command)
        'Logger.add("Sending screenshot. Width: " & Screenshot.lastRequestedWidth & " Quality: " & Screenshot.lastRequestedQuality)

        Dim screenshotBitmap As Bitmap = Screenshot.getResizedScreenshot(Screenshot.lastRequestedWidth)
        Dim screenshotData As Byte() = Converter.bitmapToByte(screenshotBitmap, Screenshot.lastRequestedQuality)
        Dim commandIdentifier = New Byte() {COMMAND_IDENTIFIER, cmd_get, cmd_get_screenshot, Screenshot.lastRequestedWidth / 100}
        responseCommand.data = buildCommandData(commandIdentifier, screenshotData)
        responseCommand.priority = Command.PRIORITY_MEDIUM
        responseCommand.send()
    End Sub

    Public Sub setValue(ByRef setCommand As Command)
        Dim app As App = Server.getApp(setCommand.source)

        Select Case setCommand.data(2)
            Case cmd_set_pin
                app.pin = Converter.byteToString(setCommand.data, 3)
                Logger.add("Pin from " & app.ip & " set to " & app.pin)
                validatePin(app)
            Case cmd_set_app_version
                app.appVersion = Converter.byteToString(setCommand.data, 3)
                Logger.add("App version from " & app.ip & " set to " & app.appVersion)
            Case cmd_set_app_name
                app.appName = Converter.byteToString(setCommand.data, 3)
                Logger.add("App name from " & app.ip & " set to " & app.appName)
            Case cmd_set_os_version
                app.osVersion = Converter.byteToString(setCommand.data, 3)
                app.detectOs()
                Logger.add("OS version from " & app.ip & " set to " & app.osVersion)
            Case cmd_set_device_name
                app.deviceName = Converter.byteToString(setCommand.data, 3)
                Logger.add("Device name from " & app.ip & " set to " & app.deviceName)
            Case Else
                Logger.add("Unknown set command: " & Converter.commandToString(setCommand))
        End Select

    End Sub

#End Region

#Region "Mouse & Keyboard"

    Public Sub parseMouseCommand(ByRef command As Command)
        Dim app As App = Server.getApp(command.source)

        Select Case command.data(2)
            Case cmd_mouse_pointers
                MouseV3.parsePointerData(command.data)
            Case cmd_mouse_pointers_absolute
                MouseV3.parseAbsolutePointerData(command.data, False)
            Case cmd_mouse_pointers_absolute_presenter
                MouseV3.parseAbsolutePointerData(command.data, True)
            Case cmd_mouse_pad_action
                Select Case command.data(3)
                    Case cmd_action_down
                        MouseV3.pointersDown()
                    Case cmd_action_up
                        MouseV3.pointersUp()
                    Case Else
                        Logger.add("Unknown mouse pad command")
                End Select
            Case cmd_mouse_left_action
                Select Case command.data(3)
                    Case cmd_action_down
                        Logger.add("Mouse left down")
                        MouseV3.leftMouseDown()
                    Case cmd_action_up
                        Logger.add("Mouse left up")
                        MouseV3.leftMouseUp()
                    Case Else
                        Logger.add("Unknown mouse left command")
                End Select
            Case cmd_mouse_right_action
                Select Case command.data(3)
                    Case cmd_action_down
                        Logger.add("Mouse right down")
                        MouseV3.rightMouseDown()
                    Case cmd_action_up
                        Logger.add("Mouse right up")
                        MouseV3.rightMouseUp()
                    Case Else
                        Logger.add("Unknown mouse right command")
                End Select
            Case Else
                Logger.add("Unknown mouse command: " & Converter.commandToString(command))
        End Select
    End Sub

    Public Sub parseKeyboardCommand(ByRef command As Command)
        Dim app As App = Server.getApp(command.source)

        Select Case command.data(2)
            Case cmd_keyboard_keycode
                Dim action As Byte = command.data(3)
                Dim keyCode As Integer = command.data(6)
                keyCode = (keyCode << 8) + command.data(7)
                'Logger.add("Received key code: " & keyCode)

                If keyCode < Keyboard.KEYCODE_C1 Or keyCode > Keyboard.KEYCODE_C12 Then
                    Select Case action
                        Case cmd_action_down
                            Keyboard.sendKeyDown(Keyboard.keycodeToKey(keyCode))
                        Case cmd_action_up
                            Keyboard.sendKeyUp(Keyboard.keycodeToKey(keyCode))
                        Case cmd_action_click
                            Dim keyboardKey As Key = Keyboard.keycodeToKey(keyCode)
                            If keyboardKey = Keyboard.KEYCODE_UNKOWN Then
                                Keyboard.keycodeToShortcut(keyCode)
                            Else
                                Keyboard.sendKeyPress(keyboardKey)
                            End If
                    End Select
                Else
                    'It's a custom key, get action from config
                    Dim actionIndex = keyCode - Keyboard.KEYCODE_C1
                    Logger.add("Received custom key code: " & (actionIndex + 1))

                    If actionIndex > Settings.customActions.Count - 1 Then
                        Logger.add("No custom action set")
                        Process.Start("http://remote-control-collection.com/help/custom/")
                        Logger.trackEevent("Server", "Custom", "Not set")
                    Else
                        Try
                            Dim path As String = Settings.customActions(actionIndex)
                            Process.Start(path)
                            Logger.trackEevent("Server", "Custom", path)
                        Catch ex As Exception
                            Logger.add("Unable to invoke custom action:")
                            Logger.add(ex.ToString)
                        End Try
                    End If


                End If
            Case cmd_keyboard_string
                Dim keyString As String = Converter.byteToString(command.data, 3)
                'Logger.add("Received key string: " & keyString)
                Keyboard.sendEachKey(keyString)
            Case Else
                Logger.add("Unknown keyboard command: " & Converter.commandToString(command))
        End Select

    End Sub

#End Region

End Module
