Imports System.Windows.Forms

Module Keyboard

    Public Sub sendKeyPress(ByVal key As Keys)
        sendKeyDown(key)
        sendKeyUp(key)
    End Sub

    Public Sub sendKeyDown(ByVal key As Keys)
        Try
            Remote.keybd_event(key, 0, 0, 0) 'Down
        Catch ex As Exception
        End Try
    End Sub

    Public Sub sendKeyUp(ByVal key As Keys)
        Remote.keybd_event(key, 0, 2, 0) 'Up
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
                    Dim unicode As Integer = AscW(character)
                    SendInput.SendUniCodeKey(unicode)
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
