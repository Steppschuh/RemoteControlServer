Public Class SettingsWindow

    Private Sub SettingsWindow_Load(sender As Object, e As EventArgs) Handles MyBase.Load
        refreshUi()
    End Sub

    Private Delegate Sub refreshUiDelegate()
    Public Sub refreshUi()
        If Me.InvokeRequired() Then
            Me.Invoke(New refreshUiDelegate(AddressOf refreshUi))
        Else
            updateValues()
        End If
    End Sub

    Private Sub updateValues()
        'Settings

        'Upgrade

        'Update
        If Updater.hasUpdatesParsed Then
            label_update_available_version.Text = Updater.updateVersionName
        Else
            label_update_available_version.Text = "Unknown"
        End If
        label_update_current_version.Text = Server.getServerVersionName()

        'Help

        'Log

    End Sub

#Region "Events"

    Private Sub btn_update_install_Click(sender As Object, e As EventArgs) Handles btn_update_install.Click
        Updater.startUpdater()
    End Sub

#End Region

End Class