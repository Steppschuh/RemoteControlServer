Imports System.Windows.Forms

Module Keyboard

    'Keycodes, as used in Android
    'http://developer.android.com/reference/android/view/KeyEvent.html

    Public Const KEYCODE_UNKOWN As Integer = 0
    Public Const KEYCODE_BACK As Integer = 4
    Public Const KEYCODE_UP As Integer = 19
    Public Const KEYCODE_DOWN As Integer = 20
    Public Const KEYCODE_LEFT As Integer = 21
    Public Const KEYCODE_RIGHT As Integer = 22
    Public Const KEYCODE_ALT As Integer = 57
    Public Const KEYCODE_SHIFT As Integer = 59
    Public Const KEYCODE_CAPS_LOCK As Integer = 115
    Public Const KEYCODE_DEL As Integer = 67
    Public Const KEYCODE_ENTER As Integer = 66
    Public Const KEYCODE_ESCAPE As Integer = 111
    Public Const KEYCODE_DEL_FORWARD As Integer = 112
    Public Const KEYCODE_CONTROL As Integer = 113
    Public Const KEYCODE_INSERT As Integer = 124
    Public Const KEYCODE_MOVE_END As Integer = 123
    Public Const KEYCODE_MOVE_HOME As Integer = 122
    Public Const KEYCODE_WINDOWS As Integer = 171
    Public Const KEYCODE_PAGE_DOWN As Integer = 93
    Public Const KEYCODE_PAGE_UP As Integer = 92
    Public Const KEYCODE_SPACE As Integer = 62
    Public Const KEYCODE_TAB As Integer = 61

    Public Const KEYCODE_F1 As Integer = 131
    Public Const KEYCODE_F2 As Integer = 132
    Public Const KEYCODE_F3 As Integer = 133
    Public Const KEYCODE_F4 As Integer = 134
    Public Const KEYCODE_F5 As Integer = 135
    Public Const KEYCODE_F6 As Integer = 136
    Public Const KEYCODE_F7 As Integer = 137
    Public Const KEYCODE_F8 As Integer = 138
    Public Const KEYCODE_F9 As Integer = 139
    Public Const KEYCODE_F10 As Integer = 140
    Public Const KEYCODE_F11 As Integer = 141
    Public Const KEYCODE_F12 As Integer = 142

    Public Const KEYCODE_COPY As Integer = 201
    Public Const KEYCODE_PASTE As Integer = 202
    Public Const KEYCODE_SELECT_ALL As Integer = 203
    Public Const KEYCODE_CUT As Integer = 204
    Public Const KEYCODE_SHOW_DESKTOP As Integer = 205
    Public Const KEYCODE_ZOOM_IN As Integer = 206
    Public Const KEYCODE_ZOOM_OUT As Integer = 207
    Public Const KEYCODE_CLOSE As Integer = 208
    Public Const KEYCODE_SHUTDOWN As Integer = 209
    Public Const KEYCODE_STANDBY As Integer = 210


    Public Sub sendKeyPress(ByVal key As Keys)
        If Not key = Nothing Then
            sendKeyDown(key)
            sendKeyUp(key)
        End If
    End Sub

    Public Sub sendUnicodeKeyPress(ByVal character As Char)
        Dim unicode As Integer = AscW(character)
        sendUnicodeKeyPress(unicode)
    End Sub

    Public Sub sendUnicodeKeyPress(ByVal unicode As Integer)
        SendInput.SendUniCodeKey(unicode)
    End Sub

    Public Sub sendKeyDown(ByVal key As Keys)
        Try
            Remote.keybd_event(key, 0, 0, 0) 'Down
        Catch ex As Exception
        End Try
    End Sub

    Public Sub sendKeyUp(ByVal key As Keys)
        Try
            Remote.keybd_event(key, 0, 2, 0) 'Up
        Catch ex As Exception

        End Try
    End Sub

    Public Sub sendEachKey(ByVal message As String)
        Dim key As Keys
        Dim character As Char
        For i As Integer = 0 To message.Length - 1
            Try
                character = message.Chars(i)
                key = stringToKey(Char.ToUpper(character))
                If key = Nothing Then
                    'Character is unicode, no Windows Key available
                    sendUnicodeKeyPress(character)
                Else
                    If Char.IsUpper(character) Then
                        sendKeyDown(Keys.ShiftKey)
                        sendKeyPress(key)
                        sendKeyUp(Keys.ShiftKey)
                    Else
                        sendKeyPress(key)
                    End If
                End If
            Catch ex As Exception

            End Try
        Next
    End Sub

    Public Sub sendKeys(ByVal message As String)
        System.Windows.Forms.SendKeys.SendWait(message)
    End Sub

    Public Sub sendShortcut(ByVal keys As List(Of Key))
        'Press each key down
        For Each singleKey As Key In keys
            sendKeyDown(singleKey)
        Next

        'Release keys in reverse order
        keys.Reverse()
        For Each singleKey As Key In keys
            sendKeyUp(singleKey)
        Next
    End Sub

    Public Function stringToKey(ByVal key As String) As Keys
        Dim kc As KeysConverter = New KeysConverter()
        Try
            Return CType(kc.ConvertFromString(key), Keys)
        Catch
            Return Keys.None
        End Try
    End Function

    Public Sub keycodeToShortcut(ByVal keyCode As Integer)
        Select Case keyCode
            Case KEYCODE_COPY
                sendShortcut(New List(Of Key) From {Keys.ControlKey, Keys.C})
            Case KEYCODE_PASTE
                sendShortcut(New List(Of Key) From {Keys.ControlKey, Keys.V})
            Case KEYCODE_SELECT_ALL
                sendShortcut(New List(Of Key) From {Keys.ControlKey, Keys.A})
            Case KEYCODE_CUT
                sendShortcut(New List(Of Key) From {Keys.ControlKey, Keys.X})
            Case KEYCODE_SHOW_DESKTOP
                sendShortcut(New List(Of Key) From {Keys.LWin, Keys.D})
            Case KEYCODE_ZOOM_IN
                MouseV2.zoom(1, 1)
            Case KEYCODE_ZOOM_OUT
                MouseV2.zoom(-1, 1)
            Case KEYCODE_CLOSE
                Keyboard.sendKeys("%{F4}")
            Case KEYCODE_SHUTDOWN
                shutdown()
            Case KEYCODE_STANDBY
                standby()
            Case Else
                Logger.add("Unknown keyboard command")
        End Select
    End Sub

    Public Function keycodeToKey(ByVal keyCode As Integer) As Key
        Select Case keyCode
            Case KEYCODE_BACK
                Return Keys.Back
            Case KEYCODE_CAPS_LOCK
                Return Keys.CapsLock
            Case KEYCODE_DEL
                Return Keys.Back
            Case KEYCODE_ENTER
                Return Keys.Return
            Case KEYCODE_ESCAPE
                Return Keys.Escape
            Case KEYCODE_INSERT
                Return Keys.Insert
            Case KEYCODE_MOVE_END
                Return Keys.End
            Case KEYCODE_MOVE_HOME
                Return Keys.Home
            Case KEYCODE_PAGE_DOWN
                Return Keys.PageDown
            Case KEYCODE_PAGE_UP
                Return Keys.PageUp
            Case KEYCODE_SPACE
                Return Keys.Space
            Case KEYCODE_TAB
                Return Keys.Tab
            Case KEYCODE_UP
                Return Keys.Up
            Case KEYCODE_DOWN
                Return Keys.Down
            Case KEYCODE_LEFT
                Return Keys.Left
            Case KEYCODE_RIGHT
                Return Keys.Right
            Case KEYCODE_ALT
                Return Keys.Alt
            Case KEYCODE_CONTROL
                Return Keys.ControlKey
            Case KEYCODE_SHIFT
                Return Keys.ShiftKey
            Case KEYCODE_DEL_FORWARD
                Return Keys.Delete
            Case KEYCODE_WINDOWS
                Return Keys.LWin
            Case KEYCODE_F1
                Return Keys.F1
            Case KEYCODE_F2
                Return Keys.F2
            Case KEYCODE_F3
                Return Keys.F3
            Case KEYCODE_F4
                Return Keys.F4
            Case KEYCODE_F5
                Return Keys.F5
            Case KEYCODE_F6
                Return Keys.F6
            Case KEYCODE_F7
                Return Keys.F7
            Case KEYCODE_F8
                Return Keys.F8
            Case KEYCODE_F9
                Return Keys.F9
            Case KEYCODE_F10
                Return Keys.F10
            Case KEYCODE_F11
                Return Keys.F11
            Case KEYCODE_F12
                Return Keys.F12
        End Select
        Return Nothing
    End Function

    Public Sub hibernate()
        Process.Start("rundll32.exe", "powrprof.dll,SetSuspendState 1,1,0")
    End Sub

    Public Sub standby()
        'If hibernate is enabled, this will call hibernate instead
        Process.Start("rundll32.exe", "powrprof.dll,SetSuspendState 0,1,0")
    End Sub

    Public Sub shutdown()
        Process.Start("shutdown.exe", "-s -t 00")
    End Sub

    Public Sub restart()
        Process.Start("shutdown.exe", "-r -t 00")
    End Sub



End Module
