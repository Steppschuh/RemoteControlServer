Public Class UpgradeWindow

    Private Sub btn_upgrade_Click(sender As Object, e As EventArgs) Handles btn_upgrade.Click
        Process.Start("http://remote-control-collection.com/download/")
    End Sub

    Private Sub UpgradeWindow_FormClosed(sender As Object, e As Forms.FormClosingEventArgs) Handles Me.FormClosing
        Server.gui.dialogActive = False
    End Sub

    Private Sub UpgradeWindow_Load(sender As Object, e As EventArgs) Handles MyBase.Load
        Server.gui.dialogActive = True
    End Sub

    Private Sub tbn_code_Click(sender As Object, e As EventArgs) Handles tbn_code.Click
        Process.Start("http://remote-control-collection.com/help/code/")
    End Sub
End Class