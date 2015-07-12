Imports System.Windows.Forms

Module Media

    Public Sub playMedia()
        Keyboard.sendKeyPress(Keys.MediaPlayPause)
    End Sub

    Public Sub stopMedia()
        Keyboard.sendKeyPress(Keys.MediaStop)
    End Sub

    Public Sub nextMedia()
        Keyboard.sendKeyPress(Keys.MediaNextTrack)
    End Sub

    Public Sub previousMedia()
        Keyboard.sendKeyPress(Keys.MediaPreviousTrack)
    End Sub

    Public Sub volumeUp()
        Keyboard.sendKeyPress(Keys.VolumeUp)
    End Sub

    Public Sub volumeDown()
        Keyboard.sendKeyPress(Keys.VolumeDown)
    End Sub

    Public Sub volumeMute()
        Keyboard.sendKeyPress(Keys.VolumeMute)
    End Sub

    Public Sub launchPlayer()
        Dim path As String = Settings.defaultMediaPlayer
        If Not My.Computer.FileSystem.FileExists(path) Then
            path = getDefaultMediaPlayerPath()
        End If

        Try
            Process.Start(path)
        Catch ex As Exception
            Logger.add("Unable to start Media Player " & path)
        End Try
    End Sub

    Public Function getDefaultMediaPlayerPath() As String
        Dim pathFound As String = ""
        Try
            Dim path As String
            'WMP
            path = My.Computer.FileSystem.SpecialDirectories.ProgramFiles
            path = path & "\Windows Media Player\wmplayer.exe"
            'Logger.add("Searching for WMP: " & path)
            If My.Computer.FileSystem.FileExists(path) Then
                pathFound = path
            End If

            'iTunes
            path = My.Computer.FileSystem.SpecialDirectories.ProgramFiles
            path = path & "\iTunes\iTunes.exe"
            'Logger.add("Searching for iTunes: " & path)
            If My.Computer.FileSystem.FileExists(path) Then
                pathFound = path
            End If

            'Spotify
            path = Environment.GetFolderPath(Environment.SpecialFolder.ApplicationData)
            path = path & "\Spotify\spotify.exe"
            'Logger.add("Searching for Spotify: " & path)
            If My.Computer.FileSystem.FileExists(path) Then
                pathFound = path
            End If
        Catch ex As Exception
        End Try
        Return pathFound
    End Function

End Module
