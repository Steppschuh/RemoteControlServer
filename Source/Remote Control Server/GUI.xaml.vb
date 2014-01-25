Imports System.Windows.Threading
Imports System.Windows.Media
Imports System.Windows.Input
Imports System.Windows.Media.Animation
Imports System.Drawing

Class MainWindow

    Private canDragMove As Boolean = True
    Private refreshTimerActive As Boolean = False
    Public dialogActive As Boolean = False

    Public notificationIcon As System.Windows.Forms.NotifyIcon

    Private refreshUiTimer As DispatcherTimer
    Public userName As String = "Home PC"

#Region "Functions"

    Public Function getServerName() As String
        Try
            Dim name As String = My.User.Name
            If name = Nothing Then
                name = System.Security.Principal.WindowsIdentity.GetCurrent.Name
            End If
            'Return name.Remove(InStr(name, "\") - 1) 
            Return name.Substring(InStr(name, "\"))
        Catch ex As Exception
            Return System.Security.Principal.WindowsIdentity.GetCurrent.Name
        End Try
    End Function

#End Region

#Region "Form Methods"

    Public Sub initialize()
        userName = getServerName()
        Server.initialize(Me)
        refreshUi()
        startRefreshUiTimer()


        showNotificationIcon()
    End Sub

    Public Sub startRefreshUiTimer()
        If Not refreshTimerActive Then
            refreshTimerActive = True
            refreshUiTimer = New DispatcherTimer
            AddHandler refreshUiTimer.Tick, AddressOf listenUiTimerTick
            refreshUiTimer.Interval = New TimeSpan(0, 0, 0, 1)
            refreshUiTimer.Start()
        End If
    End Sub

    Public Sub stopRefreshUiTimer()
        refreshTimerActive = False
    End Sub

    Private Sub listenUiTimerTick(ByVal sender As Object, ByVal e As EventArgs)
        refreshUi()
        If Not refreshTimerActive Then
            refreshUiTimer.Stop()
        End If
    End Sub

    Public Sub refreshUi()
        Dim app As App = getCurrentApp()

        setLog(Logger.getLastEntry())

        label_app_device.Content = app.deviceName
        label_app_ip.Content = app.ip
        label_app_status.Content = app.status
        label_app_version.Content = app.appVersion

        label_server_ip.Content = Network.getServerIp()
        label_server_status.Content = Server.status
        label_server_version.Content = Server.getServerVersionName()

        If Updater.isUpdateAvailable Then
            image_server_version.Visibility = Windows.Visibility.Visible
        Else
            image_server_version.Visibility = Windows.Visibility.Hidden
        End If

        If Authentication.isProtected() Then
            label_server_protected.Content = "Yes"
            Converter.setImageDrawable(image_server_protected, "ic_action_secure.png")
        Else
            label_server_protected.Content = "No"
            Converter.setImageDrawable(image_server_protected, "ic_action_not_secure.png")
        End If

        image_app_device.Visibility = Windows.Visibility.Visible
        Select Case app.detectedOs
            Case Settings.OS_DEFAULT
                image_app_device.Visibility = Windows.Visibility.Hidden
                Converter.setImageDrawable(image_back, "back.png")
            Case Settings.OS_ANDROID
                Converter.setImageDrawable(image_app_device, "icon_android.png")
                Converter.setImageDrawable(image_back, "back.png")
            Case Settings.OS_BLACKBERRY
                Converter.setImageDrawable(image_app_device, "icon_bb.png")
                Converter.setImageDrawable(image_back, "back_bb.png")
            Case Settings.OS_IOS
                Converter.setImageDrawable(image_app_device, "icon_ios.png")
                Converter.setImageDrawable(image_back, "back_ios.png")
        End Select

    End Sub

    Public Sub setLog(ByVal message As String)
        If label_log.Content.Equals(message) Then
            'Log hasn't changed, fade it out after 5 seconds
            If label_log.IsEnabled Then
                Dim fadeOutAnimation As New DoubleAnimation()
                fadeOutAnimation.BeginTime = New TimeSpan(0, 0, 5)
                fadeOutAnimation.To = 0
                fadeOutAnimation.Duration = New Duration(TimeSpan.FromMilliseconds(200))
                fadeOutAnimation.AutoReverse = False

                Dim floatUpAnimation As New ThicknessAnimation()
                floatUpAnimation.BeginTime = New TimeSpan(0, 0, 5)
                floatUpAnimation.From = New Thickness(75, 104, 0, 0)
                floatUpAnimation.To = New Thickness(75, 90, 0, 0)
                floatUpAnimation.Duration = New Duration(TimeSpan.FromMilliseconds(500))
                floatUpAnimation.AutoReverse = False

                label_log.BeginAnimation(Label.OpacityProperty, fadeOutAnimation)
                label_log.BeginAnimation(Label.MarginProperty, floatUpAnimation)
                label_log.IsEnabled = False
            End If
        Else
            'Update log message
            label_log.Content = message

            Dim fadeInAnimation As New DoubleAnimation()
            fadeInAnimation.From = 0
            fadeInAnimation.To = 1
            fadeInAnimation.Duration = New Duration(TimeSpan.FromMilliseconds(500))
            fadeInAnimation.AutoReverse = False

            Dim floatInAnimation As New ThicknessAnimation()
            floatInAnimation.To = New Thickness(75, 104, 0, 0)
            floatInAnimation.From = New Thickness(75, 90, 0, 0)
            floatInAnimation.Duration = New Duration(TimeSpan.FromMilliseconds(200))
            floatInAnimation.AutoReverse = False

            label_log.BeginAnimation(Label.OpacityProperty, fadeInAnimation)
            label_log.BeginAnimation(Label.MarginProperty, floatInAnimation)
            label_log.IsEnabled = True
        End If

    End Sub

    Private Delegate Sub showErrorDialogDelegate(ByVal title As String, ByVal message As String)
    Public Sub showErrorDialog(ByVal title As String, ByVal message As String)
        Try
            If Not Me.Dispatcher.CheckAccess Then
                Me.Dispatcher.Invoke(New showErrorDialogDelegate(AddressOf showErrorDialog), title, message)
            Else
                dialogActive = True
                MessageBox.Show(message, title, MessageBoxButton.OK, MessageBoxImage.Error)
                dialogActive = False
            End If
        Catch ex As Exception
            'Window disposed
        End Try
    End Sub

    Private Delegate Sub showDialogDelegate(ByVal title As String, ByVal message As String)
    Public Sub showDialog(ByVal title As String, ByVal message As String)
        Try
            If Not Me.Dispatcher.CheckAccess Then
                Me.Dispatcher.Invoke(New showDialogDelegate(AddressOf showDialog), title, message)
            Else
                dialogActive = True
                MessageBox.Show(message, title, MessageBoxButton.OK, MessageBoxImage.None)
                dialogActive = False
            End If
        Catch ex As Exception
            'Window disposed
        End Try
    End Sub

    Public Sub showTooltip(ByVal tooltip As String)
        label_tooltip.Content = tooltip

        Dim myDoubleAnimation As New DoubleAnimation()
        myDoubleAnimation.To = 1
        myDoubleAnimation.Duration = New Duration(TimeSpan.FromMilliseconds(500))
        myDoubleAnimation.AutoReverse = False

        label_tooltip.BeginAnimation(Label.OpacityProperty, myDoubleAnimation)
    End Sub

    Public Sub hideTooltip()
        Dim myDoubleAnimation As New DoubleAnimation()
        myDoubleAnimation.To = 0
        myDoubleAnimation.Duration = New Duration(TimeSpan.FromSeconds(1))
        myDoubleAnimation.AutoReverse = False

        label_tooltip.BeginAnimation(Label.OpacityProperty, myDoubleAnimation)
    End Sub

    Public Sub showUI()
        Me.ShowInTaskbar = True
        Me.Visibility = Windows.Visibility.Visible
        Me.WindowState = Windows.WindowState.Normal
    End Sub

    Public Sub hideUi()
        If Settings.minimizeToTray Then
            Me.ShowInTaskbar = False
            Me.Visibility = Windows.Visibility.Hidden
        Else
            Me.ShowInTaskbar = True
        End If
        Me.WindowState = Windows.WindowState.Minimized
    End Sub

    Public Sub showNotificationIcon()
        Dim trayContextMenu As New Forms.ContextMenu()

        Dim item_show As New Forms.MenuItem("Show server", New EventHandler(AddressOf menuItemShow_Click))
        Dim item_advanced As New Forms.MenuItem("Advanced", New EventHandler(AddressOf menuItemAdvanced_Click))
        Dim item_close As New Forms.MenuItem("Close", New EventHandler(AddressOf menuItemClose_Click))

        trayContextMenu.MenuItems.Add(item_show)
        trayContextMenu.MenuItems.Add(item_advanced)
        trayContextMenu.MenuItems.Add(item_close)

        notificationIcon = New System.Windows.Forms.NotifyIcon()
        notificationIcon.Icon = bitmapToIcon(My.Resources.icon_rcc_server)
        notificationIcon.Text = "Remote Control Server"
        notificationIcon.ContextMenu = trayContextMenu
        notificationIcon.Visible = True
    End Sub

    

    Private Delegate Sub showNotificationDelegate(ByVal title As String, ByVal text As String)
    Public Sub showNotification(ByVal title As String, ByVal text As String)
        If Settings.showTrayNotoifications Then
            If Not Me.Dispatcher.CheckAccess Then
                Me.Dispatcher.Invoke(New showNotificationDelegate(AddressOf showNotification), title, text)
            Else
                notificationIcon.BalloonTipTitle = title
                notificationIcon.BalloonTipText = text
                Dim icon As Windows.Forms.ToolTipIcon = Forms.ToolTipIcon.Info
                notificationIcon.BalloonTipIcon = icon
                notificationIcon.ShowBalloonTip(2000)
            End If
        End If
    End Sub

#End Region

#Region "Form Events"

    Private Sub MainWindow_Loaded(sender As Object, e As RoutedEventArgs) Handles Me.Loaded
        Me.initialize()
    End Sub

    Private Sub MainWindow_Closed(sender As Object, e As EventArgs) Handles Me.Closed
        stopRefreshUiTimer()
        Server.finish()
    End Sub

    Private Sub MainWindow_StateChanged(sender As Object, e As EventArgs) Handles Me.StateChanged
        If Me.WindowState = Windows.WindowState.Minimized Then
            'stopRefreshUiTimer()
        Else
            'startRefreshUiTimer()
        End If
    End Sub

    Private Sub image_back_MouseLeftButtonDown(sender As Object, e As MouseButtonEventArgs) Handles image_back.MouseLeftButtonDown
        If canDragMove Then
            Me.DragMove()
        End If
    End Sub
#End Region

#Region "GUI Events"

    'Menu buttons
    Private Sub button_info_MouseUp(sender As Object, e As MouseButtonEventArgs) Handles button_info.MouseUp
        Server.showAdvancedWindow(Server.advanced.tab_log)
    End Sub

    Private Sub button_help_MouseUp(sender As Object, e As MouseButtonEventArgs) Handles button_help.MouseUp
        Server.showAdvancedWindow(Server.advanced.tab_help)
    End Sub

    Private Sub button_settings_MouseUp(sender As Object, e As MouseButtonEventArgs) Handles button_settings.MouseUp
        Server.showAdvancedWindow(Server.advanced.tab_settings)
    End Sub

    Private Sub button_upgrade_MouseUp(sender As Object, e As MouseButtonEventArgs) Handles button_upgrade.MouseUp
        Server.showAdvancedWindow(Server.advanced.tab_upgrade)
    End Sub

    'Icons
    Private Sub image_server_version_MouseUp(sender As Object, e As MouseButtonEventArgs) Handles image_server_version.MouseUp
        Server.showAdvancedWindow(Server.advanced.tab_update)
    End Sub

    Private Sub image_server_protected_MouseUp(sender As Object, e As MouseButtonEventArgs) Handles image_server_protected.MouseUp
        Server.showAdvancedWindow(Server.advanced.tab_settings)
    End Sub
#Region "Footer"
    Private Sub button_footer_close_MouseEnter(sender As Object, e As MouseEventArgs) Handles button_footer_close.MouseEnter
        showTooltip("Close")
    End Sub

    Private Sub button_footer_close_MouseLeave(sender As Object, e As MouseEventArgs) Handles button_footer_close.MouseLeave
        hideTooltip()
    End Sub

    Private Sub button_footer_close_MouseUp(sender As Object, e As MouseButtonEventArgs) Handles button_footer_close.MouseUp
        Me.Close()
    End Sub

    Private Sub button_footer_home_MouseEnter(sender As Object, e As MouseEventArgs) Handles button_footer_home.MouseEnter
        showTooltip("Homepage")
    End Sub

    Private Sub button_footer_home_MouseLeave(sender As Object, e As MouseEventArgs) Handles button_footer_home.MouseLeave
        hideTooltip()
    End Sub

    Private Sub button_footer_home_MouseUp(sender As Object, e As MouseButtonEventArgs) Handles button_footer_home.MouseUp
        Process.Start("http://remote-control-collection.com")
    End Sub

    Private Sub button_footer_hide_MouseEnter(sender As Object, e As MouseEventArgs) Handles button_footer_hide.MouseEnter
        showTooltip("Minimize to tray")
    End Sub

    Private Sub button_footer_hide_MouseLeave(sender As Object, e As MouseEventArgs) Handles button_footer_hide.MouseLeave
        hideTooltip()
    End Sub
#End Region


#End Region

#Region "Menu Events"

    Private Sub menuItemShow_Click(sender As System.Object, e As System.EventArgs)
        showUI()
    End Sub

    Private Sub menuItemAdvanced_Click(sender As System.Object, e As System.EventArgs)
        showUI()
        Server.showAdvancedWindow(Server.advanced.tab_settings)
    End Sub

    Private Sub menuItemClose_Click(sender As System.Object, e As System.EventArgs)
        Me.Close()
    End Sub

#End Region

End Class
