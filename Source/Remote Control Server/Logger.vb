Imports System.Windows.Threading

Module Logger

    Public showDebug As Boolean = True
    Public maxItems As Integer = 100

    Public logMessages As List(Of String)

    Private lastUpdate As Date
    Private dispatcherTimer As DispatcherTimer
    Private dispatcherActive As Boolean = False

    Private lastEntry As String = ""

    Public Sub initialize()
        lastUpdate = Date.Now
        logMessages = New List(Of String)
    End Sub

    Public Sub add(ByVal message As String)
        Dim timestamp As String = Date.Now.ToLongTimeString
        lastEntry = message
        logMessages.Add(timestamp & ": " & message)
        trim()

        'Update UI log
        If Date.Now.Ticks > lastUpdate.Ticks + 5000000 Then
            invalidateLog()
        Else
            startInvalidateTimer()
        End If
    End Sub

    Public Sub add(ByVal message As String, ByVal isDebug As Boolean)
        If showDebug Then
            add(message)
        End If
    End Sub

    Private Delegate Sub invalidLogDelegate()
    Public Sub invalidateLog()
        Try
            If Server.advanced.InvokeRequired Then
                Server.advanced.Invoke(New invalidLogDelegate(AddressOf invalidateLog))
            Else
                Server.advanced.tb_log.Text = getString()
                Server.advanced.tb_log.SelectionStart = Server.advanced.tb_log.Text.Length
                Server.advanced.tb_log.ScrollToCaret()
                lastUpdate = Date.Now
            End If
        Catch ex As Exception
            'Window disposed
        End Try
    End Sub

    Public Sub startInvalidateTimer()
        If Not dispatcherActive Then
            dispatcherTimer = New DispatcherTimer
            AddHandler dispatcherTimer.Tick, AddressOf invalidateTimerTick
            dispatcherTimer.Interval = New TimeSpan(0, 0, 1)
            dispatcherTimer.Start()
            dispatcherActive = True
        End If
    End Sub

    Private Sub invalidateTimerTick(ByVal sender As Object, ByVal e As EventArgs)
        invalidateLog()
        'dispatcherTimer.Stop()
        'dispatcherActive = False
    End Sub

    Public Sub clearLog()
        logMessages = New List(Of String)
    End Sub

    Private Sub trim()
        If logMessages.Count > maxItems Then
            logMessages.RemoveAt(0)
        End If
    End Sub

    Public Function getString() As String
        Dim result As String = ""
        For Each message As String In logMessages
            result = result & " " & message & vbNewLine
        Next
        Return result
    End Function

    Public Function getLastEntry() As String
        Dim maxLength As Integer = 30
        Dim entry As String = lastEntry

        Do While entry.Length > maxLength
            If entry.Contains(" ") Then
                entry = entry.Remove(entry.LastIndexOf(" ")) & "..."
            Else
                entry = entry.Remove(maxLength - 3) & "..."
            End If
        Loop
        Return entry
    End Function

End Module
