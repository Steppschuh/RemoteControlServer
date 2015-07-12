Imports System.Threading

Public Module Server

    Public gui As MainWindow
    Public advanced As SettingsWindow
    Public upgrade As UpgradeWindow
    Public upgradeHandle As System.IntPtr
    Public pointer As PointerWindow
    Public status As String
    Public apps As List(Of App)

    Public Sub initialize(ByRef ui As MainWindow)
        gui = ui
        advanced = New SettingsWindow()
        upgrade = New UpgradeWindow()
        upgradeHandle = upgrade.Handle

        pointer = New PointerWindow()

        Logger.initialize()
        Logger.add("Initializing server")

        status = "Initializing"

        If isLatestServerRunning() Then
            Settings.loadSettings()
            Network.initialize()
            Updater.checkForUpdates(30)
            Screenshot.startUpdateColorTimer()

            Dim initializeThread = New Thread(AddressOf initializeAsync)
            initializeThread.IsBackground = True
            initializeThread.Start()
        Else
            'New server version has been started
            gui.stopRefreshUiTimer()
            gui.Close()
        End If

        Logger.add("UI ready")
    End Sub

    Public Sub initializeAsync()
        apps = New List(Of App)
        Logger.trackLaunchEvent()
        Logger.add("Server ready")
        status = "Ready"
        gui.showNotification("Server ready", "The Remote Control Server has been started and is waiting for a connection")
    End Sub

    Public Sub finish()
        Try
            Settings.saveSettings()

            'Stop listener threads
            Network.tcp.stopListener()
            Network.udp.stopListener()
            Serial.closeSerialPort()

            'Close all windows
            pointer.Close()
            upgrade.Close()
            advanced.Close()
        Catch ex As Exception

        End Try
    End Sub

    Public Function isLatestServerRunning() As Boolean
        'Updates for the Server are stored in the AppData directory
        Dim path_dl As String = Settings.getAppDataDirectory & "\Remote Control Server.exe"
        Dim path_cur As String = System.AppDomain.CurrentDomain.BaseDirectory & "Remote Control Server.exe"

        If path_cur.Equals(path_dl) Then
            'Currently running the latest version
            Return True
        Else
            Try
                If My.Computer.FileSystem.FileExists(path_dl) Then
                    'Start newer version
                    Process.Start(path_dl)
                    Return False
                Else
                    Return True
                End If
            Catch ex As Exception
                Logger.add("A new version of the Remote Control Server is available but could not be started." & vbNewLine & path_dl)
                Return True
            End Try
        End If
    End Function

    Public Function getApp(ByVal ip As String) As App
        Try
            For Each savedApp As App In apps
                If savedApp.ip.Equals(ip) Then
                    Return savedApp
                End If
            Next
        Catch ex As Exception
        End Try

        'No app found for the given IP, save a new one
        Dim app As App = New App
        app.ip = ip
        apps.Add(app)

        Return app
    End Function

    Public Function getCurrentApp() As App
        Return getApp(Remote.lastCommand.source)
    End Function

    Public Sub showAdvancedWindow()
        showAdvancedWindow(advanced.tab_log)
    End Sub

    Private Delegate Sub showAdvancedWindowDelegate(ByVal tab As System.Windows.Forms.TabPage)
    Public Sub showAdvancedWindow(ByVal tab As System.Windows.Forms.TabPage)
        Try
            If Server.advanced.InvokeRequired Then
                Server.advanced.Invoke(New showAdvancedWindowDelegate(AddressOf showAdvancedWindow), tab)
            Else
                Dim selectedTab As System.Windows.Forms.TabPage = tab

                'Check if form has been disposed
                If advanced.IsDisposed Then
                    advanced = New SettingsWindow()
                    For Each tabNew As System.Windows.Forms.TabPage In advanced.TabControlMain.TabPages
                        If tabNew.Name.Equals(tab.Name) Then
                            selectedTab = tabNew
                        End If
                    Next
                    Logger.invalidateLog()
                End If

                advanced.Show()
                advanced.TabControlMain.SelectedTab = selectedTab
            End If
        Catch ex As Exception

        End Try
    End Sub

    Private Delegate Sub showUpgradeWindowDelegate()
    Public Sub showUpgradeWindow()
        Try
            gui.Dispatcher.Invoke(New showUpgradeWindowDelegate(AddressOf showUpgradeWindowInvalidated))
        Catch ex As Exception
            Logger.add(ex.ToString)
        End Try
    End Sub

    Public Sub showUpgradeWindowInvalidated()
        Try
            upgrade = New UpgradeWindow()
            upgradeHandle = upgrade.Handle
            upgrade.Show()
            My.Computer.Audio.PlaySystemSound(System.Media.SystemSounds.Exclamation)
        Catch ex As Exception
            Logger.add(ex.ToString)
        End Try
    End Sub

    Public Function getServerVersionName() As String
        Return System.Reflection.Assembly.GetExecutingAssembly().GetName().Version.ToString
    End Function

    Public Function getServerOs() As String
        Return My.Computer.Info.OSFullName
    End Function

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

End Module
