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

        Settings.loadSettings()
        Network.initialize()
        Updater.checkForUpdates(10)

        Dim initializeThread = New Thread(AddressOf initializeAsync)
        initializeThread.IsBackground = True
        initializeThread.Start()

        Logger.add("UI ready")
    End Sub

    Public Sub initializeAsync()
        apps = New List(Of App)

        Authentication.initialize()

        Logger.add("Server ready")
        status = "Ready"
    End Sub

    Public Sub finish()
        Try
            'Stop listener threads
            Network.tcp.stopListener()
            Network.udp.stopListener()

            'Close all windows
            pointer.Close()
            upgrade.Close()
            advanced.Close()

            Settings.saveSettings()
        Catch ex As Exception

        End Try
    End Sub

    Public Function getApp(ByVal ip As String) As App
        For Each savedApp As App In apps
            If savedApp.ip.Equals(ip) Then
                Return savedApp
            End If
        Next

        'No app found for the given IP
        Dim app As App = New App

        If Not ip.Equals("Unknown") Then
            app.ip = ip
            apps.Add(app)
        End If

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

End Module
