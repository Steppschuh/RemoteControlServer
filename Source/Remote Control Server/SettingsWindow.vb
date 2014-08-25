Public Class SettingsWindow

    Private isUserAction As Boolean = False

    Private Sub SettingsWindow_Load(sender As Object, e As EventArgs) Handles MyBase.Load
        isUserAction = False
        refreshUi()
        isUserAction = True
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
        '   General
        cb_autostart.Checked = Settings.autoStart
        cb_startMinimized.Checked = Settings.startMinimized
        cb_minimizeToTray.Checked = Settings.minimizeToTray
        cb_showTrayNotoifications.Checked = Settings.showTrayNotoifications
        dropDown_backDesign.SelectedIndex = Settings.backDesign

        '   Mouse and pointer
        track_mouse_sensitivity.Value = Settings.mouseSensitivity
        track_mouse_acceleration.Value = Settings.mouseAcceleration * 10
        track_motion_filter.Value = Settings.motionFilter
        track_motion_acceleration.Value = Settings.motionAcceleration * 10

        '   Screen
        track_screen_quality.Value = Settings.screenQuality
        track_screen_scale.Value = Settings.screenScale * 100
        track_screen_quality_full.Value = Settings.screenQualityFull
        track_screen_scale_full.Value = Settings.screenScaleFull * 100
        cb_screen_blackwhite.Checked = Settings.screenBlackWhite

        '   Slideshow
        cb_clickOnLaserUp.Checked = Settings.clickOnLaserUp
        dropDown_pointerDesign.SelectedIndex = Settings.pointerDesign
        Select Case Settings.pointerDesign
            Case 0
                img_pointer.Image = My.Resources.pointer_white
            Case 1
                img_pointer.Image = My.Resources.pointer
        End Select
        cb_cropBlackBorder.Checked = Settings.cropBlackBorder

        '   Media
        If Not My.Computer.FileSystem.FileExists(Settings.defaultMediaPlayer) Then
            Settings.defaultMediaPlayer = Media.getDefaultMediaPlayerPath
        End If
        tb_defaultPlayer.Text = Settings.defaultMediaPlayer

        '   Misc
        cb_serialCommands.Checked = Settings.serialCommands
        tb_serialPortName.Text = Settings.serialPortName
        cb_updateAmbientColor.Checked = Settings.updateAmbientColor

        '   Protection
        cb_useWhitelist.Checked = Settings.useWhiteList
        cb_usePin.Checked = Settings.usePin
        tb_pin.Text = Settings.pin
        If Settings.whitelistedIps.Count = 1 Then
            label_whitelist.Text = Settings.whitelistedIps.Count & " device whitelisted"
        Else
            label_whitelist.Text = Settings.whitelistedIps.Count & " devices whitelisted"
        End If

        'Upgrade

        'Update
        If Updater.hasUpdatesParsed Then
            label_update_available_version.Text = Updater.updateVersionName
        Else
            label_update_available_version.Text = "Unknown"
        End If
        label_update_current_version.Text = Server.getServerVersionName()

        'Help

        'IP
        label_IP_address.Text = Network.getServerIp()

    End Sub

#Region "Events"

#Region "General settings"

    Private Sub cb_autostart_CheckedChanged(sender As Object, e As EventArgs) Handles cb_autostart.CheckedChanged
        Settings.autoStart = cb_autostart.Checked

        'Try to apply autostart setting
        Dim success As Boolean = Settings.setAutostart(Settings.autoStart)
        If Not success And isUserAction Then
            Server.gui.showErrorDialog("Permission denied", "Can't apply the autostart setting, please restart the server with Administrator rights." _
                                       & vbNewLine & vbNewLine & _
                                       "To do that, right-click on the server executable and select 'Run as Administrator'.")
        End If
    End Sub

    Private Sub cb_startMinimized_CheckedChanged(sender As Object, e As EventArgs) Handles cb_startMinimized.CheckedChanged
        Settings.startMinimized = cb_startMinimized.Checked
    End Sub

    Private Sub cb_minimizeToTray_CheckedChanged(sender As Object, e As EventArgs) Handles cb_minimizeToTray.CheckedChanged
        Settings.minimizeToTray = cb_minimizeToTray.Checked
    End Sub

    Private Sub cb_showTrayNotoifications_CheckedChanged(sender As Object, e As EventArgs) Handles cb_showTrayNotoifications.CheckedChanged
        Settings.showTrayNotoifications = cb_showTrayNotoifications.Checked
    End Sub

    Private Sub dropDown_backDesign_SelectedIndexChanged(sender As Object, e As EventArgs) Handles dropDown_backDesign.SelectedIndexChanged
        Settings.backDesign = dropDown_backDesign.SelectedIndex
    End Sub

    Private Sub btn_appData_Click(sender As Object, e As EventArgs) Handles btn_appData.Click
        Process.Start(Settings.getAppDataDirectory)
    End Sub

#End Region

#Region "Protection settings"

    Private Sub cb_useWhitelist_CheckedChanged(sender As Object, e As EventArgs) Handles cb_useWhitelist.CheckedChanged
        Settings.useWhiteList = cb_useWhitelist.Checked
    End Sub

    Private Sub cb_usePin_CheckedChanged(sender As Object, e As EventArgs) Handles cb_usePin.CheckedChanged
        Settings.usePin = cb_usePin.Checked
    End Sub

    Private Sub btn_showPin_MouseHover(sender As Object, e As EventArgs) Handles btn_showPin.MouseHover
        tb_pin.UseSystemPasswordChar = False
    End Sub

    Private Sub btn_showPin_MouseLeave(sender As Object, e As EventArgs) Handles btn_showPin.MouseLeave
        tb_pin.UseSystemPasswordChar = True
    End Sub

    Private Sub btn_manageWhitelist_Click(sender As Object, e As EventArgs) Handles btn_manageWhitelist.Click
        Dim editor As New WhitelistWindow
        editor.ShowDialog()
    End Sub

    Private Sub tb_pin_TextChanged(sender As Object, e As EventArgs) Handles tb_pin.TextChanged
        Settings.pin = tb_pin.Text
    End Sub

#End Region

#Region "Mouse settings"

    Private Sub track_mouse_sensitivity_Scroll(sender As Object, e As EventArgs) Handles track_mouse_sensitivity.Scroll
        Settings.mouseSensitivity = track_mouse_sensitivity.Value
    End Sub

    Private Sub track_mouse_acceleration_Scroll(sender As Object, e As EventArgs) Handles track_mouse_acceleration.Scroll
        Settings.mouseAcceleration = track_mouse_acceleration.Value / 10
    End Sub

    Private Sub track_motion_filter_Scroll(sender As Object, e As EventArgs) Handles track_motion_filter.Scroll
        Settings.motionFilter = track_motion_filter.Value
    End Sub

    Private Sub track_motion_acceleration_Scroll(sender As Object, e As EventArgs) Handles track_motion_acceleration.Scroll
        Settings.motionAcceleration = track_motion_acceleration.Value / 10
    End Sub

#End Region

#Region "Screen settings"

    Private Sub track_screen_quality_Scroll(sender As Object, e As EventArgs) Handles track_screen_quality.Scroll
        Settings.screenQuality = track_screen_quality.Value
    End Sub

    Private Sub track_screen_scale_Scroll(sender As Object, e As EventArgs) Handles track_screen_scale.Scroll
        Settings.screenScale = track_screen_scale.Value / 100
    End Sub

    Private Sub track_screen_quality_full_Scroll(sender As Object, e As EventArgs) Handles track_screen_quality_full.Scroll
        Settings.screenQualityFull = track_screen_quality_full.Value
    End Sub

    Private Sub track_screen_scale_full_Scroll(sender As Object, e As EventArgs) Handles track_screen_scale_full.Scroll
        Settings.screenScaleFull = track_screen_scale_full.Value / 100
    End Sub

    Private Sub cb_screen_blackwhite_CheckedChanged(sender As Object, e As EventArgs) Handles cb_screen_blackwhite.CheckedChanged
        Settings.screenBlackWhite = cb_screen_blackwhite.Checked
    End Sub

#End Region

#Region "Slideshow settings"

    Private Sub cb_clickOnLaserUp_CheckedChanged(sender As Object, e As EventArgs) Handles cb_clickOnLaserUp.CheckedChanged
        Settings.clickOnLaserUp = cb_clickOnLaserUp.Checked
    End Sub

    Private Sub dropDown_pointer_SelectedIndexChanged(sender As Object, e As EventArgs) Handles dropDown_pointerDesign.SelectedIndexChanged
        If isUserAction Then
            Settings.pointerDesign = dropDown_pointerDesign.SelectedIndex
            Select Case Settings.pointerDesign
                Case 0
                    img_pointer.Image = My.Resources.pointer_white
                Case 1
                    img_pointer.Image = My.Resources.pointer
            End Select
        End If
    End Sub

    Private Sub cb_cropBlackBorder_CheckedChanged(sender As Object, e As EventArgs) Handles cb_cropBlackBorder.CheckedChanged
        Settings.cropBlackBorder = cb_cropBlackBorder.Checked
    End Sub

#End Region

#Region "Media Settings"

    Private Sub btn_browseDefaultPlayer_Click(sender As Object, e As EventArgs) Handles btn_browseDefaultPlayer.Click
        Dim fd As Forms.OpenFileDialog = New Forms.OpenFileDialog()
        Dim defaultMediaPlayerPath As String = ""

        fd.Title = "Select default media player"
        'fd.InitialDirectory = "C:\"
        fd.Filter = "All files (*.*)|*.*|All files (*.*)|*.*"
        fd.FilterIndex = 2
        fd.RestoreDirectory = True

        If fd.ShowDialog() = Forms.DialogResult.OK Then
            defaultMediaPlayerPath = fd.FileName
        End If
        If My.Computer.FileSystem.FileExists(defaultMediaPlayerPath) Then
            Settings.defaultMediaPlayer = defaultMediaPlayerPath
            tb_defaultPlayer.Text = defaultMediaPlayerPath
        End If
    End Sub

    Private Sub tb_defaultPlayer_TextChanged(sender As Object, e As EventArgs) Handles tb_defaultPlayer.TextChanged

    End Sub

#End Region

#Region "Misc settings"

    Private Sub btn_sendDebugCommand_Click(sender As Object, e As EventArgs) Handles btn_sendDebugCommand.Click
        Dim command As New Command
        command.data = New Byte() {2}
        Serial.sendCommand(command)

        pic_test.Image = Converter.colorToBitmap(Screenshot.averageColor)
    End Sub

    Private Sub btn_reopenPort_Click(sender As Object, e As EventArgs) Handles btn_reopenPort.Click
        Serial.closeSerialPort()
        Serial.openSerialPort(Settings.serialPortName)
    End Sub

    Private Sub tb_serialPortName_TextChanged(sender As Object, e As EventArgs) Handles tb_serialPortName.TextChanged
        Settings.serialPortName = tb_serialPortName.Text
    End Sub

    Private Sub cb_serialCommands_CheckedChanged(sender As Object, e As EventArgs) Handles cb_serialCommands.CheckedChanged
        Settings.serialCommands = cb_serialCommands.Checked
    End Sub

    Private Sub cb_updateAmbientColor_CheckedChanged(sender As Object, e As EventArgs) Handles cb_updateAmbientColor.CheckedChanged
        Settings.updateAmbientColor = cb_updateAmbientColor.Checked
    End Sub

    Private Sub btn_ledOn_Click(sender As Object, e As EventArgs) Handles btn_ledOn.Click
        Serial.sendMessage("<01>")
    End Sub

    Private Sub btn_ledOff_Click(sender As Object, e As EventArgs) Handles btn_ledOff.Click
        Serial.sendMessage("<02>")
    End Sub

#End Region

#Region "Update"

    Private Sub btn_update_install_Click(sender As Object, e As EventArgs) Handles btn_update_install.Click
        Updater.startUpdater()
    End Sub

    Private Sub btn_update_changelog_Click(sender As Object, e As EventArgs) Handles btn_update_changelog.Click
        Server.gui.showDialog("Changelog", Updater.updateChangeLog)
    End Sub

    Private Sub btn_update_help_Click(sender As Object, e As EventArgs) Handles btn_update_help.Click
        Process.Start(Updater.URL_UPDATE_HELP)
    End Sub

#End Region

#Region "Help"

    Private Sub btn_help_help_Click(sender As Object, e As EventArgs) Handles btn_help_help.Click
        Logger.trackEevent("Server", "Click", "Help page")
        Process.Start("http://remote-control-collection.com/help/")
    End Sub

    Private Sub btn_help_faq_Click(sender As Object, e As EventArgs) Handles btn_help_faq.Click
        Logger.trackEevent("Server", "Click", "FAQ page")
        Process.Start("http://remote-control-collection.com/help/faq/")
    End Sub

    Private Sub btn_help_setupguide_Click(sender As Object, e As EventArgs)
        Logger.trackEevent("Server", "Click", "Setup guide")
        Process.Start("http://remote-control-collection.com/help/connect/")
    End Sub

    Private Sub btn_help_troubleshooting_Click(sender As Object, e As EventArgs)
        Logger.trackEevent("Server", "Click", "Troubleshooting")
        Process.Start("http://remote-control-collection.com/help/troubleshooting/")
    End Sub

    Private Sub btn_help_contact_Click(sender As Object, e As EventArgs) Handles btn_help_contact.Click
        Logger.trackEevent("Server", "Click", "Send support mail")
        Dim body As String = Helper.generateHelpMail() & "My problem is "
        Helper.sendMail("support@android-remote.com", "Remote Control Support", body)
    End Sub

    Private Sub btn_help_github_Click(sender As Object, e As EventArgs) Handles btn_help_github.Click
        Logger.trackEevent("Server", "Click", "GitHub")
        Process.Start("https://github.com/Steppschuh/RemoteControlServer/issues")
    End Sub

#End Region

#Region "Upgrade"

    Private Sub btn_upgrade_help_code_Click(sender As Object, e As EventArgs) Handles btn_upgrade_help_code.Click
        Logger.trackEevent("Server", "Click", "Help Code")
        Process.Start("http://remote-control-collection.com/help/code/")
    End Sub

    Private Sub btn_upgrade_get_code_Click(sender As Object, e As EventArgs) Handles btn_upgrade_get_code.Click
        Logger.trackEevent("Server", "Click", "Get Code")
        Process.Start("http://code.remote-control-collection.com/")
    End Sub

    Private Sub btn_upgrade_android_Click(sender As Object, e As EventArgs) Handles btn_upgrade_android.Click
        Logger.trackEevent("Server", "Click", "Upgrade Android")
        Process.Start("http://remote-control-collection.com/download/app/android/")
    End Sub

    Private Sub btn_upgrade_ios_Click(sender As Object, e As EventArgs) Handles btn_upgrade_ios.Click
        Logger.trackEevent("Server", "Click", "Upgrade iOS")
        Process.Start("http://remote-control-collection.com/download/app/ios/")
    End Sub

    Private Sub btn_upgrade_bb_Click(sender As Object, e As EventArgs) Handles btn_upgrade_bb.Click
        Logger.trackEevent("Server", "Click", "Upgrade BlackBerry")
        Process.Start("http://remote-control-collection.com/download/app/bb/")
    End Sub

#End Region

#End Region

End Class