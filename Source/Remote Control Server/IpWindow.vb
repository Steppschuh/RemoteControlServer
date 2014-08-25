Public Class IpWindow

    Private Sub IpWindow_Load(sender As Object, e As EventArgs) Handles MyBase.Load
        label_IP_address.Text = Network.getServerIp()
        TimerConnected.Start()
    End Sub

    Private Sub btn_IP_help_Click(sender As Object, e As EventArgs) Handles btn_IP_help.Click
        Server.showAdvancedWindow(Server.advanced.tab_help)
    End Sub


    Private Sub TimerConnected_Tick(sender As Object, e As EventArgs) Handles TimerConnected.Tick
        If Server.getCurrentApp.isConnected Then
            TimerConnected.Stop()
            finish()
        End If
    End Sub
End Class