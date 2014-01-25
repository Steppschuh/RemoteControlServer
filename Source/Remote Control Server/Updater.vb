Imports System.Threading
Imports System.Windows.Threading

Module Updater

    Public Const URL_UPDATE_INFO As String = "http://remote-control-collection.com/files/server/update.xml"
    Public Const URL_UPDATE_SERVER As String = "http://remote-control-collection.com/files/server/RemoteControlServer.exe"
    Public Const URL_UPDATE_SERVER_BETA As String = "http://remote-control-collection.com/files/server/RemoteControlServerBeta.exe"
    Public Const URL_UPDATE_HELP As String = ""

    Public Const currentVersionCode As Byte = 3

    Public updateVersionCode As Byte = 0
    Public updateVersionName As String = ""
    Public updateReleaseDate As String = ""
    Public updateChangeLog As String = ""

    Public isUpdateAvailable As Boolean = False
    Public hasUpdatesParsed As Boolean = False

    Private delayTimer As DispatcherTimer


    Public Sub checkForUpdates(ByVal delayInSeconds As Integer)
        'Start update check after <delay> seconds
        delayTimer = New DispatcherTimer
        AddHandler delayTimer.Tick, AddressOf checkForUpdates
        delayTimer.Interval = New TimeSpan(0, 0, delayInSeconds)
        delayTimer.Start()
    End Sub

    Public Sub checkForUpdates()
        Logger.add("Checking for updates")
        Dim updateThread As Thread = New Thread(Sub() checkForUpdatesThread())
        updateThread.Start()
        Try
            delayTimer.Stop()
        Catch
        End Try
    End Sub

    Public Sub checkForUpdatesThread()
        Dim callback As Func(Of String, Boolean) = AddressOf parseUpdateResult
        Network.getWebRequestAsync(URL_UPDATE_INFO, callback)
    End Sub

    Public Function parseUpdateResult(ByVal result As String) As Boolean
        'Callback function for the webrequest
        Try
            updateVersionCode = Integer.Parse(getValue("versioncode", result))
            updateVersionName = getValue("versionname", result)
            updateReleaseDate = getValue("releasedate", result)
            updateChangeLog = getValue("changelog", result).Replace(vbTab, " ")

            Dim currentVersionName As String = Server.getServerVersionName()

            If updateVersionCode > currentVersionCode Then
                Logger.add("Update available: " & updateVersionName)
                isUpdateAvailable = True
                gui.showNotification("Update available", "There is an update for the Remote Control Server available")
            Else
                Logger.add("No update available")
                isUpdateAvailable = False
            End If
            hasUpdatesParsed = True
        Catch ex As Exception
            Logger.add("Error while parsing update info")
        End Try

        Try
            Server.advanced.refreshUi()
        Catch ex As Exception
        End Try

        Return True
    End Function

    Function getValue(ByVal tag As String, source As String) As String
        Dim tmp As String = source
        Try
            Dim index_start As Integer = tmp.IndexOf(tag) + tag.Length + 1
            tmp = source.Substring(index_start)

            Dim index_end As Integer = tmp.IndexOf("</")
            tmp = tmp.Substring(0, index_end)
        Catch ex As Exception
            tmp = ""
        End Try
        Return tmp
    End Function

    Public Sub startUpdater()
        'Start the update tool or extract it
        Try
            Dim path_updater As String = Settings.getAppDataDirectory & "\Remote Control Server Updater.exe"
            If Not My.Computer.FileSystem.FileExists(path_updater) Then
                Logger.add("Extracting update tool to " & path_updater)
                My.Computer.FileSystem.WriteAllBytes(path_updater, My.Resources.Remote_Control_Server_Updater, False)
            End If

            Logger.add("Starting update tool")
            Process.Start(path_updater)

            gui.stopRefreshUiTimer()
            'Server.finish()
            gui.Close()
        Catch ex As Exception
            Logger.add("Unable to start update tool" & vbNewLine & ex.ToString)
            gui.showErrorDialog("Update failed", "Unable to start the update tool. Please start the Remote Control Server with administrator rights and try again.")
        End Try
    End Sub

End Module
