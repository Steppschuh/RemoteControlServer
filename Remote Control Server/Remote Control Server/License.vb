Public Class License

    Public isProVersion As Boolean = False

    Public Sub requestUpgrade()
        If Not Server.gui.dialogActive Then
            Server.showUpgradeWindow()
        End If
    End Sub
End Class
