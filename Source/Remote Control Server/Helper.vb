Module Helper

    'Provides different methods and function that don't fit anywhere else

    Public Function generateHelpMail() As String
        Dim app As App = Server.getCurrentApp

        'App info
        Dim temp As String = "Hey, " & vbNewLine & "I'm using "
        If app.appName.Equals("Unknown") Then
            temp = temp & "a Remote Control app "
        Else
            temp = temp & "the " & app.appName & " version " & app.appVersion & " "
        End If

        'Device info
        If app.deviceName.Equals("Unknown") Or app.osVersion.Equals("Unknown") Then
            temp = temp & "on my device. " & vbNewLine
        Else
            temp = temp & "on my " & app.deviceName & " (" & app.osVersion & "). " & vbNewLine
        End If

        'Server info
        temp = temp & "My " & My.Computer.Info.OSFullName & " pc has the Remote Control Server version " & System.Reflection.Assembly.GetExecutingAssembly().GetName().Version.ToString & " installed. " & vbNewLine

        Return temp
    End Function

    Public Function sendMail(ByVal address As String, Optional ByVal subject As String = "", Optional ByVal body As String = "") As Boolean
        Dim success As Boolean = True
        Dim params As String = address
        If LCase(Strings.Left(params, 7)) <> "mailto:" Then _
            params = "mailto:" & params
        If subject <> "" Then params = params & _
              "?subject=" & subject
        If body <> "" Then
            params = params & IIf(subject = "", "?", "&")
            params = params & "body=" & body
        End If
        Try
            System.Diagnostics.Process.Start(params)
        Catch
            success = False
        End Try
        Return success
    End Function

End Module
