Public Class WhitelistWindow

    Public Sub validateInput()
        Dim countValid As Integer = 0
        Dim countInvalid As Integer = 0
        Dim whitelist As String = tb_whitelist.Text
        For Each ip As String In whitelist.Split(vbNewLine)
            If Network.isValidIp(ip.Trim) Then
                countValid += 1
            Else
                countInvalid += 1
            End If
        Next

        Dim status As String
        If countValid = 1 Then
            status = countValid & " valid IP address found"
        Else
            status = countValid & " valid IP addresses found"
        End If
        If countInvalid > 0 Then
            status = status & ", " & countInvalid & " invalid"
            btn_apply.Enabled = False
        Else
            btn_apply.Enabled = True
        End If
        label_count.Text = status
    End Sub

    Private Sub WhitelistWindow_Load(sender As Object, e As EventArgs) Handles MyBase.Load
        For Each ip As String In Settings.whitelistedIps
            tb_whitelist.Text = tb_whitelist.Text & ip & vbNewLine
        Next

        'Remove last linebreak
        tb_whitelist.Text = tb_whitelist.Text.Substring(0, tb_whitelist.Text.LastIndexOf(vbNewLine))

        validateInput()
    End Sub

    Private Sub tb_whitelist_TextChanged(sender As Object, e As EventArgs) Handles tb_whitelist.TextChanged
        validateInput()
    End Sub

    Private Sub btn_apply_Click(sender As Object, e As EventArgs) Handles btn_apply.Click
        Settings.whitelistedIps = New List(Of String)
        Dim whitelist As String = tb_whitelist.Text
        For Each ip As String In whitelist.Split(vbNewLine)
            If Network.isValidIp(ip.Trim) Then
                If Not Settings.whitelistedIps.Contains(ip.Trim) Then
                    Settings.whitelistedIps.Add(ip.Trim)
                End If
            End If
        Next
        Me.Close()
    End Sub

End Class