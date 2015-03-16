Public Class CustomWindow

    Private Sub CustomWindow_Load(sender As Object, e As EventArgs) Handles MyBase.Load
        For Each value As String In Settings.customActions
            tb_customActions.Text = tb_customActions.Text & value & vbNewLine
        Next

        'Remove last linebreak
        Try
            tb_customActions.Text = tb_customActions.Text.Substring(0, tb_customActions.Text.LastIndexOf(vbNewLine))
        Catch ex As Exception

        End Try
    End Sub

    Private Sub btn_apply_Click(sender As Object, e As EventArgs) Handles btn_apply.Click
        Settings.customActions = New List(Of String)
        Dim actions As String = tb_customActions.Text
        For Each value As String In actions.Split(vbNewLine)
            If Not Settings.customActions.Contains(value.Trim) Then
                Settings.customActions.Add(value.Trim)
            End If
        Next
        Me.Close()
    End Sub

End Class